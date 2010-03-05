
#ifndef _BLOCS_HPP_
#define _BLOCS_HPP_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <utility>
#include <cstdlib> 

using namespace std;

typedef pair<string, int>     symbol_offset;
typedef vector<symbol_offset> offsets;

const int MODE_RCODE_NULL     = 0;
const int MODE_RCODE_REGISTER = 1;
const int MODE_RCODE_LOCAL    = 2;
const int MODE_RCODE_VALUE    = 3;
const int MODE_RCODE_LABEL    = 4;
const int MODE_RCODE_OFFSET   = 5;
const int MODE_RCODE_STRING   = 6;

struct tOpndRCode {
  int    mode;
  string opnd;

  tOpndRCode()
    : mode(MODE_RCODE_NULL), opnd("") {
  }

  tOpndRCode(int m, string op)
    : mode(m), opnd(op) {
  }
};

struct tInstrRCode {
  string     cod;
  tOpndRCode op1, op2, op3;

  tInstrRCode() {
  }

  tInstrRCode(string c, tOpndRCode o1 = tOpndRCode(), tOpndRCode o2 = tOpndRCode(), tOpndRCode o3 = tOpndRCode())
    : cod(c), op1(o1), op2(o2), op3(o3) {
  }

  void printInstrRCode(ostream & os = cout) const {
    os << cod;
    if (op1.opnd != "") {
      if (op1.mode == MODE_RCODE_LOCAL) {
	os << "\t" << op1.opnd;
      } else if (op1.mode == MODE_RCODE_STRING) {
	os << "\t" << op1.opnd;
      } else if (op1.mode == MODE_RCODE_OFFSET) {
	os << "\toffset(" << op1.opnd << ")";
      } else {
	os << "\t" << op1.opnd;
      }
      if (op2.opnd != "") {
	if (op2.mode == MODE_RCODE_LOCAL) {
	  os << "\t" << op2.opnd;
	} else if (op2.mode == MODE_RCODE_OFFSET) {
	  os << "\toffset(" << op2.opnd << ")";
	} else if (op2.mode != MODE_RCODE_NULL) {
	  os << "\t" << op2.opnd;
	}
	if (op3.opnd != "") {
	  if (op3.mode == MODE_RCODE_LOCAL) {
	    os << "\t" << op3.opnd;
	  } else if (op3.mode == MODE_RCODE_OFFSET) {
	    os << "\toffset(" << op3.opnd << ")";
	  } else if (op3.mode != MODE_RCODE_NULL) {
	    os << "\t" << op3.opnd;
	  }
	}
      }
    }
  }
};

const unsigned int TYPE_NULL_IA32   = 0;
const unsigned int TYPE_LABEL_IA32  = 1;
const unsigned int TYPE_SECT0_IA32  = 2;
const unsigned int TYPE_SECT1_IA32  = 3;
const unsigned int TYPE_SECT2_IA32  = 4;
const unsigned int TYPE_INSTR0_IA32 = 5;
const unsigned int TYPE_INSTR1_IA32 = 6;
const unsigned int TYPE_INSTR2_IA32 = 7;

struct tInstrIA32 {
  unsigned int type;
  string       code;
  string       op1;
  string       op2;
  
  tInstrIA32(const unsigned int & t, string c, string o1 = "", string o2 = "")
    : type(t), code(c), op1(o1), op2(o2) {
  }

  void printIA32(unsigned int ni) const {
    for (unsigned int i = 0; i < ni; i++) {
      cout << "\t";
    }
    switch (type) {
    case TYPE_NULL_IA32:
      cout << "TYPE_NULL_IA32 instruction" << endl;
      break;
    case TYPE_LABEL_IA32:
      cout << code << ":" << endl;
      break;
    case TYPE_SECT0_IA32:
      cout << "TYPE_SECT0_IA32 instruction:\t" << code << endl;
      break;
    case TYPE_SECT1_IA32:
      cout << "TYPE_SECT1_IA32 instruction:\t" << code << "\t" << op1 << endl;
      break;
    case TYPE_SECT2_IA32:
      cout << "TYPE_SECT2_IA32 instruction:\t" << code << "\t" << op1 << ", " << op2 << endl;
      break;
    case TYPE_INSTR0_IA32:
      cout << "\t" << code << endl;
      break;
    case TYPE_INSTR1_IA32:
      cout << "\t" << code << "\t" << op1 << endl;
      break;
    case TYPE_INSTR2_IA32:
      cout << "\t" << code << "\t" << op1 << ", " << op2 << endl;
      break;
    }
  }
};

struct tInstrBasic {
  tInstrRCode        instrRC;
  set<string>        liveAfter;
  deque<tInstrIA32>  preInstrIA;
  deque<tInstrIA32>  instrIA;
  deque<tInstrIA32>  postInstrIA;
  
  tInstrBasic(tInstrRCode ti)
    : instrRC(ti) {
  }
};

const int NO_EXIT            = 0;
const int UNCONDITIONAL_EXIT = 1;
const int CONTINUE_EXIT      = 2;
const int CONDITIONAL_EXIT   = 3;

struct tBlocBasic {
  string               label;
  int                  nFirstInstr;
  int                  nLastInstr;
  vector<tInstrBasic>  instrs;        
  int                  typeExit;
  string               label_exit1;
  string               label_exit2;
  int                  bloc_exit1;
  int                  bloc_exit2;
  deque<tInstrIA32>    exch_exit1;
  deque<tInstrIA32>    exch_exit2;
  set<string>          utilitzats;
  set<string>          assignats;
  set<string>          liveBefore;
  set<string>          liveAfter;
  map<string, string>  initialMapIA32;
  map<string, string>  finalMapIA32;
};

struct tBloc {
  string             label;
  offsets            offsets_params;
  offsets            offsets_vars;        
  int                size_of_vars;
  vector<tBlocBasic> blocsBasics;
  map<string, int>   mLabelsBasics;
  vector<string>     modifiedIAregsInBloc;
};

void printBlocs(vector<tBloc> & vBlocs);

void compute_live_registers(vector<tBloc> & vBlocs);

void registres_instruccio(tInstrRCode i, set<string> &usats, string &assignat);

#endif
