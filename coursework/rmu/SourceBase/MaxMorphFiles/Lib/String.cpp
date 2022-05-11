/*____________________________________________________________________

  Project:

      String manipulation utils
      _________________________

  (C) Maxim Proskurnya,                           1999--2004, AMC, MSU
      max@axofiber.org.ru

  Component : String
  Comments  : Represents char block container
              Implements reference count algorithm and two-way
              string representation for short and large strings
  ____________________________________________________________________

  28.II.2018                                                     02.16
	  String representation changed to accomodate both 32- 
	  and 64-bit architectures
	  Previous represtentation fitted only 32-bit.
  02.IV.2004                                                     02.15
      Modified "TrimBlanks" method (fixed "same buffer" bug also),
      added "DefaultBlankSymbols" field, added "TrimBlanksCopy"
      method.
  13.I.2004                                                      02.14
      Added "Compare" method.
  25.XI.2003                                                     02.13
      Fixed "operator != (cahr c)" bug.
  21.XI.2003                                                     02.12
      Fixed "copy on modify" bug in "char & operator [] (int Index)".
  18.X.2003                                                      02.11
      Added "Find" method for single char needles.
  17.VI.2003                                                     02.10
      Redesigned string representation. Implemented new features.
      Fixed main functionality. Tested on MS VC++ and GCC compilers.
      Implemented fast "Trim", "TrimBlanks" methods. Implemented 
      parallel methods for const and volatile representation.
  22.V.2003                                                      01.09
      Fixed fast "Find" method and added fast "RFind", "Replace",
      "ReplaceAll" methods.
  28.XI.2002                                                     01.08
      Added fast "Find" method.
  09.VI.2002                                                     01.07
      Fixed "MakeLower" function.
  14.II.2002                                                     00.06
      Added "Trim", "MakeLower" procedures.
  Jan, 1999                                                      00.01
      "CCapsule" project started. String representation is two-bytes
  ____________________________________________________________________
*/

#include "stdafx.h"
#include "Xception.h"
#include "String.h"

static  const String ModuleName ( __FILE__ );
        const String String::DefaultBlankSymbols = " \n\r\t";

/*____________________________________________________________________
                                                                  _
                                                                _| |_
    String constructors                                        |_   _|
                                                                 |_|
  ____________________________________________________________________
*/

