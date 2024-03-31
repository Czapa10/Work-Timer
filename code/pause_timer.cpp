#include "shared.h"

int main()
{
    FreeConsole();
    
    auto File = OpenFile("save.txt", io_mode::ReadWrite);
    RAssert(File, "Could not read \"save.txt\" file!\nWin32 error code: %", GetSystemErrorCode());
    u32 FileSize = GetFileSize(File);
    auto EndedOn = WhatSaveFileEndedOn(File, FileSize);
    
    switch(EndedOn)
    {
        case Nothing:
        {
            ShowInfoMessageBoxAndCloseApp("You have to create new timer before you can pause it!\n"
                                          "(currently the save.txt is empty)");
        } break;
        
        case End:
        {
            ShowInfoMessageBoxAndCloseApp("The timer was already paused! No need to pause it twice.");
        } break;
    }
    
    WriteString(File, FileSize, "\ne:%", ToString(GetLocalTime()));
}
