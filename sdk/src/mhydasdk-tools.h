/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#ifndef __SDKTOOLS_H__
#define __SDKTOOLS_H__

#ifdef __WXMSW__
  #ifdef BUILDINGDLL
    #define DLLEXPORT __declspec(dllexport)
  #else
    #define DLLEXPORT __declspec(dllimport)
  #endif
#else
  #define DLLEXPORT
#endif



#include "tools/Tools.h"
#include "tools/FortranCPP.h"
#include "tools/ColTextParser.h"
#include "tools/DataSrcFile.h"
#include "tools/DTSerie.h"
#include "tools/DistribInterp.h"



namespace openfluid { namespace tools {

class DLLEXPORT ColumnTextParser;
class DLLEXPORT DataSourcesFile;
class DLLEXPORT DateTimeSerie;
class DLLEXPORT DistributeInterpolate;
class DLLEXPORT Tools;
struct DLLEXPORT IndexedSerie;

/*class DLLEXPORT TiXmlString;
class DLLEXPORT TiXmlBase;
class DLLEXPORT TiXmlNode;
class DLLEXPORT TiXmlElement;
class DLLEXPORT TiXmlDocument;
class DLLEXPORT TiXmlDeclaration;
class DLLEXPORT TiXmlAttribute;
class DLLEXPORT TiXmlAttributeSet;
class DLLEXPORT TiXmlText;
class DLLEXPORT TiXmlComment;
class DLLEXPORT TiXmlHandle;
class DLLEXPORT TiXmlUnknown;
class DLLEXPORT TiXmlVisitor;
class DLLEXPORT TiXmlParsingData;
class DLLEXPORT TiXmlPrinter;
class DLLEXPORT TiXmlOutStream;
struct DLLEXPORT TiXmlCursor;
*/

} }

#endif






