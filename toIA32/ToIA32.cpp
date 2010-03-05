
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include "ToIA32.hpp"
#include "RCodeParser.hpp"
#include "Blocs.hpp"
#include "RegsMgr.hpp"

using namespace antlr;
using namespace std;

#define SIN_MEMCPY
#define PROGRAM_ES_MAIN

unsigned int curr_bloc_idx;
string curr_bloc_name;
string str_size_vars;
unsigned int fjmp_number;

// per a salvar els registres abans de cridar a funcions de i/o o sistema
vector<string> pushedRegs;

void translateRCodeInstrIA32(tInstrBasic & ib, RegsMgr & regsMgr, map<string, int> & mOffsets, map<string, int> & mSizes, const vector<string> & vStrWRTS);
void pushPhysicalRegsInUse(tInstrBasic & ib, RegsMgr & regsMgr);
void pushPhysicalRegsInUseExcept(const string & op1, tInstrBasic & ib, RegsMgr & regsMgr);
void popPhysicalRegsInUse(tInstrBasic & ib);

void exchangeInstrucionsBetweenBlocsBasics(tBloc & bloc);
void exchangeInstrucionsBetweenBlocsBasics(vector<tBloc> & vBlocs);

void blocBeginSaveIAregs(const vector<string> & regs, const string & label, tInstrBasic & ib);
void blocEndRestoreIAregs(const vector<string> & regs, const string & label, tInstrBasic & ib);
void blocSaveAndRestoreIAregs(tBloc & bloc);

void printVBlocsIA32(tInstrBasic & ib, ostream & os = cout);
void printIA32(tInstrIA32 inst, ostream & os = cout);



void translateVBlocsIA32(vector<tBloc> & vBlocs, RegsMgr & regsMgr, map<string, int> & mOffsets, map<string, int> & mSizes, const vector<string> & vStrWRTS) {

  fjmp_number = 0;
  for (unsigned int i = 0; i < vBlocs.size(); i++) {
    curr_bloc_idx = i;
    curr_bloc_name = vBlocs[i].label;
    stringstream ss (stringstream::out);
    ss << "$" << vBlocs[i].size_of_vars;
    str_size_vars = ss.str();
    regsMgr.restartNewBloc();
    for (unsigned int j = 0; j < vBlocs[i].blocsBasics.size(); j++) {
      regsMgr.restartNewBlocBasic();
      regsMgr.addNonUsedMappings(vBlocs[i].blocsBasics[j].liveBefore, vBlocs[i].blocsBasics[j].utilitzats);
      for (unsigned int k = 0; k < vBlocs[i].blocsBasics[j].instrs.size(); k++) {
        translateRCodeInstrIA32(vBlocs[i].blocsBasics[j].instrs[k], regsMgr, mOffsets, mSizes, vStrWRTS);
        regsMgr.unmapRegsNotInSet(vBlocs[i].blocsBasics[j].instrs[k].liveAfter);
      }
      vBlocs[i].blocsBasics[j].initialMapIA32 = regsMgr.getInitialMapping();
      vBlocs[i].blocsBasics[j].finalMapIA32 = regsMgr.getCurrentMapping();
    }
    vBlocs[i].modifiedIAregsInBloc = regsMgr.getModifiedIAregsInBloc();
    blocSaveAndRestoreIAregs(vBlocs[i]);
    exchangeInstrucionsBetweenBlocsBasics(vBlocs[i]);
  }
}

