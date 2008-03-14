# sdk-core makefile


include ../makeopts.inc

OBJDIR=$(BASEOBJDIR)
BINDIR=$(BASEBINDIR)
LIBDIR=$(BASELIBDIR)
DOCDIR=$(BASEDOCDIR)

LIBFILE=$(SDKLIBFILE)

CCFLAGS= $(CCCFLAGS) -DBUILDINGDLL $(DYNLIBCCFLAGS) $(WXFLAGS) -I .. -I.
CCLIBS=$(WXLIBS)



COREOBJ=$(OBJDIR)/core/DateTime.o \
    $(OBJDIR)/core/TimeSerieItem.o \
    $(OBJDIR)/core/TimeSerie.o \
    $(OBJDIR)/core/ChronDataSource.o \
    $(OBJDIR)/core/ValueFactory.o \
    $(OBJDIR)/core/RainSources.o \
    $(OBJDIR)/core/HydroObject.o \
    $(OBJDIR)/core/SurfaceUnit.o \
    $(OBJDIR)/core/GroundwaterUnit.o \
    $(OBJDIR)/core/ReachSegment.o \
    $(OBJDIR)/core/SpatialRepository.o \
    $(OBJDIR)/core/CoreRepository.o



BASEOBJ=$(OBJDIR)/base/SimStatus.o \
    $(OBJDIR)/base/PlugFunction.o \
    $(OBJDIR)/base/ExecMsgs.o


$(OBJDIR)/core/%.o : core/%.cpp core/%.h
	$(CPP) $(CCFLAGS) -c $< -o $@

$(OBJDIR)/base/%.o : base/%.cpp base/%.h
	$(CPP) $(CCFLAGS) -c $< -o $@


all: prechecks $(COREOBJ) $(BASEOBJ)
	$(CPP) -o $(BINDIR)/$(LIBFILE) $(COREOBJ) $(BASEOBJ) $(CCLIBS) -shared $(SDKLIBLDFLAGS) 

prechecks:
	@mkdir -p $(OBJDIR)/core
	@mkdir -p $(OBJDIR)/base
	@mkdir -p $(BINDIR)
	@mkdir -p $(LIBDIR)


clean:
	@rm -f -R $(OBJDIR)/*


doc:
	@mkdir -p $(DOCDIR)
	@doxygen


