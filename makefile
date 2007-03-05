# main makefile

include ./makeopts.inc



all: main stdplugs

.PHONY: main stdplugs




main:
	@echo ""
	@echo "==== Compiling main program ===="
	@(cd main && $(MAKE))


stdplugs:
	@echo ""
	@echo "==== Compiling plugins ===="
	@(cd stdplugs && $(MAKE))


clean:
	@(cd main && $(MAKE) clean)
	@(cd stdplugs && $(MAKE) clean)


doc:
	@(cd main && $(MAKE) doc)
	@(cd stdplugs && $(MAKE) doc)


purge: clean
	@rm -f $(BASEBINDIR)/*


	
	