String::String ( const char * NullTerminatedBody )
{
    Rep.lBuf.Low = 0;
    Rep.lBuf.Ptr = NULL;
    if ( NullTerminatedBody )
        CreateFrom ( NullTerminatedBody, strlen ( NullTerminatedBody ) );
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Append                                                      )   (
                                                                \___/
  ____________________________________________________________________
*/

String &
String::Append ( const String & NewString )
{
    if ( !NewString.IsEmpty () )
    {
        if ( IsLargeRep () )
        {
            int NewStringLength = NewString.Length ();
            if ( lRefs ( Rep.lBuf.Ptr ) == 1 && lLength ( Rep.lBuf.Ptr ) + NewStringLength + 1 < lSize ( Rep.lBuf.Ptr ) )
            {
                // New string fits the buffer size and noone refers to it
                if ( NewStringLength == 1 )
                {
                    // Simplest operation: appending single char
                    lStr ( Rep.lBuf.Ptr ) [ lLength ( Rep.lBuf.Ptr ) ] = * NewString.CStr();
                }
                else
                    memcpy ( lStr ( Rep.lBuf.Ptr ) + lLength ( Rep.lBuf.Ptr ), NewString.CStr (), NewStringLength * sizeof ( char ) );
                lLength ( Rep.lBuf.Ptr ) += NewStringLength;
                // Append null symbol
                lStr ( Rep.lBuf.Ptr ) [ lLength ( Rep.lBuf.Ptr ) ] = 0x0;
            }
            else
            {
                int NewSize;
                if ( ( lSize ( Rep.lBuf.Ptr ) << 1 ) < lLength( Rep.lBuf.Ptr ) + NewStringLength + 1 )
                {
                    NewSize = 3 * sizeof ( int ) / sizeof ( char ) + lLength( Rep.lBuf.Ptr ) + NewStringLength + 1;
                    char * TempPtr = new char [NewSize];
                    memcpy ( lStr ( TempPtr ), lStr ( Rep.lBuf.Ptr) , lLength ( Rep.lBuf.Ptr ) * sizeof ( char ) );
                    memcpy ( lStr ( TempPtr ) + lLength ( Rep.lBuf.Ptr ), NewString.CStr (), ( NewStringLength + 1 )* sizeof ( char ) );
                    lLength ( TempPtr ) = lLength ( Rep.lBuf.Ptr ) + NewStringLength;
                    lSize ( TempPtr ) = lLength ( Rep.lBuf.Ptr ) + 1;
                    lRefs ( TempPtr ) = 1;
                    ReleasePtr ( Rep.lBuf.Ptr );
                    Rep.lBuf.Ptr = TempPtr;
                }
                else
                {
                    // Doubling buffer size
                    ExpandBuffer ();
                    memcpy ( lStr ( Rep.lBuf.Ptr ) + lLength ( Rep.lBuf.Ptr ), NewString.CStr (), ( NewStringLength + 1 ) * sizeof ( char ) );
                    lLength ( Rep.lBuf.Ptr ) = lLength ( Rep.lBuf.Ptr ) + NewStringLength;
                }
            }
        }
        else
        {
            // Short representation
            int CurLength = strlen ( Rep.sBuf );
            int NewStringLength = NewString.Length (); 
            if ( MaxShortLen < CurLength + NewStringLength )
            {
                int newSize =  3 * sizeof ( int ) / sizeof ( char ) + CurLength + NewStringLength + 1;
                char * TempPtr = new char [newSize];
                memcpy ( lStr ( TempPtr ), Rep.sBuf, CurLength * sizeof ( char ) );
                memcpy ( lStr ( TempPtr ) + CurLength, NewString.CStr (), ( NewStringLength + 1 ) * sizeof( char ) );
                lLength ( TempPtr ) = CurLength + NewStringLength;
                lSize ( TempPtr ) = lLength ( TempPtr ) + 1;
                lRefs ( TempPtr ) = 1;
                Rep.lBuf.Low = 0;
                Rep.lBuf.Ptr = TempPtr;
            }
            else
			{
                for ( int i = 0; i < NewStringLength; ++i )
                    Rep.sBuf[i + CurLength] = NewString.Rep.sBuf[i];
				Rep.sBuf[CurLength + NewStringLength] = 0;
			}
        }
    }
    return * this;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Clear                                                       )   (
                                                                \___/
  ____________________________________________________________________
*/

String &
String::Clear ()
{
    if ( IsLargeRep () )
        ReleasePtr ( Rep.lBuf.Ptr );
    Rep.lBuf.Low = 0;
    Rep.lBuf.Ptr = NULL;
    return * this;
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    CloneBuffer                                               |  (o) |
                                                              |______|
  ____________________________________________________________________
*/

void
String::CloneBuffer ()
{
    int     newSize = 3 * sizeof ( int ) / sizeof ( char ) + lLength ( Rep.lBuf.Ptr ) + 1;
    char  * TempPtr = new char [newSize];
    memcpy ( TempPtr, Rep.lBuf.Ptr, newSize * sizeof ( char ) );
    lRefs ( TempPtr )   = 1;
    lLength ( TempPtr ) = lLength ( Rep.lBuf.Ptr );
    lSize ( TempPtr )   = lLength ( Rep.lBuf.Ptr ) + 1;
    ReleasePtr ( Rep.lBuf.Ptr );
    Rep.lBuf.Ptr = TempPtr;
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    CollapseBuffer                                            |  (o) |
                                                              |______|
  ____________________________________________________________________
*/

void
String::CollapseBuffer ()
{
    int newSize =  3 * sizeof ( int ) / sizeof ( char ) + ( lSize ( Rep.lBuf.Ptr ) >> 1 );
    char * TempPtr = new char [newSize];
    memcpy ( TempPtr, Rep.lBuf.Ptr, newSize * sizeof ( char ) );
    lRefs ( TempPtr )   = 1;
    lLength ( TempPtr ) = lLength ( Rep.lBuf.Ptr );
    lSize ( TempPtr )   = lSize ( Rep.lBuf.Ptr ) >> 1;
    ReleasePtr ( Rep.lBuf.Ptr );
    Rep.lBuf.Ptr = TempPtr;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Compare                                                     )   (
                                                                \___/
  ____________________________________________________________________
*/

int
String::Compare ( const String & NewString ) const
{
    if ( & NewString == this )
        // Preserve self copy
        return 0;
    else
    {
        if ( IsLargeRep () && Rep.lBuf.Ptr == NewString.Rep.lBuf.Ptr )
            // Reference to the same buffer
            return 0;
        int res (0);
        int Len    = Length ();
        int NewLen = NewString.Length ();
        if ( Len != 0 && NewLen != 0 )
            res    = memcmp ( CStr (), NewString.CStr (), ( Len < NewLen ? Len : NewLen ) * sizeof ( char ) );
        return ( res == 0 && Len != NewLen ) ? Len - NewLen : res;
    }
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    CreateFrom                                                |  (o) |
                                                              |______|
  ____________________________________________________________________
*/

void
String::CreateFrom ( const char * Body, int NewLength )
{
    if (MaxShortLen < NewLength )
    {
        // Large string
        Rep.lBuf.Low = 0;
        int NewSize =  3 * sizeof ( int ) / sizeof ( char ) + NewLength + 1;
        char * Ptr = new char [ NewSize ];
        memcpy  ( lStr ( Ptr ), Body, NewLength * sizeof ( char ) );
        lRefs   ( Ptr ) = 1;
        lLength ( Ptr ) = NewLength;
        lSize   ( Ptr ) = NewLength + 1;
        lStr    ( Ptr )[NewLength] = 0x0;
        Rep.lBuf.Ptr = Ptr;
    }
    else if ( 0 < NewLength )
    {
        // Short string
        Rep.lBuf.Ptr = NULL;
        memcpy ( Rep.sBuf, Body, NewLength * sizeof ( char ) );
        Rep.sBuf[NewLength] = 0x0;
    }
    else if ( NewLength != 0 )
        throw Xception ( Xception::X_MEMORY, "Invalid NewLength value (negative, < 0).", "void String::CreateFrom ( const char * Body, int NewLength )", "String", ModuleName );

}

void
String::CreateFrom ( const String & NewString )
{
    if ( NewString.IsLargeRep () )
    {
        // Large string
        Rep.lBuf.Low = 0;
        Rep.lBuf.Ptr = NewString.Rep.lBuf.Ptr;
        ++lRefs ( Rep.lBuf.Ptr );
    }
    else
    {
        // Short string
        Rep.lBuf.Low = NewString.Rep.lBuf.Low;
        Rep.lBuf.Ptr = NewString.Rep.lBuf.Ptr;
    }
}

void
String::CreateFrom ( int Number )
{
    * this = FromInt ( Number );
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Erase                                                       )   (
                                                                \___/
  ____________________________________________________________________
*/

String &
String::Erase ( int Start, int Size )
{
    int Len = Length ();    
    if ( Len && 0 <= Start && 0 <= Size && Start + Size <= Len  )
    {
        if ( IsLargeRep () )
        {
            if ( 1 < lRefs ( Rep.lBuf.Ptr ) )
                CloneBuffer ();
            char * Str = CStr ();
            memcpy ( Str + Start, Str + Start + Size, ( Len - Start - Size + 1 ) * sizeof ( char ) );
            lLength ( Rep.lBuf.Ptr ) -= Size;
            if ( Len - Size < lSize ( Rep.lBuf.Ptr ) >> 1 )
                CollapseBuffer ();
        }
        else
        {
            char * Str = CStr ();
            memcpy ( Str + Start, Str + Start + Size, ( Len - Start - Size + 1 ) * sizeof ( char ) );
        }
    }
    return * this;
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    ExpandBuffer                                              |  (o) |
                                                              |______|
  ____________________________________________________________________
*/

void
String::ExpandBuffer ()
{
    int newSize =  3 * sizeof ( int ) / sizeof ( char ) + ( lSize ( Rep.lBuf.Ptr ) << 1 );
    char * TempPtr = new char [newSize];
    memcpy ( lStr ( TempPtr ), lStr ( Rep.lBuf.Ptr ), ( lLength ( Rep.lBuf.Ptr ) + 1 ) * sizeof ( char ) );
    lRefs   ( TempPtr ) = 1;
    lLength ( TempPtr ) = lLength ( Rep.lBuf.Ptr );
    lSize   ( TempPtr ) = lSize ( Rep.lBuf.Ptr ) << 1;
    ReleasePtr ( Rep.lBuf.Ptr );
    Rep.lBuf.Ptr = TempPtr;
}

/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    CheckIncluding                                            |  (o) |
                                                              |______|
  ____________________________________________________________________
*/
/*
int
String::CheckIncluding ( const String& _pcPattern, int _dCurrPos, int _dPatPos, int _dLength ) const
{
    // Check bounds
    if ( m_pBody->m_dLength < _dCurrPos + _dLength || _pcPattern.m_pBody->m_dLength < _dPatPos + _dLength )
         return FALSE;
    // Compare subStrings
    for ( int i = 0; i < _dLength; i++ )
        if ( m_pBody->m_wBody [ i + _dCurrPos ] != _pcPattern.m_pBody->m_wBody [ i + _dPatPos ] )
            return FALSE;
    return TRUE;
}
*/
/*____________________________________________________________________
                                                                 __
                                                               _|__|_
    GetIncPos                                                 |  (o) |
                                                              |______|
  ____________________________________________________________________
*/
/*
int
String::GetIncPos ( const String& _pcPattern, int _dStartPos ) const
{
    int         i = _dStartPos;
    String    cTag ("%s");

    while ( i < _pcPattern.m_pBody->m_dLength - 1 )
    {
        if ( _pcPattern.m_pBody->m_wBody [ i ]      == cTag.m_pBody->m_wBody [ 0 ] &&
             _pcPattern.m_pBody->m_wBody [ i + 1 ]  == cTag.m_pBody->m_wBody [ 1 ] )
            return i;               //  successive exit
        i++;
    }
    return -1;                      //  not found
}
*/

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Find                                                        )   (
                                                                \___/
  ____________________________________________________________________
*/

int
String::Find ( const char Needle, int Start ) const
{
    const char *  Haystack = CStr ();
    int           HaystackLength ( Length () );
    
    while ( Start < HaystackLength ) 
        if ( Haystack[Start] == Needle )
            return Start;
        else
            ++Start;
    return -1;
}

int
String::Find ( const String & Needle, int Start ) const
{
    int i, j, k;
    const char * Haystack = CStr ();
    int HaystackLength ( Length () ), NeedleLength ( Needle.Length () );

    // Check string lengthes
    if ( NeedleLength == 0 )
        return 0;
    if ( HaystackLength == 0 )
        return -1;
    if ( NeedleLength == 1 )
    {
        char needle = Needle[0];
        i = Start;
        while ( i < HaystackLength ) 
            if ( Haystack[i] == needle ) return i;
            else ++i;
        return -1;
    }
    if ( HaystackLength < NeedleLength )
        return -1;
    // Process representative strings
    unsigned int d[256];
    // Init
    for ( i = 0; i < 256; ++i ) d[i] = NeedleLength;
    for ( i = 0; i < NeedleLength - 1; ++i )
        d[ ( unsigned char ) Needle[i]] = NeedleLength - i - 1;
    // Search
    i = NeedleLength + Start;
    if ( i <= HaystackLength )
    {
        do
        {
            j = NeedleLength;
            k = i;
            do
            {
                --k;
                --j;
            }
            while ( 0 <= j && Haystack[k] == Needle[j] );
            i += d[ ( unsigned char ) Haystack[i-1]];
        }
        while ( 0 <= j && i <= HaystackLength );
        return j == -1 ? k + 1 : -1;

    }
    else
        return -1;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Pattern                                                     )   (
                                                                \___/
  ____________________________________________________________________
*/
/*
int
String::Pattern ( String _cPattern, String* _cArg, ... ) const
{
    char*       ap;     // stack pop type
    String*   cArg;
    int         bProceed;
    int         dCurrPos = 0, dPatPos = 0, dTagPos, dIncPos;

    // Turn the top of stack to the second argument
    va_start ( ap, _cArg );
    cArg = _cArg;
    *cArg = String ();

    // Run extracting
    dTagPos = GetIncPos ( _cPattern, dPatPos );
    bProceed = TRUE;
    while ( bProceed )
    {
        if ( dCurrPos >= m_pBody->m_dLength || dPatPos > _cPattern.m_pBody->m_dLength )
        {
            // Check bounds
            if ( dCurrPos == m_pBody->m_dLength && dPatPos == _cPattern.m_pBody->m_dLength - 2 )
                bProceed = FALSE;
            // fail compare
            else return FALSE;
        }
        else
        {
            if ( dTagPos == -1 )
            {
                // Tag not found
                if ( !CheckIncluding ( _cPattern, dCurrPos, dPatPos, _cPattern.m_pBody->m_dLength - dPatPos ) )
                    //  fail compare
                    return FALSE;
                else bProceed = FALSE;
            }
            else 
            {
                // Continue process
                if ( dTagPos > dPatPos )
                {
                    // Swallow pre
                    if ( !CheckIncluding ( _cPattern, dCurrPos, dPatPos, dTagPos - dPatPos ) )
                        //  fail compare
                        return FALSE;
                    // Else shift markers
                    dCurrPos += ( dTagPos - dPatPos );
                    dPatPos = dTagPos;
                }
                else
                {
                    // Try to extract
                    if ( dPatPos == _cPattern.m_pBody->m_dLength - 2 )
                    {
                        // Last argument found. Extract
                        if ( cArg )
                        {
                            // Successive exit
                            *cArg = SubString ( dCurrPos, m_pBody->m_dLength - dCurrPos );
                            bProceed = FALSE;
                        }
                        //  fail extracting
                        else return FALSE;
                    }
                    else
                    {
                        // Shift marker
                        dPatPos += 2;
                        dTagPos = GetIncPos ( _cPattern, dPatPos );
                        if ( dTagPos == -1 )
                        {
                            // Last argument found, but tail remained
                            dIncPos = GetPos ( _cPattern.SubString ( dPatPos, _cPattern.m_pBody->m_dLength - dPatPos ), dCurrPos );
                            if ( dIncPos == -1 )
                                //  fail extracting
                                return FALSE;
                            // Extract
                            if ( cArg )
                            {
                                // Successive exit
                                *cArg = SubString ( dCurrPos, dIncPos - dCurrPos );
                                bProceed = FALSE;
                            }
                            //  fail extracting
                            else return FALSE;
                            // Shift marker
                            dCurrPos = dIncPos;
                        }
                        else
                        {
                            // More than one tags remained
                            dIncPos = GetPos ( _cPattern.SubString ( dPatPos, dTagPos - dPatPos ), dCurrPos );
                            if ( dIncPos == -1 )
                                //  fail extracting
                                return FALSE;
                            // Extract
                            if ( cArg ) *cArg = SubString ( dCurrPos, dIncPos - dCurrPos );
                            else
                                //  fail extracting
                                return FALSE;
                            // Shift marker
                            dCurrPos = dIncPos;
                            // Peek next argument
                            cArg = va_arg ( ap, String* );
                            *cArg = String ();
                        }
                    }
                }
            }
        }
    }
    return TRUE;

}
*/

/*____________________________________________________________________
                                                                 ____
                                                                /   /
    operator []                                                /   /
                                                              /___/
  ____________________________________________________________________
*/

char &
String::operator [] ( int Index )
{
    if ( Index < Length () ) 
    {
        if ( IsLargeRep () && 1 < lRefs ( Rep.lBuf.Ptr ) )
            // Copy on modify
            CloneBuffer ();
        return CStr () [Index];
    }
    throw Xception ( Xception::X_BOUNDARIES, String ( "String index " ) + String::FromInt ( Index ) + " is out of bounds (0-" + String::FromInt ( Length () ) + ")", "char & operator [] ( int Index )", "String", ModuleName );
}

char
String::operator [] ( int Index ) const
{
    if ( Index < Length () ) return CStr () [Index];
    throw Xception ( Xception::X_BOUNDARIES, String ( "String index " ) + String::FromInt ( Index ) + " is out of bounds (0-" + String::FromInt ( Length () ) + ")", "char operator [] ( int Index ) const", "String", ModuleName );
}

/*____________________________________________________________________
                                                                 ____
                                                                /   /
    operator ==                                                /   /
                                                              /___/
  ____________________________________________________________________
*/

bool
String::operator == ( const String & NewString ) const
{
    if ( &NewString == this )
        // Preserve self copy
        return true;
    else
    {
        if ( IsLargeRep () && Rep.lBuf.Ptr == NewString.Rep.lBuf.Ptr )
            // Reference to the same buffer
            return true;
        int Len = Length ();
        if ( Len != NewString.Length () )
            return false;
        return memcmp ( CStr (), NewString.CStr (), Len * sizeof ( char ) ) == 0;
    }
}

/*____________________________________________________________________
                                                                 ____
                                                                /   /
    operator !=                                                /   /
                                                              /___/
  ____________________________________________________________________
*/

bool
String::operator != ( const String & NewString ) const
{
    if ( &NewString == this )
        // Preserve self copy
        return false;
    else
    {
        if ( IsLargeRep () && Rep.lBuf.Ptr == NewString.Rep.lBuf.Ptr )
            // Reference to the same buffer
            return false;
        int Len = Length ();
        if ( Len != NewString.Length () )
            return true;
        return memcmp ( CStr (), NewString.CStr (), Len * sizeof ( char ) ) != 0;
    }
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Replace                                                     )   (
                                                                \___/
  ____________________________________________________________________
*/

String &
String::Replace ( const String & Needle, const String & Replacer, int Start )
{
    int pos;
    int Len = Length ();
    int NeedleLength = Needle.Length ();
    
    if ( 0 < Len && 0 < NeedleLength &&
         0 <= ( pos = Find ( Needle, Start ) ) )
    {
        String temp ( *this );
        Clear ();
        CreateFrom ( temp.SubStr ( 0, pos ) );
        Append ( Replacer );
        Append ( temp.SubStr ( pos + NeedleLength, Len - pos - NeedleLength ) );
    }
    return * this;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    ReplaceAll                                                  )   (
                                                                \___/
  ____________________________________________________________________
*/

String &
String::ReplaceAll ( const String & Needle, const String & Replacer, int Start )
{
    int   pos;

    int   Len = Length ();
    int   NeedleLength = Needle.Length ();
    
    if ( 0 < Len && 0 < NeedleLength ) 
    {
        String temp ( *this );
        Clear ();
        while ( 0 <= ( pos = temp.Find ( Needle, Start ) ) )
        {
            Append ( temp.SubStr ( Start, pos - Start ) + Replacer );
            Start = pos + NeedleLength;
        }
        Append ( temp.SubStr ( Start, Len - Start ) );
    }
    return * this;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Reverse                                                     )   (
                                                                \___/
  ____________________________________________________________________

*/

String &
String::Reverse ()
{
    int i, j = Length ();
    if ( j )
    {
        if ( IsLargeRep () && 1 < lRefs ( Rep.lBuf.Ptr ) )
            CloneBuffer ();
        
        char * Str = CStr ();
        char   c;

        for ( i = 0; i < Length () / 2; ++i )
        {
            c = Str[i];
            Str[i] = Str[--j];
            Str[j] = c;
        }
    }
    return * this;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    RFind                                                       )   (
                                                                \___/
  ____________________________________________________________________
*/

int
String::RFind ( const String & Needle, int Start ) const
{
    int i, j, k;
    const char * Haystack = CStr ();
    int HaystackLength ( Length () ), NeedleLength ( Needle.Length () );

    // Check string lengthes
    if ( NeedleLength == 0 )
        return 0;
    if ( HaystackLength == 0 )
        return -1;
    if ( NeedleLength == 1 )
    {
        char needle = Needle[0];
        i = Start;
        while ( i < HaystackLength ) 
            if ( Haystack[i] == needle ) return i;
            else --i;
        return -1;
    }
    if ( HaystackLength < NeedleLength )
        return -1;
    // Process representative strings
    int d[256];
    // Init    
    for ( i = 0; i < 256; ++i ) d[i] = NeedleLength;
    for ( i = NeedleLength - 1; 0 <= i; --i )
        d[(int)(Needle[i])] = i;
    // Search
    i = 0 <= Start ? Start : HaystackLength;
    i -= NeedleLength;
    if ( 0 <= i )
    {
        do
        {
            j = -1;
            k = i - 1;
            do
            {
                ++k;
                ++j;
//AfxMessageBox ( ( "Haystack[" + IntToStr ( k ) + "]=" + Haystack[k] + " Needle[" + IntToStr ( j ) + "]=" + Needle[j] ).c_str () );
            }
            while ( j < NeedleLength && Haystack[k] == Needle[j] );
//AfxMessageBox ( ( "d=" + IntToStr ( d[Haystack[i]] ) + " i=" +  IntToStr ( i ) ).c_str () );
            i -= d[ ( unsigned char ) Haystack[i]];
        }
        while ( j < NeedleLength && 0 <= i );
//AfxMessageBox ( ( "!! j=" + IntToStr ( j ) ).c_str () );
        return j == NeedleLength ? k - NeedleLength : -1;
    }
    else
        return -1;
}


/*____________________________________________________________________
                                                                 ___
                                                                /   \
    SetLength                                                   )   (
                                                                \___/
  ____________________________________________________________________
*/

void
String::SetLength ( int newLength )
{
    if ( 0 < newLength && newLength < Length () )
    {
        if ( IsLargeRep () ) 
        {
            lLength ( Rep.lBuf.Ptr ) = newLength;
            lStr    ( Rep.lBuf.Ptr )[newLength] = 0x0;
        }
        else 
            Rep.sBuf[newLength] = 0x0;
    }
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    SubStr                                                      )   (
                                                                \___/
  ____________________________________________________________________
*/

String
String::SubStr ( int StartPos, int Count ) const
{
    if ( ( 0 < Count ) && ( StartPos + Count <= Length () ) )
        return String ( CStr () + StartPos, Count );
    else
        return String ();
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    ToInt                                                       )   (
                                                                \___/
  ____________________________________________________________________
*/


int
String::ToInt () const
{
    int Result ( 0 );

    int Sign ( 1 );
    int Len = Length ();
    const char * Ptr = CStr ();

    int i = 0;
    while ( i < Len && Ptr[i] == ' ' ) ++i;
    if ( Ptr[i] == '-' )
    {
        Sign = -1; 
        ++i;
    }

    while ( i < Len && '0' <= Ptr[i] && Ptr[i] <= '9' )
    {
        Result = Result * 10 + ( Ptr[i] - '0' );
        ++i;
    }
    return Result * Sign;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Trim                                                        )   (
                                                                \___/
  ____________________________________________________________________
*/

String &
String::Trim ()
{
    int Len = Length ();

    if ( 0 < Len )
    {
        int     i (0), j(0);
        char  * Str = CStr ();
        
        // Skip spaces
        i = 0;
        while ( i < Len && Str[i] == ' ' ) ++i;
        j = Len - 1;
        while ( 0 < j && Str[j] == ' ' ) --j;
        if ( j < i )
            Clear ();
        else
        {
            String temp = SubStr ( i, j - i + 1 );
            Clear ();
            CreateFrom ( temp );
        }
    }
    return * this;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    TrimBlanks                                                  )   (
                                                                \___/
  ____________________________________________________________________
*/

String &
String::TrimBlanks  ( const String & BlankSymbols )
{
    int Len ( Length () );

    if ( 0 < Len )
    {
        if ( IsLargeRep () && 1 < lRefs ( Rep.lBuf.Ptr ) )
            CloneBuffer ();

        int     i (0), j(0);
        char  * Str = CStr ();
        
        // Skip spaces
        while ( j < Len )
        {
            if ( 0 <= BlankSymbols.Find ( Str[j], 0 ) )
            {
                if ( 0 < i && Str[i-1] != ' ' )
                {
                    // Collapse blanks into single space
                    Str[i] = ' ';
                    ++i;
                }
            }
            else
            {
                if ( i != j ) Str[i] = Str[j];
                ++i;
            }
            ++j;
        }
        if ( 0 < i && Str[i-1] == ' ' ) --i;
        Str[i] = 0x0;
        if ( IsLargeRep () ) lLength ( Rep.lBuf.Ptr ) = i;
    }
    return * this;
}

/*____________________________________________________________________
                                                                 _  _
                                                                \ \/ /
    String destructor                                            )  (
                                                                /_/\_\
  ____________________________________________________________________
*/

String::~String()
{
    Clear ();
}

String operator + ( const String & String1, const String & String2 )
{
    String Str ( String1 );
    return Str.Append ( String2 );
}

String operator + ( const char * String1, const String & String2 )
{
    String temp ( String1 );
    return temp.Append ( String2 );
}

String operator + ( char Char, const String & String2 )
{
    String temp ( Char );
    return temp.Append ( String2 );
}

bool operator == ( const char * String1, const String & String2 )
{
    return String ( String1 ) == String2;
}


bool operator != ( const char * String1, const String & String2 )
{
    return String ( String1 ) != String2;
}

String
String::FromInt ( int Number )
{
    if ( Number != 0 )
    {
        int  sign = 0;
        char digit[2];

        digit[1] = 0x0;
        String s;
        if ( Number < 0 )
        {
            sign = -1;
            Number = -Number;
        }
        while ( 0 != Number )
        {
            digit[0] = Number % 10 + '0';
            s.Append ( digit );
            Number /= 10;
        }
        if ( sign ) s.Append ( "-" );
        return s.Reverse ();
    }
    return "0";
}
