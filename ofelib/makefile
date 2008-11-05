# main makefile

include ./makeopts.inc


all: sdk

.PHONY: sdk

sdk: 
	@echo ""
	@echo "==== Compiling SDK ===="
	@(cd src && $(MAKE))
	@$(LDCONFIGBUILD)


ofelib-conf-file:
	@echo ""
	@echo "==== Creating openfluid-config file ===="
	@echo "#!/bin/sh" > $(BASEBINDIR)/$(OPENFLUIDCONFIGFILE)
	@echo "" >> $(BASEBINDIR)/$(OPENFLUIDCONFIGFILE)    
	@echo "OFELIBDIR=$(INSTSDKLIBDIR)" >> $(BASEBINDIR)/$(OPENFLUIDCONFIGFILE)        
	@echo "OFELIBINCDIR=$(INSTSDKINCDIR)" >> $(BASEBINDIR)/$(OPENFLUIDCONFIGFILE)        
	@echo "OFELIBROOT=$(SDKLIBROOT)" >> $(BASEBINDIR)/$(OPENFLUIDCONFIGFILE)                
	@echo "MAJORVER=$(MAJORVER)" >> $(BASEBINDIR)/$(OPENFLUIDCONFIGFILE)
	@echo "MINORVER=$(MINORVER)" >> $(BASEBINDIR)/$(OPENFLUIDCONFIGFILE)  
	@echo "SVNREV=$(SVNREV)" >> $(BASEBINDIR)/$(OPENFLUIDCONFIGFILE)	
	@cat ./ofelib-config.in >> $(BASEBINDIR)/$(OPENFLUIDCONFIGFILE)
	@chmod ugo+x $(BASEBINDIR)/$(OPENFLUIDCONFIGFILE)


clean:
	@(cd src && $(MAKE) clean)
	@rm -R -f $(BASEDOCDIR)


autodoc:
	@mkdir -p $(BASEDOCDIR)
	@doxygen
	cp resources/doxygen/openfluid.png $(BASEDOCDIR)/html/


