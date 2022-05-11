/*____________________________________________________________________

  Project:

      Configuration file manipulation utils
      _____________________________________

  (C) Maxim Proskurnya,                           1999--2003, AMC, MSU
      max@axofiber.org.ru

  Component:  FileConfig
  Comments:   Implements configuration file support.
  ____________________________________________________________________
*/

#ifndef FILECONFIG_H
#define FILECONFIG_H

#include "String.h"
#include "FileStream.h"
#include "SCArray.h"

/*____________________________________________________________________

                                                                    /
    FileConfig class definition                                 \  /
                                                                 \/
  ____________________________________________________________________
*/

class FileConfig
{
public:
// Life-cycle
                  FileConfig () {}
                  FileConfig ( const String & CfgFileName ) { Open ( CfgFileName ); }
                 ~FileConfig () {}
            
// Interface
    bool          Open  ( const String & CfgFileName );
    bool          Read  ( const String & AttrName, String& Value );
protected:
// Attributes
    FileStream    CfgFile;
    CSCDictionary<String> Attrs;
};

#endif
