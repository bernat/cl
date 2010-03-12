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
#define VAR 14
#define BOOL 15
#define STRING 16
#define ARRAY 17
#define OF 18
#define IF 19
#define THEN 20
#define ELSE 21
#define ENDIF 22
#define WHILE 23
#define DO 24
#define ENDWHILE 25
#define AND 26
#define OR 27
#define NOT 28
#define MINUS 29
#define TIMES 30
#define DIV 31
#define EQUAL 32
#define GTHAN 33
#define LTHAN 34
#define OPENCLAU 35
#define CLOSECLAU 36
#define DOT 37
#define IDENT 38
#define INTCONST 39
#define COMMENT 40
#define WHITESPACE 41
#define NEWLINE 42
#define LEXICALERROR 43
#define PROCEDURE 44
#define ENDPROCEDURE 45
#define FUNCTION 46
#define ENDFUNCTION 47

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
void comparison_exp(AST**_root);
#else
extern void comparison_exp();
#endif

#ifdef __USE_PROTOS
void plus_exp(AST**_root);
#else
extern void plus_exp();
#endif

#ifdef __USE_PROTOS
void term_exp(AST**_root);
#else
extern void term_exp();
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
extern SetWordType zzerr9[];
extern SetWordType zzerr10[];
extern SetWordType zzerr11[];
extern SetWordType zzerr12[];
extern SetWordType setwd3[];
extern SetWordType zzerr13[];
extern SetWordType zzerr14[];
extern SetWordType setwd4[];
