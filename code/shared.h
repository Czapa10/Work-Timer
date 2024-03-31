#include "my_types_and_defines.h"

#define rstd_Implementation
#define rstd_Debug 1
#include "rstd_remove_macro_prefix.h"
#include "rstd.h"

using namespace rstd;
using namespace rstd::memory_size_literals;

fn ShowInfoMessageBoxAndCloseApp
(const char* Message)
{ 
    MessageBoxA(0, Message, "", MB_ICONINFORMATION|MB_OK);
    ExitProcess(0);
}

enum ended_on
{ Nothing, Start, End };

fn WhatSaveFileEndedOn
(file File, u32 FileSize)
{
    auto Arena = AllocateArenaZero(4_MB);
    char* FileContent = (char*)PushSizeUninitialized(Arena, FileSize);
    u32 ReadBytes = Read(FileContent, File, 0, FileSize);
    RAssert(ReadBytes == FileSize, "Failed to read save.txt! (although the file was opened successfully.)");
    ended_on EndedOn = Nothing;
    while(*FileContent)
    {
        if(*FileContent == 's')
            EndedOn = Start;
        else if(*FileContent == 'e')
            EndedOn = End;
        else
            RInvalidCodePath("save.txt is corrupted!");
        
        while(*FileContent != '\n' && *FileContent != 0)
            ++FileContent;
        ++FileContent;
    }
    return EndedOn;
}