void translateRCodeInstrIA32(tInstrBasic & ib, RegsMgr & regsMgr, map<string, int> & mOffsets, map<string, int> & mSizes, const vector<string> & vStrWRTS) {
  bool   readOp1RC, readOp2RC, readOp3RC;
  bool   writeOp1RC, writeOp2RC, writeOp3RC;
  string op1IA, op2IA, op3IA;
  bool   isPhyReg1, isPhyReg2, isPhyReg3;
  bool   isABCDReg1, isABCDReg2, isABCDReg3;
  bool   isMemReg1, isMemReg2, isMemReg3;
  bool   isMem1, isMem2, isMem3;
  bool   isInm1, isInm2, isInm3;
  string regTemp, regBck;
  string regTemp1, regBck1;
  string regTemp2, regBck2;

  regsMgr.unlockRegisters();

  readOp1RC = readOp2RC = readOp3RC = false;
  writeOp1RC = writeOp2RC = writeOp3RC = false;

  op1IA = op2IA = op3IA = "";
  isPhyReg1 = isPhyReg2 = isPhyReg3 = false;
  isABCDReg1 = isABCDReg2 = isABCDReg3 = false;
  isMemReg1 = isMemReg2 = isMemReg3 = false;
  isMem1 = isMem2 = isMem3 = false;
  isInm1 = isInm2 = isInm3 = false;

  // Instruccions que LLEGEIXEN del PRIMER operand
  readOp1RC = (ib.instrRC.cod == "aload" || ib.instrRC.cod == "iload" ||
	       ib.instrRC.cod == "load" ||
	       ib.instrRC.cod == "addi" || ib.instrRC.cod == "subi" ||
	       ib.instrRC.cod == "muli" || ib.instrRC.cod == "divi" ||
	       ib.instrRC.cod == "mini" ||
	       ib.instrRC.cod == "equi" ||
	       ib.instrRC.cod == "grti" || ib.instrRC.cod == "lesi" ||
	       ib.instrRC.cod == "land" || ib.instrRC.cod == "loor" ||
	       ib.instrRC.cod == "lnot" ||
	       ib.instrRC.cod == "stor" ||
	       ib.instrRC.cod == "copy" ||
	       ib.instrRC.cod == "wrii" || ib.instrRC.cod == "wrli" ||
	       ib.instrRC.cod == "wris" || ib.instrRC.cod == "wrls" ||
	       ib.instrRC.cod == "etiq" ||
	       ib.instrRC.cod == "ujmp" ||
	       ib.instrRC.cod == "fjmp" ||
	       ib.instrRC.cod == "pushparam" || 
	       ib.instrRC.cod == "call" ||
               ib.instrRC.cod == "new" ||
               ib.instrRC.cod == "free");

  // Instruccions que ESCRIUEN al PRIMER operand
  writeOp1RC = (ib.instrRC.cod == "reai" ||
		ib.instrRC.cod == "popparam");


  // Instruccions que LLEGEIXEN del SEGON operand
  readOp2RC = (ib.instrRC.cod == "addi" || ib.instrRC.cod == "subi" ||
	       ib.instrRC.cod == "muli" || ib.instrRC.cod == "divi" ||
	       ib.instrRC.cod == "equi" ||
	       ib.instrRC.cod == "grti" || ib.instrRC.cod == "lesi" ||
	       ib.instrRC.cod == "land" || ib.instrRC.cod == "loor" ||
	       ib.instrRC.cod == "stor" ||
	       ib.instrRC.cod == "copy" ||
	       ib.instrRC.cod == "fjmp");

  // Instruccions que ESCRIUEN al SEGON operand
  writeOp2RC = (ib.instrRC.cod == "aload" || ib.instrRC.cod == "iload" ||
	        ib.instrRC.cod == "load" ||
	        ib.instrRC.cod == "mini" ||
	        ib.instrRC.cod == "lnot" ||
                ib.instrRC.cod == "new");


  // Instruccions que LLEGEIXEN del TERCER operand
  readOp3RC = (ib.instrRC.cod == "copy");

  // Instruccions que ESCRIUEN al TERCER operand
  writeOp3RC = (ib.instrRC.cod == "addi" || ib.instrRC.cod == "subi" ||
	        ib.instrRC.cod == "muli" || ib.instrRC.cod == "divi" ||
		ib.instrRC.cod == "equi" ||
		ib.instrRC.cod == "grti" || ib.instrRC.cod == "lesi" ||
		ib.instrRC.cod == "land" || ib.instrRC.cod == "loor");


  ib.instrIA.clear();
  ib.preInstrIA.clear();
  ib.postInstrIA.clear();

  // Pretractament de l'instruccio de divisio entera DIVI
  // Es necesiten els registres FISICS EAX i EDX
  string EAX_Bck, EDX_Bck;
  EAX_Bck = EDX_Bck = "";
  if (ib.instrRC.cod == "divi") {
    regsMgr.getTemporalyEAXEDX(EAX_Bck, EDX_Bck);
    if (EAX_Bck != "") {
      ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", "%eax", EAX_Bck));
      ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", EAX_Bck, "%eax"));
    }
    if (EDX_Bck != "") {
      ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", "%edx", EDX_Bck));
      ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", EDX_Bck, "%edx"));
    }
  }

#ifdef SIN_MEMCPY
  // Pretractament de l'instruccio COPY (sin memcpy)
  // Es necesiten els registres FISICS ECX, ESI i EDI
  string ECX_Bck, ESI_Bck, EDI_Bck;
  ECX_Bck = ESI_Bck = EDI_Bck = "";
  if (ib.instrRC.cod == "copy") {
    regsMgr.getTemporalyECXESIEDI(ECX_Bck, ESI_Bck, EDI_Bck);
    if (ECX_Bck != "") {
      ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", "%ecx", ECX_Bck));
      ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", ECX_Bck, "%ecx"));
    }
    if (ESI_Bck != "") {
      ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", "%esi", ESI_Bck));
      ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", ESI_Bck, "%esi"));
    }
    if (EDI_Bck != "") {
      ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", "%edi", EDI_Bck));
      ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", EDI_Bck, "%edi"));
    }
  }
#endif

  // Pretractament de l'instruccio NEW
  // Es necesita el registre FISIC EAX
  EAX_Bck = "";
  if (ib.instrRC.cod == "new") {
    regsMgr.getTemporalyEAX(EAX_Bck);
    if (EAX_Bck != "") {
      ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", "%eax", EAX_Bck));
      ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", EAX_Bck, "%eax"));
    }
  }

  stringstream ss1 (stringstream::in | stringstream::out);
  int offs;

  // Si existeix el PRIMER operand
  if (readOp1RC || writeOp1RC) {
    switch (ib.instrRC.op1.mode) {
    case MODE_RCODE_REGISTER:
      regsMgr.mapRegister(ib.instrRC.op1.opnd, readOp1RC, op1IA, isPhyReg1);
      isMem1 = !isPhyReg1;
      isMemReg1 = isMem1;
      isABCDReg1 = (op1IA == "%eax" || op1IA == "%ebx" ||
                    op1IA == "%ecx" || op1IA == "%edx");
      break;
    case MODE_RCODE_LOCAL:
      offs = - mOffsets[curr_bloc_name + ":" + ib.instrRC.op1.opnd] -
               mSizes[curr_bloc_name + ":" + ib.instrRC.op1.opnd] + 4;
      ss1 << offs << "(%ebp)";
      op1IA = ss1.str();
      isMem1 = true;
      break;
    case MODE_RCODE_VALUE:
      op1IA = "$" + ib.instrRC.op1.opnd;
      isInm1 = true;
      break;
    case MODE_RCODE_LABEL:
      if (ib.instrRC.cod == "ujmp" || ib.instrRC.cod == "etiq") {
        ss1 << "_" << curr_bloc_idx;
	op1IA = ib.instrRC.op1.opnd + ss1.str();
      } else {
	op1IA = ib.instrRC.op1.opnd;
      }
      break;
    case MODE_RCODE_OFFSET:
      offs = - mOffsets[ib.instrRC.op1.opnd] - mSizes[ib.instrRC.op1.opnd] + 4;
      ss1 << offs;
      op1IA = "$" + ss1.str();
      isInm1 = true;
      break;
    case MODE_RCODE_STRING:
      unsigned int i = 0;
      bool trobat = false;
      while (!trobat && i < vStrWRTS.size()) {
        if (ib.instrRC.op1.opnd == vStrWRTS[i]) {
	  trobat = true;
	} else {
          i++;
	}
      }
      ss1 << i;
      op1IA = "$.STRwrts" + ss1.str();
      break;
    }
  }

  stringstream ss2(stringstream::out);
  // Si existeix el SEGON operand
  if (readOp2RC || writeOp2RC) {
    switch (ib.instrRC.op2.mode) {
    case MODE_RCODE_REGISTER:
      regsMgr.mapRegister(ib.instrRC.op2.opnd, readOp2RC, op2IA, isPhyReg2);
      isMem2 = !isPhyReg2;
      isMemReg2 = isMem2;
      isABCDReg2 = (op2IA == "%eax" || op2IA == "%ebx" ||
                    op2IA == "%ecx" || op2IA == "%edx");
      break;
    case MODE_RCODE_LOCAL:
      offs = - mOffsets[curr_bloc_name + ":" + ib.instrRC.op2.opnd] -
               mSizes[curr_bloc_name + ":" + ib.instrRC.op2.opnd] + 4;
      ss2 << offs << "(%ebp)";
      op2IA = ss2.str();
      isMem2 = true;
      break;
    case MODE_RCODE_VALUE:
      op2IA = "$" + ib.instrRC.op2.opnd;
      isInm2 = true;
      break;
    case MODE_RCODE_LABEL:
      if (ib.instrRC.cod == "fjmp") {
        ss2 << "_" << curr_bloc_idx;
	op2IA = ib.instrRC.op2.opnd + ss2.str();
      } else {
	op2IA = ib.instrRC.op2.opnd;
      }
      break;
    case MODE_RCODE_OFFSET:
      offs = - mOffsets[ib.instrRC.op2.opnd] - mSizes[ib.instrRC.op2.opnd] + 4;
      ss2 << offs;
      op2IA = "$" + ss2.str();
      isInm2 = true;
      break;
    }
  }

  stringstream ss3(stringstream::out);
  // Si existeix el TERCER operand
  if (readOp3RC || writeOp3RC) {
    switch (ib.instrRC.op3.mode) {
    case MODE_RCODE_REGISTER:
      regsMgr.mapRegister(ib.instrRC.op3.opnd, readOp3RC, op3IA, isPhyReg3);
      isMem3 = !isPhyReg3;
      isMemReg3 = isMem3;
      isABCDReg3 = (op3IA == "%eax" || op3IA == "%ebx" ||
                    op3IA == "%ecx" || op3IA == "%edx");
      break;
    case MODE_RCODE_LOCAL:
      offs = - mOffsets[curr_bloc_name + ":" + ib.instrRC.op3.opnd] -
               mSizes[curr_bloc_name + ":" + ib.instrRC.op3.opnd] + 4;
      ss3 << offs << "(%ebp)";
      op3IA = ss3.str();
      isMem3 = true;
      break;
    case MODE_RCODE_VALUE:
      op3IA = "$" + ib.instrRC.op3.opnd;
      isInm3 = true;
      break;
    case MODE_RCODE_LABEL:
      op3IA = ib.instrRC.op3.opnd;
      break;
    case MODE_RCODE_OFFSET:
      offs = - mOffsets[ib.instrRC.op3.opnd] - mSizes[ib.instrRC.op3.opnd] + 4;
      ss3 << offs;
      op3IA = "$" + ss3.str();
      isInm3 = true;
      break;
    }
  }


  // Instruccio ADDI
  if (ib.instrRC.cod == "addi") {
    if (ib.instrRC.op3.opnd == ib.instrRC.op1.opnd) {
      if (!isMem3 || !isMem2) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", op2IA, op3IA));
      } else {
        regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
        if (regBck != "") {
          ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
          ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
	}
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", regTemp, op3IA));
      }
    } else if (ib.instrRC.op3.opnd == ib.instrRC.op2.opnd) {  // addi es commutativa
      if (!isMem3 || !isMem1) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", op1IA, op3IA));
      } else {
        regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
        if (regBck != "") {
          ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
          ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
	}
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", regTemp, op3IA));
      }
    } else {
      if (!isMem3) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, op3IA));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", op2IA, op3IA));
      } else if (!isMem1 && !isMem2) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, op3IA));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", op2IA, op3IA));
      } else {
	regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
	if (regBck != "") {
	  ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
	  ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
	}
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", op2IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op3IA));
      }
    }
  // Instruccio SUBI
  } else if (ib.instrRC.cod == "subi") {
     if (ib.instrRC.op3.opnd == ib.instrRC.op1.opnd) {
       if (!isMem3 || !isMem2) {
	 ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "subl", op2IA, op3IA));
       } else {
	 regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
	 if (regBck != "") {
	   ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
	   ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
	 }
	 ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, regTemp));
	 ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "subl", regTemp, op3IA));
       }
     } else if (ib.instrRC.op3.opnd == ib.instrRC.op2.opnd) {
	 ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "negl", op3IA));
         if (!isMem3 || !isMem1) {
	   ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", op1IA, op3IA));
	 } else {
	   regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
	   if (regBck != "") {
	     ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
	     ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
	   }
	   ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, regTemp));
	   ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", regTemp, op3IA));
	 }
     } else {
       if (!isMem3) {
	 ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, op3IA));
	 ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "subl", op2IA, op3IA));
       } else if (!isMem1 && !isMem2) {
	 ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, op3IA));
	 ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "subl", op2IA, op3IA));
       } else {
	 regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
	 if (regBck != "") {
	   ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
	   ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
	 }
	 ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, regTemp));
	 ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "subl", op2IA, regTemp));
	 ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op3IA));
       }
     }
  // Instruccio MULI
  } else if (ib.instrRC.cod == "muli") {
    if (ib.instrRC.op3.opnd == ib.instrRC.op2.opnd) {
      if (isPhyReg3) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "imull", op1IA, op3IA));
      } else {
        regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
        if (regBck != "") {
          ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
          ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
	}
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "imull", op1IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op3IA));
      }
    } else if (ib.instrRC.op3.opnd == ib.instrRC.op1.opnd) {  // muli es commutativa
      if (isPhyReg3) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "imull", op2IA, op3IA));
      } else {
        regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
        if (regBck != "") {
          ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
          ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
	}
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "imull", op1IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op3IA));
      }
    } else {
      if (isPhyReg3) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, op3IA));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "imull", op1IA, op3IA));
      } else {
	regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
	if (regBck != "") {
	  ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
	  ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
	}
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "imull", op1IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op3IA));
      }
    }
  // Instruccio DIVI
  } else if (ib.instrRC.cod == "divi") {
    if (op1IA != "eax_bck") {
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, "%eax"));
    }
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR0_IA32, "cltd"));
    if (isInm2) {
      regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
      if (regBck != "") {
	ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
	ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, regTemp));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "idivl", regTemp));
    } else {
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "idivl", op2IA));
    }
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", "%eax", op3IA));
  // Instruccio EQUI
  } else if (ib.instrRC.cod == "equi") {
    if (isABCDReg3) {
      if ((isInm1 && isInm2) || (isMem1 && isMem2)) { // op3 != op1 && op3 != op2
                                                      // porque ABCD3 Mem1 Mem2
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, op3IA));
        ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", op1IA, op3IA));
      } else {
        ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", op1IA, op2IA));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "sete", RegsMgr::getLowByte(op3IA)));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "andl", "$0xFF", op3IA));
    } else {
      regsMgr.getTemporalyOneOfEAXEBXECXEDX(regTemp, regBck);
      if (regBck != "") {
        ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
        ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
      }
      if ((isInm1 && isInm2) || (isMem1 && isMem2)) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, regTemp));
        ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", op1IA, regTemp));
      } else {
        ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", op1IA, op2IA));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "sete", RegsMgr::getLowByte(regTemp)));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op3IA));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "andl", "$0xFF", op3IA));
    }
  // Instruccio GRTI
  } else if (ib.instrRC.cod == "grti") {
    if (isABCDReg3) {
      if ((isInm1 && isInm2) || (isMem1 && isMem2)) { // op3 != op1 && op3 != op2
                                                      // porque ABCD3 Mem1 Mem2

	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, op3IA));
        ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", op1IA, op3IA));
      } else {
        ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", op1IA, op2IA));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "setl", RegsMgr::getLowByte(op3IA)));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "andl", "$0xFF", op3IA));
    } else {
      regsMgr.getTemporalyOneOfEAXEBXECXEDX(regTemp, regBck);
      if (regBck != "") {
        ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
        ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
      }
      if ((isInm1 && isInm2) || (isMem1 && isMem2)) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, regTemp));
        ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", op1IA, regTemp));
      } else {
        ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", op1IA, op2IA));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "setl", RegsMgr::getLowByte(regTemp)));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op3IA));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "andl", "$0xFF", op3IA));
    }
  // Instruccio LESI
  } else if (ib.instrRC.cod == "lesi") {
    if (isABCDReg3) {
      if ((isInm1 && isInm2) || (isMem1 && isMem2)) { // op3 != op1 && op3 != op2
                                                      // porque ABCD3 Mem1 Mem2

	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, op3IA));
        ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", op1IA, op3IA));
      } else {
        ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", op1IA, op2IA));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "setg", RegsMgr::getLowByte(op3IA)));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "andl", "$0xFF", op3IA));
    } else {
      regsMgr.getTemporalyOneOfEAXEBXECXEDX(regTemp, regBck);
      if (regBck != "") {
        ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
        ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
      }
      if ((isInm1 && isInm2) || (isMem1 && isMem2)) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, regTemp));
        ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", op1IA, regTemp));
      } else {
        ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", op1IA, op2IA));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "setg", RegsMgr::getLowByte(regTemp)));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op3IA));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "andl", "$0xFF", op3IA));
    }
  // Instruccio LAND
  } else if (ib.instrRC.cod == "land") {
    if (ib.instrRC.op3.opnd == ib.instrRC.op1.opnd) {
      if (!isMem3 || !isMem2) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "andl", op2IA, op3IA));
      } else {
        regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
        if (regBck != "") {
          ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
          ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
	}
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "andl", regTemp, op3IA));
      }
    } else if (ib.instrRC.op3.opnd == ib.instrRC.op2.opnd) {  // land es commutativa
      if (!isMem3 || !isMem1) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "andl", op1IA, op3IA));
      } else {
        regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
        if (regBck != "") {
          ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
          ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
	}
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "andl", regTemp, op3IA));
      }
    } else {
      if (!isMem3) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, op3IA));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "andl", op2IA, op3IA));
      } else if (!isMem1 && !isMem2) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, op3IA));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "andl", op2IA, op3IA));
      } else {
	regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
	if (regBck != "") {
	  ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
	  ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
	}
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "andl", op2IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op3IA));
      }
    }
  // Instruccio LOOR
  } else if (ib.instrRC.cod == "loor") {
    if (ib.instrRC.op3.opnd == ib.instrRC.op1.opnd) {
      if (!isMem3 || !isMem2) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "orl", op2IA, op3IA));
      } else {
        regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
        if (regBck != "") {
          ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
          ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
	}
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "orl", regTemp, op3IA));
      }
    } else if (ib.instrRC.op3.opnd == ib.instrRC.op2.opnd) {  // loor es commutativa
      if (!isMem3 || !isMem1) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "orl", op1IA, op3IA));
      } else {
        regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
        if (regBck != "") {
          ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
          ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
	}
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "orl", regTemp, op3IA));
      }
    } else {
      if (!isMem3) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, op3IA));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "orl", op2IA, op3IA));
      } else if (!isMem1 && !isMem2) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, op3IA));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "orl", op2IA, op3IA));
      } else {
	regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
	if (regBck != "") {
	  ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
	  ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
	}
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "orl", op2IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op3IA));
      }
    }
  // Instruccio MINI
  } else if (ib.instrRC.cod == "mini") {
    if (ib.instrRC.op2.opnd == ib.instrRC.op1.opnd) {
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "negl", op2IA));
    } else {
      if (!isMem2) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, op2IA));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "negl", op2IA));
      } else if (!isMem1) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, op2IA));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "negl", op2IA));
      } else {
	regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
	if (regBck != "") {
	  ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
	  ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
	}
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "negl", regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op2IA));
      }
    }
  // Instruccio LNOT
  } else if (ib.instrRC.cod == "lnot") {
    if (isABCDReg2) {
      if (!isInm1) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", "$0", op1IA));
      } else {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, op2IA));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", "$0", op2IA));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "sete", RegsMgr::getLowByte(op2IA)));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "andl", "$0xFF", op2IA));
    } else {
      regsMgr.getTemporalyOneOfEAXEBXECXEDX(regTemp, regBck);
      if (regBck != "") {
        ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
        ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
      }
      if (!isInm1) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", "$0", op1IA));
      } else {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, op2IA));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", "$0", op2IA));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "sete", RegsMgr::getLowByte(regTemp)));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op2IA));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "andl", "$0xFF", op2IA));
    }
  // Instruccio ALOAD
  } else if (ib.instrRC.cod == "aload") {
    if (isPhyReg2) {
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "leal", op1IA, op2IA));
    } else { // op1IA sempre es un simbol
      regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
      if (regBck != "") {
        ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
        ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "leal", op1IA, regTemp));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op2IA));
    }
  // Instruccio ILOAD
  } else if (ib.instrRC.cod == "iload") {
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, op2IA));
  // Instruccio LOAD
  } else if (ib.instrRC.cod == "load") {
    if (isPhyReg1 && isPhyReg2) {
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", "(" + op1IA + ")", op2IA));
    } else if (isPhyReg1 && isMem2) {
      regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
      if (regBck != "") {
	ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
	ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", "(" + op1IA + ")", regTemp));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op2IA));
    } else if (isMemReg1 && isPhyReg2) {
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, op2IA));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", "(" + op2IA + ")", op2IA));
    } else if (isMemReg1 && isMem2) {
      regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
      if (regBck != "") {
	ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
	ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, regTemp));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", "(" + regTemp + ")", regTemp));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op2IA));
    } else if (isMem1 && isPhyReg2) {
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, op2IA));
    } else if (isMem1 && isMem2) {
	regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
	if (regBck != "") {
	  ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
	  ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
	}
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, regTemp));
	ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op2IA));
    } else {
      cout << ">>>>>>>>>>>>>>>>>> load _x _y : SIN DEFINIR!!" << endl;
    }
  // Instruccio STOR
  } else if (ib.instrRC.cod == "stor") {
    if (isPhyReg2 && (isPhyReg1 || isInm1)) {
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, "(" + op2IA + ")"));
    } else if (isPhyReg2 && isMem1) {
      regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
      if (regBck != "") {
	ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
	ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, regTemp));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, "(" + op2IA + ")"));
    } else if (isMemReg2 && (isPhyReg1 || isInm1)) {
      regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
      if (regBck != "") {
	ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
	ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, regTemp));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, "(" + regTemp + ")"));
    } else if (isMemReg2 && isMem1) {
      regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
      if (regBck != "") {
	ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
	ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, regTemp));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", "(" + regTemp + ")", regTemp));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, regTemp));
    } else if (isMem2 && (isPhyReg1 || isInm1)) {
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, op2IA));
    } else if (isMem2 && isMem1) {
      regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
      if (regBck != "") {
	ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
	ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, regTemp));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op2IA));
    } else {
      cout << "STOR: caso no definido!!" << endl;
    }
  // Instruccio COPY
  } else if (ib.instrRC.cod == "copy") {
#ifdef SIN_MEMCPY
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, "%esi")); 
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op2IA, "%edi")); 
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op3IA, "%ecx")); 
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR0_IA32, "rep"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR0_IA32, "movsb"));
#else
    // memcpy encara no funciona
    pushPhysicalRegsInUse(ib, regsMgr);
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", op3IA));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", op1IA));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", op2IA));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "call", "memcpy"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", "$12", "%esp"));
    popPhysicalRegsInUse(ib);
