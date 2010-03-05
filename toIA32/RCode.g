
header "post_include_hpp" {
  // gets inserted after the antlr generated includes in the cpp file
  #include <iostream>
  #include <sstream>
  #include <string>
  #include <vector>
  #include <map>
  #include <set>
  #include <utility>
  #include <cstdlib> 
  #include "Blocs.hpp"

  using namespace std;
}

options {
    language       = "Cpp";
    genHashLines   = true;              // include line number information
    namespaceStd   = "std";
    namespaceAntlr = "antlr";
}


class RCodeLexer extends Lexer;

options {
    charVocabulary      = '\3'..'\377';
    caseSensitive       = true;
    defaultErrorHandler = false;       // Don't generate lexer error handlers
    testLiterals        = true;
}

tokens {
    PROGRAM          = "program";
    ENDPROGRAM       = "endprogram";
    SUBROUTINE       = "subroutine";
    ENDSUBROUTINE    = "endsubroutine";
    PARAMETERS       = "parameters";
    ENDPARAMETERS    = "endparameters";
    VARIABLES        = "variables";
    ENDVARIABLES     = "endvariables";
    STATIC_LINK      = "static_link";
    RETURNVALUE      = "returnvalue";
    AUX_SPACE        = "aux_space";
    LOAD             = "load";
    ALOAD            = "aload";
    ILOAD            = "iload";
    STOR             = "stor";
    ADDI             = "addi";
    SUBI             = "subi";
    MULI             = "muli";
    DIVI             = "divi";
    MINI             = "mini";
    EQUI             = "equi";
    EQUB             = "equb";
    GRTI             = "grti";
    LESI             = "lesi";
    LAND             = "land";
    LOOR             = "loor";
    LNOT             = "lnot";
    COPY             = "copy";
    REAI             = "reai";
    WRII             = "wrii";
    WRIB             = "wrib";
    WRLI             = "wrli";
    WRLB             = "wrlb";
    WRLN             = "wrln";
    WRIS             = "wris";
    WRLS             = "wrls";
    UJMP             = "ujmp";
    FJMP             = "fjmp";
    CALL             = "call";
    ETIQ             = "etiq";
    STOP             = "stop";
    RETU             = "retu";
    PUSHPARAM        = "pushparam";
    POPPARAM         = "popparam";
    KILLPARAM        = "killparam";
    NEW              = "new";
    FREE             = "free";
    OFFSET           = "offset";
}

protected
REGISTER
    : 't' (DIGIT)+
    ;

protected
LOCAL
    : '_' ALPHANUM ("_static_link")?
    ;

protected
PATH
    : "program" ('_' ALPHANUM)*
    ;

protected
LABEL
    : ("else_" DIGIT) => "else_" (DIGIT)+
    | ("endif_" DIGIT) => "endif_" (DIGIT)+
    | ("while_" DIGIT) => "while_" (DIGIT)+
    | ("endwhile_" DIGIT) => "endwhile_" (DIGIT)+
    | ("if_" DIGIT) => "if_" (DIGIT)+
    ;

protected
FULL_NAME
    : PATH ':' LOCAL
    ;

IDENT
    : ('t' DIGIT) => 't' (DIGIT)+
      { $setType(REGISTER); }
    | ('_') => '_' ALPHANUM ("_static_link")?
      { $setType(LOCAL); }
    | (PATH ':') => PATH ':' LOCAL
      { $setType(FULL_NAME); }
    | ("program_") => "program" ('_' ALPHANUM)*
      { $setType(PATH); }
    | ("else_" DIGIT) => "else_" (DIGIT)+
      { $setType(LABEL); }
    | ("endif_" DIGIT) => "endif_" (DIGIT)+
      { $setType(LABEL); }
    | ("while_" DIGIT) => "while_" (DIGIT)+
      { $setType(LABEL); }
    | ("endwhile_" DIGIT) => "endwhile_" (DIGIT)+
      { $setType(LABEL); }
    | ("if_" DIGIT) => "if_" (DIGIT)+
      { $setType(LABEL); }
    | ALPHANUM ('_' ALPHANUM)* // llenguate regular de les paraules clau:
                               // instruccions, delimitadors, ...
    ;

