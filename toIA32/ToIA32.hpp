
#ifndef _TOIA32_HPP_
#define _TOIA32_HPP_

#include <vector>
#include <string>
#include <map>
#include "Blocs.hpp"
#include "RegsMgr.hpp"

using namespace std;

void translateVBlocsIA32(vector<tBloc> & vBlocs, RegsMgr & regsMgr, map<string, int> & mOffsets, map<string, int> & mSizes, const vector<string> & vStrWRTS);
void printVBlocsIA32(vector<tBloc> & vBlocs, ostream & os = cout);
void printIA32code(vector<tBloc> & vBlocs,
                   const unsigned int numRegs,
                   const bool reaiRCInstr,
                   const bool wriiRCInstr, const bool wrliRCInstr,
                   const bool wrisRCInstr, const bool wrlsRCInstr,
                   const bool wrlnRCInstr,
                   const vector<string> vStrWRTS,
                   ostream & os = cout);

#endif
