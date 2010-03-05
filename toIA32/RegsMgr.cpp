
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <iostream>
#include "RegsMgr.hpp"

using namespace std;

RegsMgr::RegsMgr(unsigned int nr)
  : numRegs(nr), maxUsed(0) {
  names.clear();
  names.push_back("");
  names.push_back("%eax");
  names.push_back("%ebx");
  names.push_back("%ecx");
  names.push_back("%edx");
  names.push_back("%esi");
  names.push_back("%edi");
  namesBck.clear();
  namesBck.push_back("");
  namesBck.push_back("eax_bck");
  namesBck.push_back("ebx_bck");
  namesBck.push_back("ecx_bck");
  namesBck.push_back("edx_bck");
  namesBck.push_back("esi_bck");
  namesBck.push_back("edi_bck");
  for (unsigned int i = 7; i <= numRegs; i++) {
    stringstream ss (stringstream::out);
    ss << "aux_" << i;
    names.push_back(ss.str());
    namesBck.push_back(ss.str() + "_bck");
  }
  mapping.clear();
  for (unsigned int i = 1; i <= numRegs; i++) {
    stringstream ss (stringstream::out);
    ss << "t" << i;
    mapping[ss.str()] = 0;
  }
  used.clear();
  for (unsigned int i = 0; i <= numRegs; i++) {
    used.push_back(false);
  }
  lockedRC.clear();
  for (unsigned int i = 0; i < 7; i++) {
    lockedRC.push_back(false);
  }
  lockedIA.clear();
  for (unsigned int i = 0; i < 7; i++) {
    lockedIA.push_back(false);
  }
  phyRegs.clear();
  for (unsigned int i = 0; i < 7; i++) {
    phyRegs.push_back("");
  }
  memRegs.clear();
  for (unsigned int i = 6; i <= numRegs; i++) {
    memRegs.push_back("");
  }
  initialMap.clear();
  modifiedInScopeIAreg.clear();
  for (unsigned int i = 0; i <= numRegs; i++) {
    modifiedInScopeIAreg.push_back(false);
  }
}

void RegsMgr::restartNewBloc(void) {
  mapping.clear();
  for (unsigned int i = 1; i <= numRegs; i++) {
    stringstream ss (stringstream::out);
    ss << "t" << i;
    mapping[ss.str()] = 0;
  }
  used.clear();
  for (unsigned int i = 0; i <= numRegs; i++) {
    used.push_back(false);
  }
  lockedRC.clear();
  for (unsigned int i = 0; i < 7; i++) {
    lockedRC.push_back(false);
  }
  lockedIA.clear();
  for (unsigned int i = 0; i < 7; i++) {
    lockedIA.push_back(false);
  }
  phyRegs.clear();
  for (unsigned int i = 0; i < 7; i++) {
    phyRegs.push_back("");
  }
  memRegs.clear();
  for (unsigned int i = 6; i <= numRegs; i++) {
    memRegs.push_back("");
  }
  initialMap.clear();
  modifiedInScopeIAreg.clear();
  for (unsigned int i = 0; i <= numRegs; i++) {
    modifiedInScopeIAreg.push_back(false);
  }
}

void RegsMgr::restartNewBlocBasic(void) {
  mapping.clear();
  for (unsigned int i = 1; i <= numRegs; i++) {
    stringstream ss (stringstream::out);
    ss << "t" << i;
    mapping[ss.str()] = 0;
  }
  used.clear();
  for (unsigned int i = 0; i <= numRegs; i++) {
    used.push_back(false);
  }
  lockedRC.clear();
  for (unsigned int i = 0; i < 7; i++) {
    lockedRC.push_back(false);
  }
  lockedIA.clear();
  for (unsigned int i = 0; i < 7; i++) {
    lockedIA.push_back(false);
  }
  phyRegs.clear();
  for (unsigned int i = 0; i < 7; i++) {
    phyRegs.push_back("");
  }
  memRegs.clear();
  for (unsigned int i = 6; i <= numRegs; i++) {
    memRegs.push_back("");
  }
  initialMap.clear();
}

unsigned int RegsMgr::getNumOfRegs(void) const {
  return numRegs;
}

unsigned int RegsMgr::getMaxIAregUsed(void) const {
  return maxUsed;
}

