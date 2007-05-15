# main makefile

include ./makeopts.inc



all: sdk

.PHONY: sdk

sdk: sdk-core sdk-base


sdk-core:
	@echo ""
	@echo "==== Compiling core SDK ===="
	@(cd src/core && $(MAKE))

sdk-base:
	@echo ""
	@echo "==== Compiling base SDK ===="
	@(cd src/base && $(MAKE))


mhydasdk-conf-file:
	@echo ""
	@echo "==== Creating mhydasdk-config file ===="
	@echo "#!/bin/sh" > $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)
	@echo "" >> $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)    
	@echo "SDKLIBSPATH=$(INSTSDKLIBSPATH)" >> $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)        
	@echo "SDKINCPATH=$(INSTSDKINCPATH)" >> $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)        
	@echo "SDKLIBBASEROOT=$(SDKLIBBASE)" >> $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)                
	@echo "SDKLIBCOREROOT=$(SDKLIBCORE)" >> $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)
	@cat ./mhydasdk-config.in >> $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)
	@chmod ugo+x $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)


clean:
	@(cd src/core && $(MAKE) clean)
	@(cd src/base && $(MAKE) clean)


doc:
	@(cd src/base && $(MAKE) doc)
	@(cd src/core && $(MAKE) doc)


purge: clean
	@rm -f $(BASEBINDIR)/*


clean-installed-sdk:
	@echo ""
	@echo "==== Removing installed SDK ===="
	@$(SUDO) rm -f -R $(INSTSDKINCPATH)
	@$(SUDO) rm -f $(INSTSDKLIBSPATH)/lib$(SDKLIBBASE).$(DYNLIBEXT)
	@$(SUDO) rm -f $(INSTSDKLIBSPATH)/lib$(SDKLIBCORE).$(DYNLIBEXT)	
	@$(SUDO) rm -f $(INSTSDKBINPATH)/$(MHYDASDKCONFIGFILE)


install-sdk: sdk clean-installed-sdk mhydasdk-conf-file
	@echo ""
	@echo "==== Installing SDK ===="
	@echo "Installing header files"
	@$(SUDO) mkdir -p $(INSTSDKINCPATH)/core/
	@$(SUDO) mkdir -p $(INSTSDKINCPATH)/base/
	@$(SUDO) cp src/mhydasdk-*.h $(INSTSDKINCPATH)/
	@$(SUDO) cp src/core/*.h $(INSTSDKINCPATH)/core/
	@$(SUDO) cp src/base/*.h $(INSTSDKINCPATH)/base/
	@echo "Installing libraries"
	@$(SUDO) cp $(BASEBINDIR)/lib$(SDKLIBBASE).$(DYNLIBEXT) $(INSTSDKLIBSPATH)
	@$(SUDO) cp $(BASEBINDIR)/lib$(SDKLIBCORE).$(DYNLIBEXT) $(INSTSDKLIBSPATH)
	@$(SUDO) chmod 644 $(INSTSDKLIBSPATH)/lib$(SDKLIBBASE).$(DYNLIBEXT)
	@$(SUDO) chmod 644 $(INSTSDKLIBSPATH)/lib$(SDKLIBCORE).$(DYNLIBEXT)
	@$(SUDO) cp $(BASEBINDIR)/$(MHYDASDKCONFIGFILE) $(INSTSDKBINPATH)


deb-packages: all mhydasdk-conf-file
	@echo ""	
	@mkdir -p $(BASEPACKDIR)
	@rm -f -R $(BASEPACKDIR)/*
	@echo "==== Building packages (debian based distro) ===="
	@echo "Building library package"
	@mkdir -p $(BASEPACKDIR)/debian/$(LIBPACKNAME)$(INSTSDKLIBSPATH)
	@mkdir -p $(BASEPACKDIR)/debian/$(LIBPACKNAME)/usr/share/doc/$(LIBPACKROOT)
	@mkdir -p $(BASEPACKDIR)/debian/$(LIBPACKNAME)/DEBIAN
	@cp $(BASEBINDIR)/lib$(SDKLIBBASE).$(DYNLIBEXT) $(BASEPACKDIR)/debian/$(LIBPACKNAME)$(INSTSDKLIBSPATH)
	@cp $(BASEBINDIR)/lib$(SDKLIBCORE).$(DYNLIBEXT) $(BASEPACKDIR)/debian/$(LIBPACKNAME)$(INSTSDKLIBSPATH)
	@cp resources/doc/* $(BASEPACKDIR)/debian/$(LIBPACKNAME)/usr/share/doc/$(LIBPACKROOT)
	@cp resources/debian/common/* $(BASEPACKDIR)/debian/$(LIBPACKNAME)/DEBIAN
	@cp resources/debian/libmhydasdk/* $(BASEPACKDIR)/debian/$(LIBPACKNAME)/DEBIAN
	@cp resources/doc/COPYING $(BASEPACKDIR)/debian/$(LIBPACKNAME)/DEBIAN/copyright
	@echo "Package: $(LIBPACKROOT)\nVersion: $(MAJORVER).$(MINORVER)-$(SVNREV)\nSection: $(PACKSECTION)\nPriority: $(PACKPRIORITY)\nArchitecture: $(PACKARCH)\nDepends: $(PACKDEPENDS)\nMaintainer: $(PACKMAINTAINER)\nDescription: $(LIBPACKDESC).\n .\n This package is built using revision $(SVNREV)" > $(BASEPACKDIR)/debian/$(LIBPACKNAME)/DEBIAN/control
	@(cd $(BASEPACKDIR)/debian && dpkg-deb --build $(LIBPACKNAME))
	@echo "Building development package"
	@mkdir -p $(BASEPACKDIR)
	@mkdir -p $(BASEPACKDIR)/debian/$(DEVPACKNAME)$(INSTSDKINCPATH)/core/
	@mkdir -p $(BASEPACKDIR)/debian/$(DEVPACKNAME)$(INSTSDKINCPATH)/base/
	@mkdir -p $(BASEPACKDIR)/debian/$(DEVPACKNAME)$(INSTSDKBINPATH)
	@mkdir -p $(BASEPACKDIR)/debian/$(DEVPACKNAME)/usr/share/doc/$(DEVPACKROOT)
	@mkdir -p $(BASEPACKDIR)/debian/$(DEVPACKNAME)/DEBIAN
	@cp src/mhydasdk-*.h $(BASEPACKDIR)/debian/$(DEVPACKNAME)$(INSTSDKINCPATH)/
	@cp src/core/*.h $(BASEPACKDIR)/debian/$(DEVPACKNAME)$(INSTSDKINCPATH)/core/ 
	@cp src/base/*.h $(BASEPACKDIR)/debian/$(DEVPACKNAME)$(INSTSDKINCPATH)/base/
	@cp $(BASEBINDIR)/$(MHYDASDKCONFIGFILE) $(BASEPACKDIR)/debian/$(DEVPACKNAME)$(INSTSDKBINPATH)
	@cp resources/doc/* $(BASEPACKDIR)/debian/$(DEVPACKNAME)/usr/share/doc/$(DEVPACKROOT)
	@cp resources/debian/common/* $(BASEPACKDIR)/debian/$(DEVPACKNAME)/DEBIAN
	@cp resources/debian/mhydasdk-dev/* $(BASEPACKDIR)/debian/$(DEVPACKNAME)/DEBIAN
	@cp resources/doc/COPYING $(BASEPACKDIR)/debian/$(DEVPACKNAME)/DEBIAN/copyright
	@echo "Package: $(DEVPACKROOT)\nVersion: $(MAJORVER).$(MINORVER)\nSection: $(PACKSECTION)\nPriority: $(PACKPRIORITY)\nArchitecture: $(PACKARCH)\nDepends: $(PACKDEPENDS), $(LIBPACKROOT) (=$(MAJORVER).$(MINORVER))\nMaintainer: $(PACKMAINTAINER)\nDescription: $(DEVPACKDESC).\n .\n This package is built using revision $(SVNREV)" > $(BASEPACKDIR)/debian/$(DEVPACKNAME)/DEBIAN/control
	@(cd $(BASEPACKDIR)/debian && dpkg-deb --build $(DEVPACKNAME))




#	@cp -R -f $(BASEBINDIR)/$(PLUGSUBDIR)/* $(BASEPACKDIR)/to_pack/$(PLUGSUBDIR)
#	@cp -f $(BASEBINDIR)/$(EXEFILE) $(BASEPACKDIR)/to_pack/
#	@cp -f $(BASEBINDIR)/lib* $(BASEPACKDIR)/to_pack/
#	@echo ""
#	@echo "==== Building tgz package ===="
#	@(cd $(BASEPACKDIR)/to_pack/ && tar cvzf $(BASEPACKDIR)/$(BINPACKNAMEROOT).tgz *)
	
	




