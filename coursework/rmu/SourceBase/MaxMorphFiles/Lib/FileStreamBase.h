/*____________________________________________________________________

  Project:

      File stream manipulation utils
      ______________________________

  (C) Maxim Proskurnya,                           2002--2003, AMC, MSU
      max@axofiber.org.ru

  Component : FileStream
  Comments  : Declares a stream base interface
  ____________________________________________________________________
*/

/*____________________________________________________________________

                                                                    /
    IStreamBase class definition                                \  /
                                                                 \/
  ____________________________________________________________________
*/

#ifndef STREAM_BASE_2002
#define STREAM_BASE_2002

const int STREAM_BUFFER_SIZE = 65536;

class IStreamBase {
public:
    enum EStreamState { STREAM_OK,
                        STREAM_END,
                        STREAM_IO_ERROR };
    char EndL;
    // Interface
    virtual void Close() = 0;
    EStreamState GetState() { return State; }
    virtual int Read(void* ToBuffer, int Count) = 0;
    virtual bool ReadLine(String& Line) = 0;
    virtual int Write(const void* FromBuffer, int Count) = 0;
    bool WriteLine(const String& Line) {
        if (Write(Line, Line.Length()) != -1)
            if (Write(&EndL, 1) != -1)
                return true;
        return false;
    }
    void SetEOLsymbol(char EOL) { EndL = EOL; };
    void SetInput(IStreamBase* InputStream) {
        Input = InputStream;
    };
    void SetOutput(IStreamBase* OutputStream) {
        Output = OutputStream;
    };

    IStreamBase() :
// GIG: 22.01.2013 - porting to UNIX
#ifdef _WIN32
                    EndL('\n'),
#else
                    EndL('\r'),
#endif
                    State(STREAM_OK),
                    Start(0),
                    End(0),
                    Input(NULL),
                    Output(NULL){};
protected:
    EStreamState State;
    int Start;
    int End;
    IStreamBase* Input;
    IStreamBase* Output;
    char StreamBuffer[STREAM_BUFFER_SIZE];
};

#endif
