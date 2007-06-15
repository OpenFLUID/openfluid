# sdk-core makefile


include ../../makeopts.inc

OBJDIR=$(BASEOBJDIR)/core
BINDIR=$(BASEBINDIR)
LIBDIR=$(BASELIBDIR)
DOCDIR=$(BASEDOCDIR)/core

LIBFILE=$(SDKLIBCOREFILE)

CCFLAGS= $(CCCFLAGS) -DBUILDINGDLL $(DYNLIBCCFLAGS) $(WXFLAGS) -I .. -I.
CCLIBS=$(WXLIBS)


OBJ=$(OBJDIR)/DateTime.o \
    $(OBJDIR)/TimeSerieItem.o \
    $(OBJDIR)/TimeSerie.o \
    $(OBJDIR)/ChronDataSource.o \
    $(OBJDIR)/ValueFactory.o \
    $(OBJDIR)/RainEvent.o \
    $(OBJDIR)/HydroObject.o \
    $(OBJDIR)/SurfaceUnit.o \
    $(OBJDIR)/GroundwaterUnit.o \
    $(OBJDIR)/ReachSegment.o \
    $(OBJDIR)/SpatialRepository.o \
    $(OBJDIR)/CoreRepository.o


$(OBJDIR)/%.o : %.cpp %.h
	$(CPP) $(CCFLAGS) -c $< -o $@


all: prechecks $(OBJ)
	$(CPP) -o $(BINDIR)/$(LIBFILE) $(OBJ) $(CCLIBS) -shared $(SDKLIBCORELDFLAGS) 

prechecks:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)
	@mkdir -p $(LIBDIR)


clean:
	@rm -f $(OBJDIR)/*


doc:
	@mkdir -p $(DOCDIR)
	@doxygen