#endif
  // Instruccio ETIQ
  } else if (ib.instrRC.cod == "etiq") {
    ib.instrIA.push_back(tInstrIA32(TYPE_LABEL_IA32, op1IA));
  // Instruccio UJMP
  } else if (ib.instrRC.cod == "ujmp") {
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "jmp", op1IA));
  // Instruccio FJMP
  } else if (ib.instrRC.cod == "fjmp") {
    stringstream ss_fjmp (stringstream::out);
    ss_fjmp << ".FJMP_" << ++fjmp_number << "_";
    if (!isInm1) {
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", "$0", op1IA));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "je", ss_fjmp.str() + "jump"));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "jmp", ss_fjmp.str() + "cont"));
      ib.instrIA.push_back(tInstrIA32(TYPE_LABEL_IA32, ss_fjmp.str() + "jump"));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "jmp", op2IA));
      ib.instrIA.push_back(tInstrIA32(TYPE_LABEL_IA32, ss_fjmp.str() + "cont"));
    } else {
      regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
      if (regBck != "") {
        ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", op1IA, regTemp));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "cmpl", "$0", regTemp));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "je", ss_fjmp.str() + "jump"));
      if (regBck != "") {
        ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "jmp", ss_fjmp.str() + "cont"));
      ib.instrIA.push_back(tInstrIA32(TYPE_LABEL_IA32, ss_fjmp.str() + "jump"));
      if (regBck != "") {
        ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "jmp", op2IA));
      ib.instrIA.push_back(tInstrIA32(TYPE_LABEL_IA32, ss_fjmp.str() + "cont"));
    }
  // Instruccio STOP
  } else if (ib.instrRC.cod == "stop") {
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", str_size_vars, "%esp"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "popl", "%ebp"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR0_IA32, "ret"));
  // Instruccio CALL
  } else if (ib.instrRC.cod == "call") {
    if (ib.instrRC.op1.mode == MODE_RCODE_LABEL) {
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "call", op1IA));
    } else if (ib.instrRC.op1.mode == MODE_RCODE_REGISTER) {
    }
  // Instruccio RETU
  } else if (ib.instrRC.cod == "retu") {
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", str_size_vars, "%esp"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "popl", "%ebp"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR0_IA32, "ret"));
  // Instruccio PUSHPARAM
  } else if (ib.instrRC.cod == "pushparam") {
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", op1IA));
  // Instruccio POPPARAM
  } else if (ib.instrRC.cod == "popparam") {
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "popl", op1IA));
  // Instruccio KILLPARAM
  } else if (ib.instrRC.cod == "killparam") {
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", "$4", "%esp"));
  // Instruccio REAI
  } else if (ib.instrRC.cod == "reai") {
    pushPhysicalRegsInUseExcept(op1IA, ib, regsMgr);
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "$res_reai"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "$.FMTreai"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "call", "scanf"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", "$8", "%esp"));
    if (isPhyReg1) {
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", "res_reai", op1IA));
    } else {
      regsMgr.getTemporalyPhysicalRegister(regTemp, regBck);
      if (regBck != "") {
	ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, regBck));
	ib.postInstrIA.push_front(tInstrIA32(TYPE_INSTR2_IA32, "movl", regBck, regTemp));
      }
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", "res_reai", regTemp));
      ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", regTemp, op1IA));
    }
    popPhysicalRegsInUse(ib);
  // Instruccio WRII
  } else if (ib.instrRC.cod == "wrii") {
    pushPhysicalRegsInUse(ib, regsMgr);
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", op1IA));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "$.FMTwrii"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "call", "printf"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", "$8", "%esp"));
    popPhysicalRegsInUse(ib);
  // Instruccio WRLI
  } else if (ib.instrRC.cod == "wrli") {
    pushPhysicalRegsInUse(ib, regsMgr);
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", op1IA));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "$.FMTwrli"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "call", "printf"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", "$8", "%esp"));
    popPhysicalRegsInUse(ib);
  // Instruccio WRIS
  } else if (ib.instrRC.cod == "wris") {
    pushPhysicalRegsInUse(ib, regsMgr);
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", op1IA));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "$.FMTwris"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "call", "printf"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", "$8", "%esp"));
    popPhysicalRegsInUse(ib);
  // Instruccio WRLS
  } else if (ib.instrRC.cod == "wrls") {
    pushPhysicalRegsInUse(ib, regsMgr);
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", op1IA));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "$.FMTwrls"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "call", "printf"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", "$8", "%esp"));
    popPhysicalRegsInUse(ib);
  // Instruccio WRLN
  } else if (ib.instrRC.cod == "wrln") {
    pushPhysicalRegsInUse(ib, regsMgr);
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "$.FMTwrln"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "call", "printf"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", "$4", "%esp"));
    popPhysicalRegsInUse(ib);
  // Instruccio NEW
  } else if (ib.instrRC.cod == "new") {
    if (op2IA != RegsMgr::registerBackup("%eax")) {
      pushPhysicalRegsInUseExcept(op2IA, ib, regsMgr);
    } else {
      pushPhysicalRegsInUseExcept("%eax", ib, regsMgr);
    }
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", op1IA));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "call", "malloc"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", "$4", "%esp"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "movl", "%eax", op2IA));
    popPhysicalRegsInUse(ib);
  // Instruccio FREE
  } else if (ib.instrRC.cod == "free") {
    pushPhysicalRegsInUse(ib, regsMgr);
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", op1IA));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "call", "free"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "addl", "$4", "%esp"));
    popPhysicalRegsInUse(ib);
  // Instruccio fictiacia BEGIN
  } else if (ib.instrRC.cod == "begin") {
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "%ebp"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "leal", "8(%esp)", "%ebp"));
    ib.instrIA.push_back(tInstrIA32(TYPE_INSTR2_IA32, "subl", str_size_vars, "%esp"));
  }
}

