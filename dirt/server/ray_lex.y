%{
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include "common/ParserVar.h"
#include "common/SceneBuilder.h"

#define YYSTYPE CVar
#define YYDEBUG 1

void* alloca(int size){ return new BYTE[size]; }



extern int yylex();
extern CSceneBuilder* glb_scene_builder;

extern "C" int yywrap(void)
{ 
  return 1; 
}

void yyerror(const char* str_err)
{  
  glb_scene_builder->ParserError( str_err );
}



%}

%start start
%token REAL, IDSPHERE, IDLIGHT

%%

start : expr

expr :
 |expr sphere 
 |expr light 
 ;


sphere : IDSPHERE '(' vector ','  REAL ')' ';' 
 {    
    glb_scene_builder->AddSphere( $3.GetVector(), $5.GetDouble() );  
 }
;

light : IDLIGHT '(' vector ','  vector ')' ';' 
 {    
    glb_scene_builder->AddLight( $3.GetVector(), $5.GetVector() );  
 }
;


vector : '[' REAL ',' REAL ',' REAL ']'
 {
  $$.SetVector( CVector( $2.GetDouble(), $4.GetDouble(), $6.GetDouble() ) );  
 }
;



%%


// WWWWWW
