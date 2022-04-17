/*____________________________________________________________________

                                                                    /
    StreamXORFilter class definition                            \  /
                                                                 \/
  ____________________________________________________________________
*/

class StreamXORFilter : public IStreamBase {
public:
    // Life-cycle
    StreamXORFilter();
    virtual ~StreamXORFilter() {}
    // Interface
    void Close();
    //            int           Pos             () const { if ( Input ) return Input->Pos (); else return 0; }
    int Read(void* ToBuffer, int Count);
    bool ReadLine(String& Line) { return false; };
    void Seek(int pos, int origin) { throw new Xception(Xception::X_FILE_IO, "Failed to seek. Operation is not supported", "Seek", "StreamXORFilter", "__FILE__"); }
    int Write(const void* FromBuffer, int Count);

protected:
    // Attributes
    int Cursor;
    static const unsigned int Cypher[64];
};