void pushPhysicalRegsInUse(tInstrBasic & ib, RegsMgr & regsMgr) {
  pushedRegs.clear();
  if (regsMgr.isInUsePhysicalRegister("%eax")) {
    ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "%eax"));
    pushedRegs.push_back("%eax");
  }
  if (regsMgr.isInUsePhysicalRegister("%ebx")) {
    ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "%ebx"));
    pushedRegs.push_back("%ebx");
  }
  if (regsMgr.isInUsePhysicalRegister("%ecx")) {
    ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "%ecx"));
    pushedRegs.push_back("%ecx");
  }
  if (regsMgr.isInUsePhysicalRegister("%edx")) {
    ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "%edx"));
    pushedRegs.push_back("%edx");
  }
  if (regsMgr.isInUsePhysicalRegister("%esi")) {
    ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "%esi"));
    pushedRegs.push_back("%esi");
  }
  if (regsMgr.isInUsePhysicalRegister("%edi")) {
    ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "%edi"));
    pushedRegs.push_back("%edi");
  }
}

void pushPhysicalRegsInUseExcept(const string & op1, tInstrBasic & ib, RegsMgr & regsMgr) {
  pushedRegs.clear();
  if (op1 != "%eax" && regsMgr.isInUsePhysicalRegister("%eax")) {
    ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "%eax"));
    pushedRegs.push_back("%eax");
  }
  if (op1 != "%ebx" && regsMgr.isInUsePhysicalRegister("%ebx")) {
    ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "%ebx"));
    pushedRegs.push_back("%ebx");
  }
  if (op1 != "%ecx" && regsMgr.isInUsePhysicalRegister("%ecx")) {
    ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "%ecx"));
    pushedRegs.push_back("%ecx");
  }
  if (op1 != "%edx" && regsMgr.isInUsePhysicalRegister("%edx")) {
    ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "%edx"));
    pushedRegs.push_back("%edx");
  }
  if (op1 != "%esi" && regsMgr.isInUsePhysicalRegister("%esi")) {
    ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "%esi"));
    pushedRegs.push_back("%esi");
  }
  if (op1 != "%edi" && regsMgr.isInUsePhysicalRegister("%edi")) {
    ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", "%edi"));
    pushedRegs.push_back("%edi");
  }
}

