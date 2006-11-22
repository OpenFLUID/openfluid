# main program makefile


include ../makeopts.inc

OBJDIR=$(BASEOBJDIR)/main
BINDIR=$(BASEBINDIR)
DOCDIR=$(BASEDOCDIR)/main

CCFLAGS=$(CCCFLAGS) -I ../sdk -I ../sdk/base $(WXFLAGS)
CCLIBS=-L$(BASELIBDIR) -L$(BINDIR) -ltinyxml -l$(SDKLIBSROOT)-base -l$(SDKLIBSROOT)-core -Wl,-rpath,. $(WXLIBS)


OBJ= $(OBJDIR)/AppTools.o \
     $(OBJDIR)/Module.o \
     $(OBJDIR)/Engine.o \
     $(OBJDIR)/PluginManager.o \
     $(OBJDIR)/ColTextParser.o \
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
	@rm -f -R $(OBJDIR)/*


doc:
	@mkdir -p $(DOCDIR)
	@doxygen
