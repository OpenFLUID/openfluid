# main program makefile


include ../../makeopts.inc

OBJDIR=$(BASEOBJDIR)/main
BINDIR=$(BASEBINDIR)
DOCDIR=$(BASEDOCDIR)/main
LIBDIR=$(BASELIBDIR)

CCFLAGS=$(CCCFLAGS) $(OPENFLUIDFLAGS) $(WXFLAGS)
CCLIBS=$(OPENFLUIDLIBS) -L$(BASELIBDIR) $(WXLIBS)


OBJ= $(OBJDIR)/AppTools.o \
     $(OBJDIR)/RuntimeEnv.o \
     $(OBJDIR)/Engine.o \
     $(OBJDIR)/PluginManager.o \
     $(OBJDIR)/IOMan.o \
     $(OBJDIR)/main.o \
     $(OBJDIR)/xml/tinystr.o \
     $(OBJDIR)/xml/tinyxml.o \
     $(OBJDIR)/xml/tinyxmlerror.o \
     $(OBJDIR)/xml/tinyxmlparser.o


$(OBJDIR)/%.o : %.cpp %.h
	$(CPP) $(CCFLAGS) -c $< -o $@

$(OBJDIR)/xml/%.o : xml/%.cpp
	$(CPP) $(CCFLAGS) -c $< -o $@



all: prechecks $(OBJ)
	$(CPP) $(OBJ) $(CCLIBS) $(CCFLAGS) -o $(BINDIR)/$(EXEFILE)
	

prechecks:
	@mkdir -p $(OBJDIR)/xml
	@mkdir -p $(BINDIR)
	@mkdir -p $(DOCDIR)


clean:
#	@rm -f -R $(OBJDIR)


doc:
	@mkdir -p $(DOCDIR)
	@doxygen