void popPhysicalRegsInUse(tInstrBasic & ib) {
  for (unsigned int i = pushedRegs.size(); i > 0; i--) {
    ib.postInstrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "popl", pushedRegs[i - 1]));
  }
}

void exchangeInstrucionsBetweenBlocsBasics(tBloc & bloc) {
  for (unsigned int i = 0; i < bloc.blocsBasics.size(); i++) {
    bloc.blocsBasics[i].exch_exit1.clear();
    bloc.blocsBasics[i].exch_exit2.clear();
    switch (bloc.blocsBasics[i].typeExit) {
    case NO_EXIT:
      break;
    case UNCONDITIONAL_EXIT:
      for (map<string, string>::const_iterator it = bloc.blocsBasics[bloc.blocsBasics[i].bloc_exit1].initialMapIA32.begin(); it != bloc.blocsBasics[bloc.blocsBasics[i].bloc_exit1].initialMapIA32.end(); it++) {
	if (it->second != bloc.blocsBasics[i].finalMapIA32[it->first]) {
	  bloc.blocsBasics[i].exch_exit1.push_front(tInstrIA32(TYPE_INSTR1_IA32, "pushl", bloc.blocsBasics[i].finalMapIA32[it->first]));
	  bloc.blocsBasics[i].exch_exit1.push_back(tInstrIA32(TYPE_INSTR1_IA32, "popl", it->second));
	}
      }
      break;
    case CONTINUE_EXIT:
      for (map<string, string>::const_iterator it = bloc.blocsBasics[bloc.blocsBasics[i].bloc_exit1].initialMapIA32.begin(); it != bloc.blocsBasics[bloc.blocsBasics[i].bloc_exit1].initialMapIA32.end(); it++) {
	if (it->second != bloc.blocsBasics[i].finalMapIA32[it->first]) {
	  bloc.blocsBasics[i].exch_exit1.push_front(tInstrIA32(TYPE_INSTR1_IA32, "pushl", bloc.blocsBasics[i].finalMapIA32[it->first]));
	  bloc.blocsBasics[i].exch_exit1.push_back(tInstrIA32(TYPE_INSTR1_IA32, "popl", it->second));
	}
      }
      break;
    case CONDITIONAL_EXIT:
      for (map<string, string>::const_iterator it = bloc.blocsBasics[bloc.blocsBasics[i].bloc_exit1].initialMapIA32.begin(); it != bloc.blocsBasics[bloc.blocsBasics[i].bloc_exit1].initialMapIA32.end(); it++) {
	if (it->second != bloc.blocsBasics[i].finalMapIA32[it->first]) {
	  bloc.blocsBasics[i].exch_exit1.push_front(tInstrIA32(TYPE_INSTR1_IA32, "pushl", bloc.blocsBasics[i].finalMapIA32[it->first]));
	  bloc.blocsBasics[i].exch_exit1.push_back(tInstrIA32(TYPE_INSTR1_IA32, "popl", it->second));
	}
      }
      for (map<string, string>::const_iterator it = bloc.blocsBasics[bloc.blocsBasics[i].bloc_exit2].initialMapIA32.begin(); it != bloc.blocsBasics[bloc.blocsBasics[i].bloc_exit2].initialMapIA32.end(); it++) {
	if (it->second != bloc.blocsBasics[i].finalMapIA32[it->first]) {
	  bloc.blocsBasics[i].exch_exit2.push_front(tInstrIA32(TYPE_INSTR1_IA32, "pushl", bloc.blocsBasics[i].finalMapIA32[it->first]));
	  bloc.blocsBasics[i].exch_exit2.push_back(tInstrIA32(TYPE_INSTR1_IA32, "popl", it->second));
	}
      }
      break;
    }
  }
}

