/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/

/**
  @file CompletionProviderAPIData.cpp

  @author OpenFLUID team <contact@openfluid-project.org>

  Automatically generated file, to be included in CompletionProvider class at build time
*/


// OpenFLUID:stylecheck:!llen


// extracted from builderext/PluggableFeatureExtension.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,BUILDEREXT,
    {
      CompletionProvider::tr("Signature")
    },
    CompletionProvider::tr("Insert signature block"),
    "BEGIN_BUILDEREXT_SIGNATURE(\"%%SEL_START%%bext.id%%SEL_END%%\",exttype)\n\nEND_BUILDEREXT_SIGNATURE"
  )
);

// extracted from builderext/PluggableFeatureExtension.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,BUILDEREXT,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Menu context")
    },
    CompletionProvider::tr("Declare menu category"),
    "DECLARE_CATEGORY(%%SEL_START%%category%%SEL_END%%)"
  )
);

// extracted from builderext/PluggableFeatureExtension.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,BUILDEREXT,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Menu context")
    },
    CompletionProvider::tr("Declare menu text"),
    "DECLARE_MENUTEXT(\"%%SEL_START%%menu text%%SEL_END%%\")"
  )
);

// extracted from core/BooleanValue.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Boolean value"),
    "openfluid::core::BooleanValue %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from core/DateTime.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Time")
    },
    CompletionProvider::tr("Date and time"),
    "openfluid::core::DateTime %%SEL_START%%DT%%SEL_END%%"
  )
);

// extracted from core/DoubleValue.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Double precision value"),
    "openfluid::core::DoubleValue %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from core/Event.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Events")
    },
    CompletionProvider::tr("Event"),
    "openfluid::core::Event %%SEL_START%%Ev%%SEL_END%%"
  )
);

// extracted from core/EventsCollection.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Events")
    },
    CompletionProvider::tr("Events collection"),
    "openfluid::core::EventsCollection %%SEL_START%%EvColl%%SEL_END%%"
  )
);

// extracted from core/IndexedValue.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Time indexed value"),
    "openfluid::core::IndexedValue %%SEL_START%%IndexedVal%%SEL_END%%"
  )
);

// extracted from core/IndexedValue.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("List of time indexed value"),
    "openfluid::core::IndexedValueList %%SEL_START%%IndexedValList%%SEL_END%%"
  )
);

// extracted from core/IntegerValue.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Long integer value"),
    "openfluid::core::IntegerValue %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from core/MapValue.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Map of any value"),
    "openfluid::core::MapValue %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from core/MatrixValue.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Matrix of double values"),
    "openfluid::core::MatrixValue %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from core/NullValue.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Null value"),
    "openfluid::core::NullValue %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from core/SpatialUnit.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Spatial domain")
    },
    CompletionProvider::tr("List of pointers to spatial units"),
    "openfluid::core::UnitsPtrList_t %%SEL_START%%UnitsPtrList%%SEL_END%%"
  )
);

// extracted from core/SpatialUnit.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Spatial domain")
    },
    CompletionProvider::tr("Spatial unit"),
    "openfluid::core::SpatialUnit %%SEL_START%%Unit%%SEL_END%%"
  )
);

// extracted from core/SpatialUnit.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Spatial domain")
    },
    CompletionProvider::tr("Pointer to spatial unit"),
    "openfluid::core::SpatialUnit* %%SEL_START%%UnitPtr%%SEL_END%%"
  )
);

// extracted from core/StringValue.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("String value"),
    "openfluid::core::StringValue %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from core/TreeValue.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Tree value"),
    "openfluid::core::TreeValue %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from core/TypeDefs.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Spatial domain")
    },
    CompletionProvider::tr("Unit ID"),
    "openfluid::core::UnitID_t %%SEL_START%%UnitID%%SEL_END%%"
  )
);

// extracted from core/TypeDefs.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Spatial domain")
    },
    CompletionProvider::tr("Process order"),
    "openfluid::core::PcsOrd_t %%SEL_START%%PcsOrd%%SEL_END%%"
  )
);

// extracted from core/TypeDefs.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Spatial domain")
    },
    CompletionProvider::tr("Units class name"),
    "openfluid::core::UnitsClass_t %%SEL_START%%ClassName%%SEL_END%%"
  )
);

// extracted from core/TypeDefs.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Attributes")
    },
    CompletionProvider::tr("Attribute name"),
    "openfluid::core::AttributeName_t %%SEL_START%%AttrName%%SEL_END%%"
  )
);

