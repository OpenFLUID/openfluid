# standard plugins makefile



include ../makeopts.inc

OBJDIR=$(BASEOBJDIR)/stdplugs
BINDIR=$(BASEBINDIR)/plugs
LIBDIR=$(BASELIBDIR)
DOCDIR=$(BASEDOCDIR)/stdplugs

CCFLAGS=$(CCCFLAGS) $(DYNLIBCCFLAGS) -I ../sdk $(WXFLAGS) #-DBUILDINGDLL -fPIC
CCLIBS= -L$(BASEBINDIR) -l$(SDKLIBSROOT)-base -l$(SDKLIBSROOT)-core $(WXLIBS) -Wl,-rpath,. -Wl,-rpath,..

LDFLAGS=
ifeq ($(OSTYPE),msys)
  LDFLAGS=-Wl,--enable-runtime-pseudo-reloc
endif


MSEYTOUXOBJ=$(OBJDIR)/MSeytouxFunc.o \
            $(OBJDIR)/MSeytouxPlug.o

MSEYTOUXNAME=mseytoux

HAYAMISUOBJ=$(OBJDIR)/HayamiSUFunc.o \
            $(OBJDIR)/HayamiSUPlug.o

HAYAMISUNAME=hayamiSU




$(OBJDIR)/%.o : %.cpp %.h
	$(CPP) $(CCFLAGS) -c $< -o $@


all: prechecks $(MSEYTOUXOBJ) $(HAYAMISUOBJ)
	$(CPP) $(MSEYTOUXOBJ) $(CCLIBS) -o $(BINDIR)/$(MSEYTOUXNAME).$(PLUGINEXT) -shared $(LDFLAGS)
	$(CPP) $(HAYAMISUOBJ) $(CCLIBS) -o $(BINDIR)/$(HAYAMISUNAME).$(PLUGINEXT) -shared $(LDFLAGS)

prechecks:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)


clean:
	@rm -f $(OBJDIR)/*


doc:
	@mkdir -p $(DOCDIR)
	@doxygen

