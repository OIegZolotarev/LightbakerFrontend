%language "c++"

%define api.parser.class {HammerFGDParser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define parse.error verbose

%param { GoldSource::HammerFGDFile* ctx }//%param

%code requires
{
	

#include "..\application.h"
#include "..\common.h"
#include "..\hammer_fgd.h"

GoldSource::FGDTokenTypes yylex(GoldSource::HammerFGDFile* ctx);

}

%token	SolidClass "@SolidClass" BaseClass "@BaseClass" PointClass "@PointClass" 
%token  EqualsSign "=" Colon ":" StringLiteral Identifier Number BaseDef "base"
%token  OpeningParenthesis "(" ClosingParenthesis ")" OpeningBracket "[" ClosingBracket "]"  
%token SizeBoundingBox "size" Iconsprite "iconsprite" Color "color"
%token IntegerType "integer" Color255 "color255" String "string" Sprite "sprite" Studio "studio" Flags "flags" Choices "choices" 
%token EndOfFile

%%

// Общее описание файла
FGDFile: classes;

// Описание классов
classes:  /* empty */
|SolidClassDef
|PointClassDef
|BaseClassDef;


// Брашевая энтити
SolidClassDef: SolidClass BasesClassesOpt EqualsSign Identifier Colon StringLiteral OpeningBracket ClassFieldsOpt ClosingBracket;

BasesClassesOpt:  /* empty */
|BaseDef OpeningBracket identifierListOpt ClosingBracket;

// Точечная энтити
PointClassDef: PointClass  PointEntityConstructorListOpt EqualsSign Identifier Colon StringLiteral OpeningBracket ClassFieldsOpt ClosingBracket;

// Базовый класс
BaseClassDef: BaseClass EqualsSign OpeningBracket ClassFieldsOpt ClosingBracket;

// Параметры точечных энтиткй:

PointEntityConstructorListOpt: /* empty */
| PointEntityConstructorListOpt PointEntityConstructor;

// Обобщенные параметры точечных энтити
PointEntityConstructor: IconspriteDefOpt
| StudioDefOpt
| BoundingBoxDefOpt
| ColorDefOpt;

// Цвет
ColorDefOpt: Color OpeningParenthesis Number Number Number ClosingParenthesis

// ББокс
BoundingBoxDefOpt: SizeBoundingBox OpeningParenthesis Number Number Number ',' Number Number Number ClosingParenthesis

// Модель
StudioDefOpt: Studio OpeningParenthesis StringLiteral ClosingParenthesis

// Спрайт
IconspriteDefOpt: Iconsprite OpeningParenthesis StringLiteral ClosingParenthesis


identifierListOpt: /* empty */
|identifierList;

identifierList: identifierList ',' Identifier
|				Identifier;

// Список свойств энтити
ClassFieldsOpt: /* empty */
| ClassFieldDef;

// Описание свойства энтити
ClassFieldDef: 
// Обычные поля
Identifier OpeningBracket TypeId ClosingBracket Colon StringLiteral Colon DefaultValueOpt; 
// Флаги
| Identifier OpeningBracket Flags ClosingBracket EqualsSign OpeningBracket FlagsValues ClosingBracket
// Перечисление
| Identifier OpeningBracket Choices ClosingBracket Colon StringLiteral Colon Number EqualsSign OpeningBracket EnumValues ClosingBracket


// Значения флагов
FlagsValues: /* empty */
| Number Colon StringLiteral;

// Значения перечислений
EnumValues: /* empty */
| Number Colon StringLiteral;

// Типы свойств энтити
TypeId: IntegerType
| Color255
| String
| Sprite
| Studio;


// Значение по умолчанию для энтити
DefaultValueOpt: /**/
|StringLiteral;

%%


GoldSource::FGDTokenTypes yylex(GoldSource::HammerFGDFile* ctx)
{
    char* s = ctx->ParserCursor();

    /*!re2c        		
		re2c:yyfill:enable = 0;
        re2c:define:YYCTYPE = char;
		re2c:define:YYCURSOR = s;

        SolidClass = "@SolidClass";        	
		BaseClass = "@BaseClass";      		
		PointClass = "@PointClass";
		EqualsSign = '=';
		Colon = ':';
		StringLiteral = '".*"';
		Identifier = [a-zA-Z_]+[0-9]*[a-zA-Z_]*;
		Number = [0-9]+;
		OpeningParenthesis = '(';
		ClosingParenthesis = ')';
		OpeningBracket = '[';
		ClosingBracket = ']';
		Comment = '\\'.*;
        
        BaseDef = 'base';
        SizeBoundingBox = 'size';
        Iconsprite = 'iconsprite';
        Color = 'color';
        IntegerType = 'integer';
        Color255 = 'color255';
        String = 'string';
        Sprite = 'sprite';
        Studio = 'studio';
        Model = 'model';
        Flags = 'flags';
        Choices = 'choices';
		
		
		 [\x00] {
            return GoldSource::FGDTokenTypes::EndOfFile;
        }
		
		
		SolidClass { return  GoldSource::FGDTokenTypes::SolidClass; }
		BaseClass { return  GoldSource::FGDTokenTypes::BaseClass; }
        PointClass { return  GoldSource::FGDTokenTypes::PointClass; }
        EqualsSign { return  GoldSource::FGDTokenTypes::EqualsSign; }
        Colon { return  GoldSource::FGDTokenTypes::Colon; }
        StringLiteral { return  GoldSource::FGDTokenTypes::StringLiteral;}
        Identifier { return  GoldSource::FGDTokenTypes::Identifier; }
        Number { return  GoldSource::FGDTokenTypes::Number; }
        OpeningParenthesis { return  GoldSource::FGDTokenTypes::OpeningParenthesis; }
        ClosingParenthesis { return  GoldSource::FGDTokenTypes::ClosingParenthesis; }
        OpeningBracket { return  GoldSource::FGDTokenTypes::OpeningBracket; }
        ClosingBracket { return  GoldSource::FGDTokenTypes::ClosingBracket; }
        Comment { return  GoldSource::FGDTokenTypes::Comment; }

        BaseDef  { return GoldSource::FGDTokenTypes::BaseDef ; }
        SizeBoundingBox  { return GoldSource::FGDTokenTypes::SizeBoundingBox ; }
        Iconsprite  { return GoldSource::FGDTokenTypes::Iconsprite ; }
        Color {  return GoldSource::FGDTokenTypes::Color; }
        IntegerType  { return GoldSource::FGDTokenTypes::IntegerType ; }
        Color255  { return GoldSource::FGDTokenTypes::Color255 ; }
        String  { return GoldSource::FGDTokenTypes::String ; }
        Sprite  { return GoldSource::FGDTokenTypes::Sprite ; }
        Studio  { return GoldSource::FGDTokenTypes::Studio ; }
        Model  { return GoldSource::FGDTokenTypes::Model ; }
        Flags  { return GoldSource::FGDTokenTypes::Flags ; }
        Choices  { return GoldSource::FGDTokenTypes::Choices ; }
        
    */
	
	return GoldSource::FGDTokenTypes::EndOfFile;
}

void yy::HammerFGDParser::error(const std::string& m)
{
    Con_Printf("%s\n",m.c_str());
}






