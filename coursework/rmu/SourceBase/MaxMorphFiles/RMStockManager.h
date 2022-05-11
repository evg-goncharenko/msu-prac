/*____________________________________________________________________

  Project:

      Russian  Morphology   Unit
      http://axofiber.org.ru/rm/
      __________________________

  (C) Maxim Proskurnya,                           2001--2003, AMC, MSU
      max@axofiber.org.ru


  Component:  StockManager
  Comments:   Implements storage manipulation routines
  ____________________________________________________________________
*/

#if !defined(AFX_IRMSTOCKMANAGER_H__5D7A9C80_5E0D_11D5_BE32_00409516091F__INCLUDED_)
#define AFX_IRMSTOCKMANAGER_H__5D7A9C80_5E0D_11D5_BE32_00409516091F__INCLUDED_

#include "RMStock.h"

/*____________________________________________________________________

                                                                  /\
    Stock Manager interface definition                           <  >
                                                                  \/
  ____________________________________________________________________
*/

class IRMStockManager
{
public:
            enum              FileFormat    { SM_FORMAT_BINARY, SM_FORMAT_XML };
    static  IRMStockManager * Create        ();
    virtual CRMLexem*         AddLexem      ( const String& SynClass, const String& StemID, const String& PClassID, const String& StressSchemaID ) = 0;
    virtual int               GetBusyStatus () const = 0;
    virtual CRMLexems*        FindCognates  ( const String& Form ) const = 0;
    virtual CRMLexems *       FindLexems    ( const String& Stem, const String& Flex ) const = 0;
    virtual CRMLexems *       FindComparativeLexems    ( const String& Stem, const String& Flex ) const = 0;
    virtual CRMParadigm *     FindPClass    ( const String& PClassName ) = 0;
    virtual CRMPClasses *     FindPClasses  ( const String& Flex ) const = 0;
    virtual CRMPClasses *     FindPClasses  ( const String& FlexID, const String& SClass ) const = 0;
    virtual String            FindPClassesForXML ( const String& Flex, const String& SClass ) const = 0;
    virtual int               LexemsCount   () const = 0;
    virtual int               FormsCount    () const = 0;
    virtual void              LoadFromFile  ( const String& BaseName, enum IRMStockManager::FileFormat Format ) = 0;
    virtual int               PClassesCount () const = 0;
    virtual void              SaveToFile    ( const String& BaseName, enum IRMStockManager::FileFormat Format ) = 0;
    virtual int               StressSchemasCount () const = 0;
    virtual void              UpdateCounters( bool Clear = false ) = 0;
#ifdef RMU_UTIL
	virtual bool			  GetStem(int index, String& Form) const = 0;
	virtual bool			  CheckStressSchemas() const = 0;
	virtual bool			  CheckLemmas() const = 0;
#endif
    virtual                  ~IRMStockManager () {};
};

#endif // !defined(AFX_IRMSTOCKMANAGER_H__5D7A9C80_5E0D_11D5_BE32_00409516091F__INCLUDED_)
