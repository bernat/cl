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
#define REF 37
#define VAL 38
#define COMA 39
#define DOT 40
#define IDENT 41
#define INTCONST 42
#define COMMENT 43
#define WHITESPACE 44
#define NEWLINE 45
#define LEXICALERROR 46
#define PROCEDURE 47
#define ENDPROCEDURE 48
#define FUNCTION 49
#define RETURN 50
#define ENDFUNCTION 51

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
void cap_procedure(AST**_root);
#else
extern void cap_procedure();
#endif

#ifdef __USE_PROTOS
void cap_function(AST**_root);
#else
extern void cap_function();
#endif

#ifdef __USE_PROTOS
void cjt_parametres(AST**_root);
#else
extern void cjt_parametres();
#endif

#ifdef __USE_PROTOS
void parametre(AST**_root);
#else
extern void parametre();
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

#ifdef __USE_PROTOS
void params(AST**_root);
#else
extern void params();
#endif

#endif
extern SetWordType zzerr1[];
extern SetWordType zzerr2[];
extern SetWordType setwd1[];
extern SetWordType zzerr3[];
extern SetWordType zzerr4[];
extern SetWordType zzerr5[];
extern SetWordType setwd2[];
extern SetWordType zzerr6[];
extern SetWordType zzerr7[];
extern SetWordType zzerr8[];
extern SetWordType zzerr9[];
extern SetWordType zzerr10[];
extern SetWordType zzerr11[];
extern SetWordType setwd3[];
extern SetWordType zzerr12[];
extern SetWordType zzerr13[];
extern SetWordType zzerr14[];
extern SetWordType zzerr15[];
extern SetWordType setwd4[];
extern SetWordType zzerr16[];
extern SetWordType setwd5[];
