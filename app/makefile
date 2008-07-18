# main makefile

include ./makeopts.inc

export PATH := $(OPENFLUIDPATH):$(PATH)
export OPENFLUIDSDKPREFIX := $(SDKPREFIX)


all: main functions run-script

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


functions:
	@echo ""
	@echo "==== Compiling plugins ===="
	@(cd src/$(PLUGSUBDIR) && $(MAKE))


clean:
	@(cd src/main && $(MAKE) clean)
	@(cd src/$(PLUGSUBDIR) && $(MAKE) clean)


doc:
	@(cd src/main && $(MAKE) doc)
	@(cd src/stdplugs && $(MAKE) doc)


purge: clean
	@rm -f $(BASEBINDIR)




ubuntu-prepack:
	@echo ""	
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)	
	@rm -f -R $(BASEPACKDIR)/$(LOCALDIR)
	@echo "==== Building packages for ($(LOCALDIR)) ===="


ubuntu-packages: ubuntu-prepack ubuntu-packages-functions-mhydas ubuntu-packages-functions-standard
	@echo "Building package $(ENGPACKNAME)"
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
	@echo -n "Depends: " >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control
	@echo  $(ENGDEPENDS) >> $(BASEPACKDIR)/$(LOCALDIR)/$(ENGPACKNAME)/DEBIAN/control	
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

ubuntu-packages-functions-mhydas:
	@echo "Building package $(MHYDASFUNCSPACKNAME)"
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)$(STDSYSDIR)/$(APPNAME)/$(PLUGSUBDIR)	
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)/usr/share/doc/$(MHYDASFUNCSPACKROOT)
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)/DEBIAN	
	@../copy-functions.sh $(BASEMAINDIR)/$(MHYDASFUNCSPACKSRCPATH) $(BASEBINDIR)/$(PLUGSUBDIR) $(PLUGINEXT) $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)$(STDSYSDIR)/$(APPNAME)/$(PLUGSUBDIR)
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)/DEBIAN/copyright
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)/usr/share/doc/$(MHYDASFUNCSPACKROOT)/copyright
	@echo "Package: $(MHYDASFUNCSPACKROOT)" > $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)/DEBIAN/control
	@echo "Version: $(MAJORVER).$(MINORVER)-$(SVNREV)$(RELEASESTATUS)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)/DEBIAN/control
	@echo "Section: $(PACKSECTION)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)/DEBIAN/control
	@echo "Priority: $(PACKPRIORITY)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)/DEBIAN/control
	@echo "Architecture: $(PACKARCH)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)/DEBIAN/control
	@echo "Depends: $(ENGPACKROOT) (= $(MAJORVER).$(MINORVER)-$(SVNREV)$(RELEASESTATUS))" >> $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)/DEBIAN/control
	@echo "Maintainer: $(PACKMAINTAINER)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)/DEBIAN/control
	@echo "Description: $(MHYDASFUNCSPACKDESC)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)/DEBIAN/control
	@echo " CHANGELOG:" >> $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)/DEBIAN/control			
	@sed 's/^$$/./' ./resources/doc/changelog | sed 's/^/ /' >> $(BASEPACKDIR)/$(LOCALDIR)/$(MHYDASFUNCSPACKNAME)/DEBIAN/control
	@(cd $(BASEPACKDIR)/$(LOCALDIR) && dpkg-deb --build $(MHYDASFUNCSPACKNAME))

ubuntu-packages-functions-standard:
	@echo "Building package $(STDFUNCSPACKNAME)"
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)$(STDSYSDIR)/$(APPNAME)/$(PLUGSUBDIR)	
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)/usr/share/doc/$(STDFUNCSPACKROOT)
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)/DEBIAN
	@../copy-functions.sh $(BASEMAINDIR)/$(STDFUNCSPACKSRCPATH) $(BASEBINDIR)/$(PLUGSUBDIR) $(PLUGINEXT) $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)$(STDSYSDIR)/$(APPNAME)/$(PLUGSUBDIR)	
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)/DEBIAN/copyright
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)/usr/share/doc/$(STDFUNCSPACKROOT)/copyright
	@echo "Package: $(STDFUNCSPACKROOT)" > $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)/DEBIAN/control
	@echo "Version: $(MAJORVER).$(MINORVER)-$(SVNREV)$(RELEASESTATUS)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)/DEBIAN/control
	@echo "Section: $(PACKSECTION)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)/DEBIAN/control
	@echo "Priority: $(PACKPRIORITY)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)/DEBIAN/control
	@echo "Architecture: $(PACKARCH)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)/DEBIAN/control
	@echo "Depends: $(ENGPACKROOT) (= $(MAJORVER).$(MINORVER)-$(SVNREV)$(RELEASESTATUS))" >> $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)/DEBIAN/control
	@echo "Maintainer: $(PACKMAINTAINER)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)/DEBIAN/control
	@echo "Description: $(STDFUNCSPACKDESC)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)/DEBIAN/control
	@echo " CHANGELOG:" >> $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)/DEBIAN/control			
	@sed 's/^$$/./' ./resources/doc/changelog | sed 's/^/ /' >> $(BASEPACKDIR)/$(LOCALDIR)/$(STDFUNCSPACKNAME)/DEBIAN/control
	@(cd $(BASEPACKDIR)/$(LOCALDIR) && dpkg-deb --build $(STDFUNCSPACKNAME))

ubuntu-edgy-packages: LOCALDIR = "ubuntu-edgy"
ubuntu-edgy-packages: ENGDEPENDS = "$(ENGEDGYDEPENDS)"
ubuntu-edgy-packages: ubuntu-packages
	

ubuntu-dapper-packages: LOCALDIR = "ubuntu-dapper"
ubuntu-dapper-packages: ENGDEPENDS = "$(ENGDAPPERDEPENDS)"
ubuntu-dapper-packages: ubuntu-packages




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