void RegsMgr::mapRegister(const string & RCreg, const bool forReading, string & IAreg, bool & isPhy) {
  if (mapping[RCreg] != 0) {
    if (mapping[RCreg] < 7 && lockedIA[mapping[RCreg]]) {
      IAreg = namesBck[mapping[RCreg]];
      isPhy = false;
    } else {
      IAreg = names[mapping[RCreg]];
      isPhy = mapping[RCreg] < 7;
    }
    modifiedInScopeIAreg[mapping[RCreg]] = true;
    lockedRC[mapping[RCreg]] = true;
  } else {
    isPhy = true;
    unsigned int i = 1;
    while (i < 7) {
      if (phyRegs[i] == "" && !lockedIA[i]) {
	IAreg = names[i];
	if (forReading && !used[i]) {
	  initialMap[RCreg] = IAreg;
	}
        used[i] = true;
	mapping[RCreg] = i;
        phyRegs[i] = RCreg;
	lockedRC[i] = true;
	modifiedInScopeIAreg[i] = true;
        if (i > maxUsed) {
          maxUsed = i;
	}
	return;
      } else if (phyRegs[i] == "" && lockedIA[i]) {
	IAreg = namesBck[i];
	if (forReading && !used[i]) {
	  initialMap[RCreg] = names[i];
	}
        used[i] = true;
	mapping[RCreg] = i;
        phyRegs[i] = RCreg;
	lockedRC[i] = true;
        isPhy = false;
	modifiedInScopeIAreg[i] = true;
        if (i > maxUsed) {
          maxUsed = i;
	}
        return;
      } else {
	i++;
      }
    }
    isPhy = false;
    while (i <= numRegs) {
      if (memRegs[i - 6] == "") {
	IAreg = names[i];
	if (forReading && !used[i]) {
	  initialMap[RCreg] = IAreg;
	}
	used[i] = true;
	modifiedInScopeIAreg[i] = true;
	mapping[RCreg] = i;
        memRegs[i - 6] = RCreg;
        if (i > maxUsed) {
          maxUsed = i;
	}
	return;
      } else {
	i++;
      }
    }
  }
}

void RegsMgr::unmapRegister(const string & RCreg) {
  if (mapping[RCreg] == 0) {
    return;
  }
  if (mapping[RCreg] < 7) {
    phyRegs[mapping[RCreg]] = "";
    lockedRC[mapping[RCreg]] = false;
    lockedIA[mapping[RCreg]] = false;
  } else {
    memRegs[mapping[RCreg] - 6] = "";
  }
  mapping[RCreg] = 0; 
}

void RegsMgr::unmapRegsNotInSet(const set<string> & rcRegs) {
  for (unsigned int i = 1; i <= numRegs; i++) {
    stringstream ss (stringstream::out);
    ss << "t" << i - 1;
    if (rcRegs.find(ss.str()) == rcRegs.end()) {
      unmapRegister(ss.str());
    }
  } 
}

void RegsMgr::unlockRegisters(void) {
  for (unsigned int i = 1; i < 7; i++) {
    lockedRC[i] = false;
    lockedIA[i] = false;
  }
}

void RegsMgr::getTemporalyPhysicalRegister(string & IARegTemp, string & IARegBck) {
  unsigned int i = 1;
  while (i < 7) {
    if (phyRegs[i] == "" && !lockedIA[i]) {
      IARegTemp = names[i];
      IARegBck = "";
      lockedRC[i] = true;
      lockedIA[i] = true;
      modifiedInScopeIAreg[i] = true;
      return;
    } else {
      i++;
    }
  }
  i = 1;
  while (i < 7) {
    if (!lockedRC[i] && !lockedIA[i]) {
      IARegTemp = names[i];
      IARegBck = namesBck[i];
      lockedRC[i] = true;
      lockedIA[i] = true;
      modifiedInScopeIAreg[i] = true;
      return;
    } else {
      i++;
    }
  }  
}

void RegsMgr::getTemporalyOneOfEAXEBXECXEDX(string & IARegTemp, string & IARegBck) {
  unsigned int i = 1;
  while (i < 5) {
    if (phyRegs[i] == "" && !lockedIA[i]) {
      IARegTemp = names[i];
      IARegBck = "";
      lockedRC[i] = true;
      lockedIA[i] = true;
      modifiedInScopeIAreg[i] = true;
      return;
    } else {
      i++;
    }
  }
  i = 1;
  while (i < 5) {
    if (!lockedRC[i] && !lockedIA[i]) {
      IARegTemp = names[i];
      IARegBck = namesBck[i];
      lockedRC[i] = true;
      lockedIA[i] = true;
      modifiedInScopeIAreg[i] = true;
      return;
    } else {
      i++;
    }
  }  
}

