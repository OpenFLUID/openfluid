# main makefile

include ./makeopts.inc


all: sdk

.PHONY: sdk

sdk: 
	@echo ""
	@echo "==== Compiling core SDK ===="
	@(cd src && $(MAKE))
	@$(LDCONFIGBUILD)


mhydasdk-conf-file:
	@echo ""
	@echo "==== Creating mhydasdk-config file ===="
	@echo "#!/bin/sh" > $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)
	@echo "" >> $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)    
	@echo "SDKLIBPATH=$(INSTSDKLIBPATH)" >> $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)        
	@echo "SDKINCPATH=$(INSTSDKINCPATH)" >> $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)        
	@echo "SDKLIBROOT=$(SDKLIBROOT)" >> $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)                
	@echo "MAJORVER=$(MAJORVER)" >> $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)
	@echo "MINORVER=$(MINORVER)" >> $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)  
	@echo "SVNREV=$(SVNREV)" >> $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)	
	@cat ./mhydasdk-config.in >> $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)
	@chmod ugo+x $(BASEBINDIR)/$(MHYDASDKCONFIGFILE)


clean:
	@(cd src && $(MAKE) clean)
	@rm -R -f $(BASEDOCDIR)


docs:
	@mkdir -p $(BASEDOCDIR)
	@doxygen


