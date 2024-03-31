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
            ShowInfoMessageBoxAndCloseApp("You have to create new timer before you can start it!\n"
                                          "(currently the save.txt is empty)");
        } break;
        
        case Start:
        {
            ShowInfoMessageBoxAndCloseApp("The timer was already started! No need to start it twice.");
        } break;
    }
    
    WriteString(File, FileSize, "\ns:%", ToString(GetLocalTime()));
}
