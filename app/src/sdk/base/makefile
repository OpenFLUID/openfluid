# sdk-base makefile


# en cours, a revoir

include ../../makeopts.inc

OBJDIR=$(BASEOBJDIR)/sdk/base
BINDIR=$(BASEBINDIR)
LIBDIR=$(BASELIBDIR)
DOCDIR=$(BASEDOCDIR)/sdk/base

LIBNAME=lib$(SDKLIBBASE)

CCFLAGS= $(CCCFLAGS) -DBUILDINGDLL $(DYNLIBCCFLAGS) $(WXFLAGS) -I.. -I.
CCLIBS=-L$(BASELIBDIR) $(WXLIBS) -l$(SDKLIBCORE)

LDFLAGS=-Wl,-soname,$(LIBNAME).$(DYNLIBEXT)
ifeq ($(OSTYPE),msys)
  LDFLAGS=-Wl,-no-undefined -Wl,--enable-runtime-pseudo-reloc #-Wl,--out-implib,$(LIBDIR)/$(LIBNAME).$(STALIBEXT)
endif





OBJ=$(OBJDIR)/RuntimeEnv.o \
    $(OBJDIR)/SimStatus.o \
    $(OBJDIR)/CompBlock.o \
    $(OBJDIR)/Function.o \
    $(OBJDIR)/Plugin.o \
    $(OBJDIR)/LastError.o \


$(OBJDIR)/%.o : %.cpp %.h
	$(CPP) $(CCFLAGS) -c $< -o $@


all: prechecks $(OBJ)
	$(CPP) -o $(LIBDIR)/$(LIBNAME).$(DYNLIBEXT) $(OBJ) -shared $(LDFLAGS) $(CCLIBS)

prechecks:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)
	@mkdir -p $(LIBDIR)


clean:
	@rm -f $(OBJDIR)/*


doc:
	@mkdir -p $(DOCDIR)
	@doxygen