ABREPAR
    : '('
    ;

CIERRAPAR
    : ')'
    ;

ENTER
    : ('-')? (DIGIT)+
    ;

protected
ALPHANUM
    : ALPHA (ALPHA | DIGIT)*
    ;

protected
ALPHA
    : 'a'..'z' | 'A' .. 'Z'
    ;

protected
DIGIT
    : '0'..'9'
    ;

STRING
    : '"' ( ~('\n' | '"') )* '"'
    ;

BLANCOS
    : (' ' | '\t')+
      { $setType(antlr::Token::SKIP); }
    ;

SALTOLINEA
    : '\n'
      { newline();
        $setType(antlr::Token::SKIP);
      }
    ;

COMENTARIO
    : '#' ( ~('\n') )*
      { $setType(antlr::Token::SKIP); }
    ;


{
  void RCodeParser::printVInstrsOffsetsSizes(void) const {
    cout << "vInstrs: (" << vInstrs.size() << ")" << endl;
    for (unsigned int i = 0; i < vInstrs.size(); i++) {
      cout << "\t" << i + 1 << ":\t";
      vInstrs[i].printInstrRCode();
      cout << endl;
    }  
    cout << endl;
    cout << "mOffsets: (" << mOffsets.size() << ")" << endl;
    for (map<string, int>::const_iterator iter = mOffsets.begin(); iter != mOffsets.end(); ++iter) {
      cout << "\toffset[" << iter->first << "] = " << iter->second << endl;
    }
    cout << endl;
    cout << "mSizes: (" << mSizes.size() << ")" << endl;
    for (map<string, int>::const_iterator iter = mSizes.begin(); iter != mSizes.end(); ++iter) {
      cout << "\tsize[" << iter->first << "] = " << iter->second << endl;
    }
    cout << endl;
  }

  string RCodeParser::tmp_label_bloc_basic(int bloc_basic) const {
    stringstream ss (stringstream::out);

    ss << "tmp_label_" << vBlocs.size() << "_" << bloc_basic;

    return ss.str();
  }

  void RCodeParser::updateNumberOfRegs(const string & reg) {
    unsigned int r = atoi(reg.substr(1).c_str()) + 1;
    if (r > numRegs) {
      numRegs = r;
    }
  }

}

class RCodeParser extends Parser;

options {
    buildAST            = true;
    defaultErrorHandler = false;        // Don't generate parser error handlers
}

tokens {
    ListOfInstrs;
}

{
  public:
    vector<tBloc>        vBlocs;

    unsigned int         numRegs;

    bool                 reaiRCInstr;
    bool                 wriiRCInstr;
    bool                 wrliRCInstr;
    bool                 wrisRCInstr;
    bool                 wrlsRCInstr;
    bool                 wrlnRCInstr;

    vector<string>       vStrWRTS;

    map<string, int>     mOffsets;
    map<string, int>     mSizes;

    void printVInstrsOffsetsSizes(void) const;

  private:
    string tmp_label_bloc_basic(int bloc_basic) const;
    int current_label;
    int current_offset_params;
    int current_offset_vars;
    
    vector<string> blocs;
      
    symbol_offset tmp_one_offs;
    offsets tmp_offs;
    vector<offsets> offsets_params;
    vector<offsets> offsets_vars;

    unsigned int tmp_label;

    int                    current_instr;
    tInstrRCode            tmp_instr;
    vector<tInstrRCode>    vInstrs;

    int                  current_bloc_basic;

    tBlocBasic           tmp_bloc_basic;
    tBloc                tmp_bloc;         

    tInstrRCode          instruction;

    bool                 curr_label_pend;
    int                  prev_exit_label_pend;

    int                  vars_size;

    void                 updateNumberOfRegs(const string & reg);

}

