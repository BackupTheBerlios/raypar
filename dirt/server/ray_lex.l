/**********************************************************
//** ray_lex.l **
// Created By: KIRILL
// On: 1/18/2004 18:21:24
// Comments: 3D scene syntax analizer based on flex
//
//*********************************************************
// REVISION by ..., on ...
// Comments: ...
//
//********************************************************* */

%{
#include "stdafx.h"


#include "common/ParserVar.h"

//let it live here for a while
#define YYSTYPE CVar 
#include "common/SceneBuilder.h"
#include "LexParser.cpp.h"

//extern CVar yylval;
extern CSceneBuilder* glb_scene_builder;


%}



/*%option noyywrap*/

%x COMMENT

digit    [0-9]
real    [+-]?{digit}+("."{digit}+)?
ws        [ \t\n]*

%%

{ws}+         ; /* skip spaces and new line simbols  ((\-\-)/[^\-])**/

"/"+"*"                 BEGIN(COMMENT);
<COMMENT>[^*\n]*        { glb_scene_builder->AddComment(yytext); /* eat anything that's not a '*' */ } 
<COMMENT>"*"+[^*/\n]*   { glb_scene_builder->AddComment(yytext); /* eat up '*'s not followed by '/'s */ }
<COMMENT>\n             /* ++line_num; */
<COMMENT>"*"+"/"        BEGIN(INITIAL);


"("    {  return '(';  }  
")"    {  return ')';  }
","    {  return ',';  }
";"    {  return ';';  }

    /* "<"    {  return '<';  } */
    /* ">"    {  return '>';  } */
    /* "="    {  return '=';  } */
    /* "/"    {  return '/';  } */

"["    {  return '[';  }
"]"    {  return ']';  }



SPHERE    return(IDSPHERE);
LIGHT     return(IDLIGHT);


{real}  {
    yylval.SetDouble( atof(yytext) );
    return(REAL);
  }

.   {   /* unknown sentence, error */
       static char err_buf[1024];
       sprintf( err_buf, "Unknown sentence \"%s\"!\n", yytext );
       glb_scene_builder->ParserError( err_buf );
    }

%%
