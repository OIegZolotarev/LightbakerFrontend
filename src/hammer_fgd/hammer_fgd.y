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
%token  StringLiteral Identifier Number BaseDef "base" 
%token SizeBoundingBox "size" Iconsprite "iconsprite" Color "color"
%token IntegerType "integer" Color255 "color255" String "string" Sprite "sprite" Model "model" Studio "studio" Flags "flags" Choices "choices" 
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
SolidClassDef: SolidClass BasesClassesOpt '=' Identifier ':' StringLiteral '[' ClassFieldsOpt ']';

BasesClassesOpt:  /* empty */
|BaseDef '(' identifierListOpt ')';

// Точечная энтити
PointClassDef: PointClass  PointEntityConstructorListOpt '=' Identifier ':' StringLiteral '[' ClassFieldsOpt ']';

// Базовый класс
BaseClassDef: BaseClass '=' '[' ClassFieldsOpt ']';

// Параметры точечных энтиткй:

PointEntityConstructorListOpt: /* empty */
| PointEntityConstructorListOpt PointEntityConstructor;

// Обобщенные параметры точечных энтити
PointEntityConstructor: IconspriteDefOpt
| StudioDefOpt
| BoundingBoxDefOpt
| ColorDefOpt;

// Цвет
ColorDefOpt: Color '(' Number Number Number ')'

// ББокс
BoundingBoxDefOpt: SizeBoundingBox '(' Number Number Number ',' Number Number Number ')'

// Модель
StudioDefOpt: Studio '(' StringLiteral ')'

// Спрайт
IconspriteDefOpt: Iconsprite '(' StringLiteral ')'


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
Identifier '[' TypeId ']' ':' StringLiteral ':' DefaultValueOpt; 
// Флаги
| Identifier '[' Flags ']' '=' '[' FlagsValues ']'
// Перечисление
| Identifier '[' Choices ']' ':' StringLiteral ':' Number '=' '[' EnumValues ']'


// Значения флагов
FlagsValues: /* empty */
| Number ':' StringLiteral;

// Значения перечислений
EnumValues: /* empty */
| Number ':' StringLiteral;

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
		StringLiteral = '".*"';
		Identifier = [a-zA-Z_]+[0-9]*[a-zA-Z_]*;
		Number = [0-9]+;
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
		
		
		SolidClass { return GoldSource::FGDTokenTypes::SolidClass; }
		BaseClass { return GoldSource::FGDTokenTypes::BaseClass; }
        PointClass { return GoldSource::FGDTokenTypes::PointClass; }
        StringLiteral { return GoldSource::FGDTokenTypes::StringLiteral;}
        Identifier { return GoldSource::FGDTokenTypes::Identifier; }
        Number { return GoldSource::FGDTokenTypes::Number; }
        Comment { return GoldSource::FGDTokenTypes::Comment; }

        BaseDef  { return GoldSource::FGDTokenTypes::        BaseDef ; }
        SizeBoundingBox  { return GoldSource::FGDTokenTypes::        SizeBoundingBox ; }
        Iconsprite  { return GoldSource::FGDTokenTypes::        Iconsprite ; }
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






