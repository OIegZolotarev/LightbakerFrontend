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

    
    struct 
    {
        glm::vec3 color = { 1,1,1};
        glm::vec3 mins = {-4,-4,-4};
        glm::vec3 maxs = {4,4,4};

        glm::vec3 offset = {0,0,0};

        std::string model = "";
        std::string sprite = "";
        bool decal = false;
        std::string editorSprite = "";

        unsigned int flags = 0;
        
        std::list<std::string> baseClasses;

        void clear()
        {
            color = { 1,1,1};
            mins = {-4,-4,-4};
            maxs = {4,4,4};
            offset = {0,0,0};
            model = "";
            sprite = "";
            decal = false;
            editorSprite = "";
            flags = 0;
        }

    }entityCtorData;

    FGDParsingContext(GoldSource::HammerFGDFile * file)
    {
       fgd = file;
       cursor = file->Data();
    }

    FGDEntityClass* new_entity(FGDEntityClassType type, std::string className, std::string description,FGDPropertiesList & props)
    {
		
        current_entity = new FGDEntityClass(fgd, type, className, description, props);
        current_entity->SetCtorFlags(entityCtorData.flags);

        if (entityCtorData.flags & FL_SET_COLOR)            current_entity->SetColor(entityCtorData.color);
        if (entityCtorData.flags & FL_SET_SIZE)             current_entity->SetBBox(entityCtorData.mins, entityCtorData.maxs);
        if (entityCtorData.flags & FL_SET_MODEL)            current_entity->SetModel(entityCtorData.model);
        if (entityCtorData.flags & FL_SET_SPRITE)           current_entity->SetSprite(entityCtorData.sprite);
        if (entityCtorData.flags & FL_SET_DECAL)            current_entity->SetDecalEntity(entityCtorData.decal);
        if (entityCtorData.flags & FL_SET_EDITOR_SPRITE)    current_entity->SetEditorSprite(entityCtorData.editorSprite);
        if (entityCtorData.flags & FL_SET_BASE_CLASSES)    current_entity->SetBaseClasses(entityCtorData.baseClasses);
        if (entityCtorData.flags & FL_SET_BBOX_OFFSET)    current_entity->SetBBoxOffset(entityCtorData.offset);

        entityCtorData.clear();
        
        fgd->AddEntityClass(current_entity);

        return current_entity;
    }

    void SetColor(float r, float g, float b)
	{
        entityCtorData.color.r = r / 255.f;
        entityCtorData.color.g = g / 255.f;
        entityCtorData.color.b = b / 255.f;

        entityCtorData.flags |= FL_SET_COLOR;
    }

    
    void SetBbox(float x, float y, float z)
    {
		entityCtorData.mins.x = -x / 2;
		entityCtorData.mins.y = -y / 2;
		entityCtorData.mins.z = -z / 2;

        entityCtorData.maxs.x = x / 2;
		entityCtorData.maxs.y = y / 2;
		entityCtorData.maxs.z = z / 2;

        entityCtorData.flags |= FL_SET_SIZE;
    }

    void SetBbox(float x, float y, float z, float x2, float y2, float z2)
    {
		entityCtorData.mins.x = x;
		entityCtorData.mins.y = y;
		entityCtorData.mins.z = z;

        entityCtorData.maxs.x = x2;
		entityCtorData.maxs.y = y2;
		entityCtorData.maxs.z = z2;

        entityCtorData.flags |= FL_SET_SIZE;
    }

    void SetModel(std::string model)
    {
        entityCtorData.model = model;
        entityCtorData.flags |= FL_SET_MODEL;
    }

    void SetSprite(std::string model)
    {
        entityCtorData.sprite = model;
        entityCtorData.flags |= FL_SET_SPRITE;
    }

    void SetDecalEntity(bool flag)
    {
        entityCtorData.decal = flag;
        entityCtorData.flags |= FL_SET_DECAL;
    }

    void SetEditorSprite(std::string sprite)
    {
        entityCtorData.editorSprite = sprite;
        entityCtorData.flags |= FL_SET_EDITOR_SPRITE;
    }

    void SetBaseClasses(std::list<std::string> & classList)
    {        
        entityCtorData.baseClasses = classList; 
        entityCtorData.flags |= FL_SET_BASE_CLASSES;
    }

    void SetPropertyExtra(std::string property, float value)
    {
		//Con_Printf("SetPropertyExtra(%s, %f)\n", property.c_str(), value);

//         assert(current_entity);
//         current_entity->SetPropertyExtra(property, value);
    }

    void SetBboxOffset(float x, float y, float z)
    {
        entityCtorData.offset.x = x;
        entityCtorData.offset.y = y;
        entityCtorData.offset.z = z;
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
%token	Offset "offset"
%token  Sky "sky"
%token  TargetSource "target_source"
%token  TargetDestination "target_destination"
%token  Sound "sound"

%token  EndOfFile 0
%token	Comment

%type<float>        Number
%type<std::string>  Identifier StringLiteral TypeId Choices ModelType StringLiteralOpt Flags
%type<std::string>  PropertyHelpOpt

%type<OptionalDefaultValAndHelp_s> OptionalDefaultValAndHelp

%type<std::string> IntegerType Color255 String Sprite Studio TargetDestination TargetSource Sound Model Sky  

%type<GoldSource::FGDFlagsValue_t> FlagValue EnumValue
%type<GoldSource::FGDFlagsList> FlagsValues EnumValues

%type<GoldSource::FGDPropertyDescriptor*> ClassFieldDef
%type<GoldSource::FGDPropertiesList> ClassFieldsOpt

%type<GoldSource::FGDEntityClass*> EntityClassDef SolidClassDef PointClassDef BaseClassDef

%type<std::list<std::string>> identifierListOpt




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
SolidClassDef: SolidClass CtorsOpt EqualsSign Identifier Colon StringLiteral ExtraNotesOpt OpeningBracket ClassFieldsOpt ClosingBracket { $$ = ctx->new_entity(FGDEntityClassType::Solid,$4, $6, $9); };

// Точечная энтити
PointClassDef: PointClass CtorsOpt EqualsSign Identifier Colon StringLiteral ExtraNotesOpt OpeningBracket ClassFieldsOpt ClosingBracket { $$ = ctx->new_entity(FGDEntityClassType::Point,$4, $6, $9); };

// Базовый класс
BaseClassDef: BaseClass CtorsOpt EqualsSign Identifier ExtraNotesOpt OpeningBracket ClassFieldsOpt ClosingBracket { $$ = ctx->new_entity(FGDEntityClassType::BaseDef,$4, "<none>", $7); };

// Дополнительное пояснение (найдено в FGD идущих с Jackhammer'ом)
ExtraNotesOpt: Colon StringLiteral
|%empty;



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
| ColorDefOpt
| OffsetDefOpt
| FlagsDefOpt;


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

// Флаги? TODO
FlagsDefOpt: Flags OpeningParenthesis Identifier ClosingParenthesis { ctx->SetPropertyExtra($1,0); }

// Смещение ббокс'а
OffsetDefOpt: Offset OpeningParenthesis Number Number Number ClosingParenthesis { ctx->SetBboxOffset($3, $4, $5); }


ExtendedConstructor: Identifier OpeningParenthesis Number ClosingParenthesis { ctx->SetPropertyExtra($1,$3); }

// Ссылка на базовый класс
BaseClassRefOpt: BaseDef OpeningParenthesis identifierListOpt ClosingParenthesis { ctx->SetBaseClasses($3); }

StringLiteralOpt: StringLiteral { $$ = $1;}
|%empty { $$ = ""; };

//identifierListOpt: identifierList Identifier  
//|%empty;

identifierListOpt: identifierListOpt Comma Identifier { $1.push_back($3); $$ = $1; }
|Identifier { $$.push_back($1); };
|%empty { (void)0; };

// Список свойств энтити
ClassFieldsOpt: ClassFieldsOpt ClassFieldDef  { $1.push_back($2); $$ = $1;}
| %empty { (void)0; };



// Описание свойства энтити
ClassFieldDef: 
// Обычные поля
Identifier OpeningParenthesis TypeId ClosingParenthesis Colon StringLiteral OptionalDefaultValAndHelp { $$ = new FGDPropertyDescriptor($1, $3, $6, $7); };
// Флаги
| Identifier OpeningParenthesis Flags ClosingParenthesis EqualsSign OpeningBracket FlagsValues ClosingBracket { $$ = new FGDFlagsEnumProperty($1,"<spawnflags>",$7, OptionalDefaultValAndHelp_s(0,"")); };
// Перечисление
| Identifier OpeningParenthesis Choices ClosingParenthesis Colon StringLiteral OptionalDefaultValAndHelp EqualsSign OpeningBracket EnumValues ClosingBracket { $$ = new FGDFlagsEnumProperty($1,$6,$10, $7); };

// Особый случай для поля "model"
| Model OpeningParenthesis ModelType ClosingParenthesis Colon StringLiteral OptionalDefaultValAndHelp { $$ = new FGDPropertyDescriptor("model", $3, $6, $7); };

// Особый случай для поля "color"
| Color OpeningParenthesis Choices ClosingParenthesis Colon StringLiteral OptionalDefaultValAndHelp EqualsSign OpeningBracket EnumValues ClosingBracket {$$ = new FGDPropertyDescriptor("color", $3, $6, $7); };
| Color OpeningParenthesis TypeId ClosingParenthesis Colon StringLiteral  OptionalDefaultValAndHelp { $$ = new FGDPropertyDescriptor("color", $3, $6, $7); };

// Особый случай для поля "sound"
| Sound OpeningParenthesis Choices ClosingParenthesis Colon StringLiteral OptionalDefaultValAndHelp EqualsSign OpeningBracket EnumValues ClosingBracket { $$ = new FGDPropertyDescriptor("sound", $3, $6, $7); };
| Sound OpeningParenthesis TypeId ClosingParenthesis Colon StringLiteral  OptionalDefaultValAndHelp  { $$ = new FGDPropertyDescriptor("sound", $3, $6, $7); };


// Особый случай для поля "size"
| SizeBoundingBox OpeningParenthesis Choices ClosingParenthesis Colon StringLiteral OptionalDefaultValAndHelp EqualsSign OpeningBracket EnumValues ClosingBracket { $$ = new FGDFlagsEnumProperty("size", $6, $10, $7); };

// Особый случай для поля "texture(decal)"
| Identifier OpeningParenthesis Decal ClosingParenthesis { $$ = new FGDPropertyDescriptor("decal", "decal", "decal", OptionalDefaultValAndHelp_s(0,"")); };



ModelType: Studio
| Sprite;

// Значения флагов
FlagsValues: FlagsValues FlagValue { $1.push_back($2); $$ = $1;}
|%empty { (void)0; };


// Значение флага : Описание : Включен по умолчанию [ : ВключенПоУмолчанию : Подсказка ]
// $3 - Краткое описание
// $1 - Значение
// $5 - Включен по умолчанию
FlagValue:  Number Colon StringLiteral Colon Number OptionalDefaultValAndHelp { $$ = FGDFlagsValue_t($3, $1, $5, $6.propertyHelp); };

// Значения перечислений
// $1 - Список значений перечисления (std::list<FGDFlagsValue_t>)
// $2 - Значение перечисления (FGDFlagsValue_t)
EnumValues: EnumValues EnumValue  { $1.push_back($2); $$ = $1;}
|%empty { (void)0;};

// Значение  : Описание
// $3 - Краткое описание
// $1 - Значение
// $4 - Подсказка (JackHammer)
EnumValue:  Number Colon StringLiteral PropertyHelpOpt { $$ = FGDFlagsValue_t($3, $1, true, $4); };



// Типы свойств энтити
TypeId: IntegerType
| Color255
| String
| Sprite
| Studio
| Sky
| TargetDestination
| TargetSource
| Sound;


// Optional default value and help block
OptionalDefaultValAndHelp: Colon Number Colon StringLiteral { $$ = OptionalDefaultValAndHelp_s($2, $4); }
| Colon StringLiteral Colon StringLiteral { $$ = OptionalDefaultValAndHelp_s($2, $4); }
| Colon Colon StringLiteral { $$ = OptionalDefaultValAndHelp_s(0, $3); }
| Colon Number { $$ = OptionalDefaultValAndHelp_s($2,""); }
| Colon StringLiteral { $$ = OptionalDefaultValAndHelp_s($2,""); }
|%empty { (void)0; };

/*
// Значение по умолчанию для энтити
DefaultValueOpt: Colon StringLiteral { $$ = $2;}
| Colon Number { $$ = std::format("{0}", $2); }
| Colon { $$ = ""; }
|%empty { (void)0; };
*/

// Описание свойства (Jackhammer)
PropertyHelpOpt: Colon StringLiteral { $$ = $2; }
| %empty { (void)0;};

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
        'offset'                    {                     return s(yy::HammerFGDParser::make_Offset) ; }        
        'sky'                       {                     return s(yy::HammerFGDParser::make_Sky, std::string(anchor, ctx->cursor)) ; }        
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






