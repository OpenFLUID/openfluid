# main program makefile


include ../makeopts.inc

OBJDIR=$(BASEOBJDIR)/main
BINDIR=$(BASEBINDIR)
DOCDIR=$(BASEDOCDIR)/main

CCFLAGS=-I ../sdk -I ../sdk/base $(WXFLAGS)
CCLIBS=-L$(BASELIBDIR) -L$(BINDIR) -L. -ltinyxml -lsdk-base -lsdk-core -Wl,-rpath,. $(WXLIBS)


OBJ= $(OBJDIR)/AppTools.o \
     $(OBJDIR)/Module.o \
     $(OBJDIR)/HydroModule.o \
     $(OBJDIR)/Engine.o \
     $(OBJDIR)/PluginManager.o \
     $(OBJDIR)/IOMan.o \
     $(OBJDIR)/main.o


$(OBJDIR)/%.o : %.cpp %.h
	$(CPP) $(CCFLAGS) -c $< -o $@


all: prechecks $(OBJ)
	$(CPP) $(OBJ) $(CCLIBS) $(CCFLAGS) -o $(BINDIR)/$(EXEFILE)

prechecks:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)


clean:
	@rm -f $(OBJDIR)/*


doc:
	@mkdir -p $(DOCDIR)
	@doxygen
