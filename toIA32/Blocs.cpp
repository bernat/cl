
#include <vector>
#include <deque>
#include <string>
#include <set>
#include <map>
#include <iostream>
#include "Blocs.hpp"

using namespace std;

void printBlocs(vector<tBloc> & vBlocs) {
  cout << "vBlocs : (" << vBlocs.size() << ")" << endl;
  for (unsigned int i = 0; i < vBlocs.size(); i++) {
    cout << "\tvBlocs[" << i << "].label = " << vBlocs[i].label << endl;
  }
  cout << endl;
  
  for (unsigned int i = 0; i < vBlocs.size(); i++) {
    cout << "\tBloc #" << i << "  (label = " << vBlocs[i].label << ")" << endl;
    cout << "\t\tparams bloc (" << vBlocs[i].offsets_params.size() << ")" << endl;
    for (unsigned int j = 0; j < vBlocs[i].offsets_params.size(); j++) {
      cout << "\t\t\toffset(" << vBlocs[i].label << ":" << vBlocs[i].offsets_params[j].first << ") = " << vBlocs[i].offsets_params[j].second << endl;
    }
    cout << "\t\tvars bloc  (" << vBlocs[i].offsets_vars.size() << ")" << endl;
    for (unsigned int j = 0; j < vBlocs[i].offsets_vars.size(); j++) {
      cout << "\t\t\toffset(" << vBlocs[i].label << ":" << vBlocs[i].offsets_vars[j].first << ") = " << vBlocs[i].offsets_vars[j].second << endl;
    }
    cout << "\t\tsize of vars = " << vBlocs[i].size_of_vars << endl;
    cout << "\t\tbloc basics (" << vBlocs[i].blocsBasics.size() << ")" << endl;
    for (unsigned int j = 0; j < vBlocs[i].blocsBasics.size(); j++) {
      cout << "\t\t\tBloc Basic #" << j << " (label = " << vBlocs[i].blocsBasics[j].label;
      cout << "   first = " << vBlocs[i].blocsBasics[j].nFirstInstr << "   last = " << vBlocs[i].blocsBasics[j].nLastInstr;
      cout << "   #instruccions = " << vBlocs[i].blocsBasics[j].instrs.size() << ")" << endl;
      for (unsigned int k = 0; k < vBlocs[i].blocsBasics[j].instrs.size(); k++) {
	cout << "\t\t\t\t";
	vBlocs[i].blocsBasics[j].instrs[k].instrRC.printInstrRCode();
	
	if (vBlocs[i].blocsBasics[j].instrs[k].instrRC.op1.opnd == "") {
	  cout << "\t\t\t\t\tLiveAfter:  [";
	} else if (vBlocs[i].blocsBasics[j].instrs[k].instrRC.op2.opnd == "") {
	  cout << "\t\t\t\tLiveAfter:  [";
	} else if (vBlocs[i].blocsBasics[j].instrs[k].instrRC.op3.opnd == "") {
	  cout << "\t\t\tLiveAfter:  [";
	} else {
	  cout << "\t\tLiveAfter:  [";
	}
	for (set<string>::const_iterator r = vBlocs[i].blocsBasics[j].instrs[k].liveAfter.begin(); r != vBlocs[i].blocsBasics[j].instrs[k].liveAfter.end(); r++) {
	  cout << " " << *r;
	}
	cout << " ]" << endl;
	
	if (vBlocs[i].blocsBasics[j].instrs[k].preInstrIA.size() > 0) {
	  cout << "\t\t\t\t\tPreInstrIA32:" << endl;
	  for (unsigned int l = 0; l < vBlocs[i].blocsBasics[j].instrs[k].preInstrIA.size(); l++) {
	    vBlocs[i].blocsBasics[j].instrs[k].preInstrIA[l].printIA32(6);
	  }
	}
	cout << "\t\t\t\t\tInstrIA32:" << endl;
	for (unsigned int l = 0; l < vBlocs[i].blocsBasics[j].instrs[k].instrIA.size(); l++) {
	  vBlocs[i].blocsBasics[j].instrs[k].instrIA[l].printIA32(6);
	}
	if (vBlocs[i].blocsBasics[j].instrs[k].postInstrIA.size() > 0) {
	  cout << "\t\t\t\t\tPostInstrIA32:" << endl;
	  for (unsigned int l = 0; l < vBlocs[i].blocsBasics[j].instrs[k].postInstrIA.size(); l++) {
	    vBlocs[i].blocsBasics[j].instrs[k].postInstrIA[l].printIA32(6);
	  }
	}
	
      }

      cout << "\t\t\tutilitzats: [";
      for (set<string>::iterator r=vBlocs[i].blocsBasics[j].utilitzats.begin(); r!=vBlocs[i].blocsBasics[j].utilitzats.end(); r++) cout<<" "<<(*r);
      cout << " ]" << endl;
      cout << "\t\t\tassignats:  [";
      for (set<string>::iterator r=vBlocs[i].blocsBasics[j].assignats.begin(); r!=vBlocs[i].blocsBasics[j].assignats.end(); r++) cout<<" "<<(*r);
      cout << " ]" << endl;
      cout << "\t\t\tLiveBefore: [";
      for (set<string>::iterator r=vBlocs[i].blocsBasics[j].liveBefore.begin(); r!=vBlocs[i].blocsBasics[j].liveBefore.end(); r++) cout<<" "<<(*r);
      cout << " ]" << endl;
      cout << "\t\t\tLiveAfter:  [";
      for (set<string>::iterator r=vBlocs[i].blocsBasics[j].liveAfter.begin(); r!=vBlocs[i].blocsBasics[j].liveAfter.end(); r++) cout<<" "<<(*r);
      cout << " ]" << endl;
      
      cout << "\t\t\tInitialIA32Map:  [";
      for (map<string, string>::iterator r = vBlocs[i].blocsBasics[j].initialMapIA32.begin(); r != vBlocs[i].blocsBasics[j].initialMapIA32.end(); r++) {
	cout << " <" << (*r).first << "," << (*r).second << ">";
      }
      cout << " ]" << endl;
      cout << "\t\t\tFinalIA32Map:  [";
      for (map<string, string>::iterator r = vBlocs[i].blocsBasics[j].finalMapIA32.begin(); r != vBlocs[i].blocsBasics[j].finalMapIA32.end(); r++) {
	cout << " <" << (*r).first << "," << (*r).second << ">";
      }
      cout << " ]" << endl;
#define PRINT_EXCHANGE
      cout << "\t\t\ttypeExit = ";
      if (vBlocs[i].blocsBasics[j].typeExit == NO_EXIT) {
	cout << "\tNO_EXIT" << endl;
      }
      if (vBlocs[i].blocsBasics[j].typeExit == UNCONDITIONAL_EXIT) {
	cout << "\tUNCONDITIONAL_EXIT  (" << vBlocs[i].blocsBasics[j].label_exit1 << ")[" << vBlocs[i].blocsBasics[j].bloc_exit1 << "]" << endl;
#ifdef PRINT_EXCHANGE
	for (unsigned int k = 0; k < vBlocs[i].blocsBasics[j].exch_exit1.size(); k++) {
 	  vBlocs[i].blocsBasics[j].exch_exit1[k].printIA32(6);
	}
#endif
      }
      if (vBlocs[i].blocsBasics[j].typeExit == CONTINUE_EXIT) {
	cout << "\tCONTINUE_EXIT  (" << vBlocs[i].blocsBasics[j].label_exit1 << ")[" << vBlocs[i].blocsBasics[j].bloc_exit1 << "]" << endl;
#ifdef PRINT_EXCHANGE
	for (unsigned int k = 0; k < vBlocs[i].blocsBasics[j].exch_exit1.size(); k++) {
	  vBlocs[i].blocsBasics[j].exch_exit1[k].printIA32(6);
	}
#endif
      }
      if (vBlocs[i].blocsBasics[j].typeExit == CONDITIONAL_EXIT) {
	cout << "\tCONDITIONAL_EXIT (CONT) (" << vBlocs[i].blocsBasics[j].label_exit1 << ")[" << vBlocs[i].blocsBasics[j].bloc_exit1 << "]" << endl;
#ifdef PRINT_EXCHANGE
        for (unsigned int k = 0; k < vBlocs[i].blocsBasics[j].exch_exit1.size(); k++) {
 	  vBlocs[i].blocsBasics[j].exch_exit1[k].printIA32(6);
 	}
#endif
	cout << "\t\t\t\t\tCONDITIONAL_EXIT (JUMP) (" << vBlocs[i].blocsBasics[j].label_exit2 << ")[" << vBlocs[i].blocsBasics[j].bloc_exit2 << "]" << endl;
#ifdef PRINT_EXCHANGE
	for (unsigned int k = 0; k < vBlocs[i].blocsBasics[j].exch_exit2.size(); k++) {
 	  vBlocs[i].blocsBasics[j].exch_exit2[k].printIA32(6);
        }
#endif
      }
      
      if (j < vBlocs[i].blocsBasics.size() - 1) {
	cout << endl;
      }
    }

    cout << "\t\t\tModifiedIAregsInBloc " << vBlocs[i].label << ":  [";
    for (unsigned int j = 0; j < vBlocs[i].modifiedIAregsInBloc.size(); j++) {
      cout << " " << vBlocs[i].modifiedIAregsInBloc[j];
    }
    cout << " ]" << endl;


    cout << endl << endl;
    
    cout << "\t\tmLabelsBasics: (" << vBlocs[i].mLabelsBasics.size() << ")" << endl;
    for (map<string, int>::const_iterator iter = vBlocs[i].mLabelsBasics.begin(); iter != vBlocs[i].mLabelsBasics.end(); ++iter) {
      cout << "\t\t\tmLabelsBasics[" << iter->first << "] = " << iter->second << endl;
    }
    
    cout << endl;
    
  }

}

