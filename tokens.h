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
#define RETURN 14
#define CERT 15
#define FALS 16
#define PROCEDURE 17
#define ENDPROCEDURE 18
#define FUNCTION 19
#define ENDFUNCTION 20
#define VAR 21
#define BOOL 22
#define STRINGEXP 23
#define ARRAY 24
#define OF 25
#define IF 26
#define THEN 27
#define ELSE 28
#define ENDIF 29
#define WHILE 30
#define DO 31
#define ENDWHILE 32
#define AND 33
#define OR 34
#define NOT 35
#define MINUS 36
#define TIMES 37
#define DIV 38
#define EQUAL 39
#define GTHAN 40
#define LTHAN 41
#define OPENCLAU 42
#define CLOSECLAU 43
#define REF 44
#define VAL 45
#define COMA 46
#define DOT 47
#define IDENT 48
#define INTCONST 49
#define COMMENT 50
#define WHITESPACE 51
#define NEWLINE 52
#define LEXICALERROR 53
#define STRING 54

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
extern SetWordType zzerr12[];
extern SetWordType setwd3[];
extern SetWordType zzerr13[];
extern SetWordType zzerr14[];
extern SetWordType zzerr15[];
extern SetWordType zzerr16[];
extern SetWordType setwd4[];
extern SetWordType zzerr17[];
extern SetWordType setwd5[];
