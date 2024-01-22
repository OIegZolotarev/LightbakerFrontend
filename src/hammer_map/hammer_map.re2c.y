/* Generated by re2c 1.3 on Tue Jan 23 00:44:17 2024 */
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

    
{
	char yych;
	unsigned int yyaccept = 0;
	yych = *ctx->cursor;
	switch (yych) {
	case 0x00:	goto yy3;
	case 0x08:
	case '\t':
	case '\v':
	case '\f':
	case ' ':	goto yy5;
	case '\n':	goto yy7;
	case '\r':	goto yy9;
	case '!':
	case '+':	goto yy10;
	case '"':	goto yy12;
	case '(':	goto yy14;
	case ')':	goto yy16;
	case '-':	goto yy18;
	case '.':	goto yy21;
	case '/':	goto yy23;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy24;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy26;
	case '[':	goto yy29;
	case ']':	goto yy31;
	case '{':	goto yy33;
	case '}':	goto yy35;
	case '~':	goto yy37;
	default:	goto yy2;
	}
yy2:
	ctx->cursor = marker;
	switch (yyaccept) {
	case 0: 	goto yy20;
	case 1: 	goto yy34;
	default:	goto yy28;
	}
yy3:
	++ctx->cursor;
	{                     return s(yy::HammerMapParser::make_EndOfFile); }
yy5:
	++ctx->cursor;
	{ ctx->loc.columns(); return yylex(ctx); }
yy7:
	++ctx->cursor;
yy8:
	{ ctx->loc.lines();   return yylex(ctx); }
yy9:
	yych = *++ctx->cursor;
	switch (yych) {
	case '\n':	goto yy7;
	default:	goto yy8;
	}
yy10:
	yych = *++ctx->cursor;
yy11:
	switch (yych) {
	case '!':
	case '+':
	case '-':
	case '{':	goto yy10;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy26;
	case '~':	goto yy37;
	default:	goto yy2;
	}
yy12:
	yych = *++ctx->cursor;
	switch (yych) {
	case '"':	goto yy39;
	default:	goto yy12;
	}
yy14:
	++ctx->cursor;
	{                     return s(yy::HammerMapParser::make_OpeningRoundBracket); }
yy16:
	++ctx->cursor;
	{                     return s(yy::HammerMapParser::make_ClosingRoundBracket); }
yy18:
	yyaccept = 0;
	yych = *(marker = ++ctx->cursor);
	switch (yych) {
	case '!':
	case '+':
	case '{':	goto yy10;
	case '-':	goto yy18;
	case '.':	goto yy21;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy24;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy26;
	case '~':	goto yy37;
	default:	goto yy20;
	}
yy20:
	{                     return s(yy::HammerMapParser::make_Number, std::stol(std::string(anchor,ctx->cursor))); }
yy21:
	yych = *++ctx->cursor;
	switch (yych) {
	case '-':
	case '.':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy21;
	default:	goto yy20;
	}
yy23:
	yych = *++ctx->cursor;
	switch (yych) {
	case '/':	goto yy41;
	default:	goto yy2;
	}
yy24:
	yych = *++ctx->cursor;
	switch (yych) {
	case '-':
	case '.':	goto yy21;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy24;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':
	case '~':	goto yy26;
	default:	goto yy20;
	}
yy26:
	yych = *++ctx->cursor;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':
	case '~':	goto yy26;
	default:	goto yy28;
	}
yy28:
	{                     return s(yy::HammerMapParser::make_TextureName, std::string(anchor, ctx->cursor)); }
yy29:
	++ctx->cursor;
	{                     return s(yy::HammerMapParser::make_OpeningSquareBracket); }
yy31:
	++ctx->cursor;
	{                     return s(yy::HammerMapParser::make_ClosingSquareBracket); }
yy33:
	yyaccept = 1;
	yych = *(marker = ++ctx->cursor);
	switch (yych) {
	case '!':
	case '+':
	case '-':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':
	case '{':
	case '~':	goto yy11;
	default:	goto yy34;
	}
yy34:
	{                     return s(yy::HammerMapParser::make_OpeningCurlyBracket); }
yy35:
	++ctx->cursor;
	{                     return s(yy::HammerMapParser::make_ClosingCurlyBracket); }
yy37:
	yyaccept = 2;
	yych = *(marker = ++ctx->cursor);
	switch (yych) {
	case '!':
	case '+':
	case '-':
	case '{':	goto yy10;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy26;
	case '~':	goto yy37;
	default:	goto yy28;
	}
yy39:
	++ctx->cursor;
	{                     return s(yy::HammerMapParser::make_StringLiteral, std::string(anchor+1, ctx->cursor-1)); }
yy41:
	yych = *++ctx->cursor;
	switch (yych) {
	case '\n':
	case '\r':	goto yy43;
	default:	goto yy41;
	}
yy43:
	{                     return yylex(ctx); }
}

		
}

void yy::HammerMapParser::error(const location_type& l,const std::string& m)
{
    Con_Printf("[%s: line: %d, column: %d]%s\n",l.begin.filename, l.begin.line, l.begin.column ,m.c_str());
}






