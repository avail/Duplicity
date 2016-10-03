#pragma once
#include "chlamydia.h"

class BeatmapBase
{
public:
	inline BeatmapBase()
	{
		this->DifficultyApproachRate = 5;
		this->DifficultyCircleSize = 5;
		this->DifficultyHpDrainRate = 5;
		this->DifficultyOverall = 5;
		this->DifficultySliderMultiplier = 1.4;
		this->DifficultySliderTickRate = 1;
	}

public:
	float DifficultyApproachRate;
	float DifficultyCircleSize;
	float DifficultyHpDrainRate;
	float DifficultyOverall;

	double DifficultySliderMultiplier;
	double DifficultySliderTickRate;

	std::string Artist;
	std::string ArtistUnicode;

	std::string Tags;
	std::string Title;
	std::string TitleUnicode;
};

enum class HitObjectType
{
	Normal = 1,
	Slider = 2,
	NewCombo = 4,
	NormalNewCombo = 5,
	SliderNewCombo = 6,
	Spinner = 8,
	ColourHax = 112,
	Hold = 128,
	ManiaLong = 128
};

enum class HitObjectSoundType
{
	None = 0,
	Normal = 1,
	Whistle = 2,
	Finish = 4,
	Clap = 8
};

struct Vector2
{
	float x;
	float y;

	Vector2(const Vector2&) = default;

	inline Vector2()
		: x(0), y(0)
	{

	}

	inline Vector2(float x, float y)
		: x(x), y(y)
	{

	}

	inline Vector2 operator*(float scale) const
	{
		return Vector2(this->x * scale, this->y * scale);
	}

	inline Vector2 operator-(Vector2 right) const
	{
		return Vector2(this->x - right.x, this->y - right.y);
	}

	inline Vector2 operator+(Vector2 right) const
	{
		return Vector2(this->x + right.x, this->y + right.y);
	}

	inline void Normalize()
	{
		float magnitude = Length();

		x = x / magnitude;
		y = y / magnitude;
	}

	inline float Length() const
	{
		return sqrtf((x * x) + (y * y));
	}
};

struct Color
{
	int red;
	int green;
	int blue;
};

template<typename TEnum>
inline bool IsType(TEnum value, TEnum flag)
{
	return ((int)value & (int)flag) != 0;
}

class HitObjectBase
{
public:
	inline HitObjectBase()
	{
		this->SegmentCount = 1;
		this->SpatialLength = 0;
		this->ComboColourOffset = 0;
		this->TotalDistance = 0.0f;
	}

	HitObjectBase(const HitObjectBase&) = default;

public:
	int StartTime;
	int EndTime;

	inline int GetLength() const
	{
		return EndTime - StartTime;
	}

	HitObjectType Type;

	bool IsType(HitObjectType type) const
	{
		return ::IsType(Type, type);
	}

	HitObjectSoundType SoundType;

	bool Whistle(HitObjectSoundType type) const
	{
		return ::IsType(SoundType, type);
	}

	bool Finish(HitObjectSoundType type) const
	{
		return ::IsType(SoundType, type);
	}

	bool Clap(HitObjectSoundType type) const
	{
		return ::IsType(SoundType, type);
	}

	int SegmentCount;

	inline int GetSegmentLength() const
	{
		return GetLength() / SegmentCount;
	}

	double SpatialLength;

	virtual inline bool NewCombo(HitObjectType type) const
	{
		return ::IsType(Type, type);
	}

	int ComboColourOffset;
	int ComboColourIndex;

	Color Colour;
	Vector2 Position;

	std::vector<Vector2> Points;
	std::vector<Vector2> CurvePoints;
	
	int UseCount;
	int Distance;

	float TotalDistance;

	virtual inline Vector2 GetEndPosition() const
	{
		// TODO
		return Vector2();
	}

	int StackCount;
	virtual inline int GetComboNumber() const
	{
		// TODO
		return 0;
	}

	bool LastInCombo;

	virtual inline Vector2 PositionAtTime(int time) const
	{
		return Position;
	}
};


class beatmapParser
{
public:
	void ParseFile(const std::string& beatmapFileName);

	inline BeatmapBase GetParsedBase()
	{
		return parsedBase;
	}

	inline const std::vector<HitObjectBase>& GetParsedObjects()
	{
		return parsedObjects;
	}

private:
	BeatmapBase parsedBase;

	std::vector<HitObjectBase> parsedObjects;
};