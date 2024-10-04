#include "shared.h"

fn IsLeapYear
(u32 Year)
{
    if(Year % 4 == 0)
    {
        if(Year % 100 == 0)
            return Year % 400 == 0;
        else
            return true;
    }
    return false;
}

fn GetNumberOfDaysInMonth
(u32 Month, u32 Year)
{
    u32 DaysInMonth;
    switch(Month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
        DaysInMonth = 31;
        break;
        
        case 4:
        case 6:
        case 9:
        case 11:
        DaysInMonth = 30;
        break;
        
        case 2: DaysInMonth = IsLeapYear(Year) ? 29 : 28; break;
        
        InvalidDefaultCase;
    }
    
    return DaysInMonth;
}

constexpr u32 DaysPerWeek = 7;
constexpr u32 SecondsPerMinute = 60;
constexpr u32 SecondsPerHour = 60 * 60;
constexpr u32 SecondsPerDay = 24 * SecondsPerHour;
constexpr u32 DaysPerYear = 365;
constexpr u32 SecondsPerYear = SecondsPerDay * DaysPerYear;
constexpr u32 EpochYear = 1970;

u32 GetSecondsSinceEpoch
(time Time)
{
    const u32 MonthsInYearTable[2][12] =
    {
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };
    
    u32 Seconds = Time.Year * SecondsPerYear;
    
    u32 LeapYearCount = 0;
    for(u32 I = 0; I < Time.Year - 1; I++)
    {   
        if(IsLeapYear(EpochYear + I))
            LeapYearCount++;
    }
    Seconds += (LeapYearCount * SecondsPerDay);
    
    u32 Month = (u32)Time.Month;
    if(Month > 1)
    {
        u32 DaysPerMonth = 0;
        if(IsLeapYear(EpochYear + Time.Year) &&
           (Month > 2 || (Month == 2 && Time.Day >= 29))) // only counts when we're on leap day or past it
        {
            DaysPerMonth = 1;
        }
        
        for(u32 MonthIndex = 0; MonthIndex < Month - 1; MonthIndex++)
            Seconds += (MonthsInYearTable[DaysPerMonth][MonthIndex] * SecondsPerDay);
    }
    
    Seconds += ((Time.Day - 1) * SecondsPerDay);
    Seconds += (Time.Hour * SecondsPerHour);
    Seconds += (Time.Minute * SecondsPerMinute);
    Seconds += Time.Second;
    
    return Seconds;
}

fn GetTimeDifferenceInMinutes
(time Start, time End)
{
    u32 StartS = GetSecondsSinceEpoch(Start);
    u32 EndS = GetSecondsSinceEpoch(End);
    u32 DurationInSeconds = EndS - StartS;
    return (u32)(DurationInSeconds / SecondsPerMinute);
}

struct formatted_time_difference
{ u32 Hours, Minutes; };

fn GetFormatedTimeDifference
(u32 MinutesTotal)
{
    formatted_time_difference Res;
    Res.Hours = MinutesTotal / 60;
    Res.Minutes = MinutesTotal % 60;
    return Res;
}

