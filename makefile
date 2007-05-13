# main makefile

include ./makeopts.inc



all: main stdplugs

.PHONY: main stdplugs




main:
	@echo ""
	@echo "==== Compiling main program ===="
	@(cd src/main && $(MAKE))


stdplugs:
	@echo ""
	@echo "==== Compiling plugins ===="
	@(cd src/stdplugs && $(MAKE))


clean:
	@(cd src/main && $(MAKE) clean)
	@(cd src/stdplugs && $(MAKE) clean)


doc:
	@(cd src/main && $(MAKE) doc)
	@(cd src/stdplugs && $(MAKE) doc)


purge: clean
	@rm -f $(BASEBINDIR)/*


	
	




