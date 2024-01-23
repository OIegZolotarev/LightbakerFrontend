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
    int progress = -10;
    size_t length = 0;
    const char* start;

    const char* cursor;
    yy::location loc;

    HammerMap * m_pMap;
    
    HammerMapParsingContext(HammerMap* file)
    {
       m_pMap = file;
       cursor = file->Data();
       length = file->Length();
       start = cursor;
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

    
    // parser.set_debug_level(999);

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

%token	StringLiteral TextureName Number

%token	OpeningCurlyBracket "{"
%token	ClosingCurlyBracket "}"

%token	OpeningRoundBracket "("
%token	ClosingRoundBracket ")"

%token	OpeningSquareBracket "["
%token	ClosingSquareBracket "]"

%token  EndOfFile 0

%type<float>        Number
%type<std::string>  StringLiteral TextureName 
%type<glm::vec3> PlanePoint
%type<glm::vec4> TextureVector
%type<map220face_t*> brushFace

%type<map220brush_t*>  brushDef
%type<map220keyvalue_t*>  entityProperty





%%

// Общее описание файла карты
Map: entitiesOpt { (void)0;}



// Описание всех энтитей - 0 или больше энтитей
entitiesOpt: entitiesOpt entityDef 
|%empty ;

// Описание энтити 
entityDef: OpeningCurlyBracket entityPropertiesOpt brushListOpt ClosingCurlyBracket {  ctx->m_pMap->AddNewEntity();  }

entityPropertiesOpt: entityPropertiesOpt entityProperty 
|%empty;

entityProperty: StringLiteral StringLiteral { ctx->m_pMap->MakeNewKeyValue($1, $2); }

brushListOpt: brushListOpt brushDef
|%empty { (void)0; }; 

// brushDef: OpeningCurlyBracket brushFaceList ClosingCurlyBracket { map220brush_t * b = new map220brush_s; b->faces = $2; $$ = b; }
brushDef: OpeningCurlyBracket brushFaceList ClosingCurlyBracket { $$ = ctx->m_pMap->MakeNewBrush(); }

brushFaceList: brushFaceList brushFace
|%empty;

brushFace: PlanePoint PlanePoint PlanePoint TextureName TextureVector TextureVector Number Number Number { $$ =  ctx->m_pMap->MakeNewFace($1,$2,$3,$4,$5,$6,$7, glm::vec2($8,$9)); }

PlanePoint: OpeningRoundBracket Number Number Number ClosingRoundBracket { $$ = glm::vec3($2,$3,$4); }
TextureVector: OpeningSquareBracket Number Number Number Number ClosingSquareBracket { $$ = glm::vec4($2,$3,$4,$5); }

%%


yy::HammerMapParser::symbol_type yy::yylex(HammerMapParsingContext *  ctx)
{
    const char* marker = ctx->cursor;
    const char* anchor = ctx->cursor;
    ctx->loc.step();
        
    
    size_t offset = ctx->cursor - ctx->start;
    float currentProgress = offset / (float)ctx->length * 100.f;

    if ((ctx->progress + 1 <= currentProgress))
    {
        printf("\rLoading MAP %.3f", currentProgress);
        fflush(stdout);
        ctx->progress = currentProgress;
    }

    auto s = [&](auto func, auto&&... params) 
    { 
        ctx->loc.columns(ctx->cursor - anchor); 
        return func(params...,  ctx->loc); 
    };

    /*!re2c        		
		re2c:yyfill:enable = 0;
        re2c:define:YYCTYPE = char;
		re2c:define:YYCURSOR = "ctx->cursor";
        re2c:define:YYMARKER = marker;
        

		
        
        
		'\x00'                      {                     return s(yy::HammerMapParser::make_EndOfFile); }
        [-0-9\.]+                   {                     return s(yy::HammerMapParser::make_Number, std::stol(std::string(anchor,ctx->cursor))); }
        '('                         {                     return s(yy::HammerMapParser::make_OpeningRoundBracket); }
        ')'                         {                     return s(yy::HammerMapParser::make_ClosingRoundBracket); }
        '{'                         {                     return s(yy::HammerMapParser::make_OpeningCurlyBracket); }
        '}'                         {                     return s(yy::HammerMapParser::make_ClosingCurlyBracket); }
        '['                         {                     return s(yy::HammerMapParser::make_OpeningSquareBracket); }
        ']'                         {                     return s(yy::HammerMapParser::make_ClosingSquareBracket); }       
        "\"" [^"]* "\""             {                     return s(yy::HammerMapParser::make_StringLiteral, std::string(anchor+1, ctx->cursor-1)); } 
        // [{!~\-\+]*[a-z]+[a-zA-Z_0-9]+     {                     return s(yy::HammerMapParser::make_TextureName, std::string(anchor, ctx->cursor)); }
        [{!~\-\+]*[~a-zA-Z_0-9]+              {                     return s(yy::HammerMapParser::make_TextureName, std::string(anchor, ctx->cursor)); }
        "\r\n" | [\r\n]             { ctx->loc.lines();   return yylex(ctx); }
        "//" [^\r\n]*               {                     return yylex(ctx); }
        [\t\v\b\f ]                 { ctx->loc.columns(); return yylex(ctx); }        
    */
		
}

void yy::HammerMapParser::error(const location_type& l,const std::string& m)
{
    Con_Printf("[%s: line: %d, column: %d]%s\n",l.begin.filename, l.begin.line, l.begin.column ,m.c_str());
}






