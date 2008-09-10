# sdk-core makefile


include ../makeopts.inc

OBJDIR=$(BASEOBJDIR)
BINDIR=$(BASEBINDIR)
LIBDIR=$(BASELIBDIR)
DOCDIR=$(BASEDOCDIR)

LIBFILE=$(SDKLIBFILE)

CCFLAGS= $(CCCFLAGS) -DBUILDINGDLL $(DYNLIBCCFLAGS) $(WXFLAGS) -I.. -I.
CCLIBS=$(WXLIBS)



COREOBJ=$(OBJDIR)/core/ValueFactory.o \
    $(OBJDIR)/core/DateTime.o \
    $(OBJDIR)/core/ArrayValue.o \
    $(OBJDIR)/core/DistributedEvent.o \
    $(OBJDIR)/core/EventCollection.o \
    $(OBJDIR)/core/HydroObject.o \
    $(OBJDIR)/core/SurfaceUnit.o \
    $(OBJDIR)/core/GroundwaterUnit.o \
    $(OBJDIR)/core/ReachSegment.o \
    $(OBJDIR)/core/SpatialRepository.o \
    $(OBJDIR)/core/CoreRepository.o



BASEOBJ=$(OBJDIR)/base/SimStatus.o \
    $(OBJDIR)/base/PlugFunction.o \
    $(OBJDIR)/base/ExecMsgs.o \
    $(OBJDIR)/base/FunctionEnv.o
        

TOOLSOBJ= $(OBJDIR)/tools/Tools.o \
    $(OBJDIR)/tools/ColTextParser.o \
    $(OBJDIR)/tools/tinyxmlerror.o \
    $(OBJDIR)/tools/tinystr.o \
    $(OBJDIR)/tools/tinyxml.o \
    $(OBJDIR)/tools/tinyxmlparser.o \
    $(OBJDIR)/tools/DataSrcFile.o \
    $(OBJDIR)/tools/DTSerie.o \
    $(OBJDIR)/tools/DistribInterp.o           


$(OBJDIR)/core/%.o : core/%.cpp core/%.h
	$(CPP) $(CCFLAGS) -c $< -o $@

$(OBJDIR)/base/%.o : base/%.cpp base/%.h
	$(CPP) $(CCFLAGS) -c $< -o $@

$(OBJDIR)/tools/%.o : tools/%.cpp
	$(CPP) $(CCFLAGS) -c $< -o $@



all: prechecks $(COREOBJ) $(BASEOBJ) $(TOOLSOBJ)
	$(CPP) -o $(BINDIR)/$(LIBFILE) $(COREOBJ) $(BASEOBJ) $(TOOLSOBJ) $(CCLIBS) -shared $(SDKLIBLDFLAGS) 

prechecks:
	@mkdir -p $(OBJDIR)/core
	@mkdir -p $(OBJDIR)/base
	@mkdir -p $(OBJDIR)/tools	
	@mkdir -p $(BINDIR)
	@mkdir -p $(LIBDIR)


clean:
	@rm -f -R $(OBJDIR)/*


doc:
	@mkdir -p $(DOCDIR)
	@doxygen