void blocBeginSaveIAregs(const vector<string> & regs, const string & label, tInstrBasic & ib) {
  for (unsigned int i = 0; i < regs.size(); i++) {
    if (label != "program" || RegsMgr::isPhysicalIAreg(regs[i])) {
      ib.postInstrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "pushl", regs[i]));
    }
  }
}

void blocEndRestoreIAregs(const vector<string> & regs, const string & label, tInstrBasic & ib) {
  for (int i = regs.size() - 1; i >= 0; i--) {
    if (label != "program" || RegsMgr::isPhysicalIAreg(regs[i])) {
      ib.preInstrIA.push_back(tInstrIA32(TYPE_INSTR1_IA32, "popl", regs[i]));
    }
  }
}

void blocSaveAndRestoreIAregs(tBloc & bloc) {
  for (unsigned int i = 0; i < bloc.blocsBasics.size(); i++) {
    for (unsigned int j = 0; j < bloc.blocsBasics[i].instrs.size(); j++) {
      if (bloc.blocsBasics[i].instrs[j].instrRC.cod == "begin") {
	blocBeginSaveIAregs(bloc.modifiedIAregsInBloc, bloc.label, bloc.blocsBasics[i].instrs[j]);
      } else if (bloc.blocsBasics[i].instrs[j].instrRC.cod == "stop") {
	blocEndRestoreIAregs(bloc.modifiedIAregsInBloc, bloc.label, bloc.blocsBasics[i].instrs[j]);
      } else if (bloc.blocsBasics[i].instrs[j].instrRC.cod == "retu") {
	blocEndRestoreIAregs(bloc.modifiedIAregsInBloc, bloc.label, bloc.blocsBasics[i].instrs[j]);
      }
    }
  }
}

