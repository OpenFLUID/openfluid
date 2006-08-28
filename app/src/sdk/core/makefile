# sdk-core makefile


# en cours, à revoir

include ../../makeopts.inc

OBJDIR=$(BASEOBJDIR)/sdk/core
BINDIR=$(BASEBINDIR)
LIBDIR=$(BASELIBDIR)
DOCDIR=$(BASEDOCDIR)/sdk/core


CCFLAGS=-DBUILDINGDLL -fPIC -I .. $(WXFLAGS)
LDFLAGS=-Wl,-soname,$(LIBNAME).$(DYNLIBEXT)
 #-Wl,--out-implib,$(LIBDIR)/$(LIBNAME).a
CCLIBS=-L $(BASELIBDIR) $(WXLIBS)

LIBNAME=libsdk-core

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
		$(CC) -shared $(LDFLAGS) -o $(BINDIR)/$(LIBNAME).$(DYNLIBEXT) $(OBJ)

prechecks:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)
	@mkdir -p $(LIBDIR)


clean:
	@rm -f $(OBJDIR)/*


doc:
	@mkdir -p $(DOCDIR)
	@doxygen

