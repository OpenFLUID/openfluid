# standard plugins makefile



include ../../makeopts.inc

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


MSEYTOUXOBJ=$(OBJDIR)/MSeytouxFunc.o
MSEYTOUXNAME=mseytoux


HAYAMISUOBJ=$(OBJDIR)/HayamiSUFunc.o \
            $(OBJDIR)/HayamiTools.o
HAYAMISUNAME=hayamiSU


HAYAMIRSOBJ=$(OBJDIR)/HayamiRSFunc.o \
            $(OBJDIR)/HayamiTools.o
HAYAMIRSNAME=hayamiRS


TRANSFERGUOBJ=$(OBJDIR)/TransferGUFunc.o
TRANSFERGUNAME=transferGU



$(OBJDIR)/%.o : %.cpp %.h
	$(CPP) $(CCFLAGS) -c $< -o $@


all: prechecks $(MSEYTOUXOBJ) $(HAYAMISUOBJ) $(HAYAMIRSOBJ) $(TRANSFERGUOBJ)
	$(CPP) $(MSEYTOUXOBJ) $(CCLIBS) -o $(BINDIR)/$(MSEYTOUXNAME).$(PLUGINEXT) -shared $(LDFLAGS)
	$(CPP) $(HAYAMISUOBJ) $(CCLIBS) -o $(BINDIR)/$(HAYAMISUNAME).$(PLUGINEXT) -shared $(LDFLAGS)
	$(CPP) $(HAYAMIRSOBJ) $(CCLIBS) -o $(BINDIR)/$(HAYAMIRSNAME).$(PLUGINEXT) -shared $(LDFLAGS)
#	$(CPP) $(TRANSFERGUOBJ) $(CCLIBS) -o $(BINDIR)/$(TRANSFERGUNAME).$(PLUGINEXT) -shared $(LDFLAGS)	


prechecks:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)


clean:
	@rm -f $(OBJDIR)/*


doc:
	@mkdir -p $(DOCDIR)
	@doxygen