// extracted from core/TypeDefs.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Variable name"),
    "openfluid::core::VariableName_t %%SEL_START%%VarName%%SEL_END%%"
  )
);

// extracted from core/TypeDefs.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("ID..Map")
    },
    CompletionProvider::tr("ID to single precision (signed)"),
    "openfluid::core::IDFloatMap %%SEL_START%%ValMap%%SEL_END%%"
  )
);

// extracted from core/TypeDefs.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("ID..Map")
    },
    CompletionProvider::tr("ID to double precision (signed)"),
    "openfluid::core::IDDoubleMap %%SEL_START%%ValMap%%SEL_END%%"
  )
);

// extracted from core/TypeDefs.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("ID..Map")
    },
    CompletionProvider::tr("ID to DoubleValue"),
    "openfluid::core::IDDoubleValueMap %%SEL_START%%ValMap%%SEL_END%%"
  )
);

// extracted from core/TypeDefs.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("ID..Map")
    },
    CompletionProvider::tr("ID to integer (signed)"),
    "openfluid::core::IDIntMap %%SEL_START%%ValMap%%SEL_END%%"
  )
);

// extracted from core/TypeDefs.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("ID..Map")
    },
    CompletionProvider::tr("ID to boolean"),
    "openfluid::core::IDBoolMap %%SEL_START%%ValMap%%SEL_END%%"
  )
);

// extracted from core/TypeDefs.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("ID..Map")
    },
    CompletionProvider::tr("ID to VectorValue"),
    "openfluid::core::IDVectorValueMap %%SEL_START%%ValMap%%SEL_END%%"
  )
);

// extracted from core/TypeDefs.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("ID..Map")
    },
    CompletionProvider::tr("ID to pointer to VectorValue"),
    "openfluid::core::IDVectorValuePtrMap %%SEL_START%%ValMap%%SEL_END%%"
  )
);

// extracted from core/TypeDefs.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Time")
    },
    CompletionProvider::tr("Time index"),
    "openfluid::core::TimeIndex_t %%SEL_START%%TimeIndex%%SEL_END%%"
  )
);

// extracted from core/TypeDefs.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Time")
    },
    CompletionProvider::tr("Duration"),
    "openfluid::core::Duration_t %%SEL_START%%Duration%%SEL_END%%"
  )
);

// extracted from core/VectorValue.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Types"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Vector of double values"),
    "openfluid::core::VectorValue %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration")
    },
    CompletionProvider::tr("Include FortranCPP header"),
    "#include <openfluid/tools/FortranCPP.hpp>"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Declaration of external code")
    },
    CompletionProvider::tr("Declaration block"),
    "BEGIN_EXTERN_FORTRAN\n%%CURSOR%%\nEND_EXTERN_FORTRAN"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Declaration of external code")
    },
    CompletionProvider::tr("Beginning of declaration block"),
    "BEGIN_EXTERN_FORTRAN"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Declaration of external code")
    },
    CompletionProvider::tr("End of declaration block"),
    "END_EXTERN_FORTRAN"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Declaration of external code"),
      CompletionProvider::tr("Fortran")
    },
    CompletionProvider::tr("Declare function"),
    "EXTERN_FFUNCTION(%%SEL_START%%Name%%SEL_END%%)(/*parameters*/)"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Call of external code"),
      CompletionProvider::tr("Fortran")
    },
    CompletionProvider::tr("Call function"),
    "CALL_FFUNCTION(%%SEL_START%%Name%%SEL_END%%)(parameters)"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Declaration of external code"),
      CompletionProvider::tr("Fortran")
    },
    CompletionProvider::tr("Declare subroutine"),
    "EXTERN_FSUBROUTINE(%%SEL_START%%Name%%SEL_END%%)(parameters)"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Call of external code"),
      CompletionProvider::tr("Fortran")
    },
    CompletionProvider::tr("Call subroutine"),
    "CALL_FSUBROUTINE(%%SEL_START%%Name%%SEL_END%%)(parameters)"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Declaration of external code"),
      CompletionProvider::tr("Fortran90")
    },
    CompletionProvider::tr("Declare function in module"),
    "EXTERN_FMODFUNCTION(%%SEL_START%%Module%%SEL_END%%,Name)(parameters)"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Call of external code"),
      CompletionProvider::tr("Fortran90")
    },
    CompletionProvider::tr("Call function in module"),
    "CALL_FMODFUNCTION(%%SEL_START%%Module%%SEL_END%%,Name)(parameters)"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Declaration of external code"),
      CompletionProvider::tr("Fortran90")
    },
    CompletionProvider::tr("Declare subroutine in module"),
    "EXTERN_FMODSUBROUTINE(%%SEL_START%%Module%%SEL_END%%,Name)(parameters)"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Call of external code"),
      CompletionProvider::tr("Fortran90")
    },
    CompletionProvider::tr("Call subroutine in module"),
    "CALL_FMODSUBROUTINE(%%SEL_START%%Module%%SEL_END%%,Name)(parameters)"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Types")
    },
    CompletionProvider::tr("Single precision float"),
    "FREAL %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Types")
    },
    CompletionProvider::tr("Double precision float"),
    "FREAL8 %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Types")
    },
    CompletionProvider::tr("Double double precision float"),
    "FREAL16 %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Types")
    },
    CompletionProvider::tr("Integer"),
    "FINT %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Types")
    },
    CompletionProvider::tr("Short integer"),
    "FINT2 %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Types")
    },
    CompletionProvider::tr("Long integer"),
    "FINT8 %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Types")
    },
    CompletionProvider::tr("Logical"),
    "FLOGICAL %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Types")
    },
    CompletionProvider::tr("Logical (boolean)"),
    "FLOGICAL1 %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Types")
    },
    CompletionProvider::tr("Character"),
    "FCHARACTER %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Types")
    },
    CompletionProvider::tr("String"),
    "FSTRING %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Types")
    },
    CompletionProvider::tr("String length"),
    "FSTRINGLEN %%SEL_START%%Val%%SEL_END%%"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Functions")
    },
    CompletionProvider::tr("Convert std::string to Fortran string content"),
    "STD2FSTRING(%%SEL_START%%str%%SEL_END%%)"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Functions")
    },
    CompletionProvider::tr("Convert std::string to Fortran string length"),
    "STD2FSTRINGLEN(%%SEL_START%%str%%SEL_END%%)"
  )
);