compilation_unit
    : { current_label = 0;
        current_instr = 0;
        vInstrs.clear();
        vBlocs.clear();
        numRegs = 0;
        reaiRCInstr = wriiRCInstr = wrliRCInstr = false;
        wrisRCInstr = wrlsRCInstr = wrlnRCInstr = false;
        vStrWRTS.clear();
        mOffsets.clear();
        mSizes.clear();
      }
      program EOF!
    ;

program
    : bloc_program (bloc_subroutine)*
    ;

bloc_program
    : PROGRAM^
      { blocs.push_back("program");
        current_bloc_basic = 0;
        tmp_bloc.label = "program";
        tmp_bloc.offsets_params.clear();
        tmp_bloc.offsets_vars.clear();
        tmp_bloc.blocsBasics.clear();
        tmp_bloc.mLabelsBasics.clear();
        curr_label_pend = false;
        prev_exit_label_pend = 0;
      }
      params_prog
      { offsets_params.push_back(tmp_offs);
        tmp_bloc.offsets_params = tmp_offs;
        vars_size = 0;
      }
      variables
      { offsets_vars.push_back(tmp_offs);
        tmp_bloc.offsets_vars = tmp_offs;
        tmp_bloc.size_of_vars = vars_size;
      }
      { current_bloc_basic++;
        tmp_bloc_basic.label = "program";
        tmp_bloc_basic.instrs.clear();
        tmp_bloc_basic.nFirstInstr = current_instr + 1;
        tmp_bloc_basic.nLastInstr = -1;
        tmp_bloc_basic.typeExit = NO_EXIT;
        tmp_bloc_basic.label_exit1 = "";
        tmp_bloc_basic.label_exit2 = "";
        tmp_bloc_basic.bloc_exit1 = -1;
        tmp_bloc_basic.bloc_exit2 = -1;
        tmp_bloc_basic.liveBefore.clear();
        tmp_bloc_basic.liveAfter.clear();
      }
      instrs_prog
      { for (unsigned int i = 0; i < tmp_bloc.blocsBasics.size(); i++) {
          if (tmp_bloc.blocsBasics[i].typeExit == UNCONDITIONAL_EXIT) {
            if (tmp_bloc.blocsBasics[i].label_exit1 != "" && tmp_bloc.blocsBasics[i].bloc_exit1 == -1) {
              tmp_bloc.blocsBasics[i].bloc_exit1 = tmp_bloc.mLabelsBasics[tmp_bloc.blocsBasics[i].label_exit1];
            }
          }
          if (tmp_bloc.blocsBasics[i].typeExit == CONDITIONAL_EXIT) {
            if (tmp_bloc.blocsBasics[i].label_exit1 != "" && tmp_bloc.blocsBasics[i].bloc_exit1 == -1) {
              tmp_bloc.blocsBasics[i].bloc_exit1 = tmp_bloc.mLabelsBasics[tmp_bloc.blocsBasics[i].label_exit1];
            }
            if (tmp_bloc.blocsBasics[i].label_exit2 != "" && tmp_bloc.blocsBasics[i].bloc_exit2 == -1) {
              tmp_bloc.blocsBasics[i].bloc_exit2 = tmp_bloc.mLabelsBasics[tmp_bloc.blocsBasics[i].label_exit2];
            }
          }
        }
        vBlocs.push_back(tmp_bloc);
      }
      ENDPROGRAM!
    ;

params_prog
    : PARAMETERS^
      { tmp_offs.clear(); }
      sl:STATIC_LINK
      { tmp_one_offs.first = sl->getText();
        tmp_one_offs.second = 0;
        tmp_offs.push_back(tmp_one_offs);
        mOffsets[tmp_bloc.label + ":" + sl->getText()] = 0;
        mSizes[tmp_bloc.label + ":" + sl->getText()] = 4;
      }
      ENDPARAMETERS!
    ;

