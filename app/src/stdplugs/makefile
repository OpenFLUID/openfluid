# standard plugins makefile



include ../makeopts.inc

OBJDIR=$(BASEOBJDIR)/stdplugs
BINDIR=$(BASEBINDIR)/plugs
LIBDIR=$(BASELIBDIR)
DOCDIR=$(BASEDOCDIR)/stdplugs

CCFLAGS=-I ../sdk $(WXFLAGS)
CCLIBS= -L.. -L. -L$(BASEBINDIR) -lsdk-base -lsdk-core $(WXLIBS)

#CCFLAGS=-DBUILDINGDLL -fPIC -I .. $(WXFLAGS)
#LDFLAGS=-Wl,-soname,$(LIBNAME).$(DYNLIBEXT)
 #-Wl,--out-implib,$(LIBDIR)/$(LIBNAME).a
#CCLIBS=-L $(BASELIBDIR) $(WXLIBS)

MSEYTOUXOBJ=$(OBJDIR)/MSeytouxFunc.o \
            $(OBJDIR)/MSeytouxPlug.o

MSEYTOUXNAME=mseytoux


$(OBJDIR)/%.o : %.cpp %.h
	$(CPP) $(CCFLAGS) -c $< -o $@


all: prechecks $(MSEYTOUXOBJ)
	$(CPP) $(MSEYTOUXOBJ) $(CCLIBS) $(CCFLAGS) -o $(BINDIR)/$(MSEYTOUXNAME).$(DYNLIBEXT) -shared


prechecks:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)


clean:
	@rm -f $(OBJDIR)/*


doc:
	@mkdir -p $(DOCDIR)
	@doxygen

