/*____________________________________________________________________

  Project:

      Configuration file manipulation utils
      _____________________________________

  (C) Maxim Proskurnya,                           1999--2003, AMC, MSU
      max@axofiber.org.ru

  Component:  FileConfig
  Comments:   Implements configuration file support.
  ____________________________________________________________________

  10.VII.2003                                                    00.00
  ____________________________________________________________________
*/

#include "stdafx.h"

//#include "Xception.h"
#include "FileConfig.h"

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Open                                                        )   (
                                                                \___/
  ____________________________________________________________________
*/

bool FileConfig::Open(const String& CfgFileName) {
    String Line;
    String Name;
    String Value;
    int i;
    int RemarkPos;
    int SpacePos;

    Attrs.RemoveAll();
    if (CfgFile.Open(CfgFileName, FileStream::modeRead)) {
        // Cache configuration attributes
        while (CfgFile.ReadLine(Line)) {
            if (!Line.IsEmpty()) {
                RemarkPos = Line.Find("#");
                if (RemarkPos == 0)
                    Line.Clear();
                else if (0 < RemarkPos)
                    Line = Line.SubStr(0, RemarkPos);
                if (!Line.Trim().IsEmpty()) {
                    SpacePos = Line.Find(' ');
                    if (0 < SpacePos) {
                        Name = Line.SubStr(0, SpacePos);
                        for (i = 0; i < Name.Length(); ++i)
                            if (64 < unsigned(Name[i]) && unsigned(Name[i]) < 91)
                                Name[i] = unsigned(Name[i]) + 32;
                        Value = Line.SubStr(SpacePos + 1, Line.Length() - SpacePos - 1).Trim();
                        if (!Value.IsEmpty())
                            Attrs.Add(Name, new String(Value));
                    }
                }
            }
        }
        return true;
    }
    return false;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Read                                                        )   (
                                                                \___/
  ____________________________________________________________________
*/

bool FileConfig::Read(const String& AttrName, String& Value) {
    int Position;
    if (Attrs.FindKey(AttrName, Position)) {
        Value = *Attrs[Position];
        return true;
    }
    return false;
}