// extracted from tools/FortranCPP.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Fortran integration"),
      CompletionProvider::tr("Functions")
    },
    CompletionProvider::tr("Convert std::string to Fortran string content and length"),
    "STD2FSTRINGFULL(%%SEL_START%%str%%SEL_END%%)"
  )
);

// extracted from ware/LoopMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Loops")
    },
    CompletionProvider::tr("Ordered loop on spatial units of a class"),
    "OPENFLUID_UNITS_ORDERED_LOOP(\"%%SEL_START%%unitsclass%%SEL_END%%\",UnitPtr)\n%%INDENT%%{\n%%INDENT%%\n%%INDENT%%}"
  )
);

// extracted from ware/LoopMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Loops")
    },
    CompletionProvider::tr("Ordered loop on all spatial units"),
    "OPENFLUID_ALLUNITS_ORDERED_LOOP(%%SEL_START%%UnitPtrList%%SEL_END%%)\n%%INDENT%%{\n%%INDENT%%\n%%INDENT%%}"
  )
);

// extracted from ware/LoopMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Loops")
    },
    CompletionProvider::tr("Loop on a spatial units list"),
    "OPENFLUID_UNITSLIST_LOOP(%%SEL_START%%UnitsList%%SEL_END%%,UnitPtr)\n%%INDENT%%{\n%%INDENT%%\n%%INDENT%%}"
  )
);

// extracted from ware/LoopMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Loops")
    },
    CompletionProvider::tr("Loop on an events collection"),
    "OPENFLUID_EVENT_COLLECTION_LOOP(%%SEL_START%%EvList%%SEL_END%%,Ev)\n%%INDENT%%{\n%%INDENT%%\n%%INDENT%%}"
  )
);

// extracted from ware/ObserverSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,OBSERVER,
    {
      CompletionProvider::tr("Signature")
    },
    CompletionProvider::tr("Insert signature block"),
    "BEGIN_OBSERVER_SIGNATURE(\"%%SEL_START%%sim.id%%SEL_END%%\")\n\nEND_OBSERVER_SIGNATURE"
  )
);

// extracted from ware/PluggableSimulator.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Parameters")
    },
    CompletionProvider::tr("Test if a simulator parameter exists"),
    "OPENFLUID_IsSimulatorParameterExist(Params,%%SEL_START%%ParamName%%SEL_END%%)"
  )
);