variables
    : VARIABLES^
      { tmp_offs.clear(); }
      user_vars
      (aux_space)?
      ENDVARIABLES!
    ;

bloc_subroutine
    : SUBROUTINE^
      p:PATH
      { blocs.push_back(p->getText());
        current_bloc_basic = 0;
        tmp_bloc.label = p->getText();
        tmp_bloc.offsets_params.clear();
        tmp_bloc.offsets_vars.clear();
        tmp_bloc.blocsBasics.clear();
        tmp_bloc.mLabelsBasics.clear();
        curr_label_pend = false;
        prev_exit_label_pend = 0;
      }
      params_subr
      { offsets_params.push_back(tmp_offs);
        tmp_bloc.offsets_params = tmp_offs;
        vars_size = 0;
      }
      variables
      { offsets_vars.push_back(tmp_offs);
        tmp_bloc.offsets_vars = tmp_offs;
        tmp_bloc.size_of_vars = vars_size;
      }
      { current_bloc_basic++;
        tmp_bloc_basic.label = p->getText();
        tmp_bloc_basic.instrs.clear();
        tmp_bloc_basic.nFirstInstr = current_instr + 1;
        tmp_bloc_basic.nLastInstr = -1;
        tmp_bloc_basic.typeExit = NO_EXIT;
        tmp_bloc_basic.label_exit1 = "";
        tmp_bloc_basic.label_exit2 = "";
        tmp_bloc_basic.bloc_exit1 = -1;
        tmp_bloc_basic.bloc_exit2 = -1;
        tmp_bloc_basic.liveBefore.clear();
        tmp_bloc_basic.liveAfter.clear();
      }
      instrs_subr
      { for (unsigned int i = 0; i < tmp_bloc.blocsBasics.size(); i++) {
          if (tmp_bloc.blocsBasics[i].typeExit == UNCONDITIONAL_EXIT) {
            if (tmp_bloc.blocsBasics[i].label_exit1 != "" && tmp_bloc.blocsBasics[i].bloc_exit1 == -1) {
              tmp_bloc.blocsBasics[i].bloc_exit1 = tmp_bloc.mLabelsBasics[tmp_bloc.blocsBasics[i].label_exit1];
            }
          }
          if (tmp_bloc.blocsBasics[i].typeExit == CONDITIONAL_EXIT) {
            if (tmp_bloc.blocsBasics[i].label_exit1 != "" && tmp_bloc.blocsBasics[i].bloc_exit1 == -1) {
              tmp_bloc.blocsBasics[i].bloc_exit1 = tmp_bloc.mLabelsBasics[tmp_bloc.blocsBasics[i].label_exit1];
            }
            if (tmp_bloc.blocsBasics[i].label_exit2 != "" && tmp_bloc.blocsBasics[i].bloc_exit2 == -1) {
              tmp_bloc.blocsBasics[i].bloc_exit2 = tmp_bloc.mLabelsBasics[tmp_bloc.blocsBasics[i].label_exit2];
            }
          }
        }
        vBlocs.push_back(tmp_bloc);
      }
      ENDSUBROUTINE!
    ;

params_subr
    : PARAMETERS^
      { tmp_offs.clear(); }
      (r:RETURNVALUE
       { tmp_one_offs.first = r->getText();
         tmp_offs.push_back(tmp_one_offs);
       }
      )?
      user_params
      sl:STATIC_LINK
      { tmp_one_offs.first = sl->getText();
        tmp_one_offs.second = 0;
        tmp_offs.push_back(tmp_one_offs);
        mOffsets[tmp_bloc.label + ":" + sl->getText()] = 0;
        mSizes[tmp_bloc.label + ":" + sl->getText()] = 4;
        current_offset_params = -4;
        for (int i = tmp_offs.size() - 2; i >= 0; i--) {
          tmp_offs[i].second = current_offset_params;
          mOffsets[tmp_bloc.label + ":" + tmp_offs[i].first] = tmp_offs[i].second;
          mSizes[tmp_bloc.label + ":" + tmp_offs[i].first] = 4;
          current_offset_params -= 4;       
        }
      }
      ENDPARAMETERS!
    ;

