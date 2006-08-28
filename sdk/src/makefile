# main makefile

include ./makeopts.inc



all: sdk main

.PHONY: sdk


sdk:
	@echo ""
	@echo "==== Compiling SDK ===="
	@(cd sdk/xml && $(MAKE))
	@(cd sdk/base && $(MAKE))
	@(cd sdk/core && $(MAKE))


main: sdk
	@echo ""
	@echo "==== Compiling main program ===="
	@(cd main && $(MAKE))


clean:
	@(cd main && $(MAKE) clean)
	@(cd sdk/xml && $(MAKE) clean)
	@(cd sdk/base && $(MAKE) clean)
	@(cd sdk/core && $(MAKE) clean)

doc:
	@(cd main && $(MAKE) doc)
	@(cd sdk/xml && $(MAKE) doc)
	@(cd sdk/base && $(MAKE) doc)
	@(cd sdk/core && $(MAKE) doc)


purge: clean
	@rm -f $(BASEBINDIR)/*

