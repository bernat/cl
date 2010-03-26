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
#define WRITE 10
#define READ 11
#define PLUS 12
#define OPENPAR 13
#define CLOSEPAR 14
#define ASIG 15
#define RETURN 16
#define CERT 17
#define FALS 18
#define PROCEDURE 19
#define ENDPROCEDURE 20
#define FUNCTION 21
#define ENDFUNCTION 22
#define VAR 23
#define BOOL 24
#define STRING 25
#define ARRAY 26
#define OF 27
#define IF 28
#define THEN 29
#define ELSE 30
#define ENDIF 31
#define WHILE 32
#define DO 33
#define ENDWHILE 34
#define AND 35
#define OR 36
#define NOT 37
#define MINUS 38
#define TIMES 39
#define DIV 40
#define EQUAL 41
#define GTHAN 42
#define LTHAN 43
#define OPENCLAU 44
#define CLOSECLAU 45
#define REF 46
#define VAL 47
#define COMA 48
#define DOT 49
#define IDENT 50
#define INTCONST 51
#define COMMENT 52
#define WHITESPACE 53
#define NEWLINE 54
#define LEXICALERROR 55

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
extern SetWordType zzerr17[];
extern SetWordType setwd4[];
extern SetWordType zzerr18[];
extern SetWordType setwd5[];