purge: clean
	@rm -f $(BASEBINDIR)/*


#clean-installed-sdk:
#	@echo ""
#	@echo "==== Removing installed SDK ===="
#	@$(SUDO) rm -f -R $(INSTSDKINCPATH)
#	@$(SUDO) rm -f $(INSTSDKLIBPATH)/$(SDKLIBFILE)
#	@$(SUDO) rm -f $(INSTSDKBINPATH)/$(OPENFLUIDCONFIGFILE)
#	@$(SUDO) $(LDCONFIGINST)

install-sdk: sdk ofelib-conf-file #clean-installed-sdk
	@echo ""
	@echo "==== Installing SDK ===="
	@echo "Installing header files in $(INSTSDKINCPATH)"
	@$(SUDO) mkdir -p $(INSTSDKINCPATH)/core/
	@$(SUDO) mkdir -p $(INSTSDKINCPATH)/base/
	@$(SUDO) mkdir -p $(INSTSDKINCPATH)/tools/	
	@$(SUDO) cp src/openfluid-*.h $(INSTSDKINCPATH)/
	@$(SUDO) cp src/core/*.h $(INSTSDKINCPATH)/core/
	@$(SUDO) cp src/base/*.h $(INSTSDKINCPATH)/base/
	@$(SUDO) cp src/tools/*.h $(INSTSDKINCPATH)/tools/	
	@echo "Installing libraries in $(INSTSDKLIBPATH)"
	@$(SUDO) cp $(BASEBINDIR)/$(SDKLIBFILE) $(INSTSDKLIBPATH)
	@$(SUDO) chmod 644 $(INSTSDKLIBPATH)/$(SDKLIBFILE)
	@$(SUDO) $(LDCONFIGINST)
	@$(SUDO) cp $(BASEBINDIR)/$(OPENFLUIDCONFIGFILE) $(INSTSDKBINPATH)
	
for-app-dev: sdk ofelib-conf-file
	@echo ""
	@echo "==== Installing SDK for app development ===="
	@rm -R -f $(DEVPREFIX)
	@echo "Installing header files in $(DEVPREFIX)/$(INSTSDKINCDIR)"
	@mkdir -p $(DEVPREFIX)/$(INSTSDKINCDIR)/core/
	@mkdir -p $(DEVPREFIX)/$(INSTSDKINCDIR)/base/	
	@mkdir -p $(DEVPREFIX)/$(INSTSDKINCDIR)/tools/	
	@cp src/openfluid-*.h $(DEVPREFIX)/$(INSTSDKINCDIR)/
	@cp src/core/*.h $(DEVPREFIX)/$(INSTSDKINCDIR)/core/
	@cp src/base/*.h $(DEVPREFIX)/$(INSTSDKINCDIR)/base/	
	@cp src/tools/*.h $(DEVPREFIX)/$(INSTSDKINCDIR)/tools/	
	@echo "Installing libraries in $(DEVPREFIX)/$(INSTSDKLIBDIR)" 	
	@mkdir -p $(DEVPREFIX)/$(INSTSDKLIBDIR)
	@cp $(BASEBINDIR)/$(SDKLIBFILE) $(DEVPREFIX)/$(INSTSDKLIBDIR)
	@chmod 644 $(DEVPREFIX)/$(INSTSDKLIBDIR)/$(SDKLIBFILE)
	@$(LDCONFIGDEV)
	@mkdir -p $(DEVPREFIX)/$(INSTSDKBINDIR)
	@cp $(BASEBINDIR)/$(OPENFLUIDCONFIGFILE) $(DEVPREFIX)/$(INSTSDKBINDIR)
	

ubuntu-packages: #all openfluid-conf-file docs	
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
	@cp resources/debian/libopenfluid/* $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/copyright
	@echo "Package: $(LIBPACKROOT)" > $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo "Version: $(MAJORVER).$(MINORVER)-$(SVNREV)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo "Section: $(PACKSECTION)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo "Priority: $(PACKPRIORITY)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo "Architecture: $(PACKARCH)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo -n "Depends: " >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
	@echo $(DEPENDS) >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control	
	@echo "Replaces: mhydasdk-libs" >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control	
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
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKINCPATH)/tools/	
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKBINPATH)
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/usr/share/doc/$(DEVPACKROOT)/html
	@mkdir -p $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN
	@cp src/openfluid-*.h $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKINCPATH)/
	@cp src/core/*.h $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKINCPATH)/core/ 
	@cp src/base/*.h $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKINCPATH)/base/
	@cp src/tools/*.h $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKINCPATH)/tools/	
	@cp $(BASEBINDIR)/$(OPENFLUIDCONFIGFILE) $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)$(INSTSDKBINPATH)
	@cp resources/doc/* $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/usr/share/doc/$(DEVPACKROOT)
	@cp -R $(BASEDOCDIR)/html/* $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/usr/share/doc/$(DEVPACKROOT)/html
	@cp resources/debian/common/* $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN
#	@cp resources/debian/openfluid-dev/* $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN
	@cp resources/doc/COPYING $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/copyright
	@echo "Package: $(DEVPACKROOT)" > $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo "Version: $(MAJORVER).$(MINORVER)-$(SVNREV)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo "Section: $(PACKSECTION)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo "Priority: $(PACKPRIORITY)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo "Architecture: $(PACKARCH)" >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo -n "Depends: " >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo -n $(DEPENDS) >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo ", $(LIBPACKROOT) (=$(MAJORVER).$(MINORVER)-$(SVNREV))" >> $(BASEPACKDIR)/$(LOCALDIR)/$(DEVPACKNAME)/DEBIAN/control
	@echo "Replaces: mhydasdk-dev" >> $(BASEPACKDIR)/$(LOCALDIR)/$(LIBPACKNAME)/DEBIAN/control
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

ubuntu-dapper-packages: LOCALDIR = ubuntu-dapper
ubuntu-dapper-packages: DEPENDS = "$(DAPPERDEPENDS)"
ubuntu-dapper-packages: autodoc ubuntu-packages	

ubuntu-edgy-packages: LOCALDIR = ubuntu-edgy
ubuntu-edgy-packages: DEPENDS = "$(EDGYDEPENDS)"
ubuntu-edgy-packages: autodoc ubuntu-packages	



win32-packages: all openfluid-conf-file
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
	@mkdir -p $(BASEPACKDIR)/win32/$(DEVPACKNAME)_win32/include/tools/	
	@cp src/openfluid-*.h $(BASEPACKDIR)/win32/$(DEVPACKNAME)_win32/include/
	@cp src/core/*.h $(BASEPACKDIR)/win32/$(DEVPACKNAME)_win32/include/core/ 
	@cp src/base/*.h $(BASEPACKDIR)/win32/$(DEVPACKNAME)_win32/include/base/
	@cp src/tools/*.h $(BASEPACKDIR)/win32/$(DEVPACKNAME)_win32/include/tools/	
	@cp $(BASEBINDIR)/$(OPENFLUIDCONFIGFILE) $(BASEPACKDIR)/win32/$(DEVPACKNAME)_win32/
	@cd $(BASEPACKDIR)/win32/$(DEVPACKNAME)_win32/ && \
         $(ZIP) -r $(BASEPACKDIR)/win32/$(DEVPACKNAME)_win32.zip * && \
         cd $(BASEMAINDIR) > /dev/null

	




