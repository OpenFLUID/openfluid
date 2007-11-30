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
MSEYTOUXNAME=water.surf-uz.runoff-infiltration.mseytoux


HAYAMISUOBJ=$(OBJDIR)/HayamiSUFunc.o \
            $(OBJDIR)/HayamiTools.o
HAYAMISUNAME=water.surf.transfer-su.hayami


HAYAMIRSOBJ=$(OBJDIR)/HayamiRSFunc.o \
            $(OBJDIR)/HayamiTools.o
HAYAMIRSNAME=water.surf.transfer-rs.hayami


HAYAMIRSGUOBJ=$(OBJDIR)/HayamiRSGUFunc.o \
            $(OBJDIR)/HayamiTools.o
HAYAMIRSGUNAME=water.surf.transfer-rs-exchange-gu.hayami-tank



$(OBJDIR)/%.o : %.cpp %.h
	$(CPP) $(CCFLAGS) -c $< -o $@


all: prechecks $(MSEYTOUXOBJ) $(HAYAMISUOBJ) $(HAYAMIRSOBJ) $(HAYAMIRSGUOBJ)  
	$(CPP) $(MSEYTOUXOBJ) $(CCLIBS) -o $(BINDIR)/$(MSEYTOUXNAME).$(PLUGINEXT) -shared $(LDFLAGS)
	$(CPP) $(HAYAMISUOBJ) $(CCLIBS) -o $(BINDIR)/$(HAYAMISUNAME).$(PLUGINEXT) -shared $(LDFLAGS)
	$(CPP) $(HAYAMIRSOBJ) $(CCLIBS) -o $(BINDIR)/$(HAYAMIRSNAME).$(PLUGINEXT) -shared $(LDFLAGS)
	$(CPP) $(HAYAMIRSGUOBJ) $(CCLIBS) -o $(BINDIR)/$(HAYAMIRSGUNAME).$(PLUGINEXT) -shared $(LDFLAGS)	
	


prechecks:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)


clean:
	@rm -f $(OBJDIR)/*


doc:
	@mkdir -p $(DOCDIR)
	@doxygen