void compute_live_registers(vector<tBloc> & vBlocs) {
  
  /// ------- inicialitacio:  Omplir set d'assignats i utilitzats de cada blocBasic ------- ///
  for (vector<tBloc>::iterator b=vBlocs.begin(); b!=vBlocs.end(); b++) {
    for (vector<tBlocBasic>::iterator bb=b->blocsBasics.begin(); bb!=b->blocsBasics.end(); bb++) {
      
      for (vector<tInstrBasic>::iterator i=bb->instrs.begin(); i!=bb->instrs.end(); i++) {
	// analitzem cada instruccio del bloc basic (bb),
	set<string> lreg;
	string reg;
	registres_instruccio(i->instrRC, lreg, reg);
	
	// i anotem com a registres utilitzats els que son llegits...
	for (set<string>::const_iterator r=lreg.begin(); r!=lreg.end(); r++) {
	  // ...sempre que no hagin estat escrits per una instruccio previa del mateix bloc
	  if (bb->assignats.find(*r)==bb->assignats.end()) 
	    bb->utilitzats.insert(*r);
	}
	
	// anotem com a assignat el registre assignat per la instrucio (if any)
	if (reg != "") bb->assignats.insert(reg);
      }
      
      // inicialitzar conjunts de vius del bloc basic
      bb->liveBefore=bb->utilitzats;
      bb->liveAfter.clear();
    }
  }
  
  
  /// ------- bucle de punt fix:  propagar fins que no hi hagi mes canvis ------- ///
  bool canvis=true;
  vector<tBloc> nousBlocs=vBlocs; // copia estat iteracio actual
  while (canvis) {
    canvis=false;
    
    // propagar vius sobre la copia
    for (string::size_type b=0; b<vBlocs.size(); b++) {
      for (string::size_type bb=0; bb<vBlocs[b].blocsBasics.size(); bb++) {
	
	int fill;
	if (vBlocs[b].blocsBasics[bb].typeExit != NO_EXIT) { // heretar del primer fill, si n'hi ha
	  fill =  vBlocs[b].blocsBasics[bb].bloc_exit1;
	  nousBlocs[b].blocsBasics[bb].liveAfter.insert( vBlocs[b].blocsBasics[fill].liveBefore.begin(),
							 vBlocs[b].blocsBasics[fill].liveBefore.end() );
	}
	
	if (vBlocs[b].blocsBasics[bb].typeExit == CONDITIONAL_EXIT) { // heretar del segon fill, si n'hi ha
	  fill =  vBlocs[b].blocsBasics[bb].bloc_exit2;
	  nousBlocs[b].blocsBasics[bb].liveAfter.insert( vBlocs[b].blocsBasics[fill].liveBefore.begin(),
							 vBlocs[b].blocsBasics[fill].liveBefore.end() );
	}   
	
	// recalcular liveBefore
	nousBlocs[b].blocsBasics[bb].liveBefore = vBlocs[b].blocsBasics[bb].utilitzats;
	// propagar liveAfter a liveBefore, exloent els assignats en el bloc
	for ( set<string>::iterator r = vBlocs[b].blocsBasics[bb].liveAfter.begin();
	      r!= vBlocs[b].blocsBasics[bb].liveAfter.end(); r++ ) 
	  if (vBlocs[b].blocsBasics[bb].assignats.find(*r) == vBlocs[b].blocsBasics[bb].assignats.end())
	    nousBlocs[b].blocsBasics[bb].liveBefore.insert(*r);
	
	// veure si el bloc basic ha canviat
	canvis = canvis or 
	  (nousBlocs[b].blocsBasics[bb].liveBefore!=vBlocs[b].blocsBasics[bb].liveBefore or
	   nousBlocs[b].blocsBasics[bb].liveAfter!=vBlocs[b].blocsBasics[bb].liveAfter);
      }
    }
    
    vBlocs=nousBlocs; // actualitzar estat
  }
  
  
  /// ------- baixar els vius a nivell d'instruccio ------- ///
  
  for (vector<tBloc>::iterator b=vBlocs.begin(); b!=vBlocs.end(); b++) {
    for (vector<tBlocBasic>::iterator bb=b->blocsBasics.begin(); bb!=b->blocsBasics.end(); bb++) {
      
      // propagar vius per cada instruccio del BB, del final cap enrera.
      
      vector<tInstrBasic>::iterator i,ipost;
      set<string> lreg;
      string reg;
      
      i=bb->instrs.end();  i--; // ultima instruccio del BB
      i->liveAfter= bb->liveAfter;
      
      ipost=i; // ultima instruccio processada
      i--;     // instr en curs
      while (ipost!=bb->instrs.begin()) {  // acabar la volta despres de processar la primera instr del BB
	
	// analitzar instruccio seguent
	registres_instruccio(ipost->instrRC, lreg, reg);
	// els vius despres d'una instruccio son els vius despres de la seguent, ...
	i->liveAfter = ipost->liveAfter;
	// ...menys el que escriu la seguent, ...
	i->liveAfter.erase(reg);
	// ...més els que llegeix la seguent
	for (set<string>::const_iterator r=lreg.begin(); r!=lreg.end(); r++)
	  i->liveAfter.insert(*r);
	
	ipost=i; // ultima instruccio processada
	i--;     // instr en curs
      }
    }
  }
}

