# tinyxml makefile

include ../../../makeopts.inc


CCFLAGS = -Wall -Wno-unknown-pragmas -Wno-format -O3

OBJDIR = $(BASEOBJDIR)/main/xml
LIBDIR = $(BASELIBDIR)

LIB = $(LIBDIR)/libtinyxml.a


OBJ = $(OBJDIR)/tinystr.o $(OBJDIR)/tinyxmlparser.o \
      $(OBJDIR)/tinyxmlerror.o $(OBJDIR)/tinyxml.o

			
$(OBJDIR)/%.o : %.cpp
	$(CPP) $(CCFLAGS) -c $< -o $@

			
all: prechecks $(OBJ)
	ar cr $(LIB) $(OBJ)
	ranlib $(LIB)


prechecks:
	@mkdir -p $(LIBDIR)  
	@mkdir -p $(OBJDIR)  


clean:
	@rm -f $(OBJDIR)/*
	@rm -f $(LIBDIR)/*


doc:
