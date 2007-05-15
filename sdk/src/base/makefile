# sdk-base makefile


# en cours, a revoir

include ../../makeopts.inc

OBJDIR=$(BASEOBJDIR)/base
BINDIR=$(BASEBINDIR)
LIBDIR=$(BASELIBDIR)
DOCDIR=$(BASEDOCDIR)/base

LIBNAME=lib$(SDKLIBBASE)

CCFLAGS= $(CCCFLAGS) -DBUILDINGDLL $(DYNLIBCCFLAGS) $(WXFLAGS) -I.. -I.
CCLIBS=-L$(BINDIR) $(WXLIBS) -l$(SDKLIBCORE)

LDFLAGS=-Wl,-soname,$(LIBNAME).$(DYNLIBEXT)
ifeq ($(OSTYPE),msys)
  LDFLAGS=-Wl,-no-undefined -Wl,--enable-runtime-pseudo-reloc #-Wl,--out-implib,$(LIBDIR)/$(LIBNAME).$(STALIBEXT)
endif




OBJ=$(OBJDIR)/SimStatus.o \
    $(OBJDIR)/PlugFunction.o \
#    $(OBJDIR)/CompBlock.o \
#    $(OBJDIR)/Function.o \
#    $(OBJDIR)/Plugin.o \
    $(OBJDIR)/LastError.o \


$(OBJDIR)/%.o : %.cpp %.h
	$(CPP) $(CCFLAGS) -c $< -o $@


all: prechecks $(OBJ)
	$(CPP) -o $(BINDIR)/$(LIBNAME).$(DYNLIBEXT) $(OBJ) -shared $(LDFLAGS) $(CCLIBS)

prechecks:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)
	@mkdir -p $(LIBDIR)


clean:
	@rm -f $(OBJDIR)/*


doc:
	@mkdir -p $(DOCDIR)
	@doxygen