// extracted from ware/PluggableSimulator.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Parameters")
    },
    CompletionProvider::tr("Get simulator parameter (by return)"),
    "OPENFLUID_GetSimulatorParameter(Params,%%SEL_START%%ParamName%%SEL_END%%)"
  )
);

// extracted from ware/PluggableSimulator.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Parameters")
    },
    CompletionProvider::tr("Get simulator parameter"),
    "OPENFLUID_GetSimulatorParameter(Params,%%SEL_START%%ParamName%%SEL_END%%,Val)"
  )
);

// extracted from ware/PluggableSimulator.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Threading")
    },
    CompletionProvider::tr("Get maximum number of threads"),
    "OPENFLUID_GetSimulatorMaxThreads()"
  )
);

// extracted from ware/PluggableSimulator.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Threading")
    },
    CompletionProvider::tr("Set maximum number of threads"),
    "OPENFLUID_SetSimulatorMaxThreads(%%SEL_START%%Number%%SEL_END%%)"
  )
);

// extracted from ware/PluggableSimulator.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Sheduling")
    },
    CompletionProvider::tr("Run once at the end"),
    "AtTheEnd()"
  )
);

// extracted from ware/PluggableSimulator.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Sheduling")
    },
    CompletionProvider::tr("Never run again"),
    "Never()"
  )
);

// extracted from ware/PluggableSimulator.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Sheduling")
    },
    CompletionProvider::tr("Run in default deltaT"),
    "DefaultDeltaT()"
  )
);

// extracted from ware/PluggableSimulator.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Sheduling")
    },
    CompletionProvider::tr("Run in X default deltaT"),
    "MultipliedDefaultDeltaT(%%SEL_START%%X%%SEL_END%%)"
  )
);

// extracted from ware/PluggableSimulator.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Sheduling")
    },
    CompletionProvider::tr("Run in a duration in seconds"),
    "Duration(%%SEL_START%%Seconds%%SEL_END%%)"
  )
);

// extracted from ware/PluggableWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Messages")
    },
    CompletionProvider::tr("Raise fatal error"),
    "OPENFLUID_RaiseError(%%SEL_START%%\"message\"%%SEL_END%%)"
  )
);

// extracted from ware/PluggableWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Run environment")
    },
    CompletionProvider::tr("Get information about run environment"),
    "OPENFLUID_GetRunEnvironment(%%SEL_START%%\"key\"%%SEL_END%%,Value)"
  )
);

// extracted from ware/PluggableWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Run environment")
    },
    CompletionProvider::tr("Get current input dataset directory"),
    "OPENFLUID_GetRunEnvironment(\"dir.input\",%%SEL_START%%Value%%SEL_END%%)"
  )
);

// extracted from ware/PluggableWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Run environment")
    },
    CompletionProvider::tr("Get current output results directory"),
    "OPENFLUID_GetRunEnvironment(\"dir.output\",%%SEL_START%%Value%%SEL_END%%)"
  )
);

// extracted from ware/PluggableWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Run environment")
    },
    CompletionProvider::tr("Get current directory for temporary data"),
    "OPENFLUID_GetRunEnvironment(\"dir.temp\",%%SEL_START%%Value%%SEL_END%%)"
  )
);

// extracted from ware/PluggableWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Run environment")
    },
    CompletionProvider::tr("Get current ware ID"),
    "OPENFLUID_GetWareID()"
  )
);

// extracted from ware/SimulationContributorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Attributes")
    },
    CompletionProvider::tr("Set attribute value"),
    "OPENFLUID_SetAttribute(%%SEL_START%%UnitPtr%%SEL_END%%,\"attrname\",Val)"
  )
);

// extracted from ware/SimulationContributorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Initialize variable with value"),
    "OPENFLUID_InitializeVariable(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",Val)"
  )
);

// extracted from ware/SimulationContributorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Append value to a variable"),
    "OPENFLUID_AppendVariable(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",Val)"
  )
);

// extracted from ware/SimulationContributorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Update current value of a variable"),
    "OPENFLUID_SetVariable(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",Val)"
  )
);

// extracted from ware/SimulationContributorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Events")
    },
    CompletionProvider::tr("Append an event"),
    "OPENFLUID_AppendEvent(%%SEL_START%%UnitPtr%%SEL_END%%,Event)"
  )
);

// extracted from ware/SimulationContributorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Add a spatial unit to the spatial graph"),
    "OPENFLUID_AddUnit(%%SEL_START%%ClassName%%SEL_END%%,UnitID,PcsOrder)"
  )
);

