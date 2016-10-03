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

	return false; // TODO:
}

void Player::Reset()
{
	watch.Reset();
	hasStarted = false;
	wprintf(L"reset timer\n");
}

bool Player::SetSong(std::vector<HitObjectBase> base)
{
	objects = base;
	// TODO:
}

// TODO: Get rid of this
void Player::StartFirstNote()
{
	watch.Reset();
	watch.Start();
	hasStarted = true;
	// TODO: bunch more shit needed here as well	
}

void Player::Update()
{
	// TODO: Oh god.
}