void RegsMgr::getTemporalyEAXEDX(string & EAX_Bck, string & EDX_Bck) {
  bool found = false;
  unsigned int i = 1;
  while (!found && i < 7) {
    if (names[i] == "%eax") {
      found = true;
    } else {
      i++;
    }
  }
  if (phyRegs[i] != "") {
    EAX_Bck = namesBck[i];
  } else {
    EAX_Bck = "";
  }
  lockedRC[i] = true;
  lockedIA[i] = true;
  modifiedInScopeIAreg[i] = true;

  found = false;
  i = 1;
  while (!found && i < 7) {
    if (names[i] == "%edx") {
      found = true;
    } else {
      i++;
    }
  }
  if (phyRegs[i] != "") {
    EDX_Bck = namesBck[i];
  } else {
    EDX_Bck = "";
  }
  lockedRC[i] = true;
  lockedIA[i] = true;
  modifiedInScopeIAreg[i] = true;
}

void RegsMgr::getTemporalyECXESIEDI(string & ECX_Bck, string & ESI_Bck, string & EDI_Bck) {
  bool found = false;
  unsigned int i = 1;
  while (!found && i < 7) {
    if (names[i] == "%ecx") {
      found = true;
    } else {
      i++;
    }
  }
  if (phyRegs[i] != "") {
    ECX_Bck = namesBck[i];
  } else {
    ECX_Bck = "";
  }
  lockedRC[i] = true;
  lockedIA[i] = true;
  modifiedInScopeIAreg[i] = true;

  found = false;
  i = 1;
  while (!found && i < 7) {
    if (names[i] == "%esi") {
      found = true;
    } else {
      i++;
    }
  }
  if (phyRegs[i] != "") {
    ESI_Bck = namesBck[i];
  } else {
    ESI_Bck = "";
  }
  lockedRC[i] = true;
  lockedIA[i] = true;
  modifiedInScopeIAreg[i] = true;

  found = false;
  i = 1;
  while (!found && i < 7) {
    if (names[i] == "%edi") {
      found = true;
    } else {
      i++;
    }
  }
  if (phyRegs[i] != "") {
    EDI_Bck = namesBck[i];
  } else {
    EDI_Bck = "";
  }
  lockedRC[i] = true;
  lockedIA[i] = true;
  modifiedInScopeIAreg[i] = true;
}

void RegsMgr::getTemporalyEAX(string & EAX_Bck) {
  bool found = false;
  unsigned int i = 1;
  while (!found && i < 7) {
    if (names[i] == "%eax") {
      found = true;
    } else {
      i++;
    }
  }
  if (phyRegs[i] != "") {
    EAX_Bck = namesBck[i];
  } else {
    EAX_Bck = "";
  }
  lockedRC[i] = true;
  lockedIA[i] = true;
  modifiedInScopeIAreg[i] = true;
}

bool RegsMgr::isInUsePhysicalRegister(const string IAreg) {
  bool found = false;
  unsigned int i = 1;
  while (!found && i < 7) {
    if (names[i] == IAreg) {
      found = true;
    } else {
      i++;
    }
  }
  return (phyRegs[i] != "");
}

void RegsMgr::addNonUsedMappings(const set<string> & liveBefore, const set<string> & utilizats) {
  string IAreg;
  bool isPhy;

  for (set<string>::const_iterator it = liveBefore.begin(); it != liveBefore.end(); it++) {
    if (utilizats.find(*it) == utilizats.end()) {
      mapRegister(*it, true, IAreg, isPhy);
    }
  }
}

map<string, string> RegsMgr::getInitialMapping(void) const {
//   map<string, string> curr;
//   curr.clear();
//   for (map<string, string>::const_iterator it = initialMap.begin(); it != initialMap.end(); it++) {
//     curr[it->first] = it->second;
//   }
//   return curr;
  return initialMap;
}

map<string, string> RegsMgr::getCurrentMapping(void) const {
  map<string, string> curr;
  curr.clear();
  for (map<string, int>::const_iterator it = mapping.begin(); it != mapping.end(); it++) {
    if (it->second != 0) {
      curr[it->first] = names[it->second];
    }
  }
  return curr;
}