// extracted from ware/SimulationContributorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Delete a spatial unit of the spatial graph"),
    "OPENFLUID_DeleteUnit(%%SEL_START%%ClassName%%SEL_END%%,ID)"
  )
);

// extracted from ware/SimulationContributorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Add a From-To connection using classes names and IDs"),
    "OPENFLUID_AddFromToConnection(%%SEL_START%%ClassNameFrom%%SEL_END%%,UnitIDFrom,ClassNameTo,UnitIDTo)"
  )
);

// extracted from ware/SimulationContributorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Add a From-To connection using pointers to spatial units"),
    "OPENFLUID_AddFromToConnection(%%SEL_START%%FromUnitPtr%%SEL_END%%,ToUnitPtr)"
  )
);

// extracted from ware/SimulationContributorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Remove a From-To connection using classes names and IDs"),
    "OPENFLUID_RemoveFromToConnection(%%SEL_START%%ClassNameFrom%%SEL_END%%,UnitIDFrom,ClassNameTo,UnitIDTo)"
  )
);

// extracted from ware/SimulationContributorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Remove a From-To connection using spatial units pointers"),
    "OPENFLUID_RemoveFromToConnection(%%SEL_START%%FromUnitPtr%%SEL_END%%,ToUnitPtr)"
  )
);

// extracted from ware/SimulationContributorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Add a Child-parent connection using classes names and IDs"),
    "OPENFLUID_AddChildParentConnection(%%SEL_START%%ClassNameChild%%SEL_END%%,UnitIDChild,ClassNameParent,UnitIDParent)"
  )
);

// extracted from ware/SimulationContributorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Add a Child-parent connection using pointers to spatial units"),
    "OPENFLUID_AddChildParentConnection(%%SEL_START%%ChildUnitPtr%%SEL_END%%,ParentUnitPtr)"
  )
);

// extracted from ware/SimulationContributorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Remove a Child-parent connection using classes names and IDs"),
    "OPENFLUID_RemoveChildParentConnection(%%SEL_START%%ClassNameChild%%SEL_END%%,UnitIDChild,ClassNameParent,UnitIDParent)"
  )
);

// extracted from ware/SimulationContributorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Remove a Child-parent connection using pointers to spatial units"),
    "OPENFLUID_RemoveChildParentConnection(%%SEL_START%%ChildUnitPtr%%SEL_END%%,ParentUnitPtr)"
  )
);

// extracted from ware/SimulationContributorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Build a matrix of spatial units"),
    "OPENFLUID_BuildUnitsMatrix(%%SEL_START%%ClassName%%SEL_END%%,ColsNbr,RowsNbr)"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Messages")
    },
    CompletionProvider::tr("Log warning message"),
    "OPENFLUID_LogWarning(%%SEL_START%%\"message\"%%SEL_END%%)"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Messages")
    },
    CompletionProvider::tr("Display warning message"),
    "OPENFLUID_DisplayWarning(%%SEL_START%%\"message\"%%SEL_END%%)"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Messages")
    },
    CompletionProvider::tr("Log and warning debug message"),
    "OPENFLUID_LogAndDisplayWarning(%%SEL_START%%\"message\"%%SEL_END%%)"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Messages")
    },
    CompletionProvider::tr("Log information message"),
    "OPENFLUID_LogInfo(%%SEL_START%%\"message\"%%SEL_END%%)"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Messages")
    },
    CompletionProvider::tr("Display information message"),
    "OPENFLUID_DisplayInfo(%%SEL_START%%\"message\"%%SEL_END%%)"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Messages")
    },
    CompletionProvider::tr("Log and display information message"),
    "OPENFLUID_LogAndDisplayInfo(%%SEL_START%%\"message\"%%SEL_END%%)"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Messages")
    },
    CompletionProvider::tr("Log debug message"),
    "OPENFLUID_LogDebug(%%SEL_START%%\"message\"%%SEL_END%%)"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Messages")
    },
    CompletionProvider::tr("Display debug message"),
    "OPENFLUID_DisplayDebug(%%SEL_START%%\"message\"%%SEL_END%%)"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Messages")
    },
    CompletionProvider::tr("Log and display debug message"),
    "OPENFLUID_LogAndDisplayDebug(%%SEL_START%%\"message\"%%SEL_END%%)"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Time")
    },
    CompletionProvider::tr("Get begin date of simulation"),
    "OPENFLUID_GetBeginDate()"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Time")
    },
    CompletionProvider::tr("Get end date of simulation"),
    "OPENFLUID_GetEndDate()"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Time")
    },
    CompletionProvider::tr("Get begin current date of simulation"),
    "OPENFLUID_GetCurrentDate()"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Time")
    },
    CompletionProvider::tr("Get total simulation duration"),
    "OPENFLUID_GetSimulationDuration()"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Time")
    },
    CompletionProvider::tr("Get default deltaT"),
    "OPENFLUID_GetDefaultDeltaT()"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Time")
    },
    CompletionProvider::tr("Get current time index"),
    "OPENFLUID_GetCurrentTimeIndex()"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Time")
    },
    CompletionProvider::tr("Get time index of the previous ware run"),
    "OPENFLUID_GetPreviousRunTimeIndex()"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Time")
    },
    CompletionProvider::tr("Get current simulation stage"),
    "OPENFLUID_GetCurrentStage()"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Time")
    },
    CompletionProvider::tr("Get current simulation stage as a string"),
    "OPENFLUID_GetCurrentStageAsString()"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Time")
    },
    CompletionProvider::tr("Get scheduling constraint mode"),
    "OPENFLUID_GetSchedulingConstraint()"
  )
);

