/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#pragma once

#include "file_system.h"

namespace GoldSource
{


enum class FGDTokenTypes
{
	SolidClass = 0,
	BaseClass,
	PointClass,
	EqualsSign,
	Colon,
	StringLiteral,
	Identifier,
	Number,
	BaseDef,
	OpeningParenthesis,
	ClosingParenthesis,
	OpeningBracket,
	ClosingBracket,
	SizeBoundingBox,
	Iconsprite,
	Color,
	IntegerType,
	Color255,
	String,
	Sprite,
	Studio,
	Flags,
	Choices,
	Comment,
	EndOfFile
};

typedef std::pair<FGDTokenTypes, std::string> TokenTypeAndValue;

class HammerFGDFile
{


public:
	HammerFGDFile(FileData* fd);
	~HammerFGDFile();
};

}



