#include "chlamydia.h"

inline bool is_white_space(char c)
{
	return (c == ' ' || c == '\t');
}

static inline std::vector<std::string> split_csv_args(std::string line)
{
	std::vector<std::string> argsOut;

	const char* linePtr = line.c_str();

	const char* curLineStart = NULL;
	const char* curLineEnd = NULL;

	while (char c = *linePtr)
	{
		bool wantsTokenTermination = false;
		bool wantsTokenAdd = false;

		if (c == ',')
		{
			size_t curLineLen = (curLineEnd - curLineStart);

			if (curLineLen != 0)
			{
				wantsTokenTermination = true;
				wantsTokenAdd = true;
			}
		}
		else
		{
			if (curLineStart == NULL)
			{
				if (!is_white_space(c))
				{
					curLineStart = linePtr;
				}
			}
			else
			{
				if (is_white_space(c))
				{
					wantsTokenTermination = true;
				}
			}
		}

		if (wantsTokenTermination)
		{
			curLineEnd = linePtr;
		}

		if (wantsTokenAdd)
		{
			argsOut.push_back(std::string(curLineStart, curLineEnd));

			curLineStart = NULL;
			curLineEnd = NULL;
		}

		linePtr++;
	}

	// Push ending token.
	{
		size_t curLineLen = (linePtr - curLineStart);

		if (curLineLen != 0)
		{
			argsOut.push_back(std::string(curLineStart, linePtr));
		}
	}

	return argsOut;
}

void beatmapParser::ParseFile(const std::string& fileName)
{
	std::ifstream fstream(fileName, std::ios::in);

	std::string section = "";

	while (!fstream.eof())
	{
		std::string line;
		fstream >> line;

		if (!line.empty() && line[0] == '[')
		{
			section = line;
		}
		else
		{
			if (section == "[Difficulty]")
			{
				char key[64];
				double value;

				if (sscanf(line.c_str(), "%64s:%f", key, &value) == 2)
				{
					std::string keyString = key;

					if (keyString == "HPDrainRate")
					{
						parsedBase.DifficultyHpDrainRate = value;
					}
					else if (keyString == "CircleSize")
					{
						parsedBase.DifficultyCircleSize = value;
					}
					else if (keyString == "OverallDifficulty")
					{
						parsedBase.DifficultyOverall = value;
					}
					else if (keyString == "ApproachRate")
					{
						parsedBase.DifficultyApproachRate = value;
					}
					else if (keyString == "SliderMultiplier")
					{
						parsedBase.DifficultySliderMultiplier = value;
					}
					else if (keyString == "SliderTickRate")
					{
						parsedBase.DifficultySliderTickRate = value;
					}
				}
			}
			else if (section == "[HitObjects]")
			{
				if (line.empty())
				{
					continue;
				}

				std::vector<std::string> lineParts = split_csv_args(line);

				if (lineParts.empty())
				{
					continue;
				}

				HitObjectBase object;
				object.Position.x = atoi(lineParts[0].c_str());
				object.Position.y = atoi(lineParts[1].c_str());
				object.StartTime = atoi(lineParts[2].c_str());
				object.Type = (HitObjectType)atoi(lineParts[3].c_str());

				if (object.IsType(HitObjectType::Slider))
				{
					object.EndTime = atoi(lineParts[lineParts.size() - 1].c_str()) + object.StartTime;

					//x,y,time,type,hitSound,sliderType|curveX:curveY|...|repeat,pixelLength|edgeHitsound:edgeAddition,addition
					//424,96,66,2,0,B|380:120|332:96|332:96|304:124,1,130,2|0,0:0|0:0,0:0:0:0:
					int points = std::count_if(lineParts[5].begin(), lineParts[5].end(), [](auto c) { return c == ':'; });

					auto data = NumbersFromString(lineParts[5] + "," + lineParts[6] + "," + lineParts[7] + "," + lineParts[8]);
					object.Points.push_back(Vector2(object.Position.x, object.Position.y));

					for (int i = 0; i < points; i++)
					{
						object.Points.push_back(Vector2(data[i * 2], data[(i * 2) + 1]));
					}

					object.UseCount = data[points * 2];
					object.Distance = data[(points * 2) + 1];

					std::vector<double> dPoints((points + 1) * 2);

					for (int i = 0; i < points; i++)
					{
						dPoints[i * 2] = object.Points[i].x;
						dPoints[(i * 2) + 1] = object.Points[i].y;
					}

					std::vector<double> dCurvePoints(dPoints.size());

					BezierCurve curve;
					curve.Bezier2D(dPoints.data(), dPoints.size(), object.Points.size(), dCurvePoints.data());

					for (int i = 0; i < dCurvePoints.size() / 2; i++)
					{
						object.CurvePoints.push_back(Vector2(dCurvePoints[i * 2], dCurvePoints[(i * 2) + 1]));
					}

					for (int i = 0; i < object.CurvePoints.size(); i++)
					{
						if (i > 0)
						{
							Vector2 dis = object.CurvePoints[i] - object.CurvePoints[i - 1];
							object.TotalDistance += dis.Length();
						}
					}
				}

				parsedObjects.push_back(object);
			}
		}
	}
}