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

    HammerMap * m_pMap;
    
    HammerMapParsingContext(HammerMap* file)
    {
       m_pMap = file;
       cursor = file->Data();
    }



};

namespace yy { HammerMapParser::symbol_type yylex(HammerMapParsingContext* ctx); }


void ParseMap(HammerMap * file)
{

    HammerMapParsingContext ctx(file);    

    auto fileName = file->FileName();

    ctx.loc.begin.filename = &fileName;
    ctx.loc.end.filename   = &fileName;

    yy::HammerMapParser parser(&ctx);

    
    //parser.set_debug_level(999);

    parser.parse();  
}

}

%code requires
{

#include "..\application.h"
#include "..\common.h"
#include "..\hammer_map.h"

struct HammerMapParsingContext;

#pragma warning(disable: 4065)

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
%type<glm::vec3> PlanePoint
%type<glm::vec4> TextureVector
%type<map220face_t> brushFace

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

brushDefOpt: OpeningCurlyBracket brushFaceList ClosingCurlyBracket
|%empty;

brushFaceList: brushFaceList brushFace
|%empty;

brushFace: PlanePoint PlanePoint PlanePoint TextureName TextureVector TextureVector Number Number Number { $$ = map220face_t($1,$2,$3,$4,$5,$6,$7, glm::vec2($8,$9)); }

PlanePoint: OpeningRoundBracket Number Number Number ClosingRoundBracket { $$ = glm::vec3($2,$3,$4); }
TextureVector: OpeningSquareBracket Number Number Number Number ClosingSquareBracket { $$ = glm::vec4($2,$3,$4,$5); }

%%


yy::HammerMapParser::symbol_type yy::yylex(HammerMapParsingContext *  ctx)
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


		
        
        
		 '\x00'                     {                     return s(yy::HammerMapParser::make_EndOfFile); }
        [-0-9]+                     {                     return s(yy::HammerMapParser::make_Number, std::stol(std::string(anchor,ctx->cursor))); }
        '('                         {                     return s(yy::HammerMapParser::make_OpeningRoundBracket); }
        ')'                         {                     return s(yy::HammerMapParser::make_ClosingRoundBracket); }
        '['                         {                     return s(yy::HammerMapParser::make_OpeningSquareBracket); }
        ']'                         {                     return s(yy::HammerMapParser::make_ClosingSquareBracket); }       
        "\"" [^"]* "\""             {                     return s(yy::HammerMapParser::make_StringLiteral, std::string(anchor+1, ctx->cursor-1)); } 
        [a-zA-Z_]+[0-9]*[a-zA-Z_]*  {                     return s(yy::HammerMapParser::make_Identifier, std::string(anchor, ctx->cursor)); }
        "\r\n" | [\r\n]             { ctx->loc.lines();   return yylex(ctx); }
        "//" [^\r\n]*               {                     return yylex(ctx); }
        [\t\v\b\f ]                 { ctx->loc.columns(); return yylex(ctx); }        
    */
		
}

void yy::HammerMapParser::error(const location_type& l,const std::string& m)
{
    Con_Printf("%s\n",m.c_str());
}