user_vars
    : { current_offset_vars = 12; }
      (dec_var)*
    ;

dec_var
    : l:LOCAL^ e:ENTER
      { tmp_one_offs.first = l->getText();
        tmp_one_offs.second = current_offset_vars;
        tmp_offs.push_back(tmp_one_offs);
        mOffsets[tmp_bloc.label + ":" + l->getText()] = current_offset_vars;
        mSizes[tmp_bloc.label + ":" + l->getText()] = atoi(e->getText().c_str());
        current_offset_vars += atoi(e->getText().c_str());
        vars_size += atoi(e->getText().c_str());
      }
    ;

aux_space
    : a:AUX_SPACE^ e:ENTER
      { tmp_one_offs.first = a->getText();
        tmp_one_offs.second = current_offset_vars;
        tmp_offs.push_back(tmp_one_offs);
        mOffsets[tmp_bloc.label + ":" + a->getText()] = current_offset_vars;
        mSizes[tmp_bloc.label + ":" + a->getText()] = atoi(e->getText().c_str());
        current_offset_vars += atoi(e->getText().c_str());
        vars_size += atoi(e->getText().c_str());
      }
    ;

user_params
    : (l:LOCAL
       { tmp_one_offs.first = l->getText();
         tmp_offs.push_back(tmp_one_offs);
       }
      )*
    ;

instrs_prog
    : instrs
    ;

instrs_subr
    : instrs
    ;