void registres_instruccio(tInstrRCode i, set<string> &usats, string &assignat) {

  usats.clear(); assignat="";
  
  // operacions que llegeixen el 1r operand i n'escriuen el 2n
  if (i.cod=="load"  or i.cod=="aload" or i.cod=="iload" or 
      i.cod=="mini"  or i.cod=="lnot" or i.cod=="new") {
    
    if (i.op1.mode==MODE_RCODE_REGISTER) usats.insert(i.op1.opnd);
    if (i.op2.mode==MODE_RCODE_REGISTER) assignat=i.op2.opnd;
  }
  // operacions que llegeixen el 1r i 2n operands i n'escriuen el 3r
  else if (i.cod=="addi" or i.cod=="subi" or
	   i.cod=="muli" or i.cod=="divi" or 
	   i.cod=="equi" or i.cod=="grti" or i.cod=="lesi" or
	   i.cod=="land" or i.cod=="loor") {
    
    if (i.op1.mode==MODE_RCODE_REGISTER) usats.insert(i.op1.opnd);
    if (i.op2.mode==MODE_RCODE_REGISTER) usats.insert(i.op2.opnd);
    if (i.op3.mode==MODE_RCODE_REGISTER) assignat=i.op3.opnd;
  }
  // operacions que llegeixen zero operands i n'escriuen el 1r (i unic)
  else if (i.cod=="reai" or i.cod=="popparam") {
    if (i.op1.mode==MODE_RCODE_REGISTER) assignat=i.op1.opnd;
  }
  // operacions que llegeixen el 1r (i unic) operand i n'escriuen zero
  else if (i.cod=="wrii"  or i.cod=="wrli" or 
	   i.cod=="wris"  or i.cod=="wrls" or
	   i.cod=="ujmp" or i.cod=="call" or
	   i.cod=="etiq" or i.cod=="pushparam" or
           i.cod=="free") {
    
    if (i.op1.mode==MODE_RCODE_REGISTER) usats.insert(i.op1.opnd);
  }
  // operacions que llegeixen el 1r i 2n operands i n'escriuen zero
  else if (i.cod=="fjmp" or i.cod=="stor") {
    if (i.op1.mode==MODE_RCODE_REGISTER) usats.insert(i.op1.opnd);
    if (i.op2.mode==MODE_RCODE_REGISTER) usats.insert(i.op2.opnd);
  }
  // operacions que llegeixen el 1r, 2n i 3r operands i n'escriuen zero
  else if (i.cod=="copy") {
    if (i.op1.mode==MODE_RCODE_REGISTER) usats.insert(i.op1.opnd);
    if (i.op2.mode==MODE_RCODE_REGISTER) usats.insert(i.op2.opnd);
    if (i.op3.mode==MODE_RCODE_REGISTER) usats.insert(i.op3.opnd);
  }
  // operacions sense operands 
  else if (i.cod=="wrln" or i.cod=="stop" or i.cod=="retu" or
           i.cod=="killparam") {
  }
  // operacio ficticia al començament de cada  bloc
  else if (i.cod=="begin") {
  }
  else {
    cerr<<"Error: instruccio "<<i.cod<<" desconeguda."<<endl;
  }
}
