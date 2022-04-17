/*____________________________________________________________________

  Project:

      Xception class representation
      _____________________________

  (C) Maxim Proskurnya,                           1999--2003, AMC, MSU
      max@axofiber.org.ru

  Component:  Xception
  Comments:   Declares class for exceptions
  ____________________________________________________________________
*/

#ifndef XCEPTION_H_1999
#define XCEPTION_H_1999

#include "String.h"

typedef int XType;

class Xception {
public:
    // Life-cycle
    Xception(const String Msg = "", const String Method = "", const String Class = "", const String Module = "undescribed");
    Xception(XType XceptionType, const String Msg, const String Method, const String Class, const String Module);
    virtual ~Xception(){};
    // Interface
    String GetNotification();
    void Notify();
    // Attributes
    XType Type;
    String Message;
    String MethodName;
    String ClassName;
    String ModuleName;

    static const int X_GENERAL;
    static const int X_BOUNDARIES;
    static const int X_FILE_IO;
    static const int X_MEMORY;
    static const int X_NETWORK;
};

// try-finally block imitation for resource release idioma
#define finally(Saver) \
    catch (...) {      \
        Saver throw;   \
    }                  \
    Saver

#endif
