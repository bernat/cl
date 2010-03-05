#ifndef tokens_h
#define tokens_h
/* tokens.h -- List of labelled tokens and stuff
 *
 * Generated from: cl.g
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * ANTLR Version 1.33MR33
 */
#define zzEOF_TOKEN 1
#define INPUTEND 1
#define PROGRAM 2
#define ENDPROGRAM 3
#define VARS 4
#define ENDVARS 5
#define INT 6
#define STRUCT 7
#define ENDSTRUCT 8
#define WRITELN 9
#define PLUS 10
#define OPENPAR 11
#define CLOSEPAR 12
#define ASIG 13
#define BOOL 14
#define MINUS 15
#define IF 16
#define THEN 17
#define ELSE 18
#define ENDIF 19
#define WHILE 20
#define DO 21
#define ENDWHILE 22
#define AND 23
#define OR 24
#define NOT 25
#define TIMES 26
#define DIV 27
#define EQUAL 28
#define GTHAN 29
#define LTHAN 30
#define DOT 31
#define IDENT 32
#define INTCONST 33
#define COMMENT 34
#define WHITESPACE 35
#define NEWLINE 36
#define LEXICALERROR 37
#define PROCEDURE 38
#define ENDPROCEDURE 39
#define FUNCTION 40
#define ENDFUNCTION 41
#define VAR 42
#define STRING 43

#ifdef __USE_PROTOS
void program(AST**_root);
#else
extern void program();
#endif

#ifdef __USE_PROTOS
void dec_vars(AST**_root);
#else
extern void dec_vars();
#endif

#ifdef __USE_PROTOS
void l_dec_vars(AST**_root);
#else
extern void l_dec_vars();
#endif

#ifdef __USE_PROTOS
void dec_var(AST**_root);
#else
extern void dec_var();
#endif

#ifdef __USE_PROTOS
void l_dec_blocs(AST**_root);
#else
extern void l_dec_blocs();
#endif

#ifdef __USE_PROTOS
void dec_bloc(AST**_root);
#else
extern void dec_bloc();
#endif

#ifdef __USE_PROTOS
void constr_type(AST**_root);
#else
extern void constr_type();
#endif

#ifdef __USE_PROTOS
void field(AST**_root);
#else
extern void field();
#endif

#ifdef __USE_PROTOS
void l_instrs(AST**_root);
#else
extern void l_instrs();
#endif

#ifdef __USE_PROTOS
void instruction(AST**_root);
#else
extern void instruction();
#endif

#ifdef __USE_PROTOS
void expression(AST**_root);
#else
extern void expression();
#endif

#ifdef __USE_PROTOS
void expression1(AST**_root);
#else
extern void expression1();
#endif

#ifdef __USE_PROTOS
void expression2(AST**_root);
#else
extern void expression2();
#endif

#ifdef __USE_PROTOS
void expression3(AST**_root);
#else
extern void expression3();
#endif

#ifdef __USE_PROTOS
void term(AST**_root);
#else
extern void term();
#endif

#ifdef __USE_PROTOS
void unari(AST**_root);
#else
extern void unari();
#endif

#ifdef __USE_PROTOS
void expression4(AST**_root);
#else
extern void expression4();
#endif

#ifdef __USE_PROTOS
void expsimple(AST**_root);
#else
extern void expsimple();
#endif

#endif
extern SetWordType zzerr1[];
extern SetWordType zzerr2[];
extern SetWordType zzerr3[];
extern SetWordType setwd1[];
extern SetWordType zzerr4[];
extern SetWordType zzerr5[];
extern SetWordType zzerr6[];
extern SetWordType zzerr7[];
extern SetWordType zzerr8[];
extern SetWordType setwd2[];
extern SetWordType setwd3[];
extern SetWordType zzerr9[];
extern SetWordType zzerr10[];
extern SetWordType zzerr11[];
extern SetWordType setwd4[];
