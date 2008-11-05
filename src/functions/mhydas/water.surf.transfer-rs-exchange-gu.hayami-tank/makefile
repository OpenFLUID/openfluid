# makefile for openfluid function

CPP = g++
WXLIBS = $(shell wx-config --libs base)
WXFLAGS = $(shell wx-config --cxxflags base)
OPENFLUIDLIBS = $(shell ofelib-config --libs)
OPENFLUIDFLAGS = $(shell ofelib-config --cflags)
BINFILE = water.surf.transfer-rs-exchange-gu.hayami-tank
SRCFILESROOT = HayamiRSGUFunc

INSTALLPATH = $(HOME)/.openfluid/engine/functions
OBJPATH = .
BINPATH = .

LDFLAGS =
PLUGEXT = sompi
ifeq ($(OSTYPE),msys)
  LDFLAGS=-Wl,--enable-runtime-pseudo-reloc
  PLUGEXT = dllmpi
endif

ifdef FORCEINSTALLPATH
  INSTALLPATH = $(FORCEINSTALLPATH)
endif

ifdef FORCEOBJPATH
  OBJPATH = $(FORCEOBJPATH)
endif

ifdef FORCEBINPATH
  BINPATH = $(FORCEBINPATH)
endif


all:	
	$(CPP) -O3 -c $(SRCFILESROOT).cpp -o $(OBJPATH)/$(SRCFILESROOT).o -fPIC $(WXFLAGS) $(OPENFLUIDFLAGS)
	$(CPP) -O3 -c HayamiTools.cpp -o $(OBJPATH)/HayamiTools.o -fPIC $(WXFLAGS) $(OPENFLUIDFLAGS)
	$(CPP) $(OBJPATH)/$(SRCFILESROOT).o $(OBJPATH)/HayamiTools.o $(WXLIBS) $(OPENFLUIDLIBS) -o $(BINPATH)/$(BINFILE).$(PLUGEXT) -shared $(LDFLAGS)

clean:
	rm -f $(BINPATH)/$(BINFILE).$(PLUGEXT)
	rm -f $(OBJPATH)/$(SRCFILESROOT).o

install: all
	 @mkdir -p $(INSTALLPATH)
	 @cp ./$(BINFILE).$(PLUGEXT) $(INSTALLPATH)

