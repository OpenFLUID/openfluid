# sdk-base makefile


# en cours, a revoir

include ../../makeopts.inc

OBJDIR=$(BASEOBJDIR)/base
BINDIR=$(BASEBINDIR)
LIBDIR=$(BASELIBDIR)
DOCDIR=$(BASEDOCDIR)/base

LIBNAME=$(SDKLIBBASEFILE)

CCFLAGS= $(CCCFLAGS) -DBUILDINGDLL $(DYNLIBCCFLAGS) $(WXFLAGS) -I.. -I.
CCLIBS=-L$(BINDIR) $(WXLIBS) -l$(SDKLIBCOREROOT)





OBJ=$(OBJDIR)/SimStatus.o \
    $(OBJDIR)/PlugFunction.o \
    $(OBJDIR)/ExecMsgs.o    


$(OBJDIR)/%.o : %.cpp %.h
	$(CPP) $(CCFLAGS) -c $< -o $@


all: prechecks $(OBJ)
	$(CPP) -o $(BINDIR)/$(SDKLIBBASEFILE) $(OBJ) -shared $(SDKLIBBASELDFLAGS) $(CCLIBS)

prechecks:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)
	@mkdir -p $(LIBDIR)


clean:
	@rm -f $(OBJDIR)/*


doc:
	@mkdir -p $(DOCDIR)
	@doxygen

