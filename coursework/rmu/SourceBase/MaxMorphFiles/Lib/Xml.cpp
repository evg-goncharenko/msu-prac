/*____________________________________________________________________

  Project:

      XML utils
      _________

  (C) Maxim Proskurnya,                           2001--2003, AMC, MSU
      max@axofiber.org.ru

  Component:  XML
  Comments:   
  ____________________________________________________________________

  19.XI.2003                                                     02.11
      Fixed "LastChar is undefined" problem in the
      XML::ReadTag ( IStream * ) method. Fixed "empty attribute" issue
      in "XMLElement::Serialize" method.
  28.X.2003                                                      02.10
      Redesigned XMLElement family classes.      
  22.V.2003                                                      01.09
      Updated "ParseTag" method to proceed attrtibutes with inline
      quotes and unquoted attrs correctly.
  13.XI.2001                                                     00.05
      Fixed "ParseTag" method with "empty attribute" issue.
  29.IX.2001                                                     00.04
      Added "Exists" method in "CAttrArray" class.
  09.VIII.2001                                                   00.03
      Added "GetSameStringPart" function
  10.VII.2001                                                    00.02
      Added "ParseTag" routine.
  ____________________________________________________________________
*/
#include "stdafx.h"
#include "Xml.h"

const static String ModuleFileName ( __FILE__ );

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Exists                                                      )   (
                                                                \___/
  ____________________________________________________________________
*/

bool 
AttrArray::Exists ( const String & AttrName, int & AttrIndex ) const
{
    for ( int i = 0; i < Array.Count (); i++ )
        if ( Array[i].Name == AttrName )
        { 
            AttrIndex = i;
            return true;
        }
    return false;
}

/*____________________________________________________________________
                                                                 ____
                                                                /   /
    operator[]                                                 /   /
                                                              /___/
  ____________________________________________________________________
*/

String
AttrArray::operator[] ( const String & AttrName ) const
{
    for ( int i = 0; i < Array.Count (); i++ )
        if ( Array[i].Name == AttrName )
            return Array[i].Value;
    return "";
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Ungrouped methods                                           )   (
                                                                \___/
  ____________________________________________________________________
*/

String 
GetSameStringPart ( const Strings& Strings )
{
    bool    finished ( false );
    int     i, j;
    String  Result;

    if ( Strings.Count () > 0 )
    {
        i = 0;
        while ( !finished && i < Strings[0].Length () )
        {
            Result += Strings[0][i];
            for ( j = 0; j < Strings.Count (); j++ )
            {
                if ( Result != Strings[j].SubStr ( 0, i+1 ) )
                {
                    Result = "";
                    finished = true;
                }
            }
            i++;
        }
    }
    return Result;
}

inline int 
StrToInt ( const String & Str )
{
    return Str.ToInt ();
}
/*
bool IsWowel ( char a )
{
    return a == 'а' || a == 'е' || a == 'ё' || a == 'и' || a == 'о' || a == 'у' || a == 'ы' || a == 'э' || a == 'ю' || a == 'я';
}
*/
void MakeLower ( String& s, int start )
{
    for ( int i = start; i < s.Length(); ++i )
        if ( ( unsigned char ) s[i] == 168 )
            s[i] = ( char ) 184;
        else
            s[i] = ( (64 < s[i] && s[i] < 91) || (192 <= unsigned ( s[i] ) && unsigned ( s[i] ) < 224 )) ? ( unsigned ( s[i] ) + 32 ) : s[i];
}

String CompleteMultiStringResult ( Strings Strings )
{
    String Result;
    for ( int i = 0; i < Strings.Count (); ++i )
    {
        Result += Strings[i];
        Result += " />";
    }
    return Result;
}

String XMLText::emptyname = "";
char XMLElement::LastChar = '\0';

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    ParseTag                                                  |  (o) |
                                                              |______|
  ____________________________________________________________________
*/

void
XMLElement::ParseTag ( const String & Tag, XMLElem * xmlElem )
{
    int           Pos;
    int           length;
    bool          Quoted;
    bool          LastQuot;
    unsigned char Quot;
    ::Attr        attr;
    String        line ( Tag );
   
    if ( !line.IsEmpty () )
    {
        Pos = 0;
        if ( line[Pos++] == '<' )
        {
            length = line.Length ();
            // Skip spaces
            while ( Pos < length && line[Pos] == ' ' ) ++Pos;
            // Read tag name
            while ( Pos < length && line[Pos] != ' ' && line[Pos] != '>') 
                xmlElem->Name () += line[Pos++];
            while ( Pos < length && line[Pos] != '>' )
            {
                attr.Name.Clear ();
                attr.Value.Clear ();
                Quoted = false;
                LastQuot = false;
                // Skip spaces
                while ( Pos < length && line[Pos] == ' ' ) ++Pos;
                // Read attr name
                while ( Pos < length && line[Pos] != ' ' && line[Pos] != '=' && line[Pos] != '>' ) 
                    attr.Name += line[Pos++];
                // Skip spaces
                while ( Pos < length && line[Pos] == ' ' ) ++Pos;
                // Skip equal
                if ( Pos < length && line[Pos] == '=' ) ++Pos;
                // Skip spaces
                while ( Pos < length && line[Pos] == ' ' ) ++Pos;
                // Skip quote
                if ( Pos < length && ( line[Pos] == '"' || line[Pos] == '\'' ) )
                {
                    Quot = line[Pos];
                    Quoted = true;
                    ++Pos;
                }
                // Read attr value
                if ( Quoted )
                {
                    // Complex quoted attribute with possible double quotes inside
                    while ( Pos < length && Quoted ) 
                    {
                        if ( line[Pos] == Quot )
                        {
                            if ( LastQuot ) attr.Value += line[Pos];
                            LastQuot = !LastQuot;
                            ++Pos;
                        }
                        else
                        {
                            if ( LastQuot )
                            {
                                Quoted = false;
                            }
                            else 
                                attr.Value += line[Pos++];
                        }
                    }
                }
                else
                {
                    // Simple nonquoted attribute without spaces
                    while ( Pos < length && line[Pos] != ' ' && line[Pos] != '>' ) 
                        attr.Value += line[Pos++];
                }
                MakeLower ( attr.Name );
                if ( !attr.Name.IsEmpty () )
                    xmlElem->attrs->Add ( attr );
            }
        }
    }
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    ReadFrom                                                    )   (
                                                                \___/
  ____________________________________________________________________
*/

XMLElement *  
XMLElement::ReadFrom ( const String & Text, int StartPos )
{
    int     EndPos;
    return  ReadFrom ( Text, StartPos, EndPos );
}

XMLElement *
XMLElement::ReadFrom ( const String & StreamText, int StartPos, int & EndPos )
{
    XMLElement* Element = NULL, * nestedElement;
    String      EnclosingTagName;
    int         AttrPos;
    String      Name;
    String      Text;

    if ( StartPos < StreamText.Length () )
    {
        Element = ReadTag ( StreamText, StartPos, EndPos );
        StartPos = EndPos;
        if ( Element )
        {
            if ( Element->IsElem () && 
                !Element->Name ().IsEmpty () &&
                 Element->Name ()[0] != '/' && 
                !Element->Attrs()->Exists ( "/", AttrPos ) )
            {
                // Recursively read all nested tags
                EnclosingTagName = "/" + Element->Name ();
                nestedElement = ReadFrom ( StreamText, StartPos, EndPos );
                StartPos = EndPos;
                while ( StartPos < StreamText.Length () && nestedElement && nestedElement->Name () != EnclosingTagName )
                {
                    Element->Children()->Add ( nestedElement );
                    nestedElement = ReadFrom ( StreamText, StartPos, EndPos );
                    StartPos = EndPos;
                }
                if ( nestedElement && nestedElement->Name () == EnclosingTagName )
                    delete nestedElement;
            }
        }
    }
    return Element;
}

XMLElement *
XMLElement::ReadFrom ( IStreamBase * Stream )
{
    XMLElement* Element = NULL, * nestedElement;
    String      EnclosingTagName;
    int         AttrPos;
    String      Name;
    String      Text;

    if ( Stream->GetState () == IStreamBase::STREAM_OK )
    {
        Element = ReadTag ( Stream );
        if ( Element )
        {
            if ( Element->IsElem () && 
                !Element->Name ().IsEmpty () &&
                 Element->Name ()[0] != '/' && 
                !Element->Attrs()->Exists ( "/", AttrPos ) )
            {
                // Recursively read all nested tags
                EnclosingTagName = "/" + Element->Name ();
                nestedElement = ReadFrom ( Stream );
                while ( nestedElement && nestedElement->Name () != EnclosingTagName )
                {
                    Element->Children ()->Add ( nestedElement );
                    nestedElement = ReadFrom ( Stream );
                }
                if ( nestedElement && nestedElement->Name () == EnclosingTagName )
                    delete nestedElement;
            }
        }
    }
    return Element;
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    ReadTag                                                   |  (o) |
                                                              |______|
  ____________________________________________________________________
*/

XMLElement *
XMLElement::ReadTag ( const String & Line, int StartPos, int & EndPos )
{
    //bool      CanRead      ( true );
    bool      TagComplete  ( false );
    //bool      TextComplete ( false );
    bool      Quoted       ( false );
    bool      LastQuot     ( false );
    unsigned  char c;
    unsigned  char Quot ( 0 );
    String    Tag;
    String    Text;
    String    Comment;
    XMLElem * xmlElem = NULL;

    EndPos = StartPos;
    while ( EndPos < Line.Length () )
    {
        c = Line[EndPos];
        if ( c == '<' )
        {
            ++EndPos;
            Tag += c;
            // Read XML tag
            while ( EndPos < Line.Length () && !TagComplete )
            {
                c = Line[EndPos++];
                if ( ( c == '"' || c == '\'' ) && Quot == 0 )
                    Quot = c;
                if ( c == Quot )
                {
                    // This fragment determines whether '>' is the real end of tag
                    // or it's inside an attribute's body (controls double 
                    // quotation marks '""' as a single quot inside an attribute).
                    if ( LastQuot )
                    {
                        Quoted    = true;
                        LastQuot  = false;
                    }
                    else
                    {
                        if ( !Quoted )
                            Quoted = true;
                        else
                        {
                            Quoted    = false;
                            LastQuot  = true;
                            Quot      = 0;
                        }
                    }
                }
                else
                    LastQuot = false;
                if ( c != '\n' && c != '\r' ) Tag += c;
                if ( !Quoted && c == '>' ) TagComplete = true;
                if ( 3 < Tag.Length () && Tag.SubStr ( Tag.Length () - 4, 4 ) == "<!--" )
                {
                    // Treat comment
                    Tag = Tag.SubStr ( 0, Tag.Length () - 4 );
                    Comment = "   ";
                    while ( Comment != "-->" && EndPos < Line.Length () )
                    {
                        c = Line[EndPos++];
                        Comment[0] = Comment[1];
                        Comment[1] = Comment[2];
                        Comment[2] = c;
                    }
                }
            }
            if ( !Tag.IsEmpty () )
            {
                xmlElem = new XMLElem ();
                ParseTag ( Tag, xmlElem );
                return xmlElem;
            }
        }            
        else 
        {
            // Read text between tags
            while ( EndPos < Line.Length () && Line[EndPos] != '<' )
            {
                Text += Line[EndPos];
                ++EndPos;
            }
            if ( 2 < Text.Length () )
            {
                // Decode special character sequences
                Text.ReplaceAll ( "&nbsp;", " " );
                Text.ReplaceAll ( "&#151;", "-" );
                Text.ReplaceAll ( "&quot;", "\"" );
                Text.ReplaceAll ( "&laquo;", "\"" );
                Text.ReplaceAll ( "&raquo;", "\"" );
                Text.ReplaceAll ( "&lt;", "<" );
                Text.ReplaceAll ( "&gt;", ">" );
            }
            Text.TrimBlanks ();
            if ( !Text.IsEmpty () )
                return new XMLText ( Text );
        }
    }
    return NULL;
}

XMLElement *
XMLElement::ReadTag ( IStreamBase * Stream )
{
    //bool      CanRead      ( true );
    bool      TagComplete  ( false );
    //bool      TextComplete ( false );
    bool      Quoted       ( false );
    bool      LastQuot     ( false );
    unsigned  char c;
    unsigned  char Quot ( 0 );
    String    Tag;
    String    Text;
    String    Comment;
    XMLElem * xmlElem = NULL;

    
    while ( Stream->GetState () == IStreamBase::STREAM_OK )
    {
        if ( LastChar == '<' ) 
            c = LastChar;
        else
            Stream->Read ( &c, 1 );
        if ( c == '<' )
        {
            Tag += c;
            // Read XML tag
            while ( Stream->Read ( &c, 1 ) && !TagComplete )
            {
                if ( ( c == '"' || c == '\'' ) && Quot == 0 )
                    Quot = c;
                if ( c == Quot )
                {
                    // This fragment determines whether '>' is the real end of tag
                    // or it's inside an attribute's body (controls double 
                    // quotation marks '""' as a single quot inside an attribute).
                    if ( LastQuot )
                    {
                        Quoted    = true;
                        LastQuot  = false;
                    }
                    else
                    {
                        if ( !Quoted )
                            Quoted = true;
                        else
                        {
                            Quoted    = false;
                            LastQuot  = true;
                            Quot      = 0;
                        }
                    }
                }
                else
                    LastQuot = false;
                if ( c != '\n' && c != '\r' ) Tag += c;
                if ( !Quoted && c == '>' ) TagComplete = true;
                if ( 3 < Tag.Length () && Tag.SubStr ( Tag.Length () - 4, 4 ) == "<!--" )
                {
                    // Treat comment
                    Tag = Tag.SubStr ( 0, Tag.Length () - 4 );
                    Comment = "   ";
                    while ( Comment != "-->" && Stream->Read ( &c, 1 ) )
                    {
                        Comment[0] = Comment[1];
                        Comment[1] = Comment[2];
                        Comment[2] = c;
                    }
                }
            }
            LastChar = c;
            if ( !Tag.IsEmpty () )
            {
                xmlElem = new XMLElem ();
                ParseTag ( Tag, xmlElem );
                return xmlElem;
            }
        }            
        else 
        {
            // Read text between tags
            Text += c;
            while ( Stream->Read ( &c, 1 ) && c != '<' )
                Text += c;
            LastChar = c;
            if ( 2 < Text.Length () )
            {
                // Decode special character sequences
                Text.ReplaceAll ( "&nbsp;", " " );
                Text.ReplaceAll ( "&#151;", "-" );
                Text.ReplaceAll ( "&quot;", "\"" );
                Text.ReplaceAll ( "&laquo;", "\"" );
                Text.ReplaceAll ( "&raquo;", "\"" );
                Text.ReplaceAll ( "&lt;", "<" );
                Text.ReplaceAll ( "&gt;", ">" );
            }
            Text.TrimBlanks ();
            if ( !Text.IsEmpty () )
                return new XMLText ( Text );
        }
    }
    return NULL;
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    Serialize                                                 |  (o) |
                                                              |______|
  ____________________________________________________________________
*/

String
XMLElement::Serialize ( int Level ) const
{
    int     i;
    String  Result;
    String  AttrValue;
    String  Ident;

    if ( 0 < Level )
        for ( i = 0; i < Level; ++i )
            Ident += "  ";

    if ( IsText () )
        Result = Ident + Text () + "\r\n";
    else
    {
        Result = Ident + "<" + Name ();
        for ( i = 0; i < Attrs ()->Count (); ++i )
        {
            if ( (*Attrs ())[i].Name != "/" )
            {
                AttrValue = (*Attrs ())[i].Value;
                if ( !AttrValue.IsEmpty () )
                {
                    Result += " ";
                    Result += (*Attrs ())[i].Name;
                    Result += "=";
                    if ( 0 <= AttrValue.Find ( " " ) || 0 <= AttrValue.Find ( "\"" ) )
                    {
                        // Attr's value should be quoted
                        Result += "\"";
                        AttrValue.ReplaceAll ( "\"", "\"\"" );
                        Result += AttrValue;
                        Result += "\"";
                    }
                    else
                        // Simple attr's value
                        Result += AttrValue;
                }
            }
        }
        if ( Children()->Count () == 0 )
            // Flat XML element doesn't have closing trail slash-tag
            Result += " />\r\n";
        else
        {
            // XML element may contain other xml elements
            Result += ">\r\n";
            for ( i = 0; i < Children()->Count (); ++i )
                Result += (*Children())[i]->Serialize ( Level + 1 );
            Result += Ident + "</" + Name () + ">\r\n";
        }
    }
    return Result;
}

/*____________________________________________________________________
                                                                  _
                                                                _| |_
    XMLElem constructor                                        |_   _|
                                                                 |_|
  ____________________________________________________________________
*/

XMLElem::XMLElem () : 
  attrs ( new AttrArray ),
  elems ( new XMLElems )
{}

XMLElem::XMLElem ( const String & Name ) :
  attrs ( new AttrArray ),
  elems ( new XMLElems ),
  name  ( Name )
{}


/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Text                                                        )   (
                                                                \___/
  ____________________________________________________________________
*/

String
XMLElem::Text () const
{
    int     i;
    String  Result;

    if ( 0 < elems->Count () )
        for ( i = 0; i < elems->Count (); ++i )
            Result += (*elems)[i]->Text ();
    return Result;
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    RemoveAll                                                 |  (o) |
                                                              |______|
  ____________________________________________________________________
*/

void
XMLElem::RemoveAll ()
{
    if ( attrs ) delete attrs;
    if ( elems ) delete elems;
    name.Clear ();
}

/*____________________________________________________________________
                                                                 _  _
                                                                \ \/ /
    XMLElement destructor                                        )  (
                                                                /_/\_\
  ____________________________________________________________________
*/

XMLElem::~XMLElem ()
{
    RemoveAll ();
}
