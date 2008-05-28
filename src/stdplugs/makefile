# standard plugins makefile



include ../../makeopts.inc

OBJDIR=$(BASEOBJDIR)/stdplugs
BINDIR=$(BASEBINDIR)/$(PLUGSUBDIR)
LIBDIR=$(BASELIBDIR)
DOCDIR=$(BASEDOCDIR)/stdplugs



all: prechecks   
	@(cd water.surf-uz.runoff-infiltration.mseytoux && $(MAKE) FORCEOBJPATH=$(OBJDIR) FORCEBINPATH=$(BINDIR))	
	@(cd water.surf.transfer-su.hayami && $(MAKE) FORCEOBJPATH=$(OBJDIR) FORCEBINPATH=$(BINDIR))
	@(cd water.surf.transfer-rs.hayami && $(MAKE) FORCEOBJPATH=$(OBJDIR) FORCEBINPATH=$(BINDIR))
	@(cd water.surf.transfer-rs-exchange-gu.hayami-tank && $(MAKE) FORCEOBJPATH=$(OBJDIR) FORCEBINPATH=$(BINDIR))		
	@(cd water.atm-surf.rain-su.files && $(MAKE) FORCEOBJPATH=$(OBJDIR) FORCEBINPATH=$(BINDIR))	
	@(cd water.atm-surf.rain-rs.files && $(MAKE) FORCEOBJPATH=$(OBJDIR) FORCEBINPATH=$(BINDIR))	


prechecks:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)


clean:
	@rm -f $(OBJDIR)/*


doc:
#	@mkdir -p $(DOCDIR)
#	@doxygen

