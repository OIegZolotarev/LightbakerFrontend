%language "c++"
%skeleton "lalr1.cc"

%debug
%no-lines
%define api.parser.class {HammerFGDParser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define parse.error verbose
%locations   

%param { FGDParsingContext* ctx }//%param

%code
{

struct FGDParsingContext
{
    const char* cursor;
    yy::location loc;

    FGDEntityClass * current_entity = nullptr;
    FGDPropertyDescriptor* current_property = nullptr;

    GoldSource::HammerFGDFile * fgd;
    

    FGDParsingContext(GoldSource::HammerFGDFile * file)
    {
       fgd = file;
       cursor = file->Data();
    }

    FGDEntityClass* new_entity(FGDEntityClassType type, std::string className, std::string description)
    {
		Con_Printf("new_entity(%d, %s, %s)\n", type, className.c_str(), description.c_str());

        current_entity = new FGDEntityClass(type, className, description);
        fgd->AddEntityClass(current_entity);

        return current_entity;
    }

    void SetColor(float r, float g, float b)
	{
		Con_Printf("SetColor(%f, %f, %f)\n", r, g, b);
//         assert(current_entity);
//         current_entity->SetColor255(r,g,b);
    }

    
    void SetBbox(float x, float y, float z)
    {
		Con_Printf("SetBbox(%f, %f, %f)\n", x, y, z);
//         assert(current_entity);
//         current_entity->SetBBox(glm::vec3(x,y,z));
    }

    void SetBbox(float x, float y, float z, float x2, float y2, float z2)
    {
		Con_Printf("SetBbox(%f, %f, %f, %f, %f, %f)\n", x, y, z, x2, y2, z2);
//         assert(current_entity);
//         current_entity->SetBBox(glm::vec3(x,y,z), glm::vec3(x2,y2,z2));
    }

    void SetModel(std::string model)
    {
		Con_Printf("SetModel(%s)\n", model.c_str());
//         assert(current_entity);
//         current_entity->SetModel(model);
    }

    void SetSprite(std::string model)
    {
		Con_Printf("SetSprite(%s)\n", model.c_str());
//         assert(current_entity);
//         current_entity->SetSprite(model);
    }

    void SetDecalEntity(bool flag)
    {
		Con_Printf("SetDecalEntity(%d)\n", flag);
//         assert(current_entity);
//         current_entity->SetDecalEntity(flag);
    }

    void SetEditorSprite(std::string sprite)
    {
		Con_Printf("SetEditorSprite(%s)\n", sprite.c_str());
//         assert(current_entity);
//         current_entity->SetEditorSprite(sprite);
    }

    void SetPropertyExtra(std::string property, float value)
    {
		Con_Printf("SetPropertyExtra(%s, %f)\n", property.c_str(), value);

//         assert(current_entity);
//         current_entity->SetPropertyExtra(property, value);
    }

    FGDPropertyDescriptor* AddProperty(std::string name, std::string typeId, std::string description)
    {
		Con_Printf("AddProperty(%s, %s, %s)\n", name.c_str(), typeId.c_str(), description.c_str());
//         assert(current_entity);
//         current_property = new FGDPropertyDescriptor(name, typeId, description);
//         current_entity->AddProperty(current_property);
//         return current_property;

            return nullptr;
    }
};

namespace yy { HammerFGDParser::symbol_type yylex(FGDParsingContext* ctx); }


void ParseFGD(GoldSource::HammerFGDFile * file)
{

    FGDParsingContext ctx(file);    

    auto fileName = file->FileName();

    ctx.loc.begin.filename = &fileName;
    ctx.loc.end.filename   = &fileName;

    yy::HammerFGDParser parser(&ctx);

    
    //parser.set_debug_level(999);

    parser.parse();  
}

}

%code requires
{

#include "..\application.h"
#include "..\common.h"
#include "..\hammer_fgd.h"

struct FGDParsingContext;
using namespace GoldSource;

}

%token	SolidClass "@SolidClass" 
%token	BaseClass "@BaseClass" 
%token	PointClass "@PointClass" 
%token  EqualsSign "=" 
%token	Colon ":" 
%token	Comma "," 
%token	StringLiteral Identifier
%token  Number 
%token	BaseDef "base"
%token  OpeningParenthesis "(" 
%token	ClosingParenthesis ")" 
%token	OpeningBracket "[" 
%token	ClosingBracket "]"  
%token  SizeBoundingBox "size" 
%token	Iconsprite "iconsprite" 
%token	Color "color"
%token  IntegerType "integer" 
%token  Color255 "color255" 
%token	String "string" 
%token	Sprite "sprite" 
%token	Decal "decal" 
%token	Studio "studio" 
%token	Flags "flags" 
%token	Choices "choices"  
%token	Model "model"
%token  TargetSource "target_source"
%token  TargetDestination "target_destination"
%token  Sound "sound"

%token  EndOfFile 0
%token	Comment

%type<float>        Number
%type<std::string>  Identifier StringLiteral TypeId Choices ModelType StringLiteralOpt Flags

%type<std::string> IntegerType Color255 String Sprite Studio TargetDestination TargetSource Sound Model


%type<GoldSource::FGDEntityClass*> EntityClassDef SolidClassDef PointClassDef BaseClassDef
%type<GoldSource::FGDPropertyDescriptor*> ClassFieldDef



%%

// Общее описание файла
FGDFile: classesOpt;


// Описание классов
classesOpt: classesOpt EntityClassDef
|%empty;


EntityClassDef: SolidClassDef    
|PointClassDef                   
|BaseClassDef                    


// Брашевая энтити
SolidClassDef: SolidClass CtorsOpt EqualsSign Identifier Colon StringLiteral OpeningBracket ClassFieldsOpt ClosingBracket { $$ = ctx->new_entity(FGDEntityClassType::Solid,$4, $6); };

// Точечная энтити
PointClassDef: PointClass CtorsOpt EqualsSign Identifier Colon StringLiteral OpeningBracket ClassFieldsOpt ClosingBracket { $$ = ctx->new_entity(FGDEntityClassType::Point,$4, $6); };

// Базовый класс
BaseClassDef: BaseClass CtorsOpt EqualsSign Identifier OpeningBracket ClassFieldsOpt ClosingBracket { $$ = ctx->new_entity(FGDEntityClassType::BaseDef,$4, "<none>"); };

// Параметры точечных энтиткй:

CtorsOpt: CtorsOpt Ctor
|%empty;

// Обобщенные параметры точечных энтити
Ctor: IconspriteDefOpt
| StudioDefOpt
| BoundingBoxDefOpt
| BaseClassRefOpt
| SpriteDefOpt
| DecalDefOpt
| ExtendedConstructor
| ColorDefOpt;

// Цвет
ColorDefOpt: Color OpeningParenthesis Number Number Number ClosingParenthesis { ctx->SetColor($3,$4,$5); }

// ББокс
BoundingBoxDefOpt: SizeBoundingBox OpeningParenthesis Number Number Number Comma Number Number Number ClosingParenthesis { ctx->SetBbox($3,$4,$5,$7,$8,$9); }
|SizeBoundingBox OpeningParenthesis Number Number Number ClosingParenthesis { ctx->SetBbox($3,$4,$5); }

// Модель
StudioDefOpt: Studio OpeningParenthesis StringLiteralOpt ClosingParenthesis { ctx->SetModel($3); }

// Спрайтовая энтити
SpriteDefOpt: Sprite OpeningParenthesis StringLiteralOpt ClosingParenthesis { ctx->SetSprite($3); }

// Декаль
DecalDefOpt: Decal OpeningParenthesis ClosingParenthesis { ctx->SetDecalEntity(true); }

// Спрайт редактора
IconspriteDefOpt: Iconsprite OpeningParenthesis StringLiteral ClosingParenthesis { ctx->SetEditorSprite($3); }


ExtendedConstructor: Identifier OpeningParenthesis Number ClosingParenthesis { ctx->SetPropertyExtra($1,$3); }

// Ссылка на базовый класс
BaseClassRefOpt: BaseDef OpeningParenthesis identifierListOpt ClosingParenthesis





StringLiteralOpt: StringLiteral { $$ = $1;}
|%empty { $$ = ""; };

identifierListOpt: identifierListOpt identifierList
|%empty;

identifierList: identifierList Comma Identifier
|				Identifier;

// Список свойств энтити
ClassFieldsOpt: ClassFieldsOpt ClassFieldDef
| %empty;



// Описание свойства энтити
ClassFieldDef: 
// Обычные поля
Identifier OpeningParenthesis TypeId ClosingParenthesis Colon StringLiteral  DefaultValueOpt { $$ = ctx->AddProperty($1, $3, $6); };
// Флаги
| Identifier OpeningParenthesis Flags ClosingParenthesis EqualsSign OpeningBracket FlagsValues ClosingBracket { $$ = ctx->AddProperty($1, $3, "<flags>"); };
// Перечисление
| Identifier OpeningParenthesis Choices ClosingParenthesis Colon StringLiteral Colon Number EqualsSign OpeningBracket EnumValues ClosingBracket { $$ = ctx->AddProperty($1, $3, $6); };

// Особый случай для поля "model"
| Model OpeningParenthesis ModelType ClosingParenthesis Colon StringLiteral DefaultValueOpt { $$ = ctx->AddProperty("model", $3, $6); };

// Особый случай для поля "color"
| Color OpeningParenthesis Choices ClosingParenthesis Colon StringLiteral Colon Number EqualsSign OpeningBracket EnumValues ClosingBracket { $$ = ctx->AddProperty("color", $3, $6); };
| Color OpeningParenthesis TypeId ClosingParenthesis Colon StringLiteral  DefaultValueOpt { $$ = ctx->AddProperty("color", $3, $6); };

// Особый случай для поля "sound"
| Sound OpeningParenthesis Choices ClosingParenthesis Colon StringLiteral Colon Number EqualsSign OpeningBracket EnumValues ClosingBracket { $$ = ctx->AddProperty("sound", $3, $6); };
| Sound OpeningParenthesis TypeId ClosingParenthesis Colon StringLiteral  DefaultValueOpt  { $$ = ctx->AddProperty("sound", $3, $6); };


// Особый случай для поля "size"
| SizeBoundingBox OpeningParenthesis Choices ClosingParenthesis Colon StringLiteral Colon Number EqualsSign OpeningBracket EnumValues ClosingBracket { $$ = ctx->AddProperty("size", $3, $6); };

// Особый случай для поля "texture(decal)"
| Identifier OpeningParenthesis Decal ClosingParenthesis { $$ = ctx->AddProperty("decal", "decal", "decal"); };



ModelType: Studio
| Sprite;

// Значения флагов
FlagsValues: FlagsValues FlagValue
|%empty;

FlagValue:  Number Colon StringLiteral Colon Number

// Значения перечислений
EnumValues: EnumValues EnumValue
|%empty;

EnumValue:  Number Colon StringLiteral



// Типы свойств энтити
TypeId: IntegerType
| Color255
| String
| Sprite
| Studio
| TargetDestination
| TargetSource
| Sound;


// Значение по умолчанию для энтити
DefaultValueOpt: Colon StringLiteral
| Colon Number
| %empty;

%%


yy::HammerFGDParser::symbol_type yy::yylex(FGDParsingContext *  ctx)
{
    const char* anchor = ctx->cursor;
    ctx->loc.step();

    auto s = [&](auto func, auto&&... params) 
    { 
        ctx->loc.columns(ctx->cursor - anchor); 
        return func(params...,  ctx->loc); 
    };

    /*!re2c        		
		re2c:yyfill:enable = 0;
        re2c:define:YYCTYPE = char;
		re2c:define:YYCURSOR = "ctx->cursor";


		
        
        
		 '\x00' { return s(yy::HammerFGDParser::make_EndOfFile); }

        '@SolidClass'               {                     return s(yy::HammerFGDParser::make_SolidClass); }
        '@BaseClass'                {                     return s(yy::HammerFGDParser::make_BaseClass); }
        '@PointClass'               {                     return s(yy::HammerFGDParser::make_PointClass); }
        '='                         {                     return s(yy::HammerFGDParser::make_EqualsSign); }
        ':'                         {                     return s(yy::HammerFGDParser::make_Colon); }                
        ','                         {                     return s(yy::HammerFGDParser::make_Comma); }                
        [-0-9]+                     {                     return s(yy::HammerFGDParser::make_Number, std::stol(std::string(anchor,ctx->cursor))); }
        '('                         {                     return s(yy::HammerFGDParser::make_OpeningParenthesis); }
        ')'                         {                     return s(yy::HammerFGDParser::make_ClosingParenthesis); }
        '['                         {                     return s(yy::HammerFGDParser::make_OpeningBracket); }
        ']'                         {                     return s(yy::HammerFGDParser::make_ClosingBracket); }       
        'base'                      {                     return s(yy::HammerFGDParser::make_BaseDef); }
        'size'                      {                     return s(yy::HammerFGDParser::make_SizeBoundingBox); }
        'iconsprite'                {                     return s(yy::HammerFGDParser::make_Iconsprite); }
        'color'                     {                     return s(yy::HammerFGDParser::make_Color); }
        'integer'                   {                     return s(yy::HammerFGDParser::make_IntegerType, std::string(anchor, ctx->cursor)) ; }
        'color255'                  {                     return s(yy::HammerFGDParser::make_Color255, std::string(anchor, ctx->cursor)) ; }        
        'sprite'                    {                     return s(yy::HammerFGDParser::make_Sprite, std::string(anchor, ctx->cursor)) ; }
        'decal'                     {                     return s(yy::HammerFGDParser::make_Decal) ; }
        'target_source'             {                     return s(yy::HammerFGDParser::make_TargetSource, std::string(anchor, ctx->cursor)) ; }
        'target_destination'        {                     return s(yy::HammerFGDParser::make_TargetDestination, std::string(anchor, ctx->cursor)) ; }
        'sound'                     {                     return s(yy::HammerFGDParser::make_Sound, std::string(anchor, ctx->cursor)) ; }
        'studio'                    {                     return s(yy::HammerFGDParser::make_Studio, std::string(anchor, ctx->cursor)) ; }
        'model'                     {                     return s(yy::HammerFGDParser::make_Model, std::string(anchor, ctx->cursor)) ; }        
        'flags'                     {                     return s(yy::HammerFGDParser::make_Flags, std::string(anchor, ctx->cursor)) ; }
        'choices'                   {                     return s(yy::HammerFGDParser::make_Choices, std::string(anchor, ctx->cursor)) ; }
        'string'                    {                     return s(yy::HammerFGDParser::make_String, std::string(anchor, ctx->cursor)) ; }		
		"\"" [^"]* "\""             {                     return s(yy::HammerFGDParser::make_StringLiteral, std::string(anchor+1, ctx->cursor-1)); } 
        [a-zA-Z_]+[0-9]*[a-zA-Z_]*  {                     return s(yy::HammerFGDParser::make_Identifier, std::string(anchor, ctx->cursor)); }
        "\r\n" | [\r\n]             { ctx->loc.lines();   return yylex(ctx); }
        "//" [^\r\n]*               {                     return yylex(ctx); }
        [\t\v\b\f ]                 { ctx->loc.columns(); return yylex(ctx); }        
    */
		
}

void yy::HammerFGDParser::error(const location_type& l,const std::string& m)
{
    Con_Printf("%s\n",m.c_str());
}