int main()
{
    FreeConsole();
    
    auto Arena = AllocateArenaZero(2_MB);
    char* FileContent = ReadWholeFile(Arena, "save.txt");
    RAssert(FileContent, "Failed to read save.txt!");
    
    struct entry
    {
        time Time;
        ended_on Type;
    };
    singly_linked_list<entry> Entries(ShareArena(Arena));
    
    while(*FileContent)
    {
        auto& Entry = Entries.PushUninitialized();
        if(*FileContent == 's')
        {
            Entry.Type = Start;
            FileContent += strlen("s:");
        }
        else if(*FileContent == 'e')
        {
            Entry.Type = End;
            FileContent += strlen("e:");
        }
        else
        {
            RInvalidCodePath("save.txt is corrupted!");
        }
        
        Entry.Time = ReadTime(&FileContent);
        
        if(*FileContent == 0)
            break;
        
        ++FileContent;
    }
    
    if(Entries.Empty())
        ShowInfoMessageBoxAndCloseApp("There is nothing to show! (save.txt is empty)");
    
    // transform entries into chunks
    struct chunk
    {
        u32 DurationInMinutes,
        StartHour, StartMinute,
        EndHour, EndMinute;
    };
    backward_singly_linked_list_with_counter<chunk> Chunks(ShareArena(Arena));
    
    optional<time> LastEntryTime;
    for(auto& Entry : Entries)
    {
        if(LastEntryTime)
        {
            auto& Chunk = Chunks.PushUninitialized();
            Chunk.DurationInMinutes = GetTimeDifferenceInMinutes(*LastEntryTime, Entry.Time);
            Chunk.StartHour = LastEntryTime->Hour;
            Chunk.StartMinute = LastEntryTime->Minute;
            Chunk.EndHour = Entry.Time.Hour;
            Chunk.EndMinute = Entry.Time.Minute;
            LastEntryTime = Entry.Time;
        }
        else
        {
            LastEntryTime = Entry.Time;
        }
    }
    
    // add the current chunk
    auto& LastEntry = Entries.GetLast(); 
    time CurrentTime;
    auto& Chunk = Chunks.PushUninitialized();
    CurrentTime = GetLocalTime();
    Chunk.DurationInMinutes = GetTimeDifferenceInMinutes(LastEntry.Time, CurrentTime);
    Chunk.StartHour = LastEntry.Time.Hour;
    Chunk.StartMinute = LastEntry.Time.Minute;
    Chunk.EndHour = CurrentTime.Hour;
    Chunk.EndMinute = CurrentTime.Minute;
    
    // calculate summary
    time& StartTime = Entries.GetFirst().Time;
    u32 TotalTimeInMinutes = GetTimeDifferenceInMinutes(StartTime, CurrentTime);
    
    auto MostRecentChunkIsWorkChunk = [&]()
    { return Chunks.GetCount() % 2 == 1; };
    
    u32 WorkTimeInMinutes = TotalTimeInMinutes;
    u32 BreakTimeInMinutes = TotalTimeInMinutes;
    {
        bool Work = MostRecentChunkIsWorkChunk();
        for(auto& Chunk : Chunks)
        {
            if(Work)
                BreakTimeInMinutes -= Chunk.DurationInMinutes;
            else
                WorkTimeInMinutes -= Chunk.DurationInMinutes;
            Work = !Work;
        }
    }
    
    // format the message
    auto FormatHoursAndMinutes = 
    [](formatted_time_difference Diff)
    {
        if(Diff.Hours == 0)
            return Format("%min", Diff.Minutes);
        else
            return Format("%h %min", Diff.Hours, Diff.Minutes);
    };
    
    auto FormatTime = [](u32 StartHour, u32 StartMinute, u32 EndHour, u32 EndMinute)
    {
        auto AddZeroIfSingleDigit = [](u32 Number)
        {
            string<4> Res;
            if(Number < 10)
                Res += '0';
            Res += ToString(Number);
            return Res;
        };
        
        return Format("%:% - %:%",
                      AddZeroIfSingleDigit(StartHour), AddZeroIfSingleDigit(StartMinute),
                      AddZeroIfSingleDigit(EndHour), AddZeroIfSingleDigit(EndMinute));
    };
    
    auto FormatTimeInHours = []
    (formatted_time_difference TimeDiff)
    {
        f32 TimeInHours = (f32)TimeDiff.Hours + ((f32)TimeDiff.Minutes / 60);
        return ToString(TimeInHours, 2);
    };
    
    static string<8000> Message = "SUMMARY:\n";
    auto WorkTime = GetFormatedTimeDifference(WorkTimeInMinutes);
    auto WorkTimeInHours = FormatTimeInHours(WorkTime);
    auto BreakTime = GetFormatedTimeDifference(BreakTimeInMinutes);
    auto TotalTime = GetFormatedTimeDifference(TotalTimeInMinutes);
    Message += Format
    ("Work time: % (%h)\n"
     "Break time: %\n"
     "Total time: %, %\n\n",
     FormatHoursAndMinutes(WorkTime), WorkTimeInHours,
     FormatHoursAndMinutes(BreakTime),
     FormatHoursAndMinutes(TotalTime),
     FormatTime(StartTime.Hour, StartTime.Minute, CurrentTime.Hour, CurrentTime.Minute));
    
    Message += "CHUNKS:\n";
    {
        bool Work = MostRecentChunkIsWorkChunk();
        for(auto& Chunk : Chunks)
        {
            const char* Prefix = Work ? "Work" : "Break";
            auto TimeDiff = GetFormatedTimeDifference(Chunk.DurationInMinutes);
            auto HoursAndMinutes = FormatHoursAndMinutes(TimeDiff);
            auto Hours = FormatTimeInHours(TimeDiff);
            Message += Format
            ("%: % (%h), %\n",
             Prefix, HoursAndMinutes, Hours,
             FormatTime(Chunk.StartHour, Chunk.StartMinute, Chunk.EndHour, Chunk.EndMinute));
            Work = !Work;
        }
    }
    
    ShowInfoMessageBoxAndCloseApp(Message.GetCString());
}
