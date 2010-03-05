
#ifndef _REGS_MGR_HPP_
#define _REGS_MGR_HPP_

#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

class RegsMgr {
public:
  RegsMgr(unsigned int nr);
  void restartNewBloc(void);
  void restartNewBlocBasic(void);

  unsigned int getNumOfRegs(void) const;
  unsigned int getMaxIAregUsed(void) const;
  void mapRegister(const string & RCreg, const bool forReading, \
                   string & IAreg, bool & isPhy);
  void unmapRegister(const string & RCreg);
  void unmapRegsNotInSet(const set<string> & rcRegs);
  void unlockRegisters(void);

  void getTemporalyPhysicalRegister(string & IAregTemp, string & IAregBck);
  void getTemporalyOneOfEAXEBXECXEDX(string & IAregTemp, string & IAregBck);
  void getTemporalyEAXEDX(string & EAX_Bck, string & EDX_Bck);
  void getTemporalyECXESIEDI(string & ECX_Bck, string & ESI_Bck, string & EDI_Bck);
  void getTemporalyEAX(string & EAX_Bck);

  bool isInUsePhysicalRegister(const string IAreg);

  void addNonUsedMappings(const set<string> & liveBefore, const set<string> & utilizats);
  map<string, string> getInitialMapping(void) const;
  map<string, string> getCurrentMapping(void) const;
  vector<string> getModifiedIAregsInBloc(void) const;

  static string getLowByte(const string & regIA);
  string nameOfRegister(unsigned int i);
  static bool isPhysicalIAreg(const string & regIA);
  static string registerBackup(const string & regIA);
  void   print(ostream & os);

private:
  unsigned int         numRegs;
  unsigned int         maxUsed;
  vector<string>       names;
  vector<string>       namesBck;
  map<string, int>     mapping;
  vector<bool>         used;
  vector<bool>         lockedRC;
  vector<bool>         lockedIA;
  vector<string>       phyRegs;
  vector<string>       memRegs;
  map<string, string>  initialMap;
  vector<bool>         modifiedInScopeIAreg;
};

ostream & operator<<(ostream & os, RegsMgr & rm);

#endif
