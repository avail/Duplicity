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

float InterpSpeed = 1.0f;

// TODO: move out of here
void Player::PerformSingleClick(Vector2 position)
{
	POINT cursor;
	GetCursorPos(&cursor);

	Vector2 dest;
	dest.x = position.x;
	dest.y = position.y;

	RECT clientSize;
	GetClientRect(g_osuWindow, &clientSize);

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
		cursor.x += (int)(InterpSpeed * watch.Elapsed());
	}

	if (cursor.x > dest.x)
	{
		cursor.x -= (int)(InterpSpeed * watch.Elapsed());
	}

	if (cursor.y < dest.y)
	{
		cursor.y += (int)(InterpSpeed * watch.Elapsed());
	}

	if (cursor.y > dest.y)
	{
		cursor.y -= (int)(InterpSpeed * watch.Elapsed());
	}

	MoveCursor(Vector2(cursor.x, cursor.y));

	wprintf(L"%i, %i\n", cursor.x, cursor.y);

	/*Click();
	Release();*/
}

void Player::Update()
{
	HitObjectBase currentObject = objects[currentNote];
	while (currentObject.StartTime != watch.Elapsed() + noteDelay);

	wprintf(L"note to be clicked now! type: %i, currentNote: %i, watch: %i, StartTime: %i, noteDelay: %i\n", currentObject.Type, currentNote, watch.Elapsed(), currentObject.StartTime, noteDelay);

	switch (currentObject.Type)
	{
	case HitObjectType::Slider:
	case HitObjectType::SliderNewCombo:
		
		break;
	case HitObjectType::Normal:
	case HitObjectType::NormalNewCombo:
		PerformSingleClick(currentObject.Position);
		break;
	case HitObjectType::Spinner:
		break;
	}

	if (currentNote == totalNotes)
	{
		wprintf(L"finished the song!\n");
		Reset();
		return;
	}

	currentNote++;
}