vector<string> RegsMgr::getModifiedIAregsInBloc(void) const {
  vector<string> resul;
  resul.clear();
  
  for (unsigned int i = 1; i <= numRegs; i++) {
    if (modifiedInScopeIAreg[i]) {
      resul.push_back(names[i]);
    }
  }
  return resul;
}

string RegsMgr::getLowByte(const string & regIA) {
  if (regIA == "%eax") {
    return "%al";
  } else if (regIA == "%ebx") {
    return "%bl";
  } else if (regIA == "%ecx") {
    return "%cl";
  } else if (regIA == "%edx") {
    return "%dl";
  } else {
    return "NO EXISTE LOW BYTE";
  }
}

string RegsMgr::nameOfRegister(unsigned int i) {
  return names[i + 1];
}

bool RegsMgr::isPhysicalIAreg(const string & regIA) {
  return (regIA == "%eax" || regIA == "%ebx" || regIA == "%ecx" ||
          regIA == "%edx" || regIA == "%esi" || regIA == "%edi");
}

string RegsMgr::registerBackup(const string & regIA) {
  if (regIA == "%eax") {
    return "eax_bck";
  } else if (regIA == "%ebx") {
    return "ebx_bck";
  } else if (regIA == "%ecx") {
    return "ecx_bck";
  } else if (regIA == "%edx") {
    return "edx_bck";
  } else if (regIA == "%esi") {
    return "esi_bck";
  } else if (regIA == "%edi") {
    return "edi_bck";
  } else {
    return regIA + "_bck";
  }
}

void RegsMgr::print(ostream & os) {
  os << endl << "Number of Registers: " << numRegs << endl;
  os << endl << "Max Register Used: " << maxUsed << endl;
  os << endl << "Names of Registers (ordered):" << endl;
  for (unsigned int i = 1; i < names.size(); i++) {
    os << "\tnames[" << i << "] = \"" << names[i] << "\"" << endl;
  }
  os << endl << "Mapping of Registers:" << endl;
  for (unsigned int i = 0; i < numRegs; i++) {
    stringstream ss (stringstream::out);
    ss << "t" << i;
    os << "\tmapping[\"" << ss.str() << "\"] = " << mapping[ss.str()] << endl;
  }
  os << endl << "Used Registers:" << endl;
  for (unsigned int i = 1; i <= numRegs; i++) {
    os << "\tused[" << i << "] = " << ((used[i])?"true":"false") << endl;
  }
  os << endl << "LockedRC Registers:" << endl;
  for (unsigned int i = 1; i < 7; i++) {
    os << "\tlocked[" << i << "] = " << ((lockedRC[i])?"true":"false") << endl;
  }
  os << endl << "LockedIA Registers:" << endl;
  for (unsigned int i = 1; i < 7; i++) {
    os << "\tlocked[" << i << "] = " << ((lockedIA[i])?"true":"false") << endl;
  }
  os << endl << "Physical Registers:" << endl;
  for (unsigned int i = 1; i < 7; i++) {
    os << "\tphyRegs[" << i << "] = \"" << phyRegs[i] << "\"" << endl;
  }
  os << endl << "Memory Registers:" << endl;
  for (unsigned int i = 7; i <= numRegs; i++) {
    os << "\tmemRegs[" << i << "] = \"" << memRegs[i - 6] << "\"" << endl; 
  }
  os << endl << "Initial Mapping:" << endl;
  for (map<string, string>::const_iterator it = initialMap.begin(); it != initialMap.end(); it++) {
    os << "\tiniMap[\"" << it->first << "\"] = \"" << it->second << "\"" << endl;
  }
  os << endl << "Current Mapping:" << endl;
  for (map<string, int>::const_iterator it = mapping.begin(); it != mapping.end(); it++) {
    if (it->second != 0) {
      os << "\tcurrMap[\"" << it->first << "\"] = \"" << names[it->second] << "\"" << endl;
    }
  }
  os << endl << "Modified In Scope IA Registers:" << endl;
  for (unsigned int i = 1; i <= numRegs; i++) {
    os << "\tmodified[" << i << "] = " << ((modifiedInScopeIAreg[i])?"true":"false") << endl;
  }

  os << endl;
}

ostream & operator<<(ostream & os, RegsMgr & rm) {
  rm.print(os);
  return os;
}

