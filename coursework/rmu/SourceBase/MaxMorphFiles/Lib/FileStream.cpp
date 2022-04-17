/*____________________________________________________________________

  Project:

      File stream manipulation utils
      ______________________________

  (C) Maxim Proskurnya,                           2002--2003, AMC, MSU
      max@axofiber.org.ru,   +7 916 694 8709

  Component : FileStream
  Comments  : Implements file streaming and filtering: ZIP, XML.
  ____________________________________________________________________

  28.X.2003                                                      01.09
      Updated "StreamXMLFilter" class according to the new XML 
      implementation. The "IStreamBase" definition moved to the 
      "FileStreamBase.h"
  22.VI.2003                                                     01.08
      Fixed stream state maintenace.
  20.VI.2003                                                     01.07
      Project refactored into "FileStream". Compile tests on
      MS VC++ and GCC.
  05.VI.2003                                                     01.06
      Fixed "FileStream::ReadLine" method.
  22.V.2003                                                      01.05
      Fixed "CRMStreamXMLFilter::ReadElement" method to proceed
      attributes with inline quotes and unquoted attrs correctly.
  12.XI.2002                                                     01.04
      Added "Stream" and "Filter" concepts: "CRMStreamZipFilter",
      "CRMStreamXMLFilter", "CRMStreamXorFilter". Updated
      load/save methods.
  07.XI.2002                                                     01.03
      Fixed "CFileStream" class to eliminate "ReadWrite" problems
      caused by buffered input.
  09.VI.2002                                                     00.02
      Added "CXMLFileStream" class.
  14.II.2002                                                     00.01
      Fixed "FileStream" class "file open permissions" issue.
  ____________________________________________________________________
*/
#include "FileStream.h"
#include <fcntl.h>
#include <sys/stat.h>
#include "stdafx.h"

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    CFileStream methods                                         )   (
                                                                \___/
  ____________________________________________________________________
*/

const int FileStream::modeAppend = O_APPEND;
const int FileStream::modeCreate = O_CREAT;
const int FileStream::modeExclude = O_EXCL;
const int FileStream::modeRead = O_RDONLY;
const int FileStream::modeReadWrite = O_RDWR;
const int FileStream::modeWrite = O_WRONLY;
const int FileStream::modeTruncate = O_TRUNC;
#ifdef _WIN32
const int FileStream::modeBinary = O_BINARY;
#else
const int FileStream::modeBinary = 0;
#endif

FileStream::FileStream() : fd(-1) {}

FileStream::FileStream(const String& FileName, int Rights) {
    fd = _open(FileName, Rights, S_IREAD | S_IWRITE);

    FileRights = Rights;
    if (fd == -1) State = STREAM_IO_ERROR;
}

bool FileStream::Open(const String& FileName, int Rights) {
    fd = _open(FileName, Rights, S_IREAD | S_IWRITE);
    FileRights = Rights;
    Start = 0;
    End = 0;
    if (fd == -1) State = STREAM_IO_ERROR;
    return fd != -1;
}

void FileStream::Close() {
    if (fd != -1) {
        _close(fd);
        fd = -1;
        State = STREAM_OK;
    }
}

void FileStream::Flush() {
#ifdef _WIN32
    _commit(fd);
#endif
}

int FileStream::GetFileSize() {
    int Result = 0;
    struct stat buffer;

    if (!fstat(fd, &buffer))
        Result = buffer.st_size;
    return Result;
}

bool FileStream::GetFileSize(const String& FileName, int& FileSize) {
    bool Result;
    struct stat buffer;

    if ((Result = !stat(FileName, &buffer)))
        FileSize = buffer.st_size;
    return Result;
}

int FileStream::Read(void* ToBuffer, int Count) {
    int Length;
    int nRead = 0;
    bool CanRead = true;

    if (0 < Count) {
        if (ToBuffer) {
            while (CanRead && nRead < Count) {
                if (Start >= End) CanRead = ReadBuffer();
                if (CanRead) {
                    Length = End - Start < Count - nRead ? End - Start : Count - nRead;
                    memcpy((char*)(ToBuffer) + nRead, StreamBuffer + Start, Length);
                    nRead += Length;
                    Start += Length;
                }
            }
            if (0 < nRead)
                State = STREAM_OK;
            else
                State = STREAM_END;
        }
    }
    return nRead;
}

bool FileStream::ReadLine(String& Line) {
    int i;
    bool CanRead = true;
    bool Result = false;

    Line.Clear();
    State = STREAM_OK;
    while (CanRead && !Result) {
        if (End <= Start) CanRead = ReadBuffer();
        if (CanRead) {
            // Find end of line symbol
            i = Start;
            while (i < End && StreamBuffer[i] != EndL) ++i;
            if (i < End && StreamBuffer[i] == EndL) Result = true;
            // Store line fragment
            if (Start < i) Line += String(StreamBuffer + Start, i - Start);
// GIG: 22.01.2013 - porting to UNIX
#ifdef _WIN32
            Start = i + 1;
#else
            Start = i + 2;  // skip /r/n
#endif
        } else {
            // EOF is reached, so check the line length
            if (!Line.IsEmpty()) Result = true;
            State = STREAM_END;
        }
    }
    return Result;
}

void FileStream::Seek(int Pos, int Origin) {
    _lseek(fd, Start - End + Pos, Origin);
    Start = 0;
    End = 0;
}

/*____________________________________________________________________
                                                                 _  _
                                                                \ \/ /
    CFileStream destructor                                       )  (
                                                                /_/\_\
  ____________________________________________________________________
*/

FileStream::~FileStream() {
    if (fd != -1) _close(fd);
}

#include "StreamXORFilter.inc.cpp"

/*____________________________________________________________________
                                                                  _
                                                                _| |_
    StreamZipFilter constructor                                |_   _|
                                                                 |_|
  ____________________________________________________________________
*/

StreamZipFilter::StreamZipFilter() : ::IStreamBase() {
    Stream.bzalloc = NULL;
    Stream.bzfree = NULL;
    Stream.opaque = NULL;
    OutCursor = NULL;
    Res = 0;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Read                                                        )   (
                                                                \___/
  ____________________________________________________________________
*/

int StreamZipFilter::Read(void* ToBuffer, int Count) {
    int nRead = 0;
    int Length;

    if (Input) {
        while (State == STREAM_OK && nRead < Count) {
            if (Stream.next_out - OutCursor == 0) {
                if (Res == BZ_STREAM_END)
                    State = STREAM_END;
                else {
                    // Decompress new portion
                    if (Stream.avail_out <= 2) {
                        // Reset out buffer
                        Stream.avail_out = STREAM_BUFFER_SIZE;
                        Stream.next_out = Buffer;
                        OutCursor = Buffer;
                    }
                    if (Stream.avail_in == 0 && Input->GetState() != STREAM_END) {
                        // Reset in buffer
                        Stream.next_in = StreamBuffer;
                        Stream.avail_in = Input->Read(StreamBuffer, STREAM_BUFFER_SIZE);
                    }
                    Res = BZ2_bzDecompress(&Stream);
                    if (Input->GetState() != STREAM_OK)
                        State = Input->GetState();
                }
            }
            Length = Count < Stream.next_out - OutCursor ? Count : Stream.next_out - OutCursor;
            if (0 < Length) {
                // Copy decompressed portion to the client buffer
                memcpy((char*)ToBuffer + nRead, OutCursor, Length);
                OutCursor += Length;
                nRead += Length;
            }
        }
    }
    return nRead;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    ReadLine                                                    )   (
                                                                \___/
  ____________________________________________________________________
*/

bool StreamZipFilter::ReadLine(String& Line) {
    char c;

    if (State != STREAM_OK)
        return false;
    Line.Clear();
    Read(&c, 1);
    while (State == STREAM_OK && c != EndL) {
        Line += c;
        Read(&c, 1);
    }
    return true;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Write                                                       )   (
                                                                \___/
  ____________________________________________________________________
*/

int StreamZipFilter::Write(const void* FromBuffer, int Count) {
    int nWrote = 0;
    int Length;
    int Res;

    while (State == STREAM_OK && nWrote < Count) {
        if (Stream.avail_in == 0) {
            Length = Count - nWrote < STREAM_BUFFER_SIZE ? Count - nWrote : STREAM_BUFFER_SIZE;
            memcpy(StreamBuffer, FromBuffer, Length);
            Stream.avail_in = Length;
            Stream.next_in = StreamBuffer;
            nWrote += Length;
        }
        Res = BZ2_bzCompress(&Stream, BZ_RUN);
        if (Res != BZ_RUN_OK)
            State = STREAM_IO_ERROR;
        else if (Buffer < Stream.next_out) {
            // Write buffer
            Output->Write(Buffer, Stream.next_out - Buffer);
            // Reset buffer
            Stream.avail_out = STREAM_BUFFER_SIZE;
            Stream.next_out = Buffer;
            if (Output->GetState() != STREAM_OK)
                State = Output->GetState();
        }
    }
    return nWrote;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    SetInput                                                    )   (
                                                                \___/
  ____________________________________________________________________
*/

void StreamZipFilter::SetInput(IStreamBase* InputStream) {
    int Res;
    if (!Input) {
        Input = InputStream;
        Res = BZ2_bzDecompressInit(&Stream, 0, 0);
        if (Res != BZ_OK) {
            Input = NULL;
            throw new Xception(Xception::X_FILE_IO, "Can't init bzip2 decompression. Possible reason: corrupted libbzip2 or unsufficient memory",
                               "SetInput", "StreamZipFilter", "FileStream");
        } else {
            Stream.avail_in = 0;
            Stream.next_in = StreamBuffer;
            Stream.avail_out = STREAM_BUFFER_SIZE;
            Stream.next_out = Buffer;
            OutCursor = Buffer;
        }
    }
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    SetOutput                                                   )   (
                                                                \___/
  ____________________________________________________________________
*/

void StreamZipFilter::SetOutput(IStreamBase* OutputStream) {
    int Res;
    if (!Output) {
        Output = OutputStream;
        Res = BZ2_bzCompressInit(&Stream, 9, 0, 30);
        if (Res != BZ_OK) {
            Output = NULL;
            throw new Xception(Xception::X_FILE_IO, "Can't init bzip2 compression. Possible reason: corrupted libbzip2 or unsufficient memory",
                               "SetOuput", "StreamZipFilter", "FileStream");
        } else {
            Stream.avail_in = 0;
            Stream.next_in = StreamBuffer;
            Stream.avail_out = STREAM_BUFFER_SIZE;
            Stream.next_out = Buffer;
        }
    }
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Close                                                       )   (
                                                                \___/
  ____________________________________________________________________
*/

void StreamZipFilter::Close() {
    int Res;
    bool notFinished;

    if (Input) {
        BZ2_bzDecompressEnd(&Stream);
        Input = NULL;
        State = STREAM_OK;
    } else if (Output) {
        notFinished = true;
        // Finish the stream
        while (notFinished && State == STREAM_OK) {
            Res = BZ2_bzCompress(&Stream, BZ_FINISH);

            if (Res == BZ_STREAM_END)
                notFinished = false;
            else if (Res != BZ_FINISH_OK)
                throw new Xception(Xception::X_FILE_IO, "Can't finilize bzip2 decompression. Possible reason: corrupted libbzip2 or unsufficient memory",
                                   "Close", "StreamZipFilter", "FileStream");
            if (Buffer < Stream.next_out) {
                // Write buffer
                Output->Write(Buffer, Stream.next_out - Buffer);
                // Reset buffer
                Stream.avail_out = STREAM_BUFFER_SIZE;
                Stream.next_out = Buffer;
                if (Output->GetState() != STREAM_OK)
                    State = Output->GetState();
            }
        }
        BZ2_bzCompressEnd(&Stream);
        Output = NULL;
        State = STREAM_OK;
    }
}

/*____________________________________________________________________
                                                                 _  _
                                                                \ \/ /
    StreamZipFilter destructor                                   )  (
                                                                /_/\_\
  ____________________________________________________________________
*/

StreamZipFilter::~StreamZipFilter() {
    Close();
}
