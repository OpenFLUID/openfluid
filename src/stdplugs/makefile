# standard plugins makefile



include ../makeopts.inc

OBJDIR=$(BASEOBJDIR)/stdplugs
BINDIR=$(BASEBINDIR)/plugs
LIBDIR=$(BASELIBDIR)
DOCDIR=$(BASEDOCDIR)/stdplugs

CCFLAGS=$(CCCFLAGS) -DBUILDINGDLL -fPIC -I ../sdk $(WXFLAGS)
CCLIBS= -L$(BASEBINDIR) -lsdk-base -lsdk-core $(WXLIBS) -Wl,-rpath,. -Wl,-rpath,..

#CCFLAGS=-DBUILDINGDLL -fPIC -I .. $(WXFLAGS)
#LDFLAGS=-Wl,-soname,$(LIBNAME).$(PLUGINEXT)
 #-Wl,--out-implib,$(LIBDIR)/$(LIBNAME).a
#CCLIBS=-L $(BASELIBDIR) $(WXLIBS)

MSEYTOUXOBJ=$(OBJDIR)/MSeytouxFunc.o \
            $(OBJDIR)/MSeytouxPlug.o

MSEYTOUXNAME=mseytoux

HAYAMISUOBJ=$(OBJDIR)/HayamiSUFunc.o \
            $(OBJDIR)/HayamiSUPlug.o

HAYAMISUNAME=hayamiSU




$(OBJDIR)/%.o : %.cpp %.h
	$(CPP) $(CCFLAGS) -c $< -o $@


all: prechecks $(MSEYTOUXOBJ) $(HAYAMISUOBJ)
	$(CPP) -shared $(MSEYTOUXOBJ) $(CCLIBS) -o $(BINDIR)/$(MSEYTOUXNAME).$(PLUGINEXT)
	$(CPP) -shared $(HAYAMISUOBJ) $(CCLIBS) -o $(BINDIR)/$(HAYAMISUNAME).$(PLUGINEXT)

prechecks:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)


clean:
	@rm -f $(OBJDIR)/*


doc:
	@mkdir -p $(DOCDIR)
	@doxygen

