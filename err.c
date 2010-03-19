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

ANTLRChar *zztokens[52]={
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
	/* 14 */	"VAR",
	/* 15 */	"BOOL",
	/* 16 */	"STRING",
	/* 17 */	"ARRAY",
	/* 18 */	"OF",
	/* 19 */	"IF",
	/* 20 */	"THEN",
	/* 21 */	"ELSE",
	/* 22 */	"ENDIF",
	/* 23 */	"WHILE",
	/* 24 */	"DO",
	/* 25 */	"ENDWHILE",
	/* 26 */	"AND",
	/* 27 */	"OR",
	/* 28 */	"NOT",
	/* 29 */	"MINUS",
	/* 30 */	"TIMES",
	/* 31 */	"DIV",
	/* 32 */	"EQUAL",
	/* 33 */	"GTHAN",
	/* 34 */	"LTHAN",
	/* 35 */	"OPENCLAU",
	/* 36 */	"CLOSECLAU",
	/* 37 */	"REF",
	/* 38 */	"VAL",
	/* 39 */	"COMA",
	/* 40 */	"DOT",
	/* 41 */	"IDENT",
	/* 42 */	"INTCONST",
	/* 43 */	"COMMENT",
	/* 44 */	"WHITESPACE",
	/* 45 */	"NEWLINE",
	/* 46 */	"LEXICALERROR",
	/* 47 */	"PROCEDURE",
	/* 48 */	"ENDPROCEDURE",
	/* 49 */	"FUNCTION",
	/* 50 */	"RETURN",
	/* 51 */	"ENDFUNCTION"
};
SetWordType zzerr1[8] = {0x18,0x42,0x88,0x0, 0x0,0x82,0x7,0x0};
SetWordType zzerr2[8] = {0x0,0x0,0x0,0x0, 0x0,0x80,0x2,0x0};
SetWordType setwd1[52] = {0x0,0x1,0x0,0xc6,0x0,0x18,0x0,
	0x0,0x0,0xc6,0x0,0x0,0x0,0x0,0xc6,
	0x0,0x0,0x0,0x0,0xc6,0x0,0x0,0x0,
	0xc6,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0xd6,0x0,0x0,0x0,0x0,0x0,
	0xa6,0xc6,0xa6,0xc6,0x0};
SetWordType zzerr3[8] = {0x0,0x10,0x0,0x0, 0x60,0x0,0x0,0x0};
SetWordType zzerr4[8] = {0x0,0x0,0x0,0x0, 0x60,0x0,0x0,0x0};
SetWordType zzerr5[8] = {0xc0,0x80,0x2,0x0, 0x0,0x0,0x0,0x0};
SetWordType setwd2[52] = {0x0,0x0,0x0,0x20,0x2b,0x20,0x0,
	0x0,0x60,0xab,0x0,0x0,0x30,0x0,0xab,
	0x0,0x0,0x0,0x0,0xab,0x0,0x20,0x20,
	0xab,0x0,0x20,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x4,0x4,
	0x30,0x0,0xeb,0x0,0x0,0x0,0x0,0x0,
	0x2b,0x29,0x2b,0x2a,0x0};
SetWordType zzerr6[8] = {0xc0,0xa0,0x2,0x0, 0x0,0x0,0x0,0x0};
SetWordType zzerr7[8] = {0x0,0x8,0x1,0x30, 0x0,0x6,0x0,0x0};
SetWordType zzerr8[8] = {0x0,0x0,0x60,0x0, 0x0,0x0,0x0,0x0};
SetWordType zzerr9[8] = {0x0,0x42,0x88,0x0, 0x0,0x2,0x0,0x0};
SetWordType zzerr10[8] = {0x0,0x0,0x0,0xc, 0x0,0x0,0x0,0x0};
SetWordType zzerr11[8] = {0x0,0x0,0x0,0x0, 0x7,0x0,0x0,0x0};
SetWordType setwd3[52] = {0x0,0x0,0x0,0x51,0x0,0x0,0x2,
	0x2,0x0,0x50,0x0,0x8,0x40,0x0,0x50,
	0x2,0x0,0x2,0x0,0x50,0x40,0x51,0x51,
	0x50,0x40,0x51,0x20,0x20,0x8,0x8,0x0,
	0x0,0x80,0x80,0x80,0x4,0x40,0x0,0x0,
	0x40,0x4,0x58,0x8,0x0,0x0,0x0,0x0,
	0x0,0x51,0x0,0x51,0x40};
SetWordType zzerr12[8] = {0x0,0x4,0x0,0x20, 0x0,0x0,0x0,0x0};
SetWordType zzerr13[8] = {0x0,0x0,0x0,0xc0, 0x0,0x0,0x0,0x0};
SetWordType zzerr14[8] = {0x0,0x0,0x0,0x30, 0x0,0x0,0x0,0x0};
SetWordType zzerr15[8] = {0x0,0x8,0x0,0x30, 0x0,0x6,0x0,0x0};
SetWordType setwd4[52] = {0x0,0x0,0x0,0x95,0x0,0x0,0x0,
	0x0,0x0,0x95,0x92,0x40,0x95,0x0,0x95,
	0x0,0x0,0x0,0x0,0x95,0x95,0x95,0x95,
	0x95,0x95,0x95,0x95,0x95,0x20,0xb2,0x88,
	0x88,0x94,0x94,0x94,0x40,0x95,0x0,0x0,
	0x95,0x40,0x95,0x0,0x0,0x0,0x0,0x0,
	0x0,0x95,0x0,0x95,0x95};
SetWordType zzerr16[8] = {0x0,0x18,0x0,0x30, 0x0,0x6,0x0,0x0};
SetWordType setwd5[52] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x1,0x2,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x1,0x1,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x1,0x1,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0};
