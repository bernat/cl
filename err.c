/*
 * A n t l r  S e t s / E r r o r  F i l e  H e a d e r
 *
 * Generated from: cl.g
 *
 * Terence Parr, Russell Quong, Will Cohen, and Hank Dietz: 1989-2001
 * Parr Research Corporation
 * with Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 */

#define ANTLR_VERSION	13333
#include "pcctscfg.h"
#include "pccts_stdio.h"

#include <string>
#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <fstream>

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include "ptype.hh"
#include "symtab.hh"
#include "codegest.hh"

/// struct to contain information about a token.
typedef struct {
  string kind;
  string text;
  int line;
} Attrib;

/// function called by the scanner when a new token is found
/// Predeclared here, definition below.
void zzcr_attr(Attrib *attr,int type,char *text);

/// Include AST node fields declaration
#include "myASTnode.hh"

/// Macro called by the parser when a new AST node is to be created
/// createASTnode function doing the actual job is defined below.
#define zzcr_ast(as,attr,tttype,textt) as=createASTnode(attr,tttype,textt);
AST* createASTnode(Attrib* attr, int ttype, char *textt);
#define zzSET_SIZE 8
#include "antlr.h"
#include "ast.h"
#include "tokens.h"
#include "dlgdef.h"
#include "err.h"

ANTLRChar *zztokens[35]={
	/* 00 */	"Invalid",
	/* 01 */	"INPUTEND",
	/* 02 */	"PROGRAM",
	/* 03 */	"ENDPROGRAM",
	/* 04 */	"VARS",
	/* 05 */	"ENDVARS",
	/* 06 */	"INT",
	/* 07 */	"STRUCT",
	/* 08 */	"ENDSTRUCT",
	/* 09 */	"WRITELN",
	/* 10 */	"PLUS",
	/* 11 */	"OPENPAR",
	/* 12 */	"CLOSEPAR",
	/* 13 */	"ASIG",
	/* 14 */	"IF",
	/* 15 */	"THEN",
	/* 16 */	"ELSE",
	/* 17 */	"ENDIF",
	/* 18 */	"WHILE",
	/* 19 */	"DO",
	/* 20 */	"ENDWHILE",
	/* 21 */	"AND",
	/* 22 */	"OR",
	/* 23 */	"DOT",
	/* 24 */	"IDENT",
	/* 25 */	"INTCONST",
	/* 26 */	"COMMENT",
	/* 27 */	"WHITESPACE",
	/* 28 */	"NEWLINE",
	/* 29 */	"LEXICALERROR",
	/* 30 */	"PROCEDURE",
	/* 31 */	"ENDPROCEDURE",
	/* 32 */	"FUNCTION",
	/* 33 */	"ENDFUNCTION",
	/* 34 */	"STRING"
};
SetWordType zzerr1[8] = {0x18,0x42,0x4,0x41, 0x1,0x0,0x0,0x0};
SetWordType zzerr2[8] = {0x0,0x0,0x0,0x40, 0x1,0x0,0x0,0x0};
SetWordType zzerr3[8] = {0xc0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0};
SetWordType setwd1[35] = {0x0,0x1,0x0,0xc6,0x0,0x18,0x0,
	0x0,0x0,0xc6,0x0,0x0,0x0,0x0,0xc6,
	0x0,0x0,0x0,0xc6,0x0,0x0,0x0,0x0,
	0x0,0xd6,0x0,0x0,0x0,0x0,0x0,0xa6,
	0x0,0xa6,0x0,0x0};
SetWordType zzerr4[8] = {0x0,0x0,0x0,0x3, 0x4,0x0,0x0,0x0};
SetWordType zzerr5[8] = {0x0,0x0,0x3,0x0, 0x0,0x0,0x0,0x0};
SetWordType zzerr6[8] = {0x0,0x42,0x4,0x1, 0x0,0x0,0x0,0x0};
SetWordType zzerr7[8] = {0x0,0x0,0x0,0x3, 0x0,0x0,0x0,0x0};
SetWordType setwd2[35] = {0x0,0x0,0x0,0xe8,0x0,0x1,0x0,
	0x0,0x3,0xe4,0x80,0x0,0xc0,0x0,0xe4,
	0xc0,0xe8,0xe8,0xe4,0xc0,0xe8,0x0,0x0,
	0x0,0xf7,0x10,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0};
