%{
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include "common/ParserVar.h"
#include "ServerSceneBuilder.h"

#define YYSTYPE CVar
#define YYDEBUG 1

void* alloca(int size){ return malloc(size); }



extern int yylex();
extern CServerSceneBuilder* glb_scene_builder;

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
%token dlm, REAL, IDSPHERE, IDLIGHT, IDSETAMBIENTCOLOR, IDPLANE, IDTRIANGLE
%token IDCYLINDER, IDBOX

%%

start : expr

expr :
 |expr sphere 
 |expr light 
 |expr setambientcolor
 |expr plane
 |expr box 
 |expr triangle
 |expr cylinder
 ;


sphere : IDSPHERE '(' VECTOR  dlm REAL dlm VECTOR ')' ';' 
 {    
    glb_scene_builder->AddSphere( $3.GetVector(), $5.GetDouble(), $7.GetVector() );  
 }
;

light : IDLIGHT '(' VECTOR dlm  VECTOR ')' ';' 
 {    
    glb_scene_builder->AddLight( $3.GetVector(), $5.GetVector() );  
 }
;

setambientcolor: IDSETAMBIENTCOLOR '(' VECTOR  ')' ';' 
 {    
    glb_scene_builder->SetAmbientColor( $3.GetVector() );  
 }
;

plane : IDPLANE '(' VECTOR  dlm REAL dlm VECTOR ')' ';' 
 {    
    glb_scene_builder->AddPlane( $3.GetVector(), $5.GetDouble(), $7.GetVector() );  
 }
;

box : IDBOX '(' VECTOR dlm VECTOR dlm VECTOR dlm VECTOR dlm VECTOR ')' ';'
 {
    glb_scene_builder->AddBox( $3, $5, $7, $9, $11 );
 }
;

triangle : IDTRIANGLE '(' VECTOR dlm VECTOR dlm VECTOR dlm VECTOR ')' ';'
 {
    glb_scene_builder->AddTriangle( $3, $5, $7, $9 );
 }

cylinder : IDCYLINDER '(' VECTOR dlm VECTOR dlm REAL dlm REAL dlm VECTOR ')' ';'
 {
    glb_scene_builder->AddCylinder( $3, $5, $7, $9, $11 );
 } 
 

VECTOR : '[' REAL dlm REAL dlm REAL ']'
 {
  $$.SetVector( CVector( $2.GetDouble(), $4.GetDouble(), $6.GetDouble() ) );  
 }
;



%%



