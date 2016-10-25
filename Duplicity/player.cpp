#include "chlamydia.h"

void Player::SetDoubleTime(bool value)
{
	doubleTime = value;
}

bool Player::IsPlaying()
{
	if (hasStarted)
	{
		return true;
	}

	return false;
}

void Player::Reset()
{
	watch.Reset();
	currentNote = 0;
	hasStarted = false;
	wprintf(L"reset timer\n");
}

bool Player::SetSong(std::vector<HitObjectBase> base)
{
	objects = base;
	noteDelay = base[0].StartTime; // first note's start time should be the initial time (like 0 but not really)
	totalNotes = base.size();

	return true;
}

// TODO: Get rid of this
void Player::StartFirstNote()
{
	watch.Reset(); // just in case?
	watch.Start();
	hasStarted = true;
}

float InterpSpeed = 1300.0f;

void Player::CursorMovement(Vector2 position)
{
	float elapsed = moveWatch.Elapsed() / 1000.0f;

	POINT cursor;
	GetCursorPos(&cursor);

	Vector2 dest;
	dest.x = position.x;
	dest.y = position.y;

	RECT clientSize;
	GetWindowRect(g_osuWindow, &clientSize);

	Convert(dest, clientSize.right - clientSize.left, clientSize.bottom - clientSize.top);

	Vector2 newPos;

	POINT newPosPoint;
	newPosPoint.x = dest.x;
	newPosPoint.y = dest.y;

	ClientToScreen(g_osuWindow, &newPosPoint);

	newPos = Vector2(newPosPoint.x, newPosPoint.y);

	dest.x = newPos.x;
	dest.y = newPos.y;

	if (cursor.x < dest.x)
	{
		cursor.x += InterpSpeed * elapsed;
	}
	
	if (cursor.x > dest.x)
	{
		cursor.x -= InterpSpeed * elapsed;
	}

	if (cursor.y < dest.y)
	{
		cursor.y += InterpSpeed * elapsed;
	}

	if (cursor.y > dest.y)
	{
		cursor.y -= InterpSpeed * elapsed;
	}

	SetCursorPos(cursor.x, cursor.y);
}

void Player::MaybePerformClickHold()
{
	if (!isHolding)
	{
		Click();
	}
	else if (isHolding && watch.Elapsed() + noteDelay == objects[currentNote].StartTime)
	{
		Release();
		isHolding = false;
	}
}

// TODO: move out of here
void Player::PerformSingleClick()
{
	QueueUserWorkItem([](LPVOID) -> DWORD
	{
		Click();
		Sleep(10);
		Release();

		return 0;
	}, nullptr, 0);
}

void Player::Update()
{
	if (currentNote == totalNotes)
	{
		wprintf(L"finished the song!\n");
		Reset();
		return;
	}

	HitObjectBase currentObject = objects[currentNote];
	while (!(currentObject.StartTime <= watch.Elapsed() + noteDelay))
	{
		moveWatch.Start(); // omg starting a *maybe* started thing? well yeah my stopwatch doesnt re-init if already started lol

		Sleep(5);
		CursorMovement(currentObject.Position);

		moveWatch.Reset();
		moveWatch.Start(); 
	}

	wprintf(L"click! type: %i, xPos: %.2f, yPos: %.2f\n", currentObject.Type, currentObject.Position.x, currentObject.Position.y);

	switch (currentObject.Type)
	{
	case HitObjectType::Slider:
	case HitObjectType::SliderNewCombo:
		stopHoldTime = currentObject.EndTime;
		isHolding = true;
		break;
	case HitObjectType::Normal:
	case HitObjectType::NormalNewCombo:
		PerformSingleClick();
		break;
	case HitObjectType::Spinner:
		stopHoldTime = currentObject.EndTime;
		isHolding = true;
		break;
	}

	MaybePerformClickHold();

	currentNote++;
}