// extracted from ware/SimulationDrivenWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Messages")
    },
    CompletionProvider::tr("Raise fatal error"),
    "OPENFLUID_RaiseError(%%SEL_START%%\"message\"%%SEL_END%%)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Attributes")
    },
    CompletionProvider::tr("Test if an attribute exists"),
    "OPENFLUID_IsAttributeExist(%%SEL_START%%UnitPtr%%SEL_END%%,\"attrname\")"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Attributes")
    },
    CompletionProvider::tr("Get attribute value"),
    "OPENFLUID_GetAttribute(%%SEL_START%%UnitPtr%%SEL_END%%,\"attrname\",Val)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Attributes")
    },
    CompletionProvider::tr("Get attribute value (by return)"),
    "OPENFLUID_GetAttribute(%%SEL_START%%UnitPtr%%SEL_END%%,\"attrname\")"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Test if a variable exists"),
    "OPENFLUID_IsVariableExist(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\")"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Test if a variable exists and has a value on given time index"),
    "OPENFLUID_IsVariableExist(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",TimeIndex)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Test if a variable exists and has a value of given type on given time index"),
    "OPENFLUID_IsVariableExist(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",TimeIndex,Type)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Test if a variable exists with given type"),
    "OPENFLUID_IsTypedVariableExist(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",Type)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Test if a variable exists with given type and has a value on given time index"),
    "OPENFLUID_IsTypedVariableExist(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",TimeIndex,Type)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Get variable value on given time index"),
    "OPENFLUID_GetVariable(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",TimeIndex,Val)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Get variable value on given time index (by return)"),
    "OPENFLUID_GetVariable(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",TimeIndex)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Get variable value on current time index"),
    "OPENFLUID_GetVariable(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",Val)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Get variable value on current time index (by return)"),
    "OPENFLUID_GetVariable(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\")"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Get latest available variable value"),
    "OPENFLUID_GetLatestVariable(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",IndexedVal)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Get latest available variable value (by return)"),
    "OPENFLUID_GetLatestVariable(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\")"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Get latest available variable values since a time index"),
    "OPENFLUID_GetLatestVariables(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",TimeIndex,IndexedValList)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Get latest available variable values since a time index (by return)"),
    "OPENFLUID_GetLatestVariables(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",TimeIndex)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Get variable values on given period"),
    "OPENFLUID_GetVariables(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",BeginIndex,EndIndex,IndexedValList)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Get variable values on given period (by return)"),
    "OPENFLUID_GetVariables(%%SEL_START%%UnitPtr%%SEL_END%%,\"varname\",BeginIndex,EndIndex)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Events")
    },
    CompletionProvider::tr("Get events on a given period"),
    "OPENFLUID_GetEvents(%%SEL_START%%UnitPtr%%SEL_END%%,BeginDate,EndDate,EvColl)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Events")
    },
    CompletionProvider::tr("Get events on a given period (by return)"),
    "OPENFLUID_GetEvents(%%SEL_START%%UnitPtr%%SEL_END%%,BeginDate,EndDate)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Test if a spatial units class exists"),
    "OPENFLUID_IsUnitsClassExist(%%SEL_START%%ClassName%%SEL_END%%)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Test if a spatial unit exists in a given units class"),
    "OPENFLUID_IsUnitExist(%%SEL_START%%ClassName%%SEL_END%%,UnitID)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Get the number of spatial units in the spatial graph"),
    "OPENFLUID_GetUnitsCount(%%SEL_START%%UnitsCount%%SEL_END%%)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Get the number of spatial units in the spatial graph (by return)"),
    "OPENFLUID_GetUnitsCount()"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Get the number of spatial units of a given class"),
    "OPENFLUID_GetUnitsCount(%%SEL_START%%ClassName%%SEL_END%%,UnitsCount)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Get the number of spatial units of a given class (by return)"),
    "OPENFLUID_GetUnitsCount(%%SEL_START%%ClassName%%SEL_END%%)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Get a pointer to a spatial unit of a given units class"),
    "OPENFLUID_GetUnit(%%SEL_START%%ClassName%%SEL_END%%,UnitID,UnitPtr)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Get a list of pointers to spatial units of a given units class"),
    "OPENFLUID_GetUnits(%%SEL_START%%ClassName%%SEL_END%%)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Test if a spatial unit is connected to another spatial unit"),
    "OPENFLUID_IsUnitConnectedTo(%%SEL_START%%UnitPtr%%SEL_END%%,ClassNameTo,UnitIDTo)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Test if a spatial unit is connected from another spatial unit"),
    "OPENFLUID_IsUnitConnectedFrom(%%SEL_START%%UnitPtr%%SEL_END%%,ClassNameFrom,UnitIDFrom)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Test if a spatial unit is a child of another spatial unit"),
    "OPENFLUID_IsUnitChildOf(%%SEL_START%%UnitPtr%%SEL_END%%,ClassNameParent,UnitIDParent)"
  )
);