void printVBlocsIA32(const unsigned int i, vector<tBloc> & vBlocs, ostream & os) {
  for (unsigned int j = 0; j < vBlocs[i].blocsBasics.size(); j++) {
    stringstream ss  (stringstream::out);
    stringstream ss1 (stringstream::out);
    stringstream ss2 (stringstream::out);
    ss << ".EXCH_" << i << "_" << j << "_";
    switch (vBlocs[i].blocsBasics[j].typeExit) {
    case NO_EXIT:
      for (unsigned int k = 0; k < vBlocs[i].blocsBasics[j].instrs.size(); k++) {
	os << "/ ";
	vBlocs[i].blocsBasics[j].instrs[k].instrRC.printInstrRCode(os);
	os << endl;
	printVBlocsIA32(vBlocs[i].blocsBasics[j].instrs[k], os);
      }
      break;
    case CONTINUE_EXIT:
      for (unsigned int k = 0; k < vBlocs[i].blocsBasics[j].instrs.size(); k++) {
	os << "/ ";
	vBlocs[i].blocsBasics[j].instrs[k].instrRC.printInstrRCode(os);
	os << endl;
	printVBlocsIA32(vBlocs[i].blocsBasics[j].instrs[k], os);
      }
      if (vBlocs[i].blocsBasics[j].exch_exit1.size() != 0) {
	ss << vBlocs[i].blocsBasics[j].bloc_exit1 << "_cont";
	printIA32(tInstrIA32(TYPE_LABEL_IA32, ss.str()), os);
	for (unsigned int k = 0; k < vBlocs[i].blocsBasics[j].exch_exit1.size(); k++) {
	  printIA32(vBlocs[i].blocsBasics[j].exch_exit1[k], os);
	}
      }
      break;
    case UNCONDITIONAL_EXIT:
      for (int k = 0; k < int(vBlocs[i].blocsBasics[j].instrs.size()) - 1; k++) {
	os << "/ ";
	vBlocs[i].blocsBasics[j].instrs[k].instrRC.printInstrRCode(os);
	os << endl;
	printVBlocsIA32(vBlocs[i].blocsBasics[j].instrs[k], os);
      }
      os << "/ ";
      vBlocs[i].blocsBasics[j].instrs[vBlocs[i].blocsBasics[j].instrs.size() - 1].instrRC.printInstrRCode(os);
      os << endl;
      if (vBlocs[i].blocsBasics[j].exch_exit1.size() != 0) {
	ss << vBlocs[i].blocsBasics[j].bloc_exit1 << "_jump";
	printIA32(tInstrIA32(TYPE_LABEL_IA32, ss.str()), os);
	for (unsigned int k = 0; k < vBlocs[i].blocsBasics[j].exch_exit1.size(); k++) {
	  printIA32(vBlocs[i].blocsBasics[j].exch_exit1[k], os);
	}
      }
      printVBlocsIA32(vBlocs[i].blocsBasics[j].instrs[vBlocs[i].blocsBasics[j].instrs.size() - 1], os);
      break;
    case CONDITIONAL_EXIT:
      for (int k = 0; k < int(vBlocs[i].blocsBasics[j].instrs.size()) - 1; k++) {
	os << "/ ";
	vBlocs[i].blocsBasics[j].instrs[k].instrRC.printInstrRCode(os);
	os << endl;
	printVBlocsIA32(vBlocs[i].blocsBasics[j].instrs[k], os);
      }
      os << "/ ";
      vBlocs[i].blocsBasics[j].instrs[vBlocs[i].blocsBasics[j].instrs.size() - 1].instrRC.printInstrRCode(os);
      os << endl;
      deque<tInstrIA32> lIA32 = vBlocs[i].blocsBasics[j].instrs[vBlocs[i].blocsBasics[j].instrs.size() - 1].instrIA;
      for (unsigned int k = 0; k < lIA32.size() - 2; k++) {
	printIA32(lIA32[k], os);
      }
      if (vBlocs[i].blocsBasics[j].exch_exit2.size() != 0) {
	ss2 << ss.str() << vBlocs[i].blocsBasics[j].bloc_exit2 << "_jump";
	printIA32(tInstrIA32(TYPE_LABEL_IA32, ss2.str()), os);
	for (unsigned int k = 0; k < vBlocs[i].blocsBasics[j].exch_exit2.size(); k++) {
	  printIA32(vBlocs[i].blocsBasics[j].exch_exit2[k], os);
	}
      }
      printIA32(lIA32[lIA32.size() - 2], os);
      printIA32(lIA32[lIA32.size() - 1], os);
      if (vBlocs[i].blocsBasics[j].exch_exit1.size() != 0) {
	ss1 << ss.str() << vBlocs[i].blocsBasics[j].bloc_exit1 << "_jump";
	printIA32(tInstrIA32(TYPE_LABEL_IA32, ss1.str()), os);
	for (unsigned int k = 0; k < vBlocs[i].blocsBasics[j].exch_exit1.size(); k++) {
	  printIA32(vBlocs[i].blocsBasics[j].exch_exit1[k], os);
	}
      }
      break;
    }
  }
}

