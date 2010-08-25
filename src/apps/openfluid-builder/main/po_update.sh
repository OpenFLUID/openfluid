#! /bin/bash

echo "update POTFILES.in..."
ls *.hpp *.cpp resources/*.glade > po/POTFILES.in

echo "xgettext from POTFILES.in to openfluid-builder.pot..."
xgettext --files-from po/POTFILES.in -o po/openfluid-builder.pot --keyword=_ --package-name="OpenFLUID Builder" --package-version="0.0.0" --copyright-holder="2007-2010 INRA-Montpellier SupAgro" --msgid-bugs-address="libres at supagro.inra.fr" --no-wrap

cd po

while read lang
do
	echo "update $lang.po..."
	msgmerge $lang openfluid-builder -o $lang.po --no-wrap
done < LINGUAS

## comment to skip compil/install
localedir="/usr/share/locale"
while read lang
do
	echo "compilation de $lang.po..."
	msgfmt -o openfluid-builder.mo $lang.po
	instaldir="$localedir/$lang/LC_MESSAGES/"
	echo "installation dans $instaldir..."
	sudo mv openfluid-builder.mo $instaldir
done < LINGUAS


echo "DONE !"
