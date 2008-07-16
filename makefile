# main makefile

include ./makeopts.inc

export PATH := $(OPENFLUIDPATH):$(PATH)
export OPENFLUIDSDKPREFIX := $(SDKPREFIX)


all: main stdplugs run-script

.PHONY: main stdplugs


run-script:
	@echo "==== Generating run script for build environment ===="
	@echo "#!/bin/sh" > $(BASEBINDIR)/run-engine.sh
	@echo "export LD_LIBRARY_PATH=$(OPENFLUIDSDKPREFIX)/lib:$$LD_LIBRARY_PATH" >> $(BASEBINDIR)/run-engine.sh
	@echo "./$(EXEFILE) \$$*" >> $(BASEBINDIR)/run-engine.sh
	@chmod 755 $(BASEBINDIR)/run-engine.sh

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
	@rm -f $(BASEBINDIR)


ubuntu-edgy-packages: LOCALDIR = ubuntu-edgy
ubuntu-edgy-packages: all
	@echo ""	
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)
	@rm -f -R $(BASEPACKDIR)/$(LOCALDIR)
	@echo "==== Building packages ($(LOCALDIR)) ===="
	@echo "Building package $(ENGPACKNAME)"
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/usr/bin	
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/usr/share/doc/$(ENGPACKROOT)
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN
	@cp $(BASEBINDIR)/$(EXEFILE) $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/usr/bin/$(EXEFILE)	
	@cp resources/doc/* $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/usr/share/doc/$(ENGPACKROOT)
	@cp resources/debian/* $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/copyright
	@echo "Package: $(ENGPACKROOT)\nVersion: $(MAJORVER).$(MINORVER)-$(SVNREV)$(RELEASESTATUS)\nSection: $(PACKSECTION)\nPriority: $(PACKPRIORITY)\nArchitecture: $(PACKARCH)\nDepends: $(ENGEDGYDEPENDS)\nReplaces: mhydas, mhydas-engine\nMaintainer: $(PACKMAINTAINER)\nDescription: $(ENGPACKDESC)\n .\n This package is built using revision $(SVNREV)." > $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo " CHANGELOG:" >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control			
	@sed 's/^$$/./' ./resources/doc/changelog | sed 's/^/ /' >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@(cd $(BASEPACKDIR)/$(LOCALDIR) && dpkg-deb --build $(ENGPACKNAME))
	@echo "Building package $(FUNCSPACKNAME)"
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)$(STDSYSDIR)/$(APPNAME)/$(PLUGSUBDIR)	
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/usr/share/doc/$(FUNCSPACKROOT)
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN
	@cp $(BASEBINDIR)/$(PLUGSUBDIR)/* $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)$(STDSYSDIR)/$(APPNAME)/$(PLUGSUBDIR)
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/copyright
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/usr/share/doc/$(FUNCSPACKROOT)/copyright
	@echo "Package: $(FUNCSPACKROOT)\nVersion: $(MAJORVER).$(MINORVER)-$(SVNREV)$(RELEASESTATUS)\nSection: $(PACKSECTION)\nPriority: $(PACKPRIORITY)\nArchitecture: $(PACKARCH)\nDepends: $(ENGPACKROOT) (= $(MAJORVER).$(MINORVER)-$(SVNREV)$(RELEASESTATUS))\nReplaces: mhydas, mhydas-engine\nMaintainer: $(PACKMAINTAINER)\nDescription: $(FUNCSPACKDESC)\n .\n This package is built using revision $(SVNREV)." > $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@echo " CHANGELOG:" >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control			
	@sed 's/^$$/./' ./resources/doc/changelog | sed 's/^/ /' >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@(cd $(BASEPACKDIR)/$(LOCALDIR) && dpkg-deb --build $(FUNCSPACKNAME))


ubuntu-dapper-packages: LOCALDIR = ubuntu-dapper
ubuntu-dapper-packages: all
	@echo ""	
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)
	@rm -f -R $(BASEPACKDIR)/$(LOCALDIR)
	@echo "==== Building packages ($(LOCALDIR)) ===="
	@echo "Building package"
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/usr/bin	
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/usr/share/doc/$(ENGPACKROOT)
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN
	@cp $(BASEBINDIR)/$(EXEFILE) $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/usr/bin/$(EXEFILE)
	@cp resources/doc/* $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/usr/share/doc/$(ENGPACKROOT)
	@cp resources/debian/* $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/copyright
	@echo "Package: $(ENGPACKROOT)" > $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo "Version: $(MAJORVER).$(MINORVER)-$(SVNREV)$(RELEASESTATUS)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo "Section: $(PACKSECTION)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo "Priority: $(PACKPRIORITY)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo "Architecture: $(PACKARCH)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo "Depends: $(ENGDAPPERDEPENDS)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo "Replaces: mhydas, mhydas-engine" >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo "Maintainer: $(PACKMAINTAINER)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo "Description: $(ENGPACKDESC)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo " This package is built using OpenFLUID-engine revision $(SVNREV)." >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo " CHANGELOG:" >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control			
	@sed 's/^$$/./' ./resources/doc/changelog | sed 's/^/ /' >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@(cd $(BASEPACKDIR)/$(LOCALDIR) && dpkg-deb --build $(ENGPACKNAME))
	@echo "Building package $(FUNCSPACKNAME)"
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)$(STDSYSDIR)/$(APPNAME)/$(PLUGSUBDIR)	
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/usr/share/doc/$(FUNCSPACKROOT)
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN
	@cp $(BASEBINDIR)/$(PLUGSUBDIR)/* $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)$(STDSYSDIR)/$(APPNAME)/$(PLUGSUBDIR)
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/copyright
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/usr/share/doc/$(FUNCSPACKROOT)/copyright
	@echo "Package: $(FUNCSPACKROOT)" > $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@echo "Version: $(MAJORVER).$(MINORVER)-$(SVNREV)$(RELEASESTATUS)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@echo "Section: $(PACKSECTION)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@echo "Priority: $(PACKPRIORITY)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@echo "Architecture: $(PACKARCH)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@echo "Depends: $(ENGPACKROOT) (= $(MAJORVER).$(MINORVER)-$(SVNREV)$(RELEASESTATUS))" >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@echo "Maintainer: $(PACKMAINTAINER)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@echo "Description: $(FUNCSPACKDESC)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@echo " This package is built using OpenFLUID-engine revision $(SVNREV)." >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@echo " CHANGELOG:" >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control			
	@sed 's/^$$/./' ./resources/doc/changelog | sed 's/^/ /' >> $(BASEPACKDIR)/$(LOCALDIR)/$(FUNCSPACKNAME)/DEBIAN/control
	@(cd $(BASEPACKDIR)/$(LOCALDIR) && dpkg-deb --build $(FUNCSPACKNAME))


win32-packages: all
	@echo ""	
	@mkdir -p $(BASEPACKDIR)/win32
	@rm -f -R $(BASEPACKDIR)/win32
	@echo "==== Building packages (win32) ===="
	@echo "Building package"
	@mkdir -p $(BASEPACKDIR)/win32/$(ENGPACKNAME)_win32/$(PLUGSUBDIR)
	@cp $(BASEBINDIR)/$(EXEFILE) $(BASEPACKDIR)/win32/$(ENGPACKNAME)_win32/
	@cp $(BASEBINDIR)/$(PLUGSUBDIR)/* $(BASEPACKDIR)/win32/$(ENGPACKNAME)_win32/$(PLUGSUBDIR)
	@cd $(BASEPACKDIR)/win32/$(ENGPACKNAME)_win32/ && \
         $(ZIP) -r $(BASEPACKDIR)/win32/$(ENGPACKNAME)_win32.zip * && \
         cd $(BASEMAINDIR) > /dev/null