// extracted from ware/SimulationInspectorWare.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Test if a spatial unit is a parent of another spatial unit"),
    "OPENFLUID_IsUnitParentOf(%%SEL_START%%UnitPtr%%SEL_END%%,ClassNameChild,UnitIDChild)"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature")
    },
    CompletionProvider::tr("Insert signature block"),
    "BEGIN_SIMULATOR_SIGNATURE(\"%%SEL_START%%sim.id%%SEL_END%%\")\n\nEND_SIMULATOR_SIGNATURE"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Scientific context")
    },
    CompletionProvider::tr("Declare application domain"),
    "DECLARE_DOMAIN(\"%%SEL_START%%description%%SEL_END%%\")"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Scientific context")
    },
    CompletionProvider::tr("Declare modeled process(es)"),
    "DECLARE_PROCESS(\"%%SEL_START%%description%%SEL_END%%\")"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Scientific context")
    },
    CompletionProvider::tr("Declare modeling method"),
    "DECLARE_METHOD(\"%%SEL_START%%description%%SEL_END%%\")"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Parameters")
    },
    CompletionProvider::tr("Declare used parameter"),
    "DECLARE_USED_PARAMETER(\"%%SEL_START%%paramname%%SEL_END%%\",\"description\",\"SI unit\")"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Parameters")
    },
    CompletionProvider::tr("Declare required parameter"),
    "DECLARE_REQUIRED_PARAMETER(\"%%SEL_START%%paramname%%SEL_END%%\",\"description\",\"SI unit\")"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Declare produced variable"),
    "DECLARE_PRODUCED_VARIABLE(\"%%SEL_START%%var.name%%SEL_END%%\",\"unitsclass\",\"description\",\"SI unit\")"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Declare updated variable"),
    "DECLARE_UPDATED_VARIABLE(\"%%SEL_START%%var.name%%SEL_END%%\",\"unitsclass\",\"description\",\"SI unit\")"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Declare required variable"),
    "DECLARE_REQUIRED_VARIABLE(\"%%SEL_START%%var.name%%SEL_END%%\",\"unitsclass\",\"description\",\"SI unit\")"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Variables")
    },
    CompletionProvider::tr("Declare used variable"),
    "DECLARE_USED_VARIABLE(\"%%SEL_START%%var.name%%SEL_END%%\",\"unitsclass\",\"description\",\"SI unit\")"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Attributes")
    },
    CompletionProvider::tr("Declare produced attribute"),
    "DECLARE_PRODUCED_ATTRIBUTE(\"%%SEL_START%%name%%SEL_END%%\",\"unitsclass\",\"description\",\"SI unit\")"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Attributes")
    },
    CompletionProvider::tr("Declare required attribute"),
    "DECLARE_REQUIRED_ATTRIBUTE(\"%%SEL_START%%name%%SEL_END%%\",\"unitsclass\",\"description\",\"SI unit\")"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Attributes")
    },
    CompletionProvider::tr("Declare used attribute"),
    "DECLARE_USED_ATTRIBUTE(\"%%SEL_START%%name%%SEL_END%%\",\"unitsclass\",\"description\",\"SI unit\")"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Events")
    },
    CompletionProvider::tr("Declare used events"),
    "DECLARE_USED_EVENTS(\"%%SEL_START%%unitsclass%%SEL_END%%\",\"description\")"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Declare updated units graph"),
    "DECLARE_UPDATED_UNITSGRAPH(\"%%SEL_START%%description%%SEL_END%%\")"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Spatial structure")
    },
    CompletionProvider::tr("Declare updated units class"),
    "DECLARE_UPDATED_UNITSCLASS(\"%%SEL_START%%unitsclass%%SEL_END%%\",\"description\")"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Extra files")
    },
    CompletionProvider::tr("Declare required extra file"),
    "DECLARE_REQUIRED_EXTRAFILE(\"%%SEL_START%%filename%%SEL_END%%\")"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Extra files")
    },
    CompletionProvider::tr("Declare used extra file"),
    "DECLARE_USED_EXTRAFILE(\"%%SEL_START%%filename%%SEL_END%%\")"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Scheduling")
    },
    CompletionProvider::tr("Declare undefined scheduling"),
    "DECLARE_SCHEDULING_UNDEFINED"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Scheduling")
    },
    CompletionProvider::tr("Declare default deltaT scheduling"),
    "DECLARE_SCHEDULING_DEFAULT"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Scheduling")
    },
    CompletionProvider::tr("Declare fixed scheduling"),
    "DECLARE_SCHEDULING_FIXED(%%SEL_START%%duration%%SEL_END%%)"
  )
);

