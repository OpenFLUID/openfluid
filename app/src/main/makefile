# main program makefile


include ../../makeopts.inc

OBJDIR=$(BASEOBJDIR)/main
BINDIR=$(BASEBINDIR)
DOCDIR=$(BASEDOCDIR)/main
LIBDIR=$(BASELIBDIR)

CCFLAGS=$(CCCFLAGS) $(MHYDASDKFLAGS) $(WXFLAGS)
CCLIBS=$(MHYDASDKLIBS) -L$(BASELIBDIR) -ltinyxml $(WXLIBS)


OBJ= $(OBJDIR)/AppTools.o \
     $(OBJDIR)/RuntimeEnv.o \
     $(OBJDIR)/Engine.o \
     $(OBJDIR)/PluginManager.o \
     $(OBJDIR)/IOMan.o \
     $(OBJDIR)/main.o


$(OBJDIR)/%.o : %.cpp %.h
	$(CPP) $(CCFLAGS) -c $< -o $@


all: prechecks xml $(OBJ)
	$(CPP) $(OBJ) $(CCLIBS) $(CCFLAGS) -o $(BINDIR)/$(EXEFILE)

xml: prechecks
	@(cd xml && $(MAKE))
	

prechecks:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJDIR)/xml
	@mkdir -p $(BINDIR)


clean:
	@rm -f -R $(OBJDIR)


doc:
	@mkdir -p $(DOCDIR)
	@doxygen
