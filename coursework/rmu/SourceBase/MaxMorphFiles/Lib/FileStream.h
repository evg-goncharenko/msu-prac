/*____________________________________________________________________

  Project:

      File stream manipulation utils
      ______________________________

  (C) Maxim Proskurnya,                           2002--2003, AMC, MSU
      max@axofiber.org.ru

  Component : FileStream
  Comments  : Implements file streaming and filtering: ZIP, XML.
  ____________________________________________________________________
*/

#ifndef FILESTREAM_H_2002
#define FILESTREAM_H_2002

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#define _lseek lseek
#define _write write
#define _read read
#define _unlink unlink
#define _close close
#define _open open
#endif

#include "FileStreamBase.h"
#include "Xception.h"
#include "Xml.h"
#include "bzlib.h"

/*____________________________________________________________________

                                                                    /
    FileStream class definition                                 \  /
                                                                 \/
  ____________________________________________________________________
*/

class FileStream : public IStreamBase {
public:
    enum EFileStream { NEXISTS,
                       LOCKED };
    static const int modeAppend;
    static const int modeBinary;
    static const int modeCreate;
    static const int modeExclude;
    static const int modeRead;
    static const int modeReadWrite;
    static const int modeWrite;
    static const int modeTruncate;

    bool Open(const String& FileName, int Rights);
    void Close();
    bool EoF() { return State == STREAM_END; }
    void Flush();
    int GetFileSize();
    static bool GetFileSize(const String& FileName, int& FileSize);
    int Read(void* ToBuffer, int Count);
    bool ReadLine(String& Line);
    inline void WriteChar(char c);
    inline int Write(const void* FromBuffer, int Count);
    inline void Write(const String& Str);
    inline void WriteLine(const String& Line);
    void Seek(int Pos, int Origin);
    void SetInput(IStreamBase* InputStream) {}
    void SetOutput(IStreamBase* OutputStream) {}

    FileStream();
    FileStream(const String& FileName, int Rights);
    virtual ~FileStream();

protected:
    int fd;
    int FileRights;
    inline bool ReadBuffer();
};

inline bool
FileStream::ReadBuffer() {
    int Count = _read(fd, StreamBuffer, STREAM_BUFFER_SIZE);
    if (0 < Count) {
        Start = 0;
        End = Count;
        State = STREAM_OK;
        return true;
    } else
        State = STREAM_END;
    return false;
}

inline void
FileStream::Write(const String& Str) {
    if (FileRights & modeAppend)
        _lseek(fd, 0, SEEK_END);
    if (FileRights == modeReadWrite) {
        if (Start != End) {
            _lseek(fd, Start - End, SEEK_CUR);
            Start = 0;
            End = 0;
        }
    }
    _write(fd, Str, Str.Length());
}

inline int
FileStream::Write(const void* FromBuffer, int Count) {
    int nWrote = 0;

    if (FromBuffer) {
        nWrote = _write(fd, FromBuffer, Count);
    }
    return nWrote;
}

inline void
FileStream::WriteChar(char c) {
    if (FileRights == modeReadWrite) {
        if (Start != End) {
            _lseek(fd, Start - End, SEEK_CUR);
            Start = 0;
            End = 0;
        }
    }
    _write(fd, &c, 1);
}

inline void
FileStream::WriteLine(const String& Str) {
    if (FileRights == modeReadWrite) {
        if (Start != End) {
            _lseek(fd, Start - End, SEEK_CUR);
            Start = 0;
            End = 0;
        }
    }
    _write(fd, Str, Str.Length());
    _write(fd, &EndL, 1);
}

#include "StreamXORFilter.inc.h"

/*____________________________________________________________________

                                                                    /
    StreamXMLFilter class definition                            \  /
                                                                 \/
  ____________________________________________________________________
*/

class StreamXMLFilter : public IStreamBase {
public:
    // Interface
    void Close(){};
    int Read(void* ToBuffer, int Count) { return 0; };
    bool ReadLine(String& Line) { return false; };
    int Write(const void* FromBuffer, int Count) { return 0; };
    XMLElement* ReadElement() { return XMLElement::ReadFrom(Input); }
    void SetInput(IStreamBase* InputStream) {
        Input = InputStream;
    };
    void SetOutput(IStreamBase* OutputStream){};
    StreamXMLFilter() { LastChar = -1; }

protected:
    // Attributes
    int LastChar;
};

/*____________________________________________________________________

                                                                    /
    StreamZipFilter class definition                            \  /
                                                                 \/
  ____________________________________________________________________
*/

class StreamZipFilter : public IStreamBase {
public:
    // Life-cycle
    StreamZipFilter();
    virtual ~StreamZipFilter();
    // Interface
    void Close();
    int Read(void* ToBuffer, int Count);
    bool ReadLine(String& Line);
    int Write(const void* FromBuffer, int Count);
    void SetInput(IStreamBase* InputStream);
    void SetOutput(IStreamBase* OutputStream);

protected:
    // Attributes
    char* OutCursor;
    int Res;
    bz_stream Stream;
    char Buffer[STREAM_BUFFER_SIZE];
};

#endif