instrs
    { tmp_instr = tInstrRCode("begin");
      vInstrs.push_back(tmp_instr);
      tmp_bloc_basic.instrs.push_back(tmp_instr);
    }
    : ( tmp_instr = instr
       { vInstrs.push_back(tmp_instr);
          current_instr++;
          if (tmp_instr.cod == "stop" ||
              tmp_instr.cod == "retu" ||
              tmp_instr.cod == "ujmp" ||
              tmp_instr.cod == "fjmp") { // esta instruccion es la ultima del bloque actual
            if (curr_label_pend) {
              curr_label_pend = false;
              if (prev_exit_label_pend == 1) {
                // por defecto la inicializacion de la etiqueta del
                // bloque actual ya fue  tmp_i_j                
                // por defecto la inicializacion del next1 del bloque
                // previo ya apunta al bloque actual
              } else if (prev_exit_label_pend == 2) {
                // por defecto la inicializacion de la etiqueta del
                // bloque actual ya fue  tmp_i_j                
                // por defecto la inicializacion del next2 del bloque
                // previo ya apunta al bloque actual
              }
              prev_exit_label_pend = 0;
            }
            tmp_bloc_basic.instrs.push_back(tmp_instr);
            tmp_bloc_basic.nLastInstr = current_instr;
            if (tmp_instr.cod == "stop" || tmp_instr.cod == "retu" ) {
              tmp_bloc_basic.typeExit = NO_EXIT;
              tmp_bloc_basic.label_exit1 = "";
              tmp_bloc_basic.label_exit2 = "";
              tmp_bloc_basic.bloc_exit1 = -1;
              tmp_bloc_basic.bloc_exit2 = -1;
              prev_exit_label_pend = 0;
            } else if (tmp_instr.cod == "ujmp") {
              tmp_bloc_basic.typeExit = UNCONDITIONAL_EXIT;
              tmp_bloc_basic.label_exit1 = tmp_instr.op1.opnd;
              tmp_bloc_basic.label_exit2 = "";
              tmp_bloc_basic.bloc_exit1 = -1;
              tmp_bloc_basic.bloc_exit2 = -1;
              prev_exit_label_pend = 0;
            } else if (tmp_instr.cod == "fjmp") {
              tmp_bloc_basic.typeExit = CONDITIONAL_EXIT;
              tmp_bloc_basic.label_exit1 = tmp_label_bloc_basic(current_bloc_basic);
              tmp_bloc_basic.label_exit2 = tmp_instr.op2.opnd;
              tmp_bloc_basic.bloc_exit1 = current_bloc_basic;
              tmp_bloc_basic.bloc_exit2 = -1;
              prev_exit_label_pend = 1;
            }
            tmp_bloc.blocsBasics.push_back(tmp_bloc_basic);
            tmp_bloc_basic.label = tmp_label_bloc_basic(current_bloc_basic);
            curr_label_pend = true;
            tmp_bloc_basic.instrs.clear();        
            tmp_bloc_basic.nFirstInstr = current_instr + 1;
            tmp_bloc_basic.nLastInstr = -1;
            tmp_bloc_basic.typeExit = NO_EXIT;
            tmp_bloc_basic.label_exit1 = "";
            tmp_bloc_basic.label_exit2 = "";
            tmp_bloc_basic.bloc_exit1 = -1;
            tmp_bloc_basic.bloc_exit2 = -1;
            tmp_bloc_basic.liveBefore.clear();
            tmp_bloc_basic.liveAfter.clear();
            current_bloc_basic++;
          } else if (tmp_instr.cod == "etiq") { // el bloc actual ha acabat y aquesta
                                                // instruccio es la primera del nou bloc
                                                // dar de alta la etiqueta del BA en el map y el vector
            if (curr_label_pend) {
              curr_label_pend = false;
              tmp_bloc_basic.label = tmp_instr.op1.opnd;
              tmp_bloc_basic.instrs.push_back(tmp_instr);
              tmp_bloc_basic.nLastInstr = current_instr;
              tmp_bloc.mLabelsBasics[tmp_instr.op1.opnd] = current_bloc_basic - 1;
              if (prev_exit_label_pend == 1) {
                tmp_bloc.blocsBasics[tmp_bloc.blocsBasics.size()-1].label_exit1 = tmp_instr.op1.opnd;
              } else if (prev_exit_label_pend == 2) {
                tmp_bloc.blocsBasics[tmp_bloc.blocsBasics.size()-1].label_exit2 = tmp_instr.op1.opnd;
              }
              prev_exit_label_pend = 0;
            } else {
              tmp_bloc_basic.typeExit = CONTINUE_EXIT;
              tmp_bloc_basic.label_exit1 = tmp_instr.op1.opnd;
              tmp_bloc_basic.label_exit2 = "";
              tmp_bloc_basic.bloc_exit1 = current_bloc_basic;
              tmp_bloc_basic.bloc_exit2 = -1;
              tmp_bloc.blocsBasics.push_back(tmp_bloc_basic);
              tmp_bloc.mLabelsBasics[tmp_instr.op1.opnd] = current_bloc_basic;
              current_bloc_basic++;
              tmp_bloc_basic.label = tmp_instr.op1.opnd;
              curr_label_pend = false;
              tmp_bloc_basic.instrs.clear();
              tmp_bloc_basic.instrs.push_back(tmp_instr);  
              tmp_bloc_basic.nFirstInstr = current_instr;
              tmp_bloc_basic.nLastInstr = current_instr;
              tmp_bloc_basic.typeExit = NO_EXIT;
              tmp_bloc_basic.label_exit1 = "";
              tmp_bloc_basic.label_exit2 = "";
              tmp_bloc_basic.bloc_exit1 = -1;
              tmp_bloc_basic.bloc_exit2 = -1;
              tmp_bloc_basic.liveBefore.clear();
              tmp_bloc_basic.liveAfter.clear();
            }
          } else { // aquesta instruccio s'afegeix al bloc
                   // actual que encara continua
            if (curr_label_pend) {
              curr_label_pend = false;
              if (prev_exit_label_pend == 1) {
                // por defecto la inicializacion de la etiqueta del
                // bloque actual ya fue  tmp_i_j                
                // por defecto la inicializacion del next1 del bloque
                // previo ya apunta al bloque actual
              } else if (prev_exit_label_pend == 2) {
                // por defecto la inicializacion de la etiqueta del
                // bloque actual ya fue  tmp_i_j                
                // por defecto la inicializacion del next2 del bloque
                // previo ya apunta al bloque actual
              }
              prev_exit_label_pend = 0;
            }
            tmp_bloc_basic.nLastInstr = current_instr;
            tmp_bloc_basic.instrs.push_back(tmp_instr);
          }
        }
      )*
      { #instrs = #(#[ListOfInstrs, "ListOfInstrs"], #instrs);
        if (tmp_bloc_basic.instrs.size() > 0) {
          tmp_bloc.blocsBasics.push_back(tmp_bloc_basic);
        }
      }
    ;

