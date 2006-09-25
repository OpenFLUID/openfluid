# sdk-base makefile


# en cours, à revoir

include ../../makeopts.inc

OBJDIR=$(BASEOBJDIR)/sdk/base
BINDIR=$(BASEBINDIR)
LIBDIR=$(BASELIBDIR)
DOCDIR=$(BASEDOCDIR)/sdk/base

CCFLAGS= $(CCCFLAGS) -DBUILDINGDLL -fPIC $(WXFLAGS) -I .. -I.
LDFLAGS=-Wl,-soname,$(LIBNAME).$(DYNLIBEXT)
 #-Wl,--out-implib,$(LIBDIR)/$(LIBNAME).a
CCLIBS=-L $(BASELIBDIR) $(WXLIBS)

LIBNAME=libsdk-base

OBJ=$(OBJDIR)/RuntimeEnv.o \
    $(OBJDIR)/SimStatus.o \
    $(OBJDIR)/CompBlock.o \
    $(OBJDIR)/Function.o \
    $(OBJDIR)/Plugin.o \
    $(OBJDIR)/LastError.o \


$(OBJDIR)/%.o : %.cpp %.h
	$(CPP) $(CCFLAGS) -c $< -o $@


all: prechecks $(OBJ)
		$(CPP) -shared $(LDFLAGS) -o $(BINDIR)/$(LIBNAME).$(DYNLIBEXT) $(OBJ)

prechecks:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)
	@mkdir -p $(LIBDIR)


clean:
	@rm -f $(OBJDIR)/*


doc:
	@mkdir -p $(DOCDIR)
	@doxygen

