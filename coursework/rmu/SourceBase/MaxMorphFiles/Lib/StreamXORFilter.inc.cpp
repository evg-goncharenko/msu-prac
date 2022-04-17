/*____________________________________________________________________
                                                                  _
                                                                _| |_
    StreamXORFilter constructor                                |_   _|
                                                                 |_|
  ____________________________________________________________________
*/

const unsigned int StreamXORFilter::Cypher[64] =
    {

        0x5338BFC9, 0xB5DA7F07, 0x8476FD14, 0x1CD29BB5, 0xC5CCAFAA, 0xA2AEB797, 0x428663A6, 0x7E1B9165,
        0x43EA1B61, 0xBB341848, 0x87F94DB6, 0x6D1C4411, 0x48D7C0F7, 0x76555F74, 0x656D5131, 0x538F9A32,
        0xAA060B59, 0xED64BA9B, 0x018D0A60, 0x90A7DCDC, 0x0A38147D, 0x4FECB23A, 0x6BCBA385, 0x25229756,
        0x1636C810, 0x107E6995, 0x3C3B42AE, 0x99051941, 0x555E06EC, 0xB76D12C6, 0x266A42A4, 0xD68F19C6,
        0x14B5052E, 0x4694A87B, 0x44BAA3A8, 0xDBD25A76, 0x3D4A91E4, 0x83464F72, 0xFF347F36, 0x4685BD9C,
        0x7B0F04D9, 0x90E5FE2E, 0x59EE1954, 0x7966EF68, 0x159ADA6C, 0x0C0B84F8, 0x9F0377AE, 0xEDAFF034,
        0x4926ECDD, 0xE9B4017B, 0x3FFA9ECA, 0xF6DFB543, 0x2CEED1CA, 0x92D50370, 0x0D7CA577, 0x801B0449,
        0x3EC06936, 0xDA98CF9D, 0x83045F90, 0xA8237660, 0x61C73696, 0x1A228D99, 0xB66BEE61, 0x20FBDE73};

/*____________________________________________________________________
                                                                  _
                                                                _| |_
    StreamXORFilter constructor                                |_   _|
                                                                 |_|
  ____________________________________________________________________
*/

StreamXORFilter::StreamXORFilter() : Cursor(0) {
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Read                                                        )   (
                                                                \___/
  ____________________________________________________________________
*/

int StreamXORFilter::Read(void* ToBuffer, int Count) {
    int nRead = 0;

    nRead = Input->Read(ToBuffer, Count);
    if (Input->GetState() != STREAM_OK)
        State = Input->GetState();
    for (int i = 0; i < nRead; ++i) {
        if (Cursor > 255) Cursor = 0;
        ((char*)ToBuffer)[i] = ((char*)ToBuffer)[i] ^ ((char*)Cypher)[Cursor];
        ++Cursor;
    }

    return nRead;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Write                                                       )   (
                                                                \___/
  ____________________________________________________________________
*/

int StreamXORFilter::Write(const void* FromBuffer, int Count) {
    int nWrote = 0;
    int Length = 0;

    while (State == STREAM_OK && nWrote < Count) {
        Length = STREAM_BUFFER_SIZE < (Count - nWrote) ? STREAM_BUFFER_SIZE : (Count - nWrote);
        for (int i = 0; i < Length; ++i) {
            if (Cursor > 255) Cursor = 0;
            StreamBuffer[i] = ((char*)FromBuffer)[i + nWrote] ^ ((char*)Cypher)[Cursor];
            ++Cursor;
        }
        nWrote += Output->Write(StreamBuffer, Length);
        if (Output->GetState() != STREAM_OK)
            State = Output->GetState();
    }
    return nWrote;
}

/*____________________________________________________________________
                                                                 ___
                                                                /   \
    Close                                                       )   (
                                                                \___/
  ____________________________________________________________________
*/

void StreamXORFilter::Close() {
    Cursor = 0;
};
