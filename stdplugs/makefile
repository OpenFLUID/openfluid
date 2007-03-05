# standard plugins makefile



include ../makeopts.inc

OBJDIR=$(BASEOBJDIR)/stdplugs
BINDIR=$(BASEBINDIR)/$(PLUGSUBDIR)
LIBDIR=$(BASELIBDIR)
DOCDIR=$(BASEDOCDIR)/stdplugs

#CCFLAGS=$(CCCFLAGS) $(DYNLIBCCFLAGS) -I ../sdk $(WXFLAGS) #-DBUILDINGDLL -fPIC
#CCLIBS= -l$(SDKLIBBASE) -l$(SDKLIBCORE) $(WXLIBS) -L$(LIBDIR)#-L$(BASEBINDIR) -Wl,-rpath,. -Wl,-rpath,.. 

CCFLAGS=$(CCCFLAGS) $(DYNLIBCCFLAGS) $(MHYDASDKFLAGS) $(WXFLAGS)
CCLIBS=$(MHYDASDKLIBS) $(WXLIBS)

LDFLAGS=
ifeq ($(OSTYPE),msys)
  LDFLAGS=-Wl,--enable-runtime-pseudo-reloc
endif


MSEYTOUXOBJ=$(OBJDIR)/MSeytouxFunc.o \
            $(OBJDIR)/MSeytouxPlug.o
MSEYTOUXNAME=mseytoux


HAYAMISUOBJ=$(OBJDIR)/HayamiSUFunc.o \
            $(OBJDIR)/HayamiSUPlug.o \
            $(OBJDIR)/HayamiTools.o
HAYAMISUNAME=hayamiSU


HAYAMIRSOBJ=$(OBJDIR)/HayamiRSFunc.o \
            $(OBJDIR)/HayamiRSPlug.o \
            $(OBJDIR)/HayamiTools.o
HAYAMIRSNAME=hayamiRS


RSGUEXCHOBJ=$(OBJDIR)/RSGUExchFunc.o \
            $(OBJDIR)/RSGUExchPlug.o
RSGUEXCHNAME=RSGUexch


SUGUEXCHOBJ=$(OBJDIR)/SUGUExchFunc.o \
            $(OBJDIR)/SUGUExchPlug.o
SUGUEXCHNAME=SUGUexch


GUGUEXCHOBJ=$(OBJDIR)/GUGUExchFunc.o \
            $(OBJDIR)/GUGUExchPlug.o
GUGUEXCHNAME=GUGUexch





$(OBJDIR)/%.o : %.cpp %.h
	$(CPP) $(CCFLAGS) -c $< -o $@


all: prechecks $(MSEYTOUXOBJ) $(HAYAMISUOBJ) $(HAYAMIRSOBJ) $(RSGUEXCHOBJ) $(SUGUEXCHOBJ) $(GUGUEXCHOBJ)
	$(CPP) $(MSEYTOUXOBJ) $(CCLIBS) -o $(BINDIR)/$(MSEYTOUXNAME).$(PLUGINEXT) -shared $(LDFLAGS)
	$(CPP) $(HAYAMISUOBJ) $(CCLIBS) -o $(BINDIR)/$(HAYAMISUNAME).$(PLUGINEXT) -shared $(LDFLAGS)
	$(CPP) $(HAYAMIRSOBJ) $(CCLIBS) -o $(BINDIR)/$(HAYAMIRSNAME).$(PLUGINEXT) -shared $(LDFLAGS)
	$(CPP) $(RSGUEXCHOBJ) $(CCLIBS) -o $(BINDIR)/$(RSGUEXCHNAME).$(PLUGINEXT) -shared $(LDFLAGS)	
	$(CPP) $(SUGUEXCHOBJ) $(CCLIBS) -o $(BINDIR)/$(SUGUEXCHNAME).$(PLUGINEXT) -shared $(LDFLAGS)	
	$(CPP) $(GUGUEXCHOBJ) $(CCLIBS) -o $(BINDIR)/$(GUGUEXCHNAME).$(PLUGINEXT) -shared $(LDFLAGS)			


prechecks:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)


clean:
	@rm -f $(OBJDIR)/*


doc:
	@mkdir -p $(DOCDIR)
	@doxygen