void printVBlocsIA32(tInstrBasic & ib, ostream & os) {
  for (unsigned int i = 0; i < ib.preInstrIA.size(); i++) {
    printIA32(ib.preInstrIA[i], os);
  }
  for (unsigned int i = 0; i < ib.instrIA.size(); i++) {
    printIA32(ib.instrIA[i], os);
  }
  for (unsigned int i = 0; i < ib.postInstrIA.size(); i++) {
    printIA32(ib.postInstrIA[i], os);
  }
}

void printIA32(tInstrIA32 inst, ostream & os) {
  switch (inst.type) {
  case TYPE_NULL_IA32:
    os << "TYPE_NULL_IA32 instruction" << endl;
    break;
  case TYPE_LABEL_IA32:
    os << inst.code << ":" << endl;
    break;
  case TYPE_SECT0_IA32:
    os << "TYPE_SECT0_IA32 instruction:\t" << inst.code << endl;
    break;
  case TYPE_SECT1_IA32:
    os << "TYPE_SECT1_IA32 instruction:\t" << inst.code << "\t" << inst.op1 << endl;
    break;
  case TYPE_SECT2_IA32:
    os << "TYPE_SECT2_IA32 instruction:\t" << inst.code << "\t" << inst.op1 << ", " << inst.op2 << endl;
    break;
  case TYPE_INSTR0_IA32:
    os << "\t" << inst.code << endl;
    break;
  case TYPE_INSTR1_IA32:
    os << "\t" << inst.code << "\t" << inst.op1 << endl;
    break;
  case TYPE_INSTR2_IA32:
    os << "\t" << inst.code << "\t" << inst.op1 << ", " << inst.op2 << endl;
    break;
  }
}

void printIA32code(vector<tBloc> & vBlocs,
                   const unsigned int numRegs,
                   const bool reaiRCInstr,
                   const bool wriiRCInstr, const bool wrliRCInstr,
                   const bool wrisRCInstr, const bool wrlsRCInstr,
                   const bool wrlnRCInstr,
                   const vector<string> vStrWRTS,
                   ostream & os) {
  os << "\t.section\t.data" << endl;
  for (unsigned int i = 7; i <= numRegs; i++) {
    os << "\t.comm aux_" << i << ", 4" << endl;
  }
  if (numRegs >= 1) {
    os << "\t.comm eax_bck, 4" << endl;
  }
  if (numRegs >= 2) {
    os << "\t.comm ebx_bck, 4" << endl;
  }
  if (numRegs >= 3) {
    os << "\t.comm ecx_bck, 4" << endl;
  }
  if (numRegs >= 4) {
    os << "\t.comm edx_bck, 4" << endl;
  }
  if (numRegs >= 5) {
    os << "\t.comm esi_bck, 4" << endl;
  }
  if (numRegs >= 6) {
    os << "\t.comm edi_bck, 4" << endl;
  }
  for (unsigned int i = 7; i <= numRegs; i++) {
    os << "\t.comm aux_" << i << "_bck, 4" << endl;
  }
  if (reaiRCInstr) {
    os << "\t.comm res_reai, 4" << endl;
  }
  if (reaiRCInstr || wriiRCInstr || wrliRCInstr ||
      wrisRCInstr || wrlsRCInstr || wrlnRCInstr) {
    os << endl;
    os << "\t.section\t.rodata" << endl;
  }
  if (reaiRCInstr) {
    os << ".FMTreai:" << endl;
    os << "\t.string \"%d\"" << endl;
  }
  if (wriiRCInstr) {
    os << ".FMTwrii:" << endl;
    os << "\t.string \"%d\"" << endl;
  }
  if (wrliRCInstr) {
    os << ".FMTwrli:" << endl;
    os << "\t.string \"%d\\n\"" << endl;
  }
  if (wrisRCInstr) {
    os << ".FMTwris:" << endl;
    os << "\t.string \"%s\"" << endl;
  }
  if (wrlsRCInstr) {
    os << ".FMTwrls:" << endl;
    os << "\t.string \"%s\\n\"" << endl;
  }
  if (wrlnRCInstr) {
    os << ".FMTwrln:" << endl;
    os << "\t.string \"\\n\"" << endl;
  }
  for (unsigned int i = 0; i < vStrWRTS.size(); i++) {
    os << ".STRwrts" << i << ":" << endl;
    os << "\t.string " << vStrWRTS[i] << endl;
  }
  os << endl;
  os << "\t.section\t.text" << endl;
  os << ".globl main" << endl;
  os << "\t.type\tmain, @function" << endl;
  os << "main:" << endl;
#ifdef PROGRAM_ES_MAIN
  printVBlocsIA32(0, vBlocs, os);
  for (unsigned int i = 1; i < vBlocs.size(); i++) {
#else
  os << "\tleal\t4(%esp), %ecx" << endl;
  os << "\tandl\t$-16, %esp" << endl;
  os << "\tpushl\t-4(%ecx)" << endl;
  os << "\tpushl\t%ebp" << endl;
  os << "\tmovl\t%esp, %ebp" << endl;
  os << "\tpushl\t%ecx" << endl;
  os << "\tcall\tprogram" << endl;
  os << "\tmovl\t$1, %eax" << endl;
  os << "\tpopl\t%ecx" << endl;
  os << "\tpopl\t%ebp" << endl;
  os << "\tleal\t-4(%ecx), %esp" << endl;
  os << "\tret" << endl;
  for (unsigned int i = 0; i < vBlocs.size(); i++) {
#endif
    os << endl;
    os << ".globl\t" << vBlocs[i].label << endl;
    os << "\t.type\t" << vBlocs[i].label << ", @function" << endl;
    os << vBlocs[i].label << ":" << endl;
    printVBlocsIA32(i, vBlocs, os);
  }

}
