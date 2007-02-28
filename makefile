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


clean-installed-sdk:
	@echo ""
	@echo "==== Removing installed SDK ===="
	@rm -f -R /usr/include/mhydasdk
	@rm -f /usr/lib/mhydasdk*


install-sdk: sdk clean-installed-sdk
	@echo ""
	@echo "==== Installing SDK ===="
	@echo "Installing header files"
	@mkdir -p /usr/include/mhydasdk/core
	@mkdir -p /usr/include/mhydasdk/base
	@cp sdk/mhydasdk-*.h /usr/include/mhydasdk/
	@cp sdk/core/*.h /usr/include/mhydasdk/core/
	@cp sdk/base/*.h /usr/include/mhydasdk/base/
	@echo "Installing libraries"
	@cp $(BASELIBDIR)/lib$(SDKLIBBASE).$(DYNLIBEXT) /usr/lib
	@cp $(BASELIBDIR)/lib$(SDKLIBCORE).$(DYNLIBEXT) /usr/lib
	@chmod 644 /usr/lib/lib$(SDKLIBBASE).$(DYNLIBEXT)
	@chmod 644 /usr/lib/lib$(SDKLIBCORE).$(DYNLIBEXT)


packages: all
	@echo ""
	@echo "==== Preparing packages ===="
	@mkdir -p $(BASEPACKDIR)
	@rm -f -R $(BASEPACKDIR)/*
	@mkdir -p $(BASEPACKDIR)/to_pack/$(PLUGSUBDIR)
	@cp -R -f $(BASEBINDIR)/$(PLUGSUBDIR)/* $(BASEPACKDIR)/to_pack/$(PLUGSUBDIR)
	@cp -f $(BASEBINDIR)/$(EXEFILE) $(BASEPACKDIR)/to_pack/
	@cp -f $(BASEBINDIR)/lib* $(BASEPACKDIR)/to_pack/
	@echo ""
	@echo "==== Building tgz package ===="
	@(cd $(BASEPACKDIR)/to_pack/ && tar cvzf $(BASEPACKDIR)/$(BINPACKNAMEROOT).tgz *)
	
	




