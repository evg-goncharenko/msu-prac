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
*/

#ifndef STRING_H_2003
#define STRING_H_2003

// GIG: change from #include <string.h> to <cstring>
#include <cstring>
using namespace std;

/*____________________________________________________________________

                                                                    /
    String class definition                                     \  /
                                                                 \/
  ____________________________________________________________________
*/

class String;

class String {
public:
    // Life-cycle
    String() {
        Rep.lBuf.Low = 0;
        Rep.lBuf.Ptr = NULL;
    }
    String(char c) {
        Rep.sBuf[0] = c;
        Rep.sBuf[1] = 0x0;
        Rep.lBuf.Ptr = 0x0;
    }
    explicit String(int Number) { CreateFrom(Number); }
    String(const char *NullTerminatedBody);
    String(const char *Body, int NewLength) { CreateFrom(Body, NewLength); }
    String(const String &NewString) { CreateFrom(NewString); }
    ~String();
    // Interface
    String &Append(const String &NewString);
    char &At(int Index) { return (*this)[Index]; }
    char At(int Index) const { return (*this)[Index]; }
    String &Clear();
    const char *c_str() const { return CStr(); }
    int Compare(const String &NewString) const;
    String &Erase(int Start, int Size);
    bool IsEmpty() const {
        if (IsLargeRep())
            return (lLength(Rep.lBuf.Ptr) == 0x0);
        else
            return Rep.sBuf[0] == 0x0;
    }
    int Length() const {
        if (IsLargeRep())
            return lLength(Rep.lBuf.Ptr);
        else
            return strlen(Rep.sBuf);
    }
    int Find(const char Needle, int Start = 0) const;
    int Find(const String &Needle, int Start = 0) const;
    static String FromInt(int Number);
    int ToInt() const;
    void SetLength(int newLength);
    String SubStr(int StartPos, int Count) const;
    String &Trim();
    String &TrimBlanks(const String &BlankSymbols = DefaultBlankSymbols);
    String TrimBlanksCopy(const String &BlankSymbols = DefaultBlankSymbols) const {
        String temp(*this);
        return temp.TrimBlanks(BlankSymbols);
    }
    String TrimCopy() const { return String(*this).Trim(); }
    String &Reverse();
    String ReverseCopy() const { return String(*this).Reverse(); }
    String &Replace(const String &Needle, const String &Replacer, int Start = 0);
    String &ReplaceAll(const String &Needle, const String &Replacer, int Start = 0);
    int RFind(const String &Needle, int Start) const;
    operator const char *() const { return c_str(); };
    operator char *() { return CStr(); };

