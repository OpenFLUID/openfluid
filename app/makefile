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
	@rm -f $(BASEBINDIR)


deb-packages: all
	@echo ""	
	@mkdir -p $(BASEPACKDIR)/debian
	@rm -f -R $(BASEPACKDIR)/debian
	@echo "==== Building packages (debian based distro) ===="
	@echo "Building package"
	@mkdir -p $(BASEPACKDIR)/debian/$(PACKNAME)$(STDSYSDIR)/$(APPNAME)/$(PLUGSUBDIR)
	@mkdir -p $(BASEPACKDIR)/debian/$(PACKNAME)/usr/bin	
	@mkdir -p $(BASEPACKDIR)/debian/$(PACKNAME)/usr/share/doc/$(PACKROOT)
	@mkdir -p $(BASEPACKDIR)/debian/$(PACKNAME)/DEBIAN
	@cp $(BASEBINDIR)/$(EXEFILE) $(BASEPACKDIR)/debian/$(PACKNAME)/usr/bin/$(EXEFILE)
	@cp $(BASEBINDIR)/$(PLUGSUBDIR)/* $(BASEPACKDIR)/debian/$(PACKNAME)$(STDSYSDIR)/$(APPNAME)/$(PLUGSUBDIR)
	@cp resources/doc/* $(BASEPACKDIR)/debian/$(PACKNAME)/usr/share/doc/$(PACKROOT)
	@cp resources/debian/* $(BASEPACKDIR)/debian/$(PACKNAME)/DEBIAN
	@cp resources/doc/COPYING $(BASEPACKDIR)/debian/$(PACKNAME)/DEBIAN/copyright
	@echo "Package: $(PACKROOT)\nVersion: $(MAJORVER).$(MINORVER)-$(SVNREV)\nSection: $(PACKSECTION)\nPriority: $(PACKPRIORITY)\nArchitecture: $(PACKARCH)\nDepends: $(PACKDEPENDS)\nReplaces: mhydas\nMaintainer: $(PACKMAINTAINER)\nDescription: $(PACKDESC)\n .\n This package is built using mhydas-engine revision $(SVNREV)." > $(BASEPACKDIR)/debian/$(PACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/debian/$(PACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/debian/$(PACKNAME)/DEBIAN/control
	@echo " CHANGELOG:" >> $(BASEPACKDIR)/debian/$(PACKNAME)/DEBIAN/control
	@echo " ." >> $(BASEPACKDIR)/debian/$(PACKNAME)/DEBIAN/control			
	@sed 's/^$$/./' ./resources/doc/changelog | sed 's/^/ /' >> $(BASEPACKDIR)/debian/$(PACKNAME)/DEBIAN/control
	@(cd $(BASEPACKDIR)/debian && dpkg-deb --build $(PACKNAME))


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