purge: clean
	@rm -f $(BASEBINDIR)/*


clean-installed-sdk:
	@echo ""
	@echo "==== Removing installed SDK ===="
	@$(SUDO) rm -f -R $(INSTSDKINCPATH)
	@$(SUDO) rm -f $(INSTSDKLIBPATH)/$(SDKLIBFILE)
	@$(SUDO) rm -f $(INSTSDKBINPATH)/$(MHYDASDKCONFIGFILE)
	@$(SUDO) $(LDCONFIGINST)

install-sdk: sdk mhydasdk-conf-file #clean-installed-sdk
	@echo ""
	@echo "==== Installing SDK ===="
	@echo "Installing header files"
	@$(SUDO) mkdir -p $(INSTSDKINCPATH)/core/
	@$(SUDO) mkdir -p $(INSTSDKINCPATH)/base/
	@$(SUDO) cp src/mhydasdk-*.h $(INSTSDKINCPATH)/
	@$(SUDO) cp src/core/*.h $(INSTSDKINCPATH)/core/
	@$(SUDO) cp src/base/*.h $(INSTSDKINCPATH)/base/
	@echo "Installing libraries"
	@$(SUDO) cp $(BASEBINDIR)/$(SDKLIBFILE) $(INSTSDKLIBPATH)
	@$(SUDO) chmod 644 $(INSTSDKLIBPATH)/$(SDKLIBFILE)
	@$(SUDO) $(LDCONFIGINST)
	@$(SUDO) cp $(BASEBINDIR)/$(MHYDASDKCONFIGFILE) $(INSTSDKBINPATH)

ubuntu-edgy-packages: LOCALDIR = ubuntu-edgy
ubuntu-edgy-packages: all mhydasdk-conf-file docs
	@echo ""	
	@mkdir -p $(BASEPACKDIR)
	@rm -f -R $(BASEPACKDIR)/$(LOCALDIR)/*
	@echo "==== Building packages ($(LOCALDIR)) ===="
	@echo "Building library package"
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)$(INSTSDKLIBPATH)
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/usr/share/doc/$(LIBPACKROOT)
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN
	@cp $(BASEBINDIR)/$(SDKLIBFILE) $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)$(INSTSDKLIBPATH)
	@cp resources/doc/* $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/usr/share/doc/$(LIBPACKROOT)
	@cp resources/debian/common/* $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN
	@cp resources/debian/libmhydasdk/* $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/copyright
	@echo "Package: $(LIBPACKROOT)\nVersion: $(MAJORVER).$(MINORVER)-$(SVNREV)\nSection: $(PACKSECTION)\nPriority: $(PACKPRIORITY)\nArchitecture: $(PACKARCH)\nDepends: $(EDGYDEPENDS)\nMaintainer: $(PACKMAINTAINER)\nDescription: $(LIBPACKDESC).\n .\n This package is built using revision $(SVNREV)" > $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo " CHANGELOG:" >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control			
	@sed 's/^$$/./' ./resources/debian/common/changelog | sed 's/^/ /' >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@(cd $(BASEPACKDIR)/$(LOCALDIR) && dpkg-deb --build $(LIBPACKNAME))
	@echo "Building development package"
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKINCPATH)/core/
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKINCPATH)/base/
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKBINPATH)
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/usr/share/doc/$(DEVPACKROOT)/html
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN
	@cp src/mhydasdk-*.h $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKINCPATH)/
	@cp src/core/*.h $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKINCPATH)/core/ 
	@cp src/base/*.h $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKINCPATH)/base/
	@cp $(BASEBINDIR)/$(MHYDASDKCONFIGFILE) $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKBINPATH)
	@cp resources/doc/* $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/usr/share/doc/$(DEVPACKROOT)
	@cp -R $(BASEDOCDIR)/html/* $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/usr/share/doc/$(DEVPACKROOT)/html
	@cp resources/debian/common/* $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN
	@cp resources/debian/mhydasdk-dev/* $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/copyright
	@echo "Package: $(DEVPACKROOT)\nVersion: $(MAJORVER).$(MINORVER)-$(SVNREV)\nSection: $(PACKSECTION)\nPriority: $(PACKPRIORITY)\nArchitecture: $(PACKARCH)\nDepends: $(EDGYDEPENDS), $(LIBPACKROOT) (=$(MAJORVER).$(MINORVER)-$(SVNREV))\nMaintainer: $(PACKMAINTAINER)\nDescription: $(DEVPACKDESC).\n .\n This package is built using revision $(SVNREV)" > $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo " CHANGELOG:" >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control			
	@sed 's/^$$/./' ./resources/debian/common/changelog | sed 's/^/ /' >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@(cd $(BASEPACKDIR)/$(LOCALDIR) && dpkg-deb --build $(DEVPACKNAME))


ubuntu-dapper-packages: LOCALDIR = ubuntu-dapper
ubuntu-dapper-packages: all mhydasdk-conf-file docs	
	@echo ""	
	@mkdir -p $(BASEPACKDIR)
	@rm -f -R $(BASEPACKDIR)/$(LOCALDIR)/*
	@echo "==== Building packages ($(LOCALDIR)) ===="
	@echo "Building library package"
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)$(INSTSDKLIBPATH)
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/usr/share/doc/$(LIBPACKROOT)
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN
	@cp $(BASEBINDIR)/$(SDKLIBFILE) $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)$(INSTSDKLIBPATH)
	@cp resources/doc/* $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/usr/share/doc/$(LIBPACKROOT)
	@cp resources/debian/common/* $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN
	@cp resources/debian/libmhydasdk/* $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/copyright
	@echo "Package: $(LIBPACKROOT)" > $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo "Version: $(MAJORVER).$(MINORVER)-$(SVNREV)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo "Section: $(PACKSECTION)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo "Priority: $(PACKPRIORITY)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo "Architecture: $(PACKARCH)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo "Depends: $(DAPPERDEPENDS)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo "Maintainer: $(PACKMAINTAINER)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo "Description: $(LIBPACKDESC)." >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo " This package is built using revision $(SVNREV)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo " CHANGELOG:" >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control			
	@sed 's/^$$/./' ./resources/debian/common/changelog | sed 's/^/ /' >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@(cd $(BASEPACKDIR)/$(LOCALDIR) && dpkg-deb --build $(LIBPACKNAME))
	@echo "Building development package"
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKINCPATH)/core/
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKINCPATH)/base/
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKBINPATH)
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/usr/share/doc/$(DEVPACKROOT)/html
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN
	@cp src/mhydasdk-*.h $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKINCPATH)/
	@cp src/core/*.h $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKINCPATH)/core/ 
	@cp src/base/*.h $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKINCPATH)/base/
	@cp $(BASEBINDIR)/$(MHYDASDKCONFIGFILE) $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKBINPATH)
	@cp resources/doc/* $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/usr/share/doc/$(DEVPACKROOT)
	@cp -R $(BASEDOCDIR)/html/* $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/usr/share/doc/$(DEVPACKROOT)/html
	@cp resources/debian/common/* $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN
	@cp resources/debian/mhydasdk-dev/* $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/copyright
	@echo "Package: $(DEVPACKROOT)" > $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo "Version: $(MAJORVER).$(MINORVER)-$(SVNREV)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo "Section: $(PACKSECTION)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo "Priority: $(PACKPRIORITY)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo "Architecture: $(PACKARCH)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo "Depends: $(DAPPERDEPENDS), $(LIBPACKROOT) (=$(MAJORVER).$(MINORVER)-$(SVNREV))" >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo "Maintainer: $(PACKMAINTAINER)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo "Description: $(DEVPACKDESC)." >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo " This package is built using revision $(SVNREV)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo " CHANGELOG:" >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control			
	@sed 's/^$$/./' ./resources/debian/common/changelog | sed 's/^/ /' >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@(cd $(BASEPACKDIR)/$(LOCALDIR) && dpkg-deb --build $(DEVPACKNAME))




win32-packages: all mhydasdk-conf-file
	@echo ""	
	@rm -f -R $(BASEPACKDIR)/win32/*
	@mkdir -p $(BASEPACKDIR)/win32/$(LIBPACKNAME)_win32	
	@echo "==== Building packages (win32) ===="
	@echo "Building library package"
	@cp $(BASEBINDIR)/$(SDKLIBFILE) $(BASEPACKDIR)/win32/$(LIBPACKNAME)_win32
	@cd $(BASEPACKDIR)/win32/$(LIBPACKNAME)_win32/ && \
         $(ZIP) $(BASEPACKDIR)/win32/$(LIBPACKNAME)_win32.zip $(SDKLIBFILE) $(SDKLIBFILE) && \
         cd $(BASEMAINDIR) > /dev/null
	@echo "Building development package"
	@mkdir -p $(BASEPACKDIR)/win32/$(DEVPACKNAME)_win32/include/core/
	@mkdir -p $(BASEPACKDIR)/win32/$(DEVPACKNAME)_win32/include/base/
	@cp src/mhydasdk-*.h $(BASEPACKDIR)/win32/$(DEVPACKNAME)_win32/include/
	@cp src/core/*.h $(BASEPACKDIR)/win32/$(DEVPACKNAME)_win32/include/core/ 
	@cp src/base/*.h $(BASEPACKDIR)/win32/$(DEVPACKNAME)_win32/include/base/
	@cp $(BASEBINDIR)/$(MHYDASDKCONFIGFILE) $(BASEPACKDIR)/win32/$(DEVPACKNAME)_win32/
	@cd $(BASEPACKDIR)/win32/$(DEVPACKNAME)_win32/ && \
         $(ZIP) -r $(BASEPACKDIR)/win32/$(DEVPACKNAME)_win32.zip * && \
         cd $(BASEMAINDIR) > /dev/null

	




