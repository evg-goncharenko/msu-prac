/*____________________________________________________________________

  Project:

      XML utils
      _________

  (C) Maxim Proskurnya,                           2001--2003, AMC, MSU
      max@axofiber.org.ru

  Component:  XML
  Comments:   
  ___________________________________________________________________
*/

#ifndef XML_H_2003
#define XML_H_2003

// forward declarations
class  XMLElement;
class  XMLElem;
struct Attr;
class  AttrArray;


#include "Xception.h"
#include "FileStreamBase.h"
#include "SCArray.h"


extern bool
IsWowel ( char a );

extern void
MakeLower ( String& s, int start = 0 );

extern String
CompleteMultiStringResult ( Strings Strings );

struct Attr
{
    String Name;
    String Value;
    Attr () {};
    Attr ( const String & name, const String & value ) :
        Name  ( name ),
        Value ( value )
    {}
    Attr ( const Attr & Attr ) :
        Name ( Attr.Name ),
        Value ( Attr.Value )
    {}
};

class AttrArray
{
public:
    AttrArray &   Add ( Attr Attr ) 
                  {
                      Array.Add ( Attr ); return * this; 
                  }
    bool          Exists  ( const String & AttrName, int & AttrIndex ) const;
    int           Count   () const 
                  {
                      return Array.Count (); 
                  }
    Attr          operator[] ( int Index ) const 
                  {
                      return Array[Index]; 
                  }
    String        operator[] ( const String & AttrName ) const;
    AttrArray &   RemoveAll () 
                  {
                      Array.RemoveAll ();  return * this; 
                  }
                  AttrArray () {};
                  AttrArray ( const AttrArray & AttrArray ) :
                      Array ( AttrArray.Array )
                  {};
private:
    CSCTypeArray<Attr> Array;
};

/*____________________________________________________________________

                                                                    /
    XMLElement class definition                                 \  /
                                                                 \/
  ____________________________________________________________________
*/

typedef CSCPtrArray<XMLElement> XMLElems;

class XMLElement
{
public:
    virtual              ~XMLElement () {};
// Interface
    virtual AttrArray   * Attrs () const = 0;
    virtual String        AttrName ( int AttrPos ) const = 0;
    virtual String        Attr ( int AttrPos ) const = 0;
    virtual String        Attr ( const String & Name ) const = 0;
    virtual XMLElems    * Children () const = 0;
    virtual XMLElement  * Child ( int Index ) const = 0;
    virtual void          ForceNotToOwn () = 0;
    virtual bool          IsElem () const = 0;
    virtual bool          IsText () const = 0;
    virtual String      & Name () = 0;
    virtual String        Name () const = 0;
    static  XMLElement  * ReadFrom ( const String & Text, int StartPos = 0 );
    static  XMLElement  * ReadFrom ( const String & TextStream, int StartPos, int & EndPos );
    static  XMLElement  * ReadFrom ( IStreamBase * Stream );
    virtual String        Serialize () const { int Level (0); return Serialize ( Level ); }
    virtual String        Text () const = 0;
private:
// Operations
    static  void          ParseTag  ( const String & Tag, XMLElem * xmlElem );
    static  XMLElement  * ReadTag   ( const String & Line, int StartPos, int & EndPos );
    static  XMLElement  * ReadTag   ( IStreamBase * Stream );
            String        Serialize ( int Level ) const;
// Attributes
    static  char          LastChar;
};

class XMLText : public XMLElement
{
public:
// Life-cycle
                          XMLText () {}
                          XMLText ( const String & Line ) :
                            text ( Line ) 
                          {}
                          XMLText ( const XMLText & aXMLText ) : 
                            text ( aXMLText.text ) 
                          {}
                         ~XMLText () {}
// Operations
            AttrArray   * Attrs () const { return NULL; }
            String        AttrName ( int AttrPos ) const { return ""; }
            String        Attr ( int AttrPos ) const { return ""; }
            String        Attr ( const String & Name ) const { return ""; }
            XMLElems    * Children () const { return NULL; }
            XMLElement  * Child ( int Index ) const { return NULL; }
            void          ForceNotToOwn () {}
            bool          IsElem () const { return false; }
            bool          IsText () const { return true; }
            String      & Name () { return emptyname; }
            String        Name () const { return emptyname; }
            String        Text () const { return text; }
private:
// Attributes
            String        text;
    static  String        emptyname;
};

class XMLElem : public XMLElement
{
public:
// Life-cycle
                          XMLElem ();
                          XMLElem ( const String & Name );
    virtual              ~XMLElem ();
// Operations
            AttrArray   * Attrs () const { return attrs; }
            String        Attr ( int AttrPos ) const { return (*attrs)[AttrPos].Value; }
            String        AttrName ( int AttrPos ) const { return (*attrs)[AttrPos].Name; }
            String        Attr ( const String & Name ) const { return (*attrs)[Name]; }
            XMLElems    * Children () const { return elems; }
            XMLElement  * Child ( int Index ) const { return (*elems)[Index]; }
            void          ForceNotToOwn () { elems->ForceNotToOwn (); }
            bool          IsElem () const { return true; }
            bool          IsText () const { return false; }
            String      & Name () { return name; }
            String        Name () const { return name; }
            String        Text () const;
private:
// Attributes
            AttrArray   * attrs;
            XMLElems    * elems;
            String        name;
// Operations
            void          RemoveAll ();
// Restrictions
                          XMLElem ( const XMLElement & Element ) {};
            friend class  XMLElement;
};

#endif
