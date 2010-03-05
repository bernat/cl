
#include <iostream>
#include <fstream>
#include <antlr/AST.hpp>
#include "RCodeLexer.hpp"
#include "RCodeParser.hpp"
#include "Blocs.hpp"
#include "RegsMgr.hpp"
#include "ToIA32.hpp"

using namespace std;
using namespace antlr;

string lineLispPrintAST( RefAST tree );
void   indentPrintAST( RefAST tree, const char* const* tokenNames, const int ind = 0, const int inc = 4 );
void   indentPrintASTList( RefAST tree, const char* const* tokenNames, const int ind = 0, const int inc = 4 );

int main(int argc, char *argv[]) {

  try {

    if (argc != 2) {
      cerr << endl;
      cerr << "Usage: ./rc file_rcode" << endl;
      exit(0);
    }

    ifstream ifs(argv[1], ios_base::in);

    if (!(ifs.is_open())) {
      cerr << endl;
      cerr << "Error opening file: " << argv[1] << endl;
      exit(0);
    }

    string file_rc(argv[1]);
    string file_s(file_rc);
    string::size_type pos = file_s.rfind('.', file_s.size());
    if (pos != string::npos) {
      file_s.replace(pos, file_s.size() - pos, ".s");
    } else {
      file_s = file_s + ".s";
    }

    ofstream ofs(file_s.c_str(), ios_base::out);

    RCodeLexer lexer(ifs);

    ASTFactory my_factory;	// generates CommonAST per default..
    RCodeParser parser(lexer);

    // Do setup from the AST factory repeat this for all parsers using the AST
    parser.initializeASTFactory( my_factory );
    parser.setASTFactory( &my_factory );

    parser.compilation_unit();

    unsigned int nr = parser.numRegs;
    cout << "Number of Registers: " << nr << endl;

    cout << "REAI: " << parser.reaiRCInstr << endl;
    cout << "WRII: " << parser.wriiRCInstr << endl;
    cout << "WRLI: " << parser.wrliRCInstr << endl;
    cout << "WRIS: " << parser.wrisRCInstr << endl;
    cout << "WRLS: " << parser.wrlsRCInstr << endl;
    cout << "WRLN: " << parser.wrlnRCInstr << endl;

    cout << "Number of Strings: " << parser.vStrWRTS.size() << endl;
    for (unsigned int i = 0; i < parser.vStrWRTS.size(); i++) {
      cout << "\tString[" << i+1 << "] = " << parser.vStrWRTS[i] << endl; 
    }

    cout << endl;

    RefAST sintTree = parser.getAST();

    cout << lineLispPrintAST(sintTree) << endl;

    indentPrintASTList(sintTree, parser.getTokenNames());

    cout << endl;

    parser.printVInstrsOffsetsSizes();

    compute_live_registers(parser.vBlocs);

    RegsMgr regsMgr(nr);

    translateVBlocsIA32(parser.vBlocs, regsMgr, parser.mOffsets, parser.mSizes, parser.vStrWRTS);

    nr = regsMgr.getMaxIAregUsed();

    cout << "Max Register Used: " << nr << endl << endl;

    printBlocs(parser.vBlocs);

    printIA32code(parser.vBlocs, nr,
                  parser.reaiRCInstr,
                  parser.wriiRCInstr, parser.wrliRCInstr,
                  parser.wrisRCInstr, parser.wrlsRCInstr,
                  parser.wrlnRCInstr,
                  parser.vStrWRTS);

    printIA32code(parser.vBlocs, nr,
                  parser.reaiRCInstr,
                  parser.wriiRCInstr, parser.wrliRCInstr,
                  parser.wrisRCInstr, parser.wrlsRCInstr,
                  parser.wrlnRCInstr,
                  parser.vStrWRTS,
                  ofs);

  }
  catch (ANTLRException & e) {
    cerr << "Parse exception: " << e.toString() << endl;
    return -1;
  }
  catch (exception & e) {
    cerr << "exception: " << e.what() << endl;
    return -1;
  }
  return 0;

}

string lineLispPrintAST( RefAST tree ) {
  if (tree != RefAST(antlr::nullAST)) {
    return tree->toStringTree();
  } else {
    return "";
  }
}

void indentPrintASTList( RefAST tree, const char* const* tokenNames, const int ind, const int inc ) {
  for (RefAST t = tree; t; t = t->getNextSibling()) {
    indentPrintAST(t, tokenNames, ind, inc);
  }
}

void indentPrintAST( RefAST tree, const char* const* tokenNames, const int ind, const int inc ) {
  if (tree != RefAST(antlr::nullAST)) { 
    for (int i = 0; i < ind; i++) {
      cout << " ";
    }
    string text = tokenNames[tree->getType()];
    cout << text;
    switch (tree->getType()) {
    case RCodeLexerTokenTypes::REGISTER:
    case RCodeLexerTokenTypes::LOCAL:
    case RCodeLexerTokenTypes::FULL_NAME:
    case RCodeLexerTokenTypes::PATH:
    case RCodeLexerTokenTypes::LABEL:
    case RCodeLexerTokenTypes::ENTER:
    case RCodeLexerTokenTypes::STRING:
      cout << " " << tree->getText();
      break;
    }
    cout << endl;
    RefAST t = RefAST(tree->getFirstChild());
    indentPrintASTList(t, tokenNames, ind + inc, inc);
  }
}
