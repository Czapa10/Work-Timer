#include "shared.h"

int main
(i32 ArgumentCount, char** Arguments)
{
    FreeConsole();
    
    RAssert(ArgumentCount == 2, "You have to pass a signle numer of minutes argument");
    u32 BreakMinutes = StringToU32(Arguments[1]);
    
    auto Arena = AllocateArenaZero(4_MB);
    char* FileContent = ReadWholeFile(Arena, "save.txt");
    RAssert(FileContent, "Could not read \"save.txt\" file!\nWin32 error code: %", GetSystemErrorCode());
    
    time LastEntryTime;
    ended_on LastEntryType = Nothing;
    
    while(*FileContent)
    {
        if(*FileContent == 's')
        {
            LastEntryType = Start;
            FileContent += strlen("s:");
        }
        else if(*FileContent == 'e')
        {
            LastEntryType = End;
            FileContent += strlen("e:");
        }
        LastEntryTime = ReadTime(&FileContent);
        
        ++FileContent;
    }
    
    auto FileStreamOut = OpenFileStream("save.txt", io_mode::ReadWrite);
    FileStreamOut.Pos = GetFileSize(FileStreamOut.File);
    switch(LastEntryType)
    {
        case Start:
        {
            auto CurrentTime = GetLocalTime();
            auto BreakStartTime = CurrentTime;
            if(BreakStartTime.Minute >= BreakMinutes)
            {
                BreakStartTime.Minute -= BreakMinutes;
            }
            else
            {
                if(BreakStartTime.Hour == 0)
                {
                    BreakStartTime.Day--;
                    BreakStartTime.Hour = 23;
                }
                else
                {
                    BreakStartTime.Hour--;
                }
                BreakStartTime.Minute = 60 + BreakStartTime.Minute - BreakMinutes;
            }
            WriteString(FileStreamOut, "\ne:%", ToString(BreakStartTime));
            WriteString(FileStreamOut, "\ns:%", ToString(CurrentTime));
        } break;
        
        case End:
        {
            auto BreakEndTime = LastEntryTime;
            if(BreakEndTime.Minute + BreakMinutes < 60)
            {
                BreakEndTime.Minute += BreakMinutes;
            }
            else
            {
                if(BreakEndTime.Hour == 23)
                {
                    BreakEndTime.Day++;
                    BreakEndTime.Hour = 0;
                }
                else
                {                    
                    BreakEndTime.Hour++;
                }
                BreakEndTime.Minute = 60 - BreakEndTime.Minute + BreakMinutes;
            }
            WriteString(FileStreamOut, "\ns:%", ToString(BreakEndTime));
        } break;
    }
}
