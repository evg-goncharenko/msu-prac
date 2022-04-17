/*____________________________________________________________________

  Project:

      Xception class representation
      _____________________________

  (C) Maxim Proskurnya,                           1999--2003, AMC, MSU
      max@axofiber.org.ru

  Component:  Xception
  Comments:   Implements exception handling classes
  ____________________________________________________________________

  3.XI.2003                                                      02.16
      Small style changes.
  19.VI.2003                                                     02.15
      Project "GeneralDefs" had been refactored into "Xception".
  I.1999                                                         00.01
  ____________________________________________________________________
*/

#include "stdafx.h"

#ifdef WIN32
#include <windows.h>
#else
#include <iostream>
using namespace std;
#endif

#include "Xception.h"

const int Xception::X_GENERAL = 0;
const int Xception::X_BOUNDARIES = 1;
const int Xception::X_FILE_IO = 2;
const int Xception::X_MEMORY = 3;
const int Xception::X_NETWORK = 4;

Xception::Xception(const String Msg, const String Method, const String Class, const String Module) : Type(X_GENERAL),
                                                                                                     Message(Msg),
                                                                                                     MethodName(Method),
                                                                                                     ClassName(Class),
                                                                                                     ModuleName(Module) {}

Xception::Xception(XType XceptionType, const String Msg, const String Method, const String Class, const String Module) : Type(XceptionType),
                                                                                                                         Message(Msg),
                                                                                                                         MethodName(Method),
                                                                                                                         ClassName(Class),
                                                                                                                         ModuleName(Module) {}

String
Xception::GetNotification() {
    String Result = "An exception has been raised";
    if (!MethodName.IsEmpty()) Result += " in <" + MethodName + ">";
    if (!ClassName.IsEmpty()) Result += " of <" + ClassName + "> class";
    if (!ModuleName.IsEmpty()) Result += " in [" + ModuleName + "] module";
    if (!Message.IsEmpty())
        Result += " with following message:\n_____________________\n\n" + Message + "\n_____________________\n\n";
    else
        Result += ".";
    return Result;
}

void Xception::Notify() {
#ifdef WIN32
    MessageBox(NULL, GetNotification(), "Error message", MB_OK);
//    AfxMessageBox ( GetNotification () );
#else
    cout << GetNotification();
#endif
}