instr returns [tInstrRCode inst]
    { tOpndRCode op1, op2, op3; }
    : LOAD^   op1 = operand_ref  op2 = reg
      { inst = tInstrRCode("load", op1, op2); }
    | ALOAD^  op1 = operand_ref  op2 = reg
      { inst = tInstrRCode("aload", op1, op2); }
    | ILOAD^  en:ENTER       op2 = operand_ref
      { inst = tInstrRCode("iload", tOpndRCode(MODE_RCODE_VALUE, en->getText()), op2); }
    | STOR^  op1 = operand_ref  op2 = operand_ref
      { inst = tInstrRCode("stor", op1, op2); }
    | ADDI^  op1 = operand  op2 = operand  op3 = operand_ref
      { inst = tInstrRCode("addi", op1, op2, op3); }
    | SUBI^  op1 = operand  op2 = operand  op3 = operand_ref
      { inst = tInstrRCode("subi", op1, op2, op3); }
    | MULI^  op1 = operand  op2 = operand  op3 = operand_ref
      { inst = tInstrRCode("muli", op1, op2, op3); }
    | DIVI^  op1 = operand  op2 = operand  op3 = operand_ref
      { inst = tInstrRCode("divi", op1, op2, op3); }
    | MINI^  op1 = operand  op2 = operand_ref
      { inst = tInstrRCode("mini", op1, op2); }
    | EQUI^  op1 = operand  op2 = operand  op3 = operand_ref
      { inst = tInstrRCode("equi", op1, op2, op3); }
    | EQUB^  op1 = operand  op2 = operand  op3 = operand_ref
      { inst = tInstrRCode("equb", op1, op2, op3); }
    | GRTI^  op1 = operand  op2 = operand  op3 = operand_ref
      { inst = tInstrRCode("grti", op1, op2, op3); }
    | LESI^  op1 = operand  op2 = operand  op3 = operand_ref
      { inst = tInstrRCode("lesi", op1, op2, op3); }
    | LAND^  op1 = operand  op2 = operand  op3 = operand_ref
      { inst = tInstrRCode("land", op1, op2, op3); }
    | LOOR^  op1 = operand  op2 = operand  op3 = operand_ref
      { inst = tInstrRCode("loor", op1, op2, op3); }
    | LNOT^  op1 = operand  op2 = operand_ref
      { inst = tInstrRCode("lnot", op1, op2); }
    | COPY^  op1 = operand_ref  op2 = operand_ref  op3 = operand
      { inst = tInstrRCode("copy", op1, op2, op3); }
    | REAI^  op1 = operand_ref
      { reaiRCInstr = true;
        inst = tInstrRCode("reai", op1); }
    | WRII^  op1 = operand
      { wriiRCInstr = true;
        inst = tInstrRCode("wrii", op1); }
    | WRLI^  op1 = operand 
      { wrliRCInstr = true;
        inst = tInstrRCode("wrli", op1); }
    | WRIS^  sti:STRING
      { wrisRCInstr = true;
        vStrWRTS.push_back(sti->getText());
        inst = tInstrRCode("wris", tOpndRCode(MODE_RCODE_STRING, sti->getText()));
      }
    | WRLS^  stl:STRING
      { wrlsRCInstr = true;
        vStrWRTS.push_back(stl->getText());
        inst = tInstrRCode("wrls", tOpndRCode(MODE_RCODE_STRING, stl->getText()));
      }
    | WRLN
      { wrlnRCInstr = true;
        inst = tInstrRCode("wrln"); }
    | UJMP^  op1 = label_jump
      { inst = tInstrRCode("ujmp", op1); }
    | FJMP^  op1 = operand  op2 = label_jump
      { inst = tInstrRCode("fjmp", op1, op2); }
    | CALL^  op1 = label_call
      { inst = tInstrRCode("call", op1); }
    | ETIQ^  el:LABEL
      { inst = tInstrRCode("etiq", tOpndRCode(MODE_RCODE_LABEL, el->getText())); }
    | NEW^   op1 = operand  op2 = operand_ref
      { inst = tInstrRCode("new", op1, op2); }
    | FREE^  op1 = operand_ref
      { inst = tInstrRCode("free", op1); }
    | STOP
      { inst = tInstrRCode("stop"); }
    | RETU
      { inst = tInstrRCode("retu"); }
    | PUSHPARAM^ ( op1 = operand
                   { inst = tInstrRCode("pushparam", op1); }
                 | pa:PATH
                   { inst = tInstrRCode("pushparam", tOpndRCode(MODE_RCODE_LABEL, pa->getText())); }
    )
    | POPPARAM^    op1 = reg
      { inst = tInstrRCode("popparam", op1); }
    | KILLPARAM
      { inst = tInstrRCode("killparam"); }
    ;

