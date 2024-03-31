#include "shared.h"

int main()
{
    FreeConsole();
    
    CopyFile("last_save.txt", "save.txt", OverrideFileIfFileWithNewPathExists);
    
    auto FileStream = OpenFileStream("save.txt", io_mode::Write);
    RAssert(FileStream, "Could not read \"save.txt\" file!\nWin32 error code: %", GetSystemErrorCode());
    WriteString(FileStream, "s:%", ToString(GetLocalTime()));
    Close(FileStream);
}

