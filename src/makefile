# main makefile

include ./makeopts.inc



all: sdk main stdplugs

.PHONY: sdk


sdk:
	@echo ""
	@echo "==== Compiling SDK ===="
	@(cd sdk/core && $(MAKE))
	@(cd sdk/base && $(MAKE))


main: sdk
	@echo ""
	@echo "==== Compiling main program ===="
	@(cd main && $(MAKE))


stdplugs: sdk
	@echo ""
	@echo "==== Compiling plugins ===="
	@(cd stdplugs && $(MAKE))


clean:
	@(cd main && $(MAKE) clean)
	@(cd stdplugs && $(MAKE) clean)
	@(cd sdk/core && $(MAKE) clean)
	@(cd sdk/base && $(MAKE) clean)


doc:
	@(cd sdk/base && $(MAKE) doc)
	@(cd sdk/core && $(MAKE) doc)
	@(cd main && $(MAKE) doc)
	@(cd stdplugs && $(MAKE) doc)


purge: clean
	@rm -f $(BASEBINDIR)/*

