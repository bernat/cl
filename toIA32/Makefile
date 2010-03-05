THISHOST = $(shell hostname)

ANTLR_H = /usr/local/antlr-2.7.6/include
ANTLR_L = /usr/local/antlr-2.7.6/lib
BIN     = /usr/local/antlr-2.7.6/bin

ifeq ($(THISHOST), aelia)
    ANTLR_H = /usr/include
    ANTLR_L = /usr/lib
    BIN     = /usr/bin
endif
ifeq ($(THISHOST), iguerri)
    ANTLR_H = /usr/include
    ANTLR_L = /usr/lib
    BIN     = /usr/bin
endif
ifeq ($(THISHOST), brisa)
    ANTLR_H = /usr/include
    ANTLR_L = /usr/lib
    BIN     = /usr/bin
endif

ANTLR   = $(BIN)/runantlr
CC      = g++
COTHER  = -Wall -ansi
CFLAGS  = -I$(ANTLR_H) $(COTHER)
LFLAGS  = -L$(ANTLR_L) -lantlr

GRM       = RCode.g
AFLAGS    =
MYFILES   = Main.cpp Blocs.cpp RegsMgr.cpp ToIA32.cpp
SRC       = RCodeLexer.cpp RCodeParser.cpp $(MYFILES)
OBJ       = $(SRC:.cpp=.o)
TOKENS_LEXPARSER   = RCodeLexerTokenTypes.hpp
ANTLR_SPAWN  = RCodeLexer.hpp RCodeLexer.cpp RCodeParser.hpp RCodeParser.cpp\
                 $(TOKENS_LEXPARSER) $(TOKENS_LEXPARSER:.hpp=.txt)

rc: $(OBJ) $(SRC)
	$(CC) -o rc $(CFLAGS) $(OBJ) $(LFLAGS)

Main.o: Main.cpp RCodeLexer.hpp RCodeParser.hpp ToIA32.hpp RegsMgr.hpp
	$(CC) -c $(CFLAGS) Main.cpp

Blocs.o: Blocs.hpp Blocs.cpp
	$(CC) -c $(CFLAGS) Blocs.cpp

RegsMgr.o: RegsMgr.cpp RegsMgr.hpp
	$(CC) -c $(CFLAGS) RegsMgr.cpp

RCodeLexer.o: RCodeLexer.cpp
	$(CC) -c $(CFLAGS) RCodeLexer.cpp

RCodeParser.o: RCodeParser.cpp Blocs.hpp
	$(CC) -c $(CFLAGS) RCodeParser.cpp

ToIA32.o: ToIA32.cpp RCodeLexer.hpp RCodeParser.hpp RegsMgr.hpp
	$(CC) -c $(CFLAGS) ToIA32.cpp

$(ANTLR_SPAWN): $(GRM) Blocs.hpp
	$(ANTLR) $(AFLAGS) $(GRM)

tar:
	tar cvf rc2ia.tar RCode.g Blocs.[hc]pp RegsMgr.[hc]pp ToIA32.[hc]pp Main.cpp Makefile

clean:
	rm -f $(OBJ) rc
	rm -f $(ANTLR_SPAWN)
