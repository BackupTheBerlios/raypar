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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "common/ParserVar.h"

//let it live here for a while
#define YYSTYPE CVar 
#include "ServerSceneBuilder.h"
#include "LexParser.cpp.h"
extern CServerSceneBuilder* glb_scene_builder;


%}



/*%option noyywrap*/

%x COMMENT

digit    [0-9]
real    [+-]?{digit}+("."{digit}+)?
ws        [ \t]*

%%

{ws}+         ; /* skip spaces and new line simbols  ((\-\-)/[^\-])**/
\n			  glb_scene_builder->IncreaseLineNumber(); /* ++line_num; */

"/"+"*"                 BEGIN(COMMENT);
<COMMENT>[^*\n]*        { glb_scene_builder->AddComment(yytext); /* eat anything that's not a '*' */ } 
<COMMENT>"*"+[^*/\n]*   { glb_scene_builder->AddComment(yytext); /* eat up '*'s not followed by '/'s */ }
<COMMENT>\n             glb_scene_builder->IncreaseLineNumber(); /* ++line_num; */
<COMMENT>"*"+"/"        BEGIN(INITIAL);


"("    {  return '(';  }  
")"    {  return ')';  }
","    {  return dlm;  }
";"    {  return ';';  }

"["    {  return '[';  }
"]"    {  return ']';  }

"{"    {  return '{';  }
"}"    {  return '}';  }



Sphere			 return ( IDSPHERE );
Light			 return ( IDLIGHT );
SetAmbientColor  return ( IDSETAMBIENTCOLOR );
Plane		     return ( IDPLANE ); 
Triangle		 return ( IDTRIANGLE );
Cylinder		 return ( IDCYLINDER );
Box			     return ( IDBOX );
Render			 return ( IDRENDER );


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