    String &operator=(const String &NewString) {
        if (&NewString == this)
            // Preserve self copy
            return *this;
        else {
            Clear();
            CreateFrom(NewString);
            return *this;
        }
    }
    String &operator=(const char c) {
        Clear();
        Rep.sBuf[0] = c;
        Rep.sBuf[1] = 0;
        return *this;
    }
    String operator+(const String &NewString) {
        String temp(*this);
        return temp.Append(NewString);
    }
    String &operator+=(const char c) { return Append(c); }
    String &operator+=(const String &NewString) { return Append(NewString); }
    bool operator==(const String &NewString) const;
    bool operator==(const char *CharBuffer) const { return strcmp(CStr(), CharBuffer) == 0; }
    bool operator==(const char c) const {
        if (IsLargeRep())
            return lLength(Rep.lBuf.Ptr) == 1 && lStr(Rep.lBuf.Ptr)[0] == c;
        else
            return (Rep.sBuf[0] == c) && (Rep.sBuf[1] == 0x0);
    }
    bool operator!=(const String &NewString) const;
    bool operator!=(const char *CharBuffer) const { return strcmp(CStr(), CharBuffer) != 0; }
    bool operator!=(char c) const {
        if (IsLargeRep())
            return lLength(Rep.lBuf.Ptr) != 1 || lStr(Rep.lBuf.Ptr)[0] != c;
        else
            return (Rep.sBuf[0] != c) || (Rep.sBuf[1] != 0x0);
    }
    bool operator<(const String &NewString) const { return Compare(NewString) < 0; }
    bool operator<(const char *CharBuffer) const { return strcmp(CStr(), CharBuffer) < 0; }
    bool operator>(const String &NewString) const { return 0 < Compare(NewString); }
    bool operator>(const char *CharBuffer) const { return 0 < strcmp(CStr(), CharBuffer); }
    bool operator<=(const String &NewString) const { return Compare(NewString) <= 0; }
    bool operator<=(const char *CharBuffer) const { return strcmp(CStr(), CharBuffer) <= 0; }
    bool operator>=(const String &NewString) const { return 0 <= Compare(NewString); }
    bool operator>=(const char *CharBuffer) const { return 0 <= strcmp(CStr(), CharBuffer); }
    char &operator[](int Index);
    char operator[](int Index) const;
    // Atributes
    static const String DefaultBlankSymbols;

private:
    /*______________________________________________________
              GIG-28.02.2018 - representation changed to accomodate both 32- and64-bit architectures

              Small strings (upto N chars) represented in short form
                  sBuf -> short : array of N+1 chars
              Large strings (over N chars) have another view
                  lBuf -> long strings : Low (fictive N/2 bytes) - 4 or 8 bytes depending on architecture
                                Ptr ---> Reference counter 
                                         String length
                                         Buffer size
                                         Null-terminated char array
              ______________________________________________________
            */
    struct C {
        // Buffer view for large strings
        long Low;  // GIG - rely on the fact that sizeof (long) == sizeof (void*)
        char *Ptr;
    };
    static const int MaxShortLen = sizeof(struct C) - 1;

    union Container {
        // Buffer alternative views
        char sBuf[MaxShortLen + 1];
        C lBuf;
    };

    // Attributes
    Container Rep;

    // Operations
    void CloneBuffer();
    void CollapseBuffer();
    void CreateFrom(int Number);
    void CreateFrom(const char *Body, int NewLength);
    void CreateFrom(const String &NewString);
    char *CStr() {
        if (IsLargeRep())
            return lStr(Rep.lBuf.Ptr);
        else
            return Rep.sBuf;
    }
    const char *CStr() const {
        if (IsLargeRep())
            return lStr(Rep.lBuf.Ptr);
        else
            return Rep.sBuf;
    }
    void ExpandBuffer();
    bool IsLargeRep() const { return (Rep.lBuf.Low == 0 && Rep.lBuf.Ptr); };
    int &lRefs(char *Ptr) { return (reinterpret_cast<int *>(Ptr))[0]; }
    int lRefs(char *Ptr) const { return (reinterpret_cast<int *>(Ptr))[0]; }
    int &lLength(char *Ptr) { return (reinterpret_cast<int *>(Ptr))[1]; }
    int lLength(char *Ptr) const { return (reinterpret_cast<int *>(Ptr))[1]; }
    int &lSize(char *Ptr) { return (reinterpret_cast<int *>(Ptr))[2]; }
    int lSize(char *Ptr) const { return (reinterpret_cast<int *>(Ptr))[2]; }
    char *lStr(char *Ptr) { return (Ptr + 3 * sizeof(int) / sizeof(char)); }
    const char *lStr(char *Ptr) const { return (Ptr + 3 * sizeof(int) / sizeof(char)); }
    void ReleasePtr(char *Ptr) {
        if (--lRefs(Ptr) <= 0) {
            delete[] Ptr;
        }
    }
};

extern String operator+(const String &, const String &);
extern String operator+(const char *, const String &);
extern String operator+(char, const String &);
extern bool operator==(const char *String1, const String &String2);
extern bool operator!=(const char *String1, const String &String2);

#endif
