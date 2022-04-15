/*____________________________________________________________________

  Project:

      Russian  Morphology   Unit
      http://axofiber.org.ru/rm/
      __________________________

  (C) Maxim Proskurnya,                           2001--2003, AMC, MSU
      max@axofiber.org.ru

  (C) Volkova Irina,                              1982--2003, AMC, MSU
      volkova@cs.msu.su
  ____________________________________________________________________
*/

#ifndef RMUNIT_H
#define RMUNIT_H

#include "RMStockManager.h"

/*____________________________________________________________________

                                                                  /\
    Russian Morphology unit interface definition                 <  >
                                                                  \/
  ____________________________________________________________________
*/

class IRMUnit
{
public:
    static  IRMUnit * Create        ();
    virtual int       GetBusyStatus () const = 0;
    virtual int       LexemsCount   () const = 0;
    virtual int       FormsCount    () const = 0;
    virtual void      LoadFromFile  ( const String & BaseName, IRMStockManager::FileFormat Format = IRMStockManager::SM_FORMAT_BINARY ) = 0;
    virtual int       PClassesCount () const = 0;
    virtual String    ProcessQuery  ( const String & Query ) = 0;
    virtual void      SaveToFile    ( const String & BaseName, IRMStockManager::FileFormat Format = IRMStockManager::SM_FORMAT_BINARY ) = 0;
    virtual int       StressSchemasCount () const = 0;
    virtual          ~IRMUnit       () {};
};

#endif // !defined(AFX_IRMUNIT_H__78BD1CC0_6182_11D5_BE32_00409516091F__INCLUDED_)
