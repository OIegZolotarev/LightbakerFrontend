%language "c++"
%skeleton "lalr1.cc"

%debug
%no-lines
%define api.parser.class {HammerMapParser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define parse.error verbose
%locations   

%param { HammerMapParsingContext * ctx }//%param

%code
{

struct HammerMapParsingContext
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

%token	StringLiteral Identifier TextureName Number

%token	OpeningCurlyBracket "{"
%token	ClosingCurlyBracket "}"

%token	OpeningRoundBracket "("
%token	ClosingRoundBracket ")"

%token	OpeningSquareBracket "["
%token	ClosingSquareBracket "]"

%token  EndOfFile 0

%type<float>        Number
%type<std::string>  StringLiteral TextureName Identifier

%%

// Общее описание файла карты
Map: entitiesOpt;


// Описание всех энтитей - 0 или больше энтитей
entitiesOpt: entitiesOpt entityDef
|%empty;


// Описание энтити 
entityDef: OpeningCurlyBracket entityPropertiesOpt brushDefOpt ClosingCurlyBracket;


entityPropertiesOpt: entityPropertiesOpt entityProperty
|%empty;

entityProperty: StringLiteral StringLiteral

brushDefOpt: OpeningCurlyBracket brushFaceRecursive ClosingCurlyBracket
|%empty;

brushFaceRecursive: brushFaceRecursive brushFace
|%empty;

brushFace: PlanePoint PlanePoint PlanePoint TextureName TextureVector TextureVector Number Number Number;

PlanePoint: OpeningRoundBracket Number Number Number ClosingRoundBracket;
TextureVector: OpeningSquareBracket Number Number Number Number ClosingSquareBracket;

%%


yy::HammerMapParser::symbol_type yy::yylex(FGDParsingContext *  ctx)
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


		
        
        
		 '\x00' { return s(yy::HammerMapParser::make_EndOfFile); }
        [-0-9]+                     {                     return s(yy::HammerFGDParser::make_Number, std::stol(std::string(anchor,ctx->cursor))); }
        '('                         {                     return s(yy::HammerFGDParser::make_OpeningRoundBracket); }
        ')'                         {                     return s(yy::HammerFGDParser::make_ClosingRoundBracket); }
        '['                         {                     return s(yy::HammerFGDParser::make_OpeningSquareBracket); }
        ']'                         {                     return s(yy::HammerFGDParser::make_ClosingSquareBracket); }       
								"\"" [^"]* "\""             {                     return s(yy::HammerFGDParser::make_StringLiteral, std::string(anchor+1, ctx->cursor-1)); } 
        [a-zA-Z_]+[0-9]*[a-zA-Z_]*  {                     return s(yy::HammerFGDParser::make_Identifier, std::string(anchor, ctx->cursor)); }
        "\r\n" | [\r\n]             { ctx->loc.lines();   return yylex(ctx); }
        "//" [^\r\n]*               {                     return yylex(ctx); }
        [\t\v\b\f ]                 { ctx->loc.columns(); return yylex(ctx); }        
    */
		
}

void yy::HammerMapParser::error(const location_type& l,const std::string& m)
{
    Con_Printf("%s\n",m.c_str());
}






