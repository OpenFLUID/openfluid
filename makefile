# main makefile

include ./makeopts.inc

export PATH := $(MHYDASDKPATH):$(PATH)
export MHYDASDKPREFIX := $(SDKPREFIX)


all: main stdplugs run-script

.PHONY: main stdplugs


run-script:
	@echo "==== Generating run script for build environment ===="
	@echo "#!/bin/sh" > $(BASEBINDIR)/run-engine.sh
	@echo "LD_LIBRARY_PATH=$(MHYDASDKPREFIX)/lib:$LD_LIBRARY_PATH" >> $(BASEBINDIR)/run-engine.sh
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
	@echo "Building package"
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)$(STDSYSDIR)/$(APPNAME)/$(PLUGSUBDIR)
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/usr/bin	
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/usr/share/doc/$(PACKROOT)
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN
	@cp $(BASEBINDIR)/$(EXEFILE) $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/usr/bin/$(EXEFILE)
	@cp $(BASEBINDIR)/$(PLUGSUBDIR)/* $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)$(STDSYSDIR)/$(APPNAME)/$(PLUGSUBDIR)
	@cp resources/doc/* $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/usr/share/doc/$(PACKROOT)
	@cp resources/debian/* $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/copyright
	@echo "Package: $(PACKROOT)\nVersion: $(MAJORVER).$(MINORVER)-$(SVNREV)\nSection: $(PACKSECTION)\nPriority: $(PACKPRIORITY)\nArchitecture: $(PACKARCH)\nDepends: $(EDGYDEPENDS)\nReplaces: mhydas\nMaintainer: $(PACKMAINTAINER)\nDescription: $(PACKDESC)\n .\n This package is built using mhydas-engine revision $(SVNREV)." > $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo " CHANGELOG:" >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control			
	@sed 's/^$$/./' ./resources/doc/changelog | sed 's/^/ /' >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@(cd $(BASEPACKDIR)/$(LOCALDIR) && dpkg-deb --build $(PACKNAME))


ubuntu-dapper-packages: LOCALDIR = ubuntu-dapper
ubuntu-dapper-packages: all
	@echo ""	
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)
	@rm -f -R $(BASEPACKDIR)/$(LOCALDIR)
	@echo "==== Building packages ($(LOCALDIR)) ===="
	@echo "Building package"
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)$(STDSYSDIR)/$(APPNAME)/$(PLUGSUBDIR)
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/usr/bin	
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/usr/share/doc/$(PACKROOT)
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN
	@cp $(BASEBINDIR)/$(EXEFILE) $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/usr/bin/$(EXEFILE)
	@cp $(BASEBINDIR)/$(PLUGSUBDIR)/* $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)$(STDSYSDIR)/$(APPNAME)/$(PLUGSUBDIR)
	@cp resources/doc/* $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/usr/share/doc/$(PACKROOT)
	@cp resources/debian/* $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/copyright
	@echo "Package: $(PACKROOT)" > $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo "Version: $(MAJORVER).$(MINORVER)-$(SVNREV)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo "Section: $(PACKSECTION)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo "Priority: $(PACKPRIORITY)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo "Architecture: $(PACKARCH)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo "Depends: $(DAPPERDEPENDS)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo "Replaces: mhydas" >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo "Maintainer: $(PACKMAINTAINER)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo "Description: $(PACKDESC)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo " This package is built using mhydas-engine revision $(SVNREV)." >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo " CHANGELOG:" >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control			
	@sed 's/^$$/./' ./resources/doc/changelog | sed 's/^/ /' >> $(BASEPACKDIR)/$(LOCALDIR)/$(PACKNAME)/DEBIAN/control
	@(cd $(BASEPACKDIR)/$(LOCALDIR) && dpkg-deb --build $(PACKNAME))


win32-packages: all
	@echo ""	
	@mkdir -p $(BASEPACKDIR)/win32
	@rm -f -R $(BASEPACKDIR)/win32
	@echo "==== Building packages (win32) ===="
	@echo "Building package"
	@mkdir -p $(BASEPACKDIR)/win32/$(PACKNAME)_win32/$(PLUGSUBDIR)
	@cp $(BASEBINDIR)/$(EXEFILE) $(BASEPACKDIR)/win32/$(PACKNAME)_win32/
	@cp $(BASEBINDIR)/$(PLUGSUBDIR)/* $(BASEPACKDIR)/win32/$(PACKNAME)_win32/$(PLUGSUBDIR)
	@cd $(BASEPACKDIR)/win32/$(PACKNAME)_win32/ && \
         $(ZIP) -r $(BASEPACKDIR)/win32/$(PACKNAME)_win32.zip * && \
         cd $(BASEMAINDIR) > /dev/null