operand returns [tOpndRCode op]
    : rg:REGISTER
      { op = tOpndRCode(MODE_RCODE_REGISTER, rg->getText());
        updateNumberOfRegs(rg->getText());
      }
    | lc:LOCAL
      { op = tOpndRCode(MODE_RCODE_LOCAL, lc->getText()); }
    | au:AUX_SPACE
      { op = tOpndRCode(MODE_RCODE_LOCAL, au->getText()); }
    | sl:STATIC_LINK
      { op = tOpndRCode(MODE_RCODE_LOCAL, sl->getText()); }
    | rv:RETURNVALUE
      { op = tOpndRCode(MODE_RCODE_LOCAL, rv->getText()); }
    | en:ENTER
      { op = tOpndRCode(MODE_RCODE_VALUE, en->getText()); }
    | OFFSET^ ABREPAR! fn:FULL_NAME CIERRAPAR!
      { op = tOpndRCode(MODE_RCODE_OFFSET, fn->getText()); }
    ;

operand_ref returns [tOpndRCode op]
    : rg:REGISTER
      { op = tOpndRCode(MODE_RCODE_REGISTER, rg->getText());
        updateNumberOfRegs(rg->getText());
      }
    | lc:LOCAL
      { op = tOpndRCode(MODE_RCODE_LOCAL, lc->getText()); }
    | au:AUX_SPACE
      { op = tOpndRCode(MODE_RCODE_LOCAL, au->getText()); }
    | sl:STATIC_LINK
      { op = tOpndRCode(MODE_RCODE_LOCAL, sl->getText()); }
    | rv:RETURNVALUE
      { op = tOpndRCode(MODE_RCODE_LOCAL, rv->getText()); }
    ;

reg returns [tOpndRCode op]
    : rg:REGISTER
      { op = tOpndRCode(MODE_RCODE_REGISTER, rg->getText());
        updateNumberOfRegs(rg->getText());
      }
    ;

label_jump returns [tOpndRCode op]
    : la:LABEL
      { op = tOpndRCode(MODE_RCODE_LABEL, la->getText()); }
    ;

label_call returns [tOpndRCode op]
    : pa:PATH
      { op = tOpndRCode(MODE_RCODE_LABEL, pa->getText()); }
    | rg:REGISTER
      { op = tOpndRCode(MODE_RCODE_REGISTER, rg->getText());
        updateNumberOfRegs(rg->getText());
      }
    ;