// extracted from ware/SimulatorSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Scheduling")
    },
    CompletionProvider::tr("Declare scheduling in range"),
    "DECLARE_SCHEDULING_RANGE(%%SEL_START%%min%%SEL_END%%,max)"
  )
);

// extracted from ware/ThreadedLoopMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Loops")
    },
    CompletionProvider::tr("Threaded ordered loop on spatial units of a class"),
    "APPLY_UNITS_ORDERED_LOOP_THREADED(\"%%SEL_START%%UnitsClass%%SEL_END%%\",FuncPtr)"
  )
);

// extracted from ware/ThreadedLoopMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,SIMULATOR | OBSERVER,
    {
      CompletionProvider::tr("Compute code"),
      CompletionProvider::tr("Loops")
    },
    CompletionProvider::tr("Threaded ordered loop on all spatial units"),
    "APPLY_ALLUNITS_ORDERED_LOOP_THREADED(%%SEL_START%%FuncPtr%%SEL_END%%)"
  )
);

// extracted from ware/WareSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("General")
    },
    CompletionProvider::tr("Declare name"),
    "DECLARE_NAME(\"%%SEL_START%%name%%SEL_END%%\")"
  )
);

// extracted from ware/WareSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("General")
    },
    CompletionProvider::tr("Declare description"),
    "DECLARE_DESCRIPTION(\"%%SEL_START%%desc%%SEL_END%%\")"
  )
);

// extracted from ware/WareSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("General")
    },
    CompletionProvider::tr("Declare author"),
    "DECLARE_AUTHOR(\"%%SEL_START%%firstname lastname%%SEL_END%%\",\"email\")"
  )
);

// extracted from ware/WareSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("General")
    },
    CompletionProvider::tr("Declare version"),
    "DECLARE_VERSION(\"%%SEL_START%%1.0%%SEL_END%%\")"
  )
);

// extracted from ware/WareSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Status")
    },
    CompletionProvider::tr("Declare status as experimental"),
    "DECLARE_STATUS(openfluid::ware::EXPERIMENTAL)"
  )
);

// extracted from ware/WareSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Status")
    },
    CompletionProvider::tr("Declare status as beta"),
    "DECLARE_STATUS(openfluid::ware::BETA)"
  )
);

// extracted from ware/WareSignatureMacros.hpp
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,ANYWARE,
    {
      CompletionProvider::tr("Signature"),
      CompletionProvider::tr("Status")
    },
    CompletionProvider::tr("Declare status as stable"),
    "DECLARE_STATUS(openfluid::ware::STABLE)"
  )
);

