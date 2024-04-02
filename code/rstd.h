
/*
RSTD
Reasonale/Robust/Rapid C++ Standard Library Replacement
github.com/Czapa10/rstd

Made by Grzegorz "Czapa" Bednorz

YOU HAVE TO
define rstd_Implementation in one of C++ files that include this header
You also have to define rstd_Debug before including rstd.h (in all files)
Define rstd_Debug to 1 if you want to have assertions and debug only code turn on
Define rstd_Debug to 0 to turn off assertions and to make release only code be compiled

#define rstd_Implementation
#define rstd_Debug 1
#include "rstd.h"
*/

#include <cstdint>

// TODO: Get rid of these headers
#include <algorithm>
#include <cstdio>
#include <cmath>

#ifndef rstd_Debug
#error "You have to define rstd_Debug. Define it to 0 for release build. Define it to 1 for debug build in which rstd_Assert() works"
#endif

#ifndef rstd_MemoryProfileFunction
#define rstd_MemoryProfileFunction
#endif

// TODO: Make memory profiler not suck
#ifndef rstd_MemoryProfilerEnabled
#define rstd_MemoryProfilerEnabled 0
#endif

#ifndef rstd_FileDebugEnabled
#define rstd_FileDebugEnabled 0
#endif

#ifndef rstd_DoublyLinkedListAdvancedSanityCheckEnabled
#define rstd_DoublyLinkedListAdvancedSanityCheckEnabled rstd_Debug
#endif

#ifndef rstd_MultiThreadingEnabled
#define rstd_MultiThreadingEnabled 1
#endif

#ifndef rstd_bool
#define rstd_bool bool
#endif

#ifdef _WIN32
#include "intrin.h"
#endif

namespace rstd
{
    //////////////////////
    // TYPE DEFINITIONS //
    //////////////////////
#ifdef rstd_size
    using size = rstd_size;
#else
    using size = size_t;
#endif
    
    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;
    
    using i8 = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;
    
    using f32 = float;
    using f64 = double;
    
    ///////////////
    // constants //
    ///////////////
#ifndef rstd_FastMathAlreadyIncludedRstdAndFastMathSharedFeature
    // NOTE: define this macro when you're using FastMath library and you declare both 'using namespace fm' and 'using namespace rstd'
    
    static constexpr uint8_t MaxU8 = 255;
    static constexpr uint16_t MaxU16 = 65535;
    static constexpr uint32_t MaxU32 = 4294967295;
    static constexpr uint64_t MaxU64 = 18446744073709551615;
    
    static constexpr int8_t MaxI8 = 127;
    static constexpr int8_t MinI8 = -MaxI8 - 1;
    
    static constexpr int16_t MaxI16 = 32767;
    static constexpr int16_t MinI16 = -MaxI16 - 1;
    
    static constexpr int32_t MaxI32 = 2147483647;
    static constexpr int32_t MinI32 = -MaxI32 - 1;
    
    static constexpr int64_t MaxI64 = 9223372036854775807;
    static constexpr int64_t MinI64 = -MaxI64 - 1;
    
    static constexpr float MinPositiveF32 = 1.401298464e-45f;
    static constexpr float MaxF32 = 3.402823466e+38f;
    static constexpr float MinF32 = -MaxF32;
    
    static constexpr double MinPositiveF64 = 4.9406564584124654e-324; 
    static constexpr double MaxF64 = 1.7976931348623158e+308;
    static constexpr double MinF64 = -MaxF64;
    
    static constexpr float EpsilonF32 = 1.19209290E-07f;
    static constexpr double EpsilonF64 = 2.2204460492503131e-16;
    
    static constexpr float Pi32 = 3.14159265359f;
    static constexpr double Pi64 = 3.14159265358979323846;
    static constexpr float Tau32 = Pi32 * 2;
    static constexpr double Tau64 = Pi64 * 2;
    
    template<class t> class constants
    {
        static constexpr rstd_bool IsSpecialized = false;
        static constexpr t Epsilon() { return {}; }
        static constexpr t Min() { return {}; }
        static constexpr t Max() { return {}; }
        static constexpr t Pi() { return {}; }
        static constexpr t Tau() { return {}; }
        static constexpr t MinPositive() { return {}; }
    };
    
    template<> class constants<float>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr float Epsilon() { return EpsilonF32; }
        static constexpr float Min() { return MinF32; }
        static constexpr float Max() { return MaxF32; }
        static constexpr float Pi() { return Pi32; }
        static constexpr float Tau() { return Tau32; }
        static constexpr float MinPositive() { return MinPositiveF32; }
    };
    
    template<> class constants<double>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr double Epsilon() { return EpsilonF64; }
        static constexpr double Min() { return MinF64; }
        static constexpr double Max() { return MaxF64; }
        static constexpr double Pi() { return Pi64; }
        static constexpr double Tau() { return Tau64; }
        static constexpr double MinPositive() { return MinPositiveF64; }
    };
    
    template<> class constants<uint8_t>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr uint8_t Epsilon() { return 0; }
        static constexpr uint8_t Min() { return 0; }
        static constexpr uint8_t Max() { return MaxU8; }
        static constexpr uint8_t Pi() { return 0; }
        static constexpr uint8_t Tau() { return 0; }
        static constexpr uint8_t MinPositive() { return 1; }
    };
    
    template<> class constants<int8_t>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr int8_t Epsilon() { return 0; }
        static constexpr int8_t Min() { return MinI8; }
        static constexpr int8_t Max() { return MaxI8; }
        static constexpr int8_t Pi() { return 0; }
        static constexpr int8_t Tau() { return 0; }
        static constexpr int8_t MinPositive() { return 1; }
    };
    
    template<> class constants<uint16_t>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr uint16_t Epsilon() { return 0; }
        static constexpr uint16_t Min() { return 0; }
        static constexpr uint16_t Max() { return MaxU16; }
        static constexpr uint16_t Pi() { return 0; }
        static constexpr uint16_t Tau() { return 0; }
        static constexpr uint16_t MinPositive() { return 1; }
    };
    
    template<> class constants<int16_t>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr int16_t Epsilon() { return 0; }
        static constexpr int16_t Min() { return MinI16; }
        static constexpr int16_t Max() { return MaxI16; }
        static constexpr int16_t Pi() { return 0; }
        static constexpr int16_t Tau() { return 0; }
        static constexpr int16_t MinPositive() { return 1; }
    };
    
    template<> class constants<uint32_t>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr uint32_t Epsilon() { return 0; }
        static constexpr uint32_t Min() { return 0; }
        static constexpr uint32_t Max() { return MaxU32; }
        static constexpr uint32_t Pi() { return 0; }
        static constexpr uint32_t Tau() { return 0; }
        static constexpr uint32_t MinPositive() { return 1; }
    };
    
    template<> class constants<int32_t>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr int32_t Epsilon() { return 0; }
        static constexpr int32_t Min() { return MinI32; }
        static constexpr int32_t Max() { return MaxI32; }
        static constexpr int32_t Pi() { return 0; }
        static constexpr int32_t Tau() { return 0; }
        static constexpr int32_t MinPositive() { return 1; }
    };
    
    template<> class constants<uint64_t>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr uint64_t Epsilon() { return 0; }
        static constexpr uint64_t Min() { return 0; }
        static constexpr uint64_t Max() { return MaxU64; }
        static constexpr uint64_t Pi() { return 0; }
        static constexpr uint64_t Tau() { return 0; }
        static constexpr uint64_t MinPositive() { return 1; }
    };
    
    template<> class constants<int64_t>
    {
        static constexpr rstd_bool IsSpecialized = true;
        static constexpr int64_t Epsilon() { return 0; }
        static constexpr int64_t Min() { return MinI64; }
        static constexpr int64_t Max() { return MaxI64; }
        static constexpr int64_t Pi() { return 0; }
        static constexpr int64_t Tau() { return 0; }
        static constexpr int64_t MinPositive() { return 1; }
    };
    
    ////////////////////
    // invalid values //
    ////////////////////
    static constexpr uint8_t InvalidU8 = MaxU8;
    static constexpr uint16_t InvalidU16 = MaxU16;
    static constexpr uint32_t InvalidU32 = MaxU32;
    static constexpr uint64_t InvalidU64 = MaxU64;
    static constexpr int8_t InvalidI8 = MinI8;
    static constexpr int16_t InvalidI16 = MinI16;
    static constexpr int32_t InvalidI32 = MinI32;
    static constexpr int64_t InvalidI64 = MinI64;
    static constexpr float InvalidF32 = MinF32;
    static constexpr double InvalidF64 = MinF64;
#endif
    
#ifndef rstd_ExcludeDebugPrintingFunctions
    ////////////////////
    // DEBUG PRINTING //
    ////////////////////
    u32 GetSystemErrorCode();
    void InternalPrintInDebugger(const char* Message);
    void InternalWarningMessageBox(const char* Message);
    void ShowErrorMessageBox(const char* Message);
    void ShowErrorMessageBoxAndExitProcess(const char* Message);
#endif
    
    ////////////
    // STRING //
    ////////////
    static rstd_bool AdvanceIfStringsMatchUntilRightStringTerminates(char** APtr, const char* B);
    
    // TODO: Try to change it into template function
#define rstd_PrintInDebugger(_Fmt, ...) \
InternalPrintInDebugger(Format(_Fmt, __VA_ARGS__));
    
    //////////////////
    // DEBUG MACROS //
    //////////////////
    // TODO: Try to change it into template function
#define rstd_RInvalidCodePath(_String, ...) \
{ ShowErrorMessageBoxAndExitProcess(Format<string<1020>>(_String, __VA_ARGS__).GetCString()); }
    
    // TODO: Try to change it into template function
#define rstd_WarningMessageBox(_String, ...) \
{ InternalWarningMessageBox(Format<string<1020>>(_String, __VA_ARGS__).GetCString()); }
    
#define rstd_RAssert(Expression, String, ...) \
if(!(Expression)){rstd_RInvalidCodePath(String, __VA_ARGS__)}
    
#define rstd_Concat2(_A, _B) _A ## _B
#define rstd_Concat(_A, _B) rstd_Concat2(_A, _B)
#define rstd_LineName(_NameWithoutLine) rstd_Concat(_NameWithoutLine, __LINE__)
    
#if rstd_Debug
    
#define rstd_DebugOnly(Code) Code
#define rstd_ReleaseOnly(Code)
#define rstd_BreakInDebugger __debugbreak()
    
#define rstd_InvalidCodePath {\
rstd_WarningMessageBox("InvalidCodePath! file:% line:%", __FILE__, __LINE__); \
rstd_BreakInDebugger;}
    
#define rstd_InvalidCodePathM(Message, ...) {\
rstd_WarningMessageBox("InvalidCodePath! \n%\n file:% line:%", Message, __VA_ARGS__, __FILE__, __LINE__); \
rstd_BreakInDebugger;}
    
#define rstd_RawAssert(_Expr) if(!(_Expr)){\
ShowErrorMessageBox("Assertion failed!");\
rstd_BreakInDebugger;}
    
#define rstd_Assert(Expr) if(!(Expr)){\
rstd_WarningMessageBox("Assertion failed! \nfile:% line:%", __FILE__, __LINE__); \
rstd_BreakInDebugger;}
    
#define rstd_AssertM(Expr, Message, ...) \
if(!(Expr))\
{\
rstd_WarningMessageBox("Assertion failed! \n%\n file:% line:%",\
Format<string<1020>>(Message, __VA_ARGS__), __FILE__, __LINE__);\
rstd_BreakInDebugger;\
}
    
#define rstd_InvalidDefaultCase default: rstd_InvalidCodePath; break;
    
#else
    
#define rstd_DebugOnly(Code)
#define rstd_ReleaseOnly(Code) Code
#define rstd_Assert(Expr) 
#define rstd_AssertM(Expr, Message, ...) 
#define rstd_InvalidCodePath
#define rstd_InvalidCodePathM(Message, ...)
#define rstd_RawAssert(Expr)
#define rstd_InvalidDefaultCase default:;
    
#endif
    
    template<class code> struct _defer 
    {
        code Code;
        _defer(code Code) : Code(Code) {}
        ~_defer() { Code(); }
    };
    
    //TODO: Do we need this?
    template<class code> _defer<code> MakeDefer(code Code) 
    { return _defer<code>(Code); }
    
#define rstd_Defer1(x, y) x##y
#define rstd_Defer2(x, y) rstd_Defer1(x, y)
#define rstd_Defer3(x)    rstd_Defer2(x, __COUNTER__)
#define rstd_defer(code)   auto rstd_Defer3(_defer_) = rstd::MakeDefer([&](){code;})
    
    struct nullopt_t{};
    constexpr nullopt_t NullOpt;
    
    template<class type> struct optional
    {
        optional& operator=
        (const type& D)
        {
            Data = D;
            Valid = true;
            return *this;
        }
        
        optional(const type& D)
        { *this = D; }
        
        optional(nullopt_t)
        { Valid = false; }
        
        optional()
        { Valid = false; }
        
        type& Validate()
        {
            Valid = true;
            return Data;
        }
        
        operator rstd_bool()
        { return Valid; }
        
        type* operator->()
        {
            rstd_Assert(Valid);
            return &Data;
        }
        
        type& operator*()
        {
            rstd_Assert(Valid);
            return Data;
        }
        
        private:
        
        type Data;
        rstd_bool Valid;
    };
    
    template<class type> void Swap(type& A, type& B)
    {
        auto TempA = A;
        A = B;
        B = TempA;
    }
    
    //////////
    // TIME //
    //////////
    enum class month : u16
    {
        January = 1,
        February = 2,
        March = 3,
        April = 4,
        May = 5,
        June = 6,
        July = 7,
        August = 8,
        September = 9,
        October = 10,
        November = 11,
        December = 12,
    };
    
    enum class day_of_week : u8
    {
        Monday = 1,
        Tuesday = 2,
        Wednesday = 3,
        Thursday = 4,
        Friday = 5,
        Saturday = 6,
        Sunday = 7,
    };
    
    struct time
    {
        union
        {
            struct
            {
                u16 Day;    
                month Month;
                u32 Year;
            };
            u64 DayMonthYearPack;
        };
        
        day_of_week DayOfWeek;
        
        union
        {
            struct
            {
                u16 Millisecond;
                u16 Second;
                u16 Minute;
                u16 Hour;
            };
            u64 MillisecondSecondMinuteHourPack;
        };
    };
    
    time GetUtcTime();
    time GetLocalTime();
    
    ////////////
    // RANDOM //
    ////////////
    struct random_sequence
    { u32 A; };
    
    static random_sequence MakeRandomSequenceFromTime()
    {
        auto T = GetUtcTime();
        return {(u32)T.Millisecond * (u32)T.Day * (u32)T.DayOfWeek / ((u32)T.Second + 1)};
    }
    
    static u32 RandomU32
    (random_sequence& S)
    {
        // Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs"
        S.A ^= S.A << 13;
        S.A ^= S.A >> 17;
        S.A ^= S.A << 5;
        return S.A;
    }
    
    static u32 RandomU32
    (random_sequence& S, u32 Max) // Min is 0
    {
        u32 N = RandomU32(S);
        return N % (Max + 1);
    }
    
    static u32 RandomU32
    (random_sequence& S, u32 Min, u32 Max)
    {
        u32 N = RandomU32(S);
        u32 Range = Max - Min + 1;
        return Min + (N % Range);
    }
    
    static u16 RandomU16(random_sequence& S)
    { return (u16)(RandomU32(S) % MaxU16); }
    
    static i32 RandomI32
    (random_sequence& S)
    {
        union u
        {
            u32 U32;
            i32 I32;
        };
        u U;
        U.U32 = RandomU32(S);
        return U.I32;
    }
    
    static f32 RandomF32Between1AndMinus1(random_sequence& S)
    { return (f32)RandomI32(S) / MaxI32; }
    
    static f32 RandomF32Between0And1(random_sequence& S)
    { return (f32)RandomU32(S) / MaxU32; }
    
    static f32 RandomF32
    (random_sequence& S, f32 Min, f32 Max)
    {
        rstd_RawAssert(Min <= Max);
        f32 Factor = RandomF32Between0And1(S);
        return Min + Factor * (Max - Min);
    }
    
    ///////////////
    // ITERATION //
    ///////////////
#define rstd_For(Index, MaxIndex) for(u32 Index = 0; Index < MaxIndex; ++Index)
#define rstd_ForF32(Index, MaxIndex) for(f32 Index = 0; Index < MaxIndex; ++Index)
#define rstd_ArrayCount(Arr) (sizeof(Arr)/sizeof((Arr)[0]))
    
#define internal_rstd_RestOfIteratorFunctions \
const iterator CBegin() const \
{ return const_cast<const iterator>(Begin()); } \
    \
const iterator CEnd() const \
{ return const_cast<const iterator>(End()); } \
    \
iterator begin() \
{ return Begin(); } \
    \
iterator end() \
{ return End(); } \
    \
const iterator cbegin() const \
{ return CBegin(); } \
    \
const iterator cend() const \
{ return CEnd(); } \
    \
view<iterator> GetView() \
{ return {Begin(), End()}; } \
    
    template<class iterator> struct view
    {
        iterator Begin;
        iterator End;
        
        iterator begin()
        { return Begin; }
        
        iterator end()
        { return End; }
    };
    
    ////////////
    // STRING //
    ////////////
    template<class type> concept string_concept = requires(type Object)
    {
        {Object.IsRstdString()};
    };
#define rstd_generic_string rstd::string_concept auto
    
    template<class type> concept string_view_concept = requires(type Object)
    {
        {Object.IsRstdStringView()};
    };
#define rstd_generic_string_view rstd::string_view_concept auto
    
    template<class type> concept stringlike_concept = requires(type Object)
    {
        {Object.GetCString()};
    };
#define rstd_stringlike rstd::stringlike_concept auto
    
    template<class character> struct basic_string_view;
    
    template<size Size = 256 - sizeof(u32), class character_type = char> struct string
    {
        using character = character_type;
        
        mutable character Characters[Size];
        size Count;
        
        using iterator = character*;
        using string_type = string<Size, character>;
        
        constexpr void IsRstdString() {}
        
        constexpr size GetMaxCount()
        { return Size - 1; }
        
        constexpr size GetMaxCountWithNull()
        { return Size; }
        
        size GetCount() const
        { return Count; }
        
        size GetCountWithNull() const
        { return Count + 1; }
        
        void InsertNullTerminator() const
        { Characters[Count] = 0; }
        
        iterator Begin()
        { return Characters; }
        
        iterator End()
        { return Characters + Count; }
        
        internal_rstd_RestOfIteratorFunctions;
        
        void UpdateCount()
        {
            char* C = Characters;
            while(*C)
                ++C;
            Count = (size)(C - Characters);
        }
        
        string_type& operator=
        (const character* CString)
        { 
            rstd_AssertM(CString, "Call Clear() instead of assigning null or assign empty c-string");
            Count = 0;
            while(*CString)
            {
                rstd_Assert(Count < GetMaxCount());
                Characters[Count++] = *CString++;
            }
            rstd_DebugOnly(InsertNullTerminator());
            return *this;
        }
        
        template<size OtherSize> string_type& operator=
        (const string<OtherSize, character>& Other)
        {
            rstd_Assert(Other.Count <= GetMaxCount());
            rstd_For(CharIndex, Other.Count)
                Characters[CharIndex] = Other[CharIndex];
            Count = Other.Count;
            rstd_DebugOnly(InsertNullTerminator();)
                return *this;
        }
        
        string_type& operator=(basic_string_view<character> View)
        { return *this = View.GetCString(); }
        
        character* GetCString() const
        {
            InsertNullTerminator();
            return Characters;
        }
        
        void Clear()
        { 
            Count = 0;
            rstd_DebugOnly(InsertNullTerminator();)
        }
        
        string()
        { Clear(); }
        
        string(const character* CString)
        { *this = CString; }
        
        template<size OtherSize> string(const string<OtherSize, character>& Other)
        { *this = Other; }
        
        string(basic_string_view<character> String)
        { *this = String; }
        
        rstd_bool Empty()
        { return Count == 0; }
        
        character& operator[]
        (size Index)
        {
            rstd_AssertM(Index < Count, Format<string<>>("You tried to get character [%], but this string has only % characters", Index, Count).GetCString());
            return Characters[Index];
        }
        
        character operator[]
        (size Index) const
        {
            rstd_AssertM(Index < Count, Format<string<>>("You tried to get character [%], but this string has only % characters", Index, Count).GetCString());
            return Characters[Index];
        }
        
        void operator+=
        (char C)
        {
            Characters[Count++] = C;
            rstd_DebugOnly(InsertNullTerminator());
        }
        
        void operator+=
        (const char* CString)
        {
            for(auto* C = CString; *C; ++C)
            {
                rstd_AssertM(Count < GetMaxCount(), "C string is too long to append it to this string<>");
                Characters[Count++] = *C;
            }
            rstd_DebugOnly(InsertNullTerminator();)
        }
        
        void operator+=
        (const rstd_generic_string& AppendedString)
        {
            rstd_Assert(Count + AppendedString.Count < GetMaxCount());
            rstd_For(CharIndex, AppendedString.Count)
                Characters[Count++] = AppendedString[CharIndex];
            rstd_DebugOnly(InsertNullTerminator();)
        }
        
        void operator+=
        (rstd_generic_string_view View)
        {
            rstd_Assert(Count + View.Count < GetMaxCount());
            rstd_For(CharIndex, View.Count)
                Characters[Count++] = View[CharIndex];
            rstd_DebugOnly(InsertNullTerminator();)
        }
        
        rstd_bool operator==
        (const rstd_generic_string& Other)
        {
            if(Count != Other.Count)
                return false;
            
            rstd_For(CharIndex, Count)
            {
                if((character)(operator[](CharIndex)) != (character)(Other[CharIndex]))
                    return false;
            }
            
            return true;
        }
        
        rstd_bool operator!=
        (const rstd_generic_string& Other)
        {
            return !(*this == Other);
        }
        
        rstd_bool operator==
        (const character* CString)
        {
            size CharIndex = 0;
            for(;;)
            {
                if(CharIndex == Count && *CString == 0)
                    return true;
                
                if(CharIndex == Count || *CString == 0)
                    return false;
                
                if(Characters[CharIndex] != *CString)
                    return false;
                
                ++CharIndex;
                ++CString;
            }
        }
        
        rstd_bool operator!=
        (const character* CString)
        {
            return !(*this == CString);
        }
    };
    
#define rstd_ForCString(_Char, _String) for(char* _Char = _String; *_Char != 0; ++_Char)
    
    template<string_concept string_type, class... args> static string_type Format(const char* Format, args... Args);
    
    template<class string_type = string<>> static auto MakeStringUpToCount
    (const char* CString, size CharacterCount)
    {
        string_type Res;
        while(CString && Res.Count < CharacterCount)
            Res += *CString++;
        return Res;
    }
    
    template<class string_type = string<>> static auto MakeStringUntilCharacter
    (const char* CString, char EndCharacter)
    {
        string_type Res;
        while(*CString != EndCharacter)
        {
            rstd_AssertM(*CString, "Null terminator was encountered before EndCharacter");
            Res += *CString++;
        }
        return Res;
    }
    
    template<class string_type = string<>> static auto MakeStringUntilCharacter
    (const rstd_stringlike& String, char EndCharacter)
    { return MakeStringUntilCharacter(String.GetCString(), EndCharacter); }
    
    template<class string_type = string<>, class end_condition> static auto MakeStringUntil
    (const char* CString, end_condition EndCondition)
    {
        string_type Res;
        while(!EndCondition(CString))
        {
            rstd_AssertM(CString, "Null terminator was encontered before EndCondition");
            Res += *CString++;
        }
        return Res;
    }
    
    template<class string_type = string<>, class end_condition> static auto MakeStringUntil
    (const rstd_stringlike& String, end_condition EndCondition)
    { return MakeStringUntil(String.GetCString(), EndCondition); }
    
    // TODO: Compress MakeStringTillCharacterAndAdvanceCString functions
    template<class string_type = string<>> static auto MakeStringUntilCharacterAndAdvanceCString
    (char** CStringPtr, char EndCharacter)
    {
        string_type Res;
        char* CString = *CStringPtr;
        while(*CString != EndCharacter)
        {
            rstd_AssertM(*CString, "Null terminator was encontered before EndCharacter");
            Res += *CString++;
        }
        *CStringPtr = CString;
        return Res;
    }
    
    template<class string_type = string<>, class comparison_fn> static auto MakeStringUntilAndAdvanceCString
    (char** CStringPtr, comparison_fn EndCondition)
    {
        string_type Res;
        char* CString = *CStringPtr;
        while(!EndCondition(CString))
        {
            rstd_AssertM(*CString, "Null terminator was encontered before EndCharacter");
            Res += *CString++;
        }
        *CStringPtr = CString;
        return Res;
    }
    
    static char DigitToChar
    (u32 Digit)
    { 
        rstd_RawAssert(Digit < 10);
        return '0' + (char)Digit;
    }
    
    static u32 CharToDigit
    (char Char)
    {
        rstd_RawAssert(Char >= '0' && Char <= '9');
        return (u32)(Char - '0');
    }
    
    static u32 GetDigitCount
    (u64 Int)
    {
        u32 Res = 1;
        while(Int /= 10)
            ++Res;
        return Res;
    }
    
    template<u32 StringSize, class unsigned_integer> static auto UnsignedIntegerToString
    (unsigned_integer Int)
    {
        string<StringSize> Res;
        
        u32 DigitCount = GetDigitCount((u64)Int);
        Res.Count = DigitCount;
        
        while(DigitCount)
        {
            Res[--DigitCount] = DigitToChar(Int % 10);
            Int /= 10;    
        }
        
        rstd_DebugOnly(Res.InsertNullTerminator());
        
        return Res;
    }
    
    template<u32 StringSize, class signed_integer> static auto SignedIntegerToString
    (signed_integer Int)
    {
        string<StringSize> Res;
        
        u32 StopIndex;
        u32 CharCount;
        if(Int < 0)
        {
            StopIndex = 1;
            Int = -Int;
            Res.Count = CharCount = GetDigitCount((u64)Int) + 1;
            Res[0] = '-';
        }
        else
        {
            StopIndex = 0;
            Res.Count = CharCount = GetDigitCount((u64)Int);
        }
        
        while(CharCount > StopIndex)
        {
            Res[--CharCount] = DigitToChar(Int % 10);
            Int /= 10;
        }
        
        rstd_DebugOnly(Res.InsertNullTerminator());
        
        return Res;
    }
    
    static auto ToString(u8 A)
    { return UnsignedIntegerToString<4>(A); }
    
    static auto ToString(u16 A)
    { return UnsignedIntegerToString<8>(A); }
    
    static auto ToString(u32 A)
    { return UnsignedIntegerToString<12>(A); }
    
    static auto ToString(u64 A)
    { return UnsignedIntegerToString<24>(A); }
    
    static auto ToString(i8 A)
    { return SignedIntegerToString<4>(A); }
    
    static auto ToString(i16 A)
    { return SignedIntegerToString<8>(A); }
    
    static auto ToString(i32 A)
    { return SignedIntegerToString<12>(A); }
    
    static auto ToString(i64 A)
    { return SignedIntegerToString<24>(A); }
    
    template<u32 string_size, class float_type> static string<string_size> FloatToString
    (float_type F, u32 Precision)
    {
        string<string_size> Res;
        
        f32 Rounding = 5.f / pow(10.f, (f32)Precision + 1.f);
        
        if(F < 0)
        {
            F *= -1;
            Res += '-';
        }
        
        u64 Integer = (u64)F;
        f64 Fraction = F - Integer + Rounding;
        if(Fraction >= 1)
        {
            Integer++;
            Res += ToString(Integer);
            Res += '.';
            for(u32 I = 0; I < Precision; I++)
                Res += '0';
            return Res;
        }
        
        Res += ToString(Integer);
        
        if(Precision > 0)
        {
            Res += '.';
            while(Fraction < 0.1f && Precision > 1)
            {
                Res += '0';
                Fraction *= 10;
                --Precision;
            }
            
            Fraction *= pow(10, Precision);
            Res += ToString((u64)Fraction);
        }
        
        return Res;
    }
    
    // TODO: Check if sizes of string are appropriate
    static auto ToString(f32 F, u32 Precision = 5)
    { return FloatToString<64>(F, Precision); }
    
    static auto ToString(f64 F, u32 Precision = 5)
    { return FloatToString<128>(F, Precision); }
    
    template<class bool_type> static const char* BoolToString(bool_type Bool)
    { return Bool ? "true" : "false"; }
    
    static const char* ToString(rstd_bool Bool)
    { return BoolToString(Bool); }
    
    // NOTE: This is only to make Format<string> template craziness work
    // TODO: Maybe do that differently, this is too silly!
    static char ToString(char C)
    { return C; }
    
    static char* ToString(char* C)
    { return C; }
    
    static auto ToString(const char* C)
    { return C; }
    
    template<u32 Size> static auto ToString(string<Size>& String)
    { return String; }
    
    static char* GetNullTerminator
    (char* String)
    {
        char* C = String;
        while(*C)
            ++C;
        return C;
    }
    
    // is space character
    template<class character> static rstd_bool GenericCharIsSpaceCharacter(character C)
    { return C == ' ' || C == '\t' || C == '\n' || C == '\r'; }
    
    template<class character> static rstd_bool GenericFirstCharIsSpaceCharacter(const character* S)
    { return GenericCharIsSpaceCharacter(*S); }
    
    static rstd_bool CharIsSpaceCharacter(char C)
    { return C == ' ' || C == '\t' || C == '\n' || C == '\r'; }
    
    static rstd_bool FirstCharIsSpaceCharacter(const char* S)
    { return CharIsSpaceCharacter(*S); }
    
    // is alpha
    template<class character> static rstd_bool GenericCharIsAlpha(character C)
    { return (C >= 'A' && C <= 'Z') || (C >= 'a' && C <= 'z'); }
    
    template<class character> static rstd_bool GenericFirstCharIsAlpha(const character* S)
    { return GenericCharIsAlpha(*S); }
    
    static rstd_bool CharIsAlpha(char C)
    { return (C >= 'A' && C <= 'Z') || (C >= 'a' && C <= 'z'); }
    
    static rstd_bool FirstCharIsAlpha(const char* S)
    { return CharIsAlpha(*S); }
    
    // is digit
    template<class character> static rstd_bool GenericCharIsDigit(character C)
    { return C >= '0' && C <= '9'; }
    
    template<class character> static rstd_bool GenericFirstCharIsDigit(const character* S)
    { return GenericCharIsDigit(*S); }
    
    static rstd_bool CharIsDigit(char C)
    { return C >= '0' && C <= '9'; }
    
    static rstd_bool FirstCharIsDigit(const char* S)
    { return CharIsDigit(*S); }
    
    // is digit or point
    template<class character> static rstd_bool GenericCharIsDigitOrPoint(character C)
    { return CharIsDigit(C) || C == '.'; }
    
    template<class character> static rstd_bool GenericFirstCharIsDigitOrPoint(const character* S)
    { return GenericCharIsDigitOrPoint(*S); }
    
    static rstd_bool CharIsDigitOrPoint(char C)
    { return CharIsDigit(C) || C == '.'; }
    
    static rstd_bool FirstCharIsDigitOrPoint(const char* S)
    { return CharIsDigitOrPoint(*S); }
    
    static char ToUppercase
    (char C)
    {
        if(C >= 97 && C <= 122)
            C -= ('a' - 'A');
        return C;
    }
    
    static char ToLowercase
    (char C)
    {
        if(C >= 65 && C <= 90)
            C += ('a' - 'A');
        return C;
    }
    
    static rstd_bool StringsMatch
    (const char* A, const char* B)
    {
        for(;;)
        {
            if(*A == 0 || *B == 0)
                return *A == 0 && *B == 0;
            
            if(*A != *B)
                return false;
            
            ++A;
            ++B;
        }
    }
    
    static rstd_bool StringsMatchUntilCharacter
    (const char* A, const char* B, char Character)
    {
        for(;;)
        {
            if(*A == Character || *B == Character)
                return *A == Character && *B == Character;
            
            if(*A == 0 || *B == 0)
                return false;
            
            if(*A != *B)
                return false;
            
            ++A;
            ++B;
        }
    }
    
    static rstd_bool StringsMatchUntilRightStringTerminates
    (const char* Left, const char* Right)
    {
        for(;;)
        {
            if(*Right == 0)
                return true;
            
            if(*Left != *Right)
                return false;
            
            ++Left;
            ++Right;
        }
    }
    
    template<class... right_strings>
        static rstd_bool StringsMatchUntilRightStringTerminates
    (const char* Left, const char* Right, right_strings... RightStrings)
    {
        if(StringsMatchUntilRightStringTerminates(Left, Right))
            return true;
        else
            return StringsMatchUntilRightStringTerminates(Left, RightStrings...);
    }
    
    static rstd_bool StringsMatch
    (const char* A, const char* B, u32 Length)
    {
        rstd_For(CharIndex, Length)
        {
            if(A[CharIndex] != B[CharIndex])
                return false;
        }
        return true;
    }
    
    static rstd_bool AdvanceUntilCharacter
    (char** StringPtr, char Character)
    {
        char* C = *StringPtr;
        for(;;)
        {
            if(*C == Character)
            {
                *StringPtr = C;
                return true;
            }
            else if(*C == 0)
            {
                return false;
            }
            
            ++C;
        }
    }
    
    static rstd_bool AdvanceIfStringsMatchUntilRightStringTerminates
    (char** LeftPtr, const char* Right)
    {
        char* Left = *LeftPtr;
        for(;;)
        {
            if(*Right == 0)
                break;
            
            if(*Left != *Right)
                return false;
            
            ++Left;
            ++Right;
        }
        
        *LeftPtr = Left;
        return true;
    }
    
    static rstd_bool AdvanceAfterCharacter
    (char** StringPtr, char Character)
    {
        rstd_bool Res = AdvanceUntilCharacter(StringPtr, Character);
        ++(*StringPtr);
        return Res;
    }
    
    static rstd_bool AdvanceUntilNextLine(char** StringPtr)
    { return AdvanceAfterCharacter(StringPtr, '\n'); }
    
    static rstd_bool AdvanceUntilAlpha
    (char** StringPtr)
    {
        char* C = *StringPtr;
        for(;;)
        {
            if(CharIsAlpha(*C))
            {
                *StringPtr = C;
                return true;
            }
            else if(*C == 0)
            {
                return false;
            }
            
            ++C;
        }
    }
    
    static void CopyStringUntilChar
    (char* Dest, const char* Source, char EndChar)
    {
        while(*Source != EndChar)
        {
            rstd_Assert(*Source != 0);
            *Dest++ = *Source++;
        }
        *Dest = 0;
    }
    
    template<class string_type> static void CopyStringUntilChar
    (string_type* Dest, const char* Source, char EndChar)
    {
        while(*Source != EndChar)
        {
            rstd_Assert(*Source != 0);
            *Dest += *Source++;
        }
    }
    
    static void CopyString
    (char* Dest, const char* Source, u32 LengthWithNullChar)
    {
        u32 CharIndex = 0;
        u32 Length = LengthWithNullChar - 1;
        for(; CharIndex < Length; ++CharIndex)
            Dest[CharIndex] = Source[CharIndex];
        Dest[CharIndex + 1] = 0;
    }
    
    static u32 GetOffsetToChar
    (const char* String, char Char)
    {
        u32 Offset = 0;
        while(String[Offset++] != Char);
        return Offset;
    }
    
    static void JumpAfterChar
    (char** String, char Char)
    {
        while(**String != Char)
        (*String)++;
        (*String)++;
    }
    
    static void JumpToNextLine(char** String)
    { JumpAfterChar(String, '\n'); }
    
    static const char* GetPtrAfter
    (const char* String, const char* StringToLookFor)
    {
        while(*String)
        {
            if(*String == *StringToLookFor)
            {
                const char* A = String;
                const char* B = StringToLookFor;
                for(;;)
                {
                    ++A;
                    ++B;
                    
                    if(*B == 0)
                        return A;
                    
                    if(*A != *B)
                        break;
                }
            }
            
            ++String;
        }
        return nullptr;
    }
    
    static const char* GetPtrAfter
    (const char* String, char CharacterToLookFor)
    {
        while(*String)
        {
            if(*String == CharacterToLookFor)
                return ++String;
            
            ++String;
        }
        return nullptr;
    }
    
    static char* GetPtrAfter(char* String, const char* StringToLookFor)
    { return const_cast<char*>(GetPtrAfter(const_cast<const char*>(String), StringToLookFor)); }
    
    static const char* GetPtrAfter(const rstd_stringlike& String, const char* StringToLookFor)
    { return GetPtrAfter(String.GetCString(), StringToLookFor); }
    
    static const char* GetPtrAfter(const rstd_stringlike& String, char CharacterToLookFor)
    { return GetPtrAfter(String.GetCString(), CharacterToLookFor); }
    
    static rstd_bool StringContains(const char* String, const char* StringToLookFor)
    { return GetPtrAfter(String, StringToLookFor); }
    
    static rstd_bool StringContains(const rstd_stringlike& String, const char* StringToLookFor)
    { return GetPtrAfter(String.GetCString(), StringToLookFor); }
    
    static rstd_bool StringContains(const char* String, char CharacterToLookFor)
    { return GetPtrAfter(String, CharacterToLookFor); }
    
    static rstd_bool StringContains(const rstd_stringlike& String, char CharacterToLookFor)
    { return GetPtrAfter(String.GetCString(), CharacterToLookFor); }
    
    static const char* GetPtrAfter
    (const char* String, const char* StringToLookFor, char EndChar)
    {
        while(*String && *String != EndChar)
        {
            if(*String == *StringToLookFor)
            {
                const char* A = String;
                const char* B = StringToLookFor;
                for(;;)
                {
                    ++A;
                    ++B;
                    
                    if(*B == 0)
                        return A;
                    
                    if(*A != *B)
                        break;
                }
            }
            
            ++String;
        }
        return nullptr;
    }
    
    static char* GetPtrAfter(char* String, const char* StringToLookFor, char EndChar)
    { return const_cast<char*>(GetPtrAfter(const_cast<const char*>(String), StringToLookFor, EndChar)); }
    
    static rstd_bool StringContains(const char* String, const char* StringToLookFor, char EndChar)
    { return GetPtrAfter(String, StringToLookFor, EndChar); }
    
    static f32 ReadF32
    (char** StringPtr)
    {
        const char* String = *StringPtr;
        
        f32 Res = 0;
        u32 CharIndex = 0;
        
        // parse sign
        f32 Sign;
        if(*String == '-')
        {
            Sign = -1;
            ++CharIndex;
        }
        else
        {
            Sign = 1;
        }
        
        rstd_Assert(CharIsDigit(String[CharIndex]));
        
        // extract integer part
        rstd_bool ThereIsPartAfterPoint;
        for(;;)
        {
            char C = String[CharIndex++];
            
            if(!CharIsDigitOrPoint(C))
            {
                ThereIsPartAfterPoint = false;
                break;
            }
            if(C == '.')
            {
                ThereIsPartAfterPoint = true;
                break;
            }
            
            Res = 10 * Res + C - '0';
        }
        
        if(ThereIsPartAfterPoint)
        {
            // extract part after point
            f32 DigitSignificance = 1;
            for(;;)
            {
                char C = String[CharIndex++];
                
                if(!CharIsDigit(C))
                    break;
                
                DigitSignificance *= 0.1f;
                Res += DigitSignificance * (C - '0');
            }
        }
        
        Res *= Sign;
        *StringPtr += CharIndex - 1;
        return Res;
    }
    
    static u32 ReadU32
    (char** StringPtr)
    {
        const char* String = *StringPtr;
        rstd_Assert(String);
        rstd_Assert(CharIsDigit(*String));
        u32 Res = 0, CharIndex = 0;
        while(CharIsDigit(String[CharIndex]))
        {
            Res = Res * 10 + String[CharIndex] - '0';
            ++CharIndex;
        }
        *StringPtr += CharIndex;
        return Res;
    }
    
    static i32 ReadI32
    (char** StringPtr)
    {
        const char* String = *StringPtr;
        rstd_Assert(String);
        rstd_Assert(CharIsDigit(*String) || *String == '-');
        i32 Sign = 1;
        if(*String == '-')
        {
            Sign = -1;
            ++(*StringPtr);
        }
        return (i32)ReadU32(StringPtr) * Sign;
    }
    
    static u16 ReadU16
    (char** StringPtr)
    {
        u32 Res = ReadU32(StringPtr);
        rstd_Assert(Res <= MaxU16);
        return (u16)Res;
    }
    
    static f32 StringToF32(char* String)
    { return ReadF32(&String); }
    
    static u32 StringToU32(char* String)
    { return ReadU32(&String); }
    
    static u16 StringToU16(char* String)
    { return ReadU16(&String); }
    
    static u8 StringToU8(char* String)
    { return (u8)StringToU32(String); }
    
    static f32 StringToF32(const rstd_stringlike& String)
    { return StringToF32(String.GetCString()); }
    
    static u32 StringToU32(const rstd_stringlike& String)
    { return StringToU32(String.GetCString()); }
    
    static u16 StringToU16(const rstd_stringlike& String)
    { return StringToU16(String.GetCString()); }
    
    static u8 StringToU8(const rstd_stringlike& String)
    { return StringToU8(String.GetCString()); }
    
    static rstd_bool OneZeroStringToBool
    (const char* String)
    {
        if(*String == '1')
            return true;
        else if(*String == '0')
            return false;
        
        rstd_InvalidCodePath;
        return false;
    }
    
    static rstd_bool OneZeroStringToBool(const rstd_stringlike& String)
    { return OneZeroStringToBool(String.GetCString()); }
    
    static u64 HashString
    (const char* String)
    {
        // NOTE: Murmur string hash, one byte at a time version
        
        u64 Hash = 525201411107845655;
        for(; *String; ++String)
        {
            Hash ^= *String;
            Hash *= 0x5bd1e9955bd1e995;
            Hash ^= Hash >> 47;
        }
        return Hash;
    }
    
    template<class string_type> static u64 HashString
    (const string_type& String)
    {
        // NOTE: Murmur string hash, one byte at a time version
        
        u64 Hash = 525201411107845655;
        for(u32 CharIndex = 0; CharIndex < String.Count; ++CharIndex)
        {
            Hash ^= String[CharIndex];
            Hash *= 0x5bd1e9955bd1e995;
            Hash ^= Hash >> 47;
        }
        return Hash;
    }
    
#ifdef rstd_FastMathStringFunctions
#include "rstd_fast_math_string_functions.h"
#endif
    
    template<class string_type> static void InternalFormat
    (string_type& Res, const char* Format)
    { Res = Format; }
    
    template<class string_type, class arg> static void InternalFormat
    (string_type& Res, const char* Format, arg Arg)
    {
        rstd_DebugOnly(rstd_bool LastArgPeeledOff = false);
        rstd_RawAssert(*Format); // You passed more variadic arguments then you have % signs in Format<string>()
        while(*Format)
        {
            if(*Format == '%')
            {
                ++Format;
                if(*Format == '%')
                {
                    Res += '%';
                    ++Format;
                }
                else
                {
                    Res += ToString(Arg);
                    rstd_RawAssert(!LastArgPeeledOff); // You have more % signs then variadic arguments in Format<string>()
                    rstd_DebugOnly(LastArgPeeledOff = true;)
                }
            }
            else
            {
                Res += *Format;
                ++Format;
            }
        }
    }
    
    template<class string_type, class arg, class... args> static void InternalFormat
    (string_type& Res, const char* Format, arg Arg, args... Args)
    {
        for(;; ++Format)
        {
            if(*Format == '%')
            {
                ++Format;
                if(*Format == '%')
                {
                    Res += '%';
                }
                else
                {
                    Res += ToString(Arg);
                    InternalFormat(Res, Format, Args...);
                    return;
                }
            }
            else
            {
                rstd_RawAssert(*Format); // You passed more variadic arguments then you have % signs in Format<string>()
                Res += *Format;
            }
        }
    }
    
    template<string_concept string_type = string<>, class... args> static string_type Format
    (const char* Fmt, args... Args)
    {
        string_type Res;
        InternalFormat(Res, Fmt, Args...);
        return Res;
    }
    
    static const char* TypeName(u64)
    { return "u64"; }
    
    static const char* TypeName(u32)
    { return "u32"; }
    
    static const char* TypeName(u16)
    { return "u16"; }
    
    static const char* TypeName(u8)
    { return "u8"; }
    
    static const char* TypeName(i64)
    { return "i64"; }
    
    static const char* TypeName(i32)
    { return "i32"; }
    
    static const char* TypeName(i16)
    { return "i16"; }
    
    static const char* TypeName(i8)
    { return "i8"; }
    
    static const char* TypeName(f32)
    { return "f32"; }
    
    static const char* TypeName(f64)
    { return "f64"; }
    
    // TODO: Add iterators, Add more helper functions
    template<class character_type> struct basic_string_view
    {
        using character = character_type;
        
        // TODO: In theory we shouldn't use InvalidU32 for Count because Count is of type size and not u32
        
        mutable character* Characters;
        private:
        size Count;
        public:
        
        constexpr void IsRstdStringView() {}
        
        basic_string_view(const character* CString)
            :Characters(const_cast<character*>(CString)), Count(InvalidU32) {}
        
        template<size Size> basic_string_view
        (const string<Size, character>& String)
            :Characters((character*)String.GetCString()), Count(String.Count) {}
        
        basic_string_view(){}
        
        character* GetCString() const
        { return Characters; }
        
        void AssertThatCountIsValid() const
        {
            rstd_AssertM(Count != InvalidU32,
                         "This string_view object was probably initialized with c string\n"
                         "because of that you have to call UpdateCount() before calling the function you called!");
        }
        
        size GetCount() const
        { 
            AssertThatCountIsValid();
            return Count;
        }
        
        void UpdateCount()
        {
            Count = 0;
            char* C = Characters;
            while(*C)
            {
                ++Count;
                ++C;
            }
        }
        
        void UpdateCountIfIsInvalid()
        {
            if(Count == InvalidU32)
                UpdateCount();
        }
        
        character operator[]
        (size Index)
        {
            AssertThatCountIsValid();
            return Characters[Index];
        }
        
        void RemovePrefix(size NumberOfCharactesToRemove)
        { Characters += NumberOfCharactesToRemove; }
        
        void RemoveSuffix
        (size NumberOfCharactesToRemove)
        {
            AssertThatCountIsValid();
            Count -= NumberOfCharactesToRemove;
        }
    };
    
    using string_view = basic_string_view<char>;
    
    static auto ToString(rstd_generic_string_view View)
    { return View.Characters; }
    
    static rstd_bool operator==(rstd_generic_string_view A, rstd_generic_string_view B)
    { return (A.GetCount() == B.GetCount()) ? StringsMatch(A.GetCString(), B.GetCString()) : false; }
    
    static rstd_bool operator!=(rstd_generic_string_view A, rstd_generic_string_view B)
    { return !(A == B); }
    
    static rstd_bool operator==(rstd_generic_string_view A, rstd_generic_string B)
    { return A == string_view(B); }
    
    static rstd_bool operator!=(rstd_generic_string_view A, rstd_generic_string B)
    { return !(A == B); }
    
    //////////
    // TIME //
    //////////
    rstd_bool operator==
    (time A, time B)
    {
        if(A.Year == B.Year && A.Month == B.Month && A.Day == B.Day)
        {
            rstd_Assert(A.DayOfWeek == B.DayOfWeek);
            return A.Hour == B.Hour && A.Minute == B.Minute && A.Second == B.Second && A.Millisecond == B.Millisecond;
        }
        return false;
    }
    
    rstd_bool operator<=
    (time A, time B)
    {
        if(A.DayMonthYearPack < B.DayMonthYearPack)
            return true;
        else if(A.DayMonthYearPack > B.DayMonthYearPack)
            return false;
        else
            return A.MillisecondSecondMinuteHourPack <= B.MillisecondSecondMinuteHourPack;
    }
    
    rstd_bool operator>=
    (time A, time B)
    {
        if(A.DayMonthYearPack > B.DayMonthYearPack)
            return true;
        else if(A.DayMonthYearPack < B.DayMonthYearPack)
            return false;
        else
            return A.MillisecondSecondMinuteHourPack >= B.MillisecondSecondMinuteHourPack;
    }
    
    rstd_bool operator<(time A, time B)
    { return !(A >= B); }
    
    rstd_bool operator>(time A, time B)
    { return !(A <= B); }
    
    // TODO: Those functions can be implemented better with having static array of month strings.
    //       besides they could return const char* instead, Couldn't they
    
    static string<12> ToString
    (month M)
    {
        switch(M)
        {
            case month::January: return "January";
            case month::February: return "February";
            case month::March: return "March";
            case month::April: return "April";
            case month::May: return "May";
            case month::June: return "June";
            case month::July: return "July";
            case month::August: return "August";
            case month::September: return "September";
            case month::October: return "October";
            case month::November: return "November";
            case month::December: return "December";
            default: return "ERROR";
        }
        return {};
    }
    
    static month ReadMonth
    (char** StringPtr)
    {
        if(AdvanceIfStringsMatchUntilRightStringTerminates(StringPtr, "January"))
            return month::January;
        else if(AdvanceIfStringsMatchUntilRightStringTerminates(StringPtr, "February"))
            return month::February;
        else if(AdvanceIfStringsMatchUntilRightStringTerminates(StringPtr, "March"))
            return month::March;
        else if(AdvanceIfStringsMatchUntilRightStringTerminates(StringPtr, "April"))
            return month::April;
        else if(AdvanceIfStringsMatchUntilRightStringTerminates(StringPtr, "May"))
            return month::May;
        else if(AdvanceIfStringsMatchUntilRightStringTerminates(StringPtr, "June"))
            return month::June;
        else if(AdvanceIfStringsMatchUntilRightStringTerminates(StringPtr, "July"))
            return month::July;
        else if(AdvanceIfStringsMatchUntilRightStringTerminates(StringPtr, "August"))
            return month::August;
        else if(AdvanceIfStringsMatchUntilRightStringTerminates(StringPtr, "September"))
            return month::September;
        else if(AdvanceIfStringsMatchUntilRightStringTerminates(StringPtr, "October"))
            return month::October;
        else if(AdvanceIfStringsMatchUntilRightStringTerminates(StringPtr, "November"))
            return month::November;
        else if(AdvanceIfStringsMatchUntilRightStringTerminates(StringPtr, "December"))
            return month::December;
        rstd_InvalidCodePath;
        return {};
    }
    
    static month StringToMonth(char* String)
    { return ReadMonth(&String); }
    
    static string<12> ToString
    (day_of_week D)
    {
        switch(D)
        {
            case day_of_week::Monday: return "Monday";
            case day_of_week::Tuesday: return "Tuesday";
            case day_of_week::Wednesday: return "Wednesday";
            case day_of_week::Thursday: return "Thursday";
            case day_of_week::Friday: return "Friday";
            case day_of_week::Saturday: return "Saturday";
            case day_of_week::Sunday: return "Sunday";
            return "ERROR";
        }
        return {};
    }
    
    static string<4> MinuteToReadableString
    (u16 Minute)
    { 
        rstd_Assert(Minute <= 60);
        return Minute < 10 ? Format<string<4>>("0%", Minute) : ToString((u8)Minute); 
    }
    
    static string<> ToReadableString(time T)
    { return Format("% | % % % | %:%", T.DayOfWeek, T.Day, T.Month, T.Year, T.Hour, MinuteToReadableString(T.Minute)); }
    
    static string<> ToString(time T)
    { return Format("%.%.%.%_%.%.%.%", T.Year, (u32)T.Month, T.Day, (u32)T.DayOfWeek, T.Hour, T.Minute, T.Second, T.Millisecond); }
    
    static time ReadTime
    (char** StringPtr)
    {
        time T;
        T.Year = ReadU32(StringPtr);
        ++(*StringPtr);
        T.Month = (month)ReadU32(StringPtr);
        ++(*StringPtr);
        T.Day = (u8)ReadU32(StringPtr);
        ++(*StringPtr);
        T.DayOfWeek = (day_of_week)ReadU32(StringPtr); // TODO
        ++(*StringPtr);
        T.Hour = (u8)ReadU32(StringPtr);
        ++(*StringPtr);
        T.Minute = (u8)ReadU32(StringPtr);
        ++(*StringPtr);
        T.Second = (u8)ReadU32(StringPtr);
        ++(*StringPtr);
        T.Millisecond = (u16)ReadU32(StringPtr);
        return T;
    }
    
    static time StringToTime(char* String)
    { return ReadTime(&String); }
    
    
    struct calling_info
    {
        const char* FilePath;
        const char* Function;
        u32 Line;
    };
    
#define rstd_GetCallingInfo() rstd::calling_info{__FILE__, __FUNCTION__, __LINE__}
    
    
    using allocator_name = string<16>;
    
    struct memory_block
    {
        memory_block* Prev;
        u8* Base;
        size Used;
        size MaxHistoricalUsed;
        size Size;
    };
    
    struct arena
    {
        memory_block* MemoryBlock;
        size MinimalAllocationSize;
        u32 TempMemCount;
        rstd_DebugOnly(const char* DebugName;)
    };
    
    struct push_size_uninitialized_ex_res
    {
        u8* Memory;
        size GarbageBytes;
        rstd_DebugOnly(rstd_bool NewMemoryBlockWasAllocated;)
    };
    
    struct temporary_memory
    {
        arena* Arena;
        memory_block* MemBlockOnBeginTemporaryMemory;
        size ArenaUsedOnBeginTemporaryMemory;
        rstd_DebugOnly(u32 DebugId;)
    };
    
#define rstd_AllocateArenaZero(_Size, ...) \
InternalAllocateArenaZero(_Size, rstd_GetCallingInfo(), __VA_ARGS__)
    
#define rstd_SubArena(_MasterArena, _Size, ...) \
InternalSubArena(_MasterArena, _Size, rstd_GetCallingInfo(), __VA_ARGS__)
    
#define rstd_PushSizeUninitialized(_Arena, _Size) \
InternalPushSizeUninitialized(_Arena, _Size, rstd_GetCallingInfo())
    
#define rstd_PushSizeZero(_Arena, _Size) \
InternalPushSizeZero(_Arena, _Size, rstd_GetCallingInfo())
    
#define rstd_PushStructUninitialized(_Arena, _Type) \
(*(_Type*)(rstd_PushSizeUninitialized(_Arena, sizeof(_Type))))
    
#define rstd_PushStructZero(_Arena, _Type) \
(*(_Type*)(rstd_PushSizeZero(_Arena, sizeof(_Type))))
    
#define rstd_PushArrayUninitialized(_Arena, _Type, _ArrayCount) \
(_Type*)(rstd_PushSizeUninitialized(_Arena, sizeof(_Type) * _ArrayCount))
    
#define rstd_PushArrayZero(_Arena, _Type, _ArrayCount) \
(_Type*)(rstd_PushSizeZero(_Arena, sizeof(_Type) * _ArrayCount))
    
#define rstd_PushStringCopy(_Arena, _InitString) \
InternalPushStringCopy(_Arena, _InitString, rstd_GetCallingInfo())
    
    namespace MemoryDebug
    {
        
        enum class allocation_type
        {
            ArenaPushUninitialized,
            ArenaPushZero,
            ArenaPushStringCopy,
            GenAlloc,
        };
        
        static void DummyScopeMemoryGroup(){}
        
#if rstd_MemoryProfilerEnabled
#define rstd_MemoryProfilerFunctionSignature
#else
#define rstd_MemoryProfilerFunctionSignature {}
#define ScopeMemoryGroup(Name) DummyScopeMemoryGroup()
#endif
        
        static void Init() rstd_MemoryProfilerFunctionSignature;
        static void BeginMemoryGroup(const char*) rstd_MemoryProfilerFunctionSignature;
        static void EndMemoryGroup() rstd_MemoryProfilerFunctionSignature;
        static void NextCallMemoryGroup(const char*) rstd_MemoryProfilerFunctionSignature;
        static void RegisterArenaPush(arena, push_size_uninitialized_ex_res, size Size, allocation_type, calling_info) rstd_MemoryProfilerFunctionSignature;
        static void RegisterGenAlloc(void* Memory, size Size, calling_info) rstd_MemoryProfilerFunctionSignature;
        static void RegisterGenFree(void* Memory, calling_info) rstd_MemoryProfilerFunctionSignature;
        static void RegisterCreateArena(arena&, const char* ArenaName, const char* MasterArenaName, calling_info) rstd_MemoryProfilerFunctionSignature;
        static void RegisterDeallocateArena(arena, calling_info) rstd_MemoryProfilerFunctionSignature;
        static void RegisterArenaAllocateNextMemoryBlock(arena) rstd_MemoryProfilerFunctionSignature;
        static void RegisterArenaDeallocateMemoryBlock(arena) rstd_MemoryProfilerFunctionSignature;
        static void RegisterBeginTemporaryMemory(temporary_memory TempMem) rstd_MemoryProfilerFunctionSignature;
        static void RegisterEndTemporaryMemory(temporary_memory TempMem) rstd_MemoryProfilerFunctionSignature;
        
    }
    
    template<class type> static type BytesToKilobytes
    (type Bytes)
    { 
        type Kilobytes = Bytes / 1024;
        return Kilobytes;
    }
    
    template<class type> static type BytesToMegabytes
    (type Bytes)
    { 
        type Megabytes = Bytes / 1024 / 1024;
        return Megabytes;
    }
    
    template<class type> static type KilobytesToBytes(type Kilobytes)
    { return Kilobytes * 1024; }
    
    template<class type> static type MegabytesToBytes(type Megabytes)
    { return Megabytes * 1024 * 1024; }
    
    namespace memory_size_literals
    {
        constexpr size operator"" _B(unsigned long long Bytes)
        { return (size)Bytes; }
        
        constexpr size operator"" _KB(unsigned long long Kilobytes)
        { return (size)Kilobytes * 1024; }
        
        constexpr size operator"" _MB(unsigned long long Megabytes)
        { return (size)Megabytes * 1024 * 1024; }
    }
    
    constexpr size MemoryPageSize = 4*1024; // 4_KB
    
    static size Align(size Size, size Alignment)
    { return (Size + Alignment - 1) & ~(Alignment - 1); }
    
    constexpr size ConstAlign(size Size, size Alignment)
    { return (Size + Alignment - 1) & ~(Alignment - 1); }
    
    static void ZeroOut(void* Ptr, size Size)
    { memset(Ptr, 0, Size); }
    
    template<class type> static void ZeroOutStruct(type& Instance)
    { ZeroOut(&Instance, sizeof(type)); }
    
    static rstd_bool Equal(void* A, void* B, size Size)
    { return memcmp(A, B, Size) == 0; }
    
    template<class type> static rstd_bool Equal(type& A, type& B)
    { return Equal(&A, &B, sizeof(type)); }
    
    static rstd_bool IsMemoryInitializedToZero
    (void* Memory, u32 MemorySize)
    {
        u8* Mem = (u8*)Memory;
        rstd_For(Byte, MemorySize)
        {
            if(Mem[Byte] != 0)
                return false;
        }
        return true;
    }
    
    template<class type> static rstd_bool IsStructInitializedToZero(type& Instance)
    { return IsMemoryInitializedToZero(&Instance, sizeof(type)); }
    
    static string<> GetChoppedSizeText
    (u64 Size)
    {
        string<> Res;
        
        u64 Megabytes = BytesToMegabytes(Size);
        u64 Kilobytes = BytesToKilobytes(Size - MegabytesToBytes(Megabytes));
        u64 Bytes = Size - MegabytesToBytes(Megabytes) - KilobytesToBytes(Kilobytes);
        
        if(Megabytes && Kilobytes && Bytes)
            Res = Format("% MB + % KB + % B", Megabytes, Kilobytes, Bytes);
        else if(Megabytes && Kilobytes)
            Res = Format("% MB + % KB", Megabytes, Kilobytes);
        else if(Megabytes && Bytes)
            Res = Format("% MB + % B", Megabytes, Bytes);
        else if(Kilobytes && Bytes)
            Res = Format("% KB + % B", Kilobytes, Bytes);
        else if(Megabytes)
            Res = Format("% MB", Megabytes);
        else if(Kilobytes)
            Res = Format("% KB", Kilobytes);
        else
            Res = Format("% B", Bytes);
        
        return Res;
    }
    
    static string<> GetChoppedSizeText(u32 Size)
    { return GetChoppedSizeText((u64)Size); }
    
    static void* InternalGenAllocUninitialized
    (u32 Size, calling_info CallingInfo)
    {
        rstd_MemoryProfileFunction;
        void* Memory = malloc(Size);
        rstd_RAssert(Memory, "malloc failed (probably you don't have any more memory in your machine)");
        MemoryDebug::RegisterGenAlloc(Memory, Size, CallingInfo);
        return Memory;
    }
    
    static void* InternalGenAllocZero
    (u32 Size, calling_info CallingInfo)
    {
        void* Memory = InternalGenAllocUninitialized(Size, CallingInfo);
        memset(Memory, 0, Size);
        return Memory;
    }
    
    static void InternalGenFree
    (void* Memory, calling_info CallingInfo)
    {
        rstd_MemoryProfileFunction;
        MemoryDebug::RegisterGenFree(Memory, CallingInfo);
        free(Memory);
    }
    
    static void InternalGenFreeAndSetPtrToNull
    (void** Memory, calling_info CallingInfo)
    {
        InternalGenFree(*Memory, CallingInfo);
        *Memory = nullptr;
    }
    
#define GenAllocSizeUninitialized(_Size) InternalGenAllocUninitialized(_Size, rstd_GetCallingInfo())
#define GenAllocStructUninitialized(_Type) (_Type*)GenAllocSizeUninitialized(sizeof(_Type))
#define GenAllocArrayUninitialized(_Type, _Count) (_Type*)GenAllocSizeUninitialized(sizeof(_Type) * _Count)
#define GenAllocSizeZero(_Size) InternalGenAllocZero(_Size, rstd_GetCallingInfo())
#define GenAllocStructZero(_Type) (_Type*)GenAllocSizeZero(sizeof(_Type))
#define GenAllocArrayZero(_Type, _Count) (_Type*)GenAllocSizeZero(sizeof(_Type) * _Count)
#define GenFree(_Memory) InternalGenFree((void*)_Memory, rstd_GetCallingInfo())
#define GenFreeAndSetPtrToNull(_Memory) InternalGenFreeAndSetPtrToNull((void**)&_Memory, rstd_GetCallingInfo())
    
    void* PageAlloc(size Bytes);
    void PageFree(void* Memory);
    
    static push_size_uninitialized_ex_res PushSizeUninitializedEx
    (arena& Arena, size Size)
    {
        rstd_MemoryProfileFunction;
        
        push_size_uninitialized_ex_res Res = {};
        
        auto* MemBlock = Arena.MemoryBlock;
        size UsedBeforeAllocation = MemBlock->Used;
        size UsedAfterAllocation = UsedBeforeAllocation + Size;
        
        if(UsedAfterAllocation > MemBlock->Size)
        {
            rstd_Assert(Arena.MinimalAllocationSize >= MemoryPageSize);
            
            auto Max = [](size A, size B){ return A > B ? A : B; };
            size AllocationSize = Max(Size + (size)sizeof(memory_block), Arena.MinimalAllocationSize);
            u8* NewBase = (u8*)PageAlloc(AllocationSize);
            rstd_RAssert(NewBase, "OS Allocation call failed (probably your machine ran out of memory)");
            
            size NewMemBlockSize = AllocationSize - sizeof(memory_block);
            auto* NewMemBlock = (memory_block*)(NewBase + NewMemBlockSize);
            NewMemBlock->Prev = MemBlock;
            NewMemBlock->Base = NewBase;
            NewMemBlock->Used = Size;
            NewMemBlock->MaxHistoricalUsed = Size;
            NewMemBlock->Size = NewMemBlockSize;
            Arena.MemoryBlock = NewMemBlock;
            
            Res.Memory = NewMemBlock->Base;
            
            MemoryDebug::RegisterArenaAllocateNextMemoryBlock(Arena);
        }
        else
        {
            Res.Memory = MemBlock->Base + MemBlock->Used;
            
            if(UsedBeforeAllocation < MemBlock->MaxHistoricalUsed)
                Res.GarbageBytes = MemBlock->MaxHistoricalUsed - UsedBeforeAllocation;
            
            if(UsedAfterAllocation > MemBlock->MaxHistoricalUsed)
                MemBlock->MaxHistoricalUsed = UsedAfterAllocation;
            
            MemBlock->Used = UsedAfterAllocation;
        }
        
        return Res;
    }
    
    static u8* InternalPushSizeUninitialized
    (arena& Arena, size Size, calling_info CallingInfo)
    { 
        rstd_MemoryProfileFunction;
        
        auto Res = PushSizeUninitializedEx(Arena, Size);
        MemoryDebug::RegisterArenaPush(Arena, Res, Size,
                                       MemoryDebug::allocation_type::ArenaPushUninitialized,
                                       CallingInfo);
        return Res.Memory;
    }
    
    static u8* InternalPushSizeZero
    (arena& Arena, u32 Size, calling_info CallingInfo)
    {
        rstd_MemoryProfileFunction;
        
        auto Res = PushSizeUninitializedEx(Arena, Size);
        if(Res.GarbageBytes)
            ZeroOut(Res.Memory, Res.GarbageBytes);
        
        MemoryDebug::RegisterArenaPush(Arena, Res, Size,
                                       MemoryDebug::allocation_type::ArenaPushZero, CallingInfo);
        
        return Res.Memory;
    }
    
    static char* InternalPushStringCopy
    (arena& Arena, const char* InitString, calling_info CallingInfo)
    {
        rstd_MemoryProfileFunction;
        
        u32 LengthWithNullChar = (u32)strlen(InitString) + 1;
        auto Res = PushSizeUninitializedEx(Arena, LengthWithNullChar);
        char* String = (char*)Res.Memory;
        strcpy(String, InitString);
        
        MemoryDebug::RegisterArenaPush(Arena, Res, LengthWithNullChar,
                                       MemoryDebug::allocation_type::ArenaPushStringCopy, CallingInfo);
        
        return String;
    }
    
    template<class type> static type& PushStruct
    (arena& Arena, const type& Init)
    {
        auto& A = rstd_PushStructUninitialized(Arena, type);
        A = Init;
        return A;
    }
    
    static arena InternalAllocateArenaZero
    (size Size, calling_info CallingInfo, const char* DebugName = nullptr)
    {
        arena Arena;
        
        Size += sizeof(memory_block);
        Size = Align(Size, MemoryPageSize);
        u8* Base = (u8*)PageAlloc(Size);
        Size -= sizeof(memory_block);
        
        auto* MemBlock = (memory_block*)(Base + Size);
        MemBlock->Base = Base;
        MemBlock->Size = Size;
        Arena.MemoryBlock = MemBlock;
        
        Arena.MinimalAllocationSize = MegabytesToBytes(1);
        Arena.TempMemCount = 0;
        
        MemoryDebug::RegisterCreateArena(Arena, DebugName, nullptr, CallingInfo);
        
        return Arena;
    }
    
    static arena InternalSubArena
    (arena& MasterArena, size Size, calling_info CallingInfo, const char* DebugName = nullptr)
    {
        rstd_Assert(MasterArena.TempMemCount == 0);
        
        arena SubArena;
        
        u8* Base = InternalPushSizeUninitialized(MasterArena,
                                                 Size + sizeof(memory_block), CallingInfo);
        
        memory_block* MemBlock = (memory_block*)(Base + Size);
        ZeroOutStruct(*MemBlock);
        MemBlock->Base = Base;
        MemBlock->Size = Size;
        SubArena.MemoryBlock = MemBlock;
        
        SubArena.MinimalAllocationSize = MasterArena.MinimalAllocationSize;
        SubArena.TempMemCount = 0;
        
#if rstd_Debug
        MemoryDebug::RegisterCreateArena(SubArena, DebugName, MasterArena.DebugName, CallingInfo);
#endif
        
        return SubArena;
    }
    
    static void Clear
    (arena& Arena)
    {
        rstd_AssertM(Arena.TempMemCount == 0, "You forgot to call EndTemporaryMemory()");
        
        while(Arena.MemoryBlock->Prev)
        {
            auto* PrevMemBlock = Arena.MemoryBlock->Prev;
            MemoryDebug::RegisterArenaDeallocateMemoryBlock(Arena);
            PageFree(Arena.MemoryBlock->Base);
            Arena.MemoryBlock = PrevMemBlock;
        }
        Arena.MemoryBlock->Used = 0;
    }
    
    // TODO: Can't we just store arena copy on stack?
    static void DeallocateArenaStoredInItself
    (arena& Arena)
    {
        auto* MemBlock = Arena.MemoryBlock;
        while(MemBlock)
        {
            auto* PrevMemBlock = MemBlock->Prev;
            MemoryDebug::RegisterArenaDeallocateMemoryBlock(Arena);
            // TODO: Don't try to deallocate first block if it comes from SubArena()
            PageFree(MemBlock);
            MemBlock = PrevMemBlock;
        }
    }
    
    static void DeallocateArena
    (arena& Arena)
    {
        DeallocateArenaStoredInItself(Arena);
        Arena.MemoryBlock = nullptr;
    }
    
    static rstd_bool AllocatedFromSingleOsAllocationCall(arena& Arena)
    { return !Arena.MemoryBlock->Prev; }
    
    static size GetUsed
    (arena& Arena)
    {
        size Bytes = 0;
        auto* MemBlock = Arena.MemoryBlock;
        while(MemBlock)
        {
            Bytes += MemBlock->Used;
            MemBlock = MemBlock->Prev;
        }
        return Bytes;
    }
    
    static size GetUsedWithNewBlockAllocationAlignment
    (arena& Arena)
    {
        size Bytes = 0;
        auto* MemBlock = Arena.MemoryBlock;
        if(MemBlock)
        {
            Bytes = MemBlock->Used;
            while(MemBlock->Prev)
            {
                MemBlock = MemBlock->Prev;
                Bytes += MemBlock->Size;
            }
        }
        return Bytes;
    }
    
    static size GetAllocatedBytes
    (arena& Arena)
    {
        size Bytes = 0;
        auto* MemBlock = Arena.MemoryBlock;
        while(MemBlock)
        {
            Bytes += MemBlock->Size;
            MemBlock = MemBlock->Prev;
        }
        return Bytes;
    }
    
    static size GetUnusedBytes(memory_block MemBlock)
    { return MemBlock.Size - MemBlock.Used; }
    
    static temporary_memory BeginTemporaryMemory
    (arena& Arena)
    {
        temporary_memory TempMem;
        TempMem.Arena = &Arena;
        TempMem.MemBlockOnBeginTemporaryMemory = Arena.MemoryBlock;
        TempMem.ArenaUsedOnBeginTemporaryMemory = Arena.MemoryBlock->Used;
        ++Arena.TempMemCount;
        rstd_DebugOnly(TempMem.DebugId = Arena.TempMemCount;)
            MemoryDebug::RegisterBeginTemporaryMemory(TempMem);
        return TempMem;
    }
    
    static void EndTemporaryMemory
    (temporary_memory TempMem)
    {
        rstd_AssertM(TempMem.DebugId == TempMem.Arena->TempMemCount, "Temporary memory that was created most recently has to be ended first");
        
        auto* Arena = TempMem.Arena;
        while(Arena->MemoryBlock != TempMem.MemBlockOnBeginTemporaryMemory)
        {
            auto* PrevMemBlock = Arena->MemoryBlock->Prev;
            PageFree(Arena->MemoryBlock->Base);
            Arena->MemoryBlock = PrevMemBlock;
        }
        
        TempMem.Arena->MemoryBlock->Used = TempMem.ArenaUsedOnBeginTemporaryMemory;
        --TempMem.Arena->TempMemCount;
        
        MemoryDebug::RegisterEndTemporaryMemory(TempMem);
    }
    
    struct scope_temporary_memory
    {    
        temporary_memory TempMem;
        
        scope_temporary_memory(arena& Arena)
        { TempMem = BeginTemporaryMemory(Arena); }
        
        ~scope_temporary_memory()
        { EndTemporaryMemory(TempMem); }
    };
    
#define ScopeTemporaryMemory(Arena) scope_temporary_memory ScopeTempMem##__LINE__(Arena)
    
    
    struct arena_revert_point
    {
        memory_block* MemBlock;
        size Used;
    };
    
    static arena_revert_point GetArenaRevertPoint(arena Arena)
    { return {Arena.MemoryBlock, Arena.MemoryBlock->Used}; }
    
    static void RevertArena
    (arena& Arena, arena_revert_point RevertPoint)
    {
        while(Arena.MemoryBlock != RevertPoint.MemBlock)
        {
            auto* PrevArenaMemBlock = Arena.MemoryBlock->Prev;
            PageFree(Arena.MemoryBlock->Base);
            Arena.MemoryBlock = PrevArenaMemBlock;
        }
        rstd_Assert(Arena.MemoryBlock);
        Arena.MemoryBlock->Used = RevertPoint.Used;
    }
    
    struct arena_ref
    {
        arena_ref()
            :ArenaPtr(nullptr)
            ,OwnsArena(false)
        {}
        
        arena* Ptr()
        { return OwnsArena ? &Arena : ArenaPtr; }
        
        arena* operator->()
        { return Ptr(); }
        
        arena& operator*()
        { return *Ptr(); }
        
        operator rstd_bool()
        { return ArenaPtr != nullptr; }
        
        union
        {
            arena Arena;
            arena* ArenaPtr;
        };
        rstd_bool OwnsArena;
    };
    
    static arena_ref ShareArena
    (arena& Arena)
    {
        arena_ref Ref;
        Ref.ArenaPtr = &Arena;
        Ref.OwnsArena = false;
        return Ref;
    }
    
    static arena_ref OwnArena
    (const arena& Arena)
    {
        arena_ref Ref;
        Ref.Arena = Arena;
        Ref.OwnsArena = true;
        return Ref;
    }
    
    template<class dest_container, class source_container> void CopyElements
    (dest_container* Dest, source_container& Source)
    {
        Dest->Clear();
        for(auto& E : Source)
            Dest->Push(E);
    }
    
#ifdef rstd_DefaultArena
    
#ifndef rstd_DefaultArenaInitialSize
#define rstd_DefaultArenaInitialSize MegabytesToBytes(8)
#endif
    
    arena DefaultArena = InternalAllocateArenaZero(rstd_DefaultArenaInitialSize, calling_info(__FILE__, "none", __LINE__), "Default arena");
#endif
    ////////////////////////////////
    // CONTAINER COMMON FUNCTIONS //
    ////////////////////////////////
    template<class compare_type>           
        compare_type* FindEqual                            
    (auto& Container, const compare_type& ThingToComare)        
    {                                          
        for(auto& Element : Container)         
        {                                      
            if(Element == ThingToComare)               
                return &Element;               
        }
        return (decltype(Container.GetVariableOfElementType())*)nullptr;
    }  
    
    auto* Find                           
    (auto& Container, auto ComparisonFunc)
    {                                    
        for(auto& Element : Container)       
        {                                
            if(ComparisonFunc(Element))      
                return &Element;         
        }                                
        return (decltype(Container.GetVariableOfElementType())*)nullptr;
    }                                    
    
    auto& FindWithAssert 
    (auto& Container, auto ComparisonFunc) 
    { 
        auto* Found = Find(Container, ComparisonFunc); 
        rstd_Assert(Found); 
        return *Found; 
    } 
    
    rstd_bool HasEqual 
    (auto& Container, const auto& ThingToCompare) 
    { return (rstd_bool)FindEqual(Container, ThingToCompare); } 
    
    rstd_bool Has 
    (auto& Container, auto ComparisonFunc) 
    { return (rstd_bool)Find(Container, ComparisonFunc); } 
    
    u32 FindIndexOfFirstEqual 
    (auto& Container, const auto& ThingToCompare) 
    { 
        u32 ElementIndex = 0; 
        for(auto& Element : Container) 
        { 
            if(Element == ThingToCompare) 
                return ElementIndex;
            ++ElementIndex; 
        } 
        return InvalidU32; 
    } 
    
    u32 FindIndexOfFirst 
    (auto& Container, auto ComparisonFunc) 
    { 
        u32 ElementIndex = 0;
        for(auto& Element : Container) 
        { 
            if(ComparisonFunc(Element)) 
                return ElementIndex; 
            ++ElementIndex; 
        } 
        return InvalidU32; 
    } 
    
    u32 HowManyEqualHas 
    (auto& Container, const auto& ThingToCompare) 
    { 
        u32 Res = 0; 
        for(auto& Element : Container) 
        { 
            if(Element == ThingToCompare) 
                ++Res; 
        } 
        return Res; 
    } 
    
    u32 HowManyHas 
    (auto& Container, auto ComparisonFunc) 
    { 
        u32 Res = 0; 
        for(auto& Element : Container)
        { 
            if(ComparisonFunc(Element))
                ++Res; 
        } 
        return Res;
    } 
    
    template<class value_type> struct get_value_and_index_result 
    { 
        value_type* Value; 
        u32 Index;
        
        operator rstd_bool() 
        { return Value; } 
        
        value_type* operator->() 
        {      
            rstd_Assert(Value); 
            return Value; 
        } 
    }; 
    
    template<class value_type>
        get_value_and_index_result<value_type> FindValueAndIndex 
    (const auto& Container, auto ComparisonFunc) 
    { 
        get_value_and_index_result<value_type> Res = {}; 
        for(auto& E : Container) 
        { 
            if(ComparisonFunc(E))
            {
                Res.Value = &E; 
                break;
            }
            ++Res.Index;
        }
        return Res;
    }
    
    void ArraylikeRemovePointerAsserts
    (auto& Container, auto* E) 
    {
        rstd_Assert(!Container.Empty());
        auto* Elements = Container.Elements;
        rstd_Assert(E >= Elements);
        rstd_Assert(E < Elements + Container.Count);
    }
    
    auto* ArraylikeRemove
    (auto& Container, auto* E)
    {
        auto* Elements = Container.Elements;
        u32& Count = Container.Count;
        ArraylikeRemovePointerAsserts(Container, E);
        if(E < Elements + Count - 1)
            *E = Elements[Count - 1];
        --Count;
        return E - 1;
    }
    
    auto* ArraylikeRemoveAndPersistOrder
    (auto& Container, auto* E)
    {
        auto* Elements = Container.Elements;
        u32& Count = Container.Count;
        ArraylikeRemovePointerAsserts(Container, E);
        if(&E < Elements + Count - 1)
            memcpy(&E, &E + 1, (size_t)(((char*)(Elements + Count) - (char*)(&E))));
        --Count;
        return &E - 1;
    }
    
    void ArraylikeRemove
    (auto& Container, u32 Index)
    {
        auto* Elements = Container.Elements;
        u32& Count = Container.Count;
        rstd_Assert(!Container.Empty());
        rstd_Assert(Index >= 0);
        rstd_Assert(Index < Count);
        if(Index < Count - 1)
            Elements[Index] = Elements[Count - 1];
        --Count;
    }
    
    ///////////
    // ARRAY //
    ///////////
    template<class type, u32 size>
        struct array
    {
        using iterator = type*;
        
        type Elements[size];
        
        array(){}
        
        iterator Begin()
        { return Elements; }
        
        iterator End()
        { return Elements + size; }
        
        internal_rstd_RestOfIteratorFunctions;
        
        type& operator[]
        (u32 Index)
        {
            rstd_AssertM(Index < size,
                         "You tried to get element [%], but this array has only % elements", Index, size);
            return Elements[Index];
        }
        
        constexpr u32 GetCount()
        { return size; }
        
        void Zero()
        { Zero(Elements, size * sizeof(type)); }
        
        type GetVariableOfElementType()
        { return Elements[0]; }
        
        type& GetFirst()
        { return *Elements; }
        
        type& GetLast()
        { return *(Elements + size - 1); }
        
        type* FindEqual(const auto& ThingToComare)        
        { return ::FindEqual(*this, ThingToComare); }
        
        type* Find(auto ComparisonFunc)           
        { return ::Find(*this, ComparisonFunc); }                                    
        
        type& FindWithAssert(auto ComparisonFunc)
        { return ::FindWithAssert(*this, ComparisonFunc); }
        
        rstd_bool HasEqual(const auto& ThingToCompare) 
        { return ::HasEqual(*this, ThingToCompare); } 
        
        rstd_bool Has(auto ComparisonFunc) 
        { return ::Find(ComparisonFunc); }
        
        u32 FindIndexOfFirstEqual(const auto& ThingToCompare)
        {  return ::FindIndexOfFirstEqual(*this, ThingToCompare); } 
        
        u32 FindIndexOfFirst(auto ComparisonFunc) 
        { return ::FindIndexOfFirst(*this, ComparisonFunc); } 
        
        u32 HowManyEqualHas(const auto& ThingToCompare) 
        { return ::HowManyEqualHas(*this, ThingToCompare); } 
        
        u32 HowManyHas(auto ComparisonFunc) 
        { return ::HowManyHas(*this, ComparisonFunc); } 
        
        get_value_and_index_result<type> FindValueAndIndex 
        (auto ComparisonFunc) 
        { return ::FindValueAndIndex<type>(*this, ComparisonFunc); }
        
        rstd_bool HasPtr(const type* Ptr) 
        { return Ptr >= Elements && Ptr < Elements + size; } 
        
        u32 GetIndexFromPtr 
        (const type* Ptr) 
        { 
            /* TODO: rstd_Assert(Is aligned to array elements) */ 
            rstd_Assert(HasPtr(Ptr)); 
            return (u32)((size_t)(Ptr - Elements)); 
        }
    };
    
    ////////////////////
    // PUSHABLE ARRAY //
    ////////////////////
    template<class type, u32 size>
        struct pushable_array
    {
        using iterator = type*;
        
        type Elements[size];
        u32 Count;
        
        pushable_array() 
        { Count = 0; }
        
        template<class... args>
            pushable_array
        (const args&&... InitElements)
        {
            Count = 0;
            Push(InitElements...);
        }
        
        iterator Begin()
        { return Elements; }
        
        iterator End()
        { return Elements + Count; }
        
        internal_rstd_RestOfIteratorFunctions;
        
        type& operator[]
        (u32 Index)
        {
            rstd_AssertM(Index < Count,
                         "You tried to get element [%], but this pushable_array has only % elements", Index, Count);
            return Elements[Index];
        }
        
        type GetVariableOfElementType()
        { return Elements[0]; }
        
        static constexpr u32 GetMaxCount()
        { return size; }
        
        u32 GetCount()
        { return Count; }
        
        void Clear()
        { Count = 0; }
        
        void Zero()
        {
            Zero(Elements, Count * sizeof(type));
            Count = 0;
        }
        
        rstd_bool Full()
        { return Count == size; }
        
        rstd_bool Empty()
        { return Count == 0; }
        
        type& GetFirst()
        {
            rstd_Assert(!Empty());
            return *Elements; 
        }
        
        type& GetLast()
        { 
            rstd_Assert(!Empty());
            return Elements[Count - 1]; 
        }
        
        type& PushUninitialized()
        {
            rstd_Assert(!Full());
            ++Count;
            return Elements[Count - 1];
        }
        
        type& PushZero()
        {
            auto& Data = PushUninitialized();
            ZeroOutStruct(Data);
            return Data;
        }
        
        type* PushZeroIfNotFull()
        {
            if(!Full())
                return &PushZero();
            return nullptr;
        }
        
        type* PushUninitializedIfNotFull()
        {
            if(!Full())
                return &PushUninitialized();
            return nullptr;
        }
        
        void PushMany
        (type* Elems, u32 ElemsCount)
        {
            Assert(Count + ElemsCount < size);
            memcpy(Elements + Count, Elems, ElemsCount * sizeof(type));
        }
        
        void Pop()
        {
            rstd_Assert(!Empty());
            --Count;
        }
        
        type* Remove(type* E)
        { return ::ArraylikeRemove(*this, E); }
        
        void Remove(type& E)
        { Remove(&E); }
        
        type* RemoveAndPersistOrder(type* E)
        { ::ArraylikeRemoveAndPersistOrder(*this, E); }
        
        void RemoveAndPersistOrder(type& E)
        { RemoveAndPersistOrder(&E); }
        
        void Remove(u32 Index)
        { ::ArraylikeRemove(*this, Index); }
        
        void PopFirst()
        { Remove(Elements); }
        
        void PopLast()
        {
            rstd_Assert(!Empty());
            --Count;
        }
        
        void PopFrontAndPersistOrder()
        { RemoveAndPersistOrder(Elements); }
        
        template<class comparison_fn>
            void Sort(comparison_fn Comparison)
        { std::sort(Elements, Elements + Count, Comparison); }
        
        template<class comparison_fn>
            void StableSort(comparison_fn Comparison)
        { std::stable_sort(Elements, Elements + Count, Comparison); }
        
        type* FindEqual(const auto& ThingToComare)        
        { return ::FindEqual(*this, ThingToComare); }                                          
        
        type* Find(auto ComparisonFunc)
        { return ::Find(*this, ComparisonFunc); }                                    
        
        type& FindWithAssert(auto ComparisonFunc) 
        { return ::FindWithAssert(*this, ComparisonFunc); } 
        
        rstd_bool HasEqual(const auto& ThingToCompare) 
        { return ::HasEqual(*this, ThingToCompare); } 
        
        rstd_bool Has(auto ComparisonFunc) 
        { return ::Find(*this, ComparisonFunc); }
        
        u32 FindIndexOfFirstEqual(const auto& ThingToCompare)
        {  return ::FindIndexOfFirstEqual(*this, ThingToCompare); } 
        
        u32 FindIndexOfFirst(auto ComparisonFunc) 
        { return ::FindIndexOfFirst(*this, ComparisonFunc); } 
        
        u32 HowManyEqualHas(const auto& ThingToCompare) 
        { return ::HowManyEqualHas(*this, ThingToCompare); } 
        
        u32 HowManyHas(auto ComparisonFunc) 
        { return ::HowManyHas(*this, ComparisonFunc); } 
        
        get_value_and_index_result<type> FindValueAndIndex 
        (auto ComparisonFunc) 
        { return ::FindValueAndIndex<type>(*this, ComparisonFunc); }
        
        type& Push 
        (const type& InitialData) 
        { 
            auto& Data = PushUninitialized(); 
            Data = InitialData; 
            return Data; 
        } 
        
        type& PushDefault() 
        { 
            type E; 
            return Push(E); 
        } 
        
        type* PushIfNotFull 
        (const type& InitialData) 
        { 
            if(!Full()) 
                return &Push(InitialData); 
            return nullptr; 
        } 
        
        type* PushIfUnique 
        (const type& InitialData) 
        { 
            if(!HasEqual(InitialData)) 
                return &Push(InitialData); 
            return nullptr; 
        } 
        
        type* PushIfUnique
        (const type& InitialData, auto ComparisonFunc) 
        { 
            if(!Has(ComparisonFunc)) 
                return &Push(InitialData); 
            return nullptr; 
        } 
        
        type& GetIfHasEqualOrPush
        (const type& ThingToCompareOrInitialData) 
        { 
            if(auto* Found = FindEqual(ThingToCompareOrInitialData)) 
                return *Found;
            return Push(ThingToCompareOrInitialData);
        } 
        
        type& GetIfHasOrPush
        (auto ComparisonFunc, const type& InitialData) 
        { 
            if(auto* Found = Find(ComparisonFunc)) 
                return *Found; 
            return Push(InitialData); 
        } 
        
        template<class... args> void Push 
        (const type& CurrentPushElement, const args&&... NextPushElements) 
        { 
            Push(CurrentPushElement); 
            Push(NextPushElements...); 
        }
        
        template<class compare_type> 
            rstd_bool RemoveFirstEqualTo 
        (const compare_type& ThingToCompare) 
        { 
            if(auto* Found = FindEqual(ThingToCompare)) 
            { 
                Remove(*Found); 
                return true; 
            } 
            return false; 
        } 
        
        template<class compare_type> 
            void RemoveFirstEqualToWithAssert 
        (const compare_type& ThingToCompare) 
        { 
            bool ManagedToRemove = RemoveFirstEqualTo(ThingToCompare); 
            rstd_Assert(ManagedToRemove); 
        } 
        
        template<class comparison_fn> 
            rstd_bool RemoveFirstIf 
        (comparison_fn Comparison) 
        { 
            if(auto* Found = Find(Comparison)) 
            { 
                Remove(*Found); 
                return true; 
            } 
            return false; 
        } 
        
        template<class comparison_fn> 
            void RemoveFirstIfWithAssert 
        (comparison_fn Comparison) 
        { 
            bool ManagedToRemove = RemoveFirstIf(Comparison); 
            rstd_Assert(ManagedToRemove); 
        } 
        
        template<class comparison_fn> 
            optional<type> RemoveFirstIfReturnCopy 
        (comparison_fn Comparison) 
        { 
            if(auto* Found = Find(Comparison)) 
            { 
                Remove(*Found); 
                return {*Found}; 
            } 
            return {}; 
        } 
        
        template<class comparison_fn> 
            u32 RemoveIf 
        (comparison_fn Comparison) 
        { 
            u32 RemovedCount = 0; 
            for(auto It = Begin(); It != End(); ++It) 
            { 
                if(Comparison(*It)) 
                { 
                    Remove(It); 
                    ++RemovedCount; 
                } 
            } 
            return RemovedCount; 
        } 
        
        type GetAndPopFirst() 
        { 
            auto FirstCopy = GetFirst(); 
            PopFirst(); 
            return FirstCopy; 
        } 
        
        rstd_bool PopFirstIfNotEmpty() 
        { 
            if(Empty()) 
            { 
                return false; 
            } 
            else 
            { 
                PopFirst(); 
                return true; 
            } 
        } 
        
        optional<type> GetAndPopFirstIfNotEmpty() 
        { 
            if(Empty()) 
            { 
                return {}; 
            } 
            else 
            { 
                auto* First = &GetFirst(); 
                type FirstCopy = *First; 
                Remove(First); 
                return FirstCopy; 
            } 
        } 
        
        rstd_bool PopLastIfNotEmpty() 
        { 
            if(Empty()) 
            { 
                return false; 
            } 
            else 
            { 
                PopLast(); 
                return true; 
            } 
        } 
        
        type GetAndPopLast() 
        { 
            type LastCopy = GetLast(); 
            PopLast(); 
            return LastCopy; 
        } 
        
        optional<type> GetAndPopLastIfNotEmpty() 
        { 
            if(Empty()) 
            { 
                return {}; 
            } 
            else 
            { 
                auto* Last = &GetLast(); 
                type LastCopy = *Last;
                Remove(Last); 
                return LastCopy; 
            } 
        }
        
        rstd_bool HasPtr(const type* Ptr) 
        { return Ptr >= Elements && Ptr < Elements + Count; } 
        
        u32 GetIndexFromPtr 
        (const type* Ptr) 
        { 
            /* TODO: rstd_Assert(Is aligned to array elements) */ 
            rstd_Assert(HasPtr(Ptr)); 
            return (u32)((size_t)(Ptr - Elements)); 
        }
        
        pushable_array<type, size> SubArray
        (auto Comparison)
        {
            pushable_array<type, size> Res;
            
            for(auto& E : *this)
            {
                if(Comparison(E))
                    Res.Push(E);
            }
            
            return Res;
        }
    };
    
    ///////////////////
    // DYNAMIC ARRAY //
    ///////////////////
    template<class type> struct dynamic_array
    {
        using iterator = type*;
        
        type* Elements;
        u32 Count;
        u32 AllocatedCount;
        arena_ref ArenaRef;
        
        void Init
        (arena_ref ArenaRef_, u32 AllocationCount)
        {
            ArenaRef = ArenaRef_;
            Count = 0;
            Elements = rstd_PushArrayUninitialized(*ArenaRef, type, AllocationCount);
            AllocatedCount = AllocationCount;
        }
        
        dynamic_array(arena_ref ArenaRef, u32 AllocationCount)
        { Init(ArenaRef, AllocationCount); }
        
        dynamic_array()
        {
            Elements = nullptr;
            Count = AllocatedCount = 0;
        }
        
#ifdef rstd_DefaultArena
        dynamic_array(u32 AllocationCount)
            :dynamic_array(ShareArena(DefaultArena), AllocationCount) {}
        
        void Init(u32 AllocationCount)
        { Init(ShareArena(DefaultArena), AllocationCount); }
#endif
        
        iterator Begin()
        { return Elements; }
        
        iterator End()
        { return Elements + Count; }
        
        internal_rstd_RestOfIteratorFunctions;
        
        type GetVariableOfElementType()
        { return Elements[0]; }
        
        type& operator[]
        (u32 Index)
        {
            rstd_AssertM(Index < Count,
                         "You tried to get element [%], but this pushable_array has only % elements", Index, Count);
            return Elements[Index];
        }
        
        rstd_bool Empty()
        { return Count == 0; }
        
        u32 GetCount()
        { return Count; }
        
        void Clear()
        { Count = 0; }
        
        bool IsInitialized()
        { return AllocatedCount != 0; }
        
        void InternalPrepareSpaceForPush()
        {
            rstd_Assert(Elements);
            
            if(Count < AllocatedCount)
                return;
            
            if(ArenaRef.OwnsArena)
            {
                // reserve more space on the arena, leaving the memory where it is
                // we own the arena so we assume that nothing else has pushed the arena since our previous push
                
                // TODO: This doesn't work if arena expands, Fix it!
                rstd_PushArrayUninitialized(*ArenaRef, type, AllocatedCount);
                AllocatedCount *= 2;
            }
            else
            {
                // reserve totally new space on the arena and copy all the previously existing elements to new space
                u32 AllocationCount = AllocatedCount * 2;
                auto* NewElements = rstd_PushArrayUninitialized(*ArenaRef, type, AllocationCount);
                memcpy(NewElements, Elements, AllocatedCount * sizeof(type));
                AllocatedCount = AllocationCount;
                Elements = NewElements;
            }
        }
        
        type& PushUninitialized()
        { 
            InternalPrepareSpaceForPush();
            return Elements[Count++];
        }
        
        type& PushZero()
        { 
            InternalPrepareSpaceForPush();
            return Elements[Count++] = {};
        }
        
        type& Push 
        (const type& InitialData) 
        { 
            auto& Data = PushUninitialized(); 
            Data = InitialData; 
            return Data; 
        } 
        
        type* FindEqual(const auto& ThingToComare)        
        { return ::FindEqual(*this, ThingToComare); }
        
        type* Find(auto ComparisonFunc)
        { return ::Find(*this, ComparisonFunc); }
        
        type& FindWithAssert(auto ComparisonFunc)
        { return ::FindWithAssert(*this, ComparisonFunc); }
        
        rstd_bool HasEqual(const auto& ThingToCompare) 
        { return ::HasEqual(*this, ThingToCompare); } 
        
        rstd_bool Has(auto ComparisonFunc) 
        { return ::Find(ComparisonFunc); }
        
        u32 FindIndexOfFirstEqual(const auto& ThingToCompare)
        {  return ::FindIndexOfFirstEqual(*this, ThingToCompare); } 
        
        u32 FindIndexOfFirst(auto ComparisonFunc) 
        { return ::FindIndexOfFirst(*this, ComparisonFunc); } 
        
        u32 HowManyEqualHas(const auto& ThingToCompare) 
        { return ::HowManyEqualHas(*this, ThingToCompare); } 
        
        u32 HowManyHas(auto ComparisonFunc) 
        { return ::HowManyHas(*this, ComparisonFunc); } 
        
        get_value_and_index_result<type> FindValueAndIndex 
        (auto ComparisonFunc) 
        { return ::FindValueAndIndex<type>(*this, ComparisonFunc); }
        
        type* Remove(type* E)
        { return ArraylikeRemove(*this, E); }
        
        void Remove(type& E)
        { Remove(&E); }
        
        type* RemoveAndPersistOrder(type* E)
        { ArraylikeRemoveAndPersistOrder(*this, E); }
        
        void RemoveAndPersistOrder(type& E)
        { RemoveAndPersistOrder(&E); }
        
        void Remove(u32 Index)
        { ArraylikeRemove(*this, Index); }
        
        type& GetIfHasEqualOrPush
        (const type& ThingToCompareOrInitialData) 
        { 
            if(auto* Found = FindEqual(ThingToCompareOrInitialData)) 
                return *Found;
            return Push(ThingToCompareOrInitialData);
        } 
        
        type& GetIfHasOrPush
        (auto ComparisonFunc, const type& InitialData) 
        { 
            if(auto* Found = Find(ComparisonFunc)) 
                return *Found; 
            return Push(InitialData); 
        } 
    };
    
    /////////
    // MAP //
    /////////
    template<class key, class value, u32 size>
        struct map
    {
        template<class type, u32 size>
            struct internal_array
        {
            using iterator = type*;
            
            type Elements[size];
            u32& Count;
            
            internal_array(u32& _Count) : Count(_Count) {}
            
            iterator Begin()
            { return Elements; }
            
            iterator End()
            { return Elements + Count; }
            
            internal_rstd_RestOfIteratorFunctions;
            
            type& operator[]
            (u32 Index)
            {
                rstd_AssertM(Index < Count,
                             "You tried to get element [%], but the element count of map is only %", Index, Count);
                return Elements[Index];
            }
            
            constexpr u32 GetCount()
            { return Count; }
            
            void Zero()
            { Zero(Elements, size * sizeof(type)); }
            
            type GetVariableOfElementType()
            { return Elements[0]; }
            
            type& GetFirst()
            { return *Elements; }
            
            type& GetLast()
            { return *(Elements + Count - 1); }
            
            type* FindEqual(const auto& ThingToComare)        
            { return ::FindEqual(*this, ThingToComare); }
            
            type* Find(auto ComparisonFunc)           
            { return ::Find(*this, ComparisonFunc); }                                    
            
            type& FindWithAssert(auto ComparisonFunc)
            { return ::FindWithAssert(*this, ComparisonFunc); }
            
            rstd_bool HasEqual(const auto& ThingToCompare) 
            { return ::HasEqual(*this, ThingToCompare); } 
            
            rstd_bool Has(auto ComparisonFunc) 
            { return ::Find(ComparisonFunc); }
            
            u32 FindIndexOfFirstEqual(const auto& ThingToCompare)
            {  return ::FindIndexOfFirstEqual(*this, ThingToCompare); } 
            
            u32 FindIndexOfFirst(auto ComparisonFunc) 
            { return ::FindIndexOfFirst(*this, ComparisonFunc); } 
            
            u32 HowManyEqualHas(const auto& ThingToCompare) 
            { return ::HowManyEqualHas(*this, ThingToCompare); } 
            
            u32 HowManyHas(auto ComparisonFunc) 
            { return ::HowManyHas(*this, ComparisonFunc); } 
            
            get_value_and_index_result<type> FindValueAndIndex 
            (auto ComparisonFunc) 
            { return ::FindValueAndIndex<type>(*this, ComparisonFunc); }
            
            rstd_bool HasPtr(const type* Ptr) 
            { return Ptr >= Elements && Ptr < Elements + Count; } 
            
            u32 GetIndexFromPtr 
            (const type* Ptr) 
            { 
                /* TODO: rstd_Assert(Is aligned to array elements) */ 
                rstd_Assert(HasPtr(Ptr)); 
                return (u32)((size_t)(Ptr - Elements)); 
            }
        };
        
        internal_array<key, size> Keys;
        internal_array<value, size> Values;
        u32 Count;
        
        map() :Keys(Count), Values(Count), Count(0) {}
        
        void Clear()
        { Count = 0; }
        
        u32 GetCount()
        { return Count; }
        
        constexpr u32 GetMaxCount()
        { return size; }
        
        void Push
        (const key& Key, const value& Value)
        {
            rstd_Assert(Count < size);
            Keys.Elements[Count] = Key;
            Values.Elements[Count] = Value;
            Count++;
        }
        
        value& PushUninitialized
        (const key& Key)
        {
            rstd_Assert(Count < size);
            Keys.Elements[Count] = Key;
            auto& Res = Values.Elements[Count];
            Count++;
            return Res;
        }
        
        value* FindEqualValue
        (const key& Key)
        {
            for(u32 I = 0; I < Count; I++)
            {
                if(Keys.Elements[I] == Key)
                    return &Values.Elements[I];
            }
            return nullptr;
        }
        
        value& FindEqualValueWithAssert
        (const key& Key)
        {
            auto* Found = FindEqualValue(Key);
            rstd_Assert(Found);
            return *Found;
        }
        
        value* FindEqualValue
        (const value& Value)
        {
            for(u32 I = 0; I < Count; I++)
            {
                if(Values.Elements[I] == Value)
                    return &Values.Elements[I];
            }
            return nullptr;
        }
        
        value& FindEqualValueWithAssert
        (const value& Value)
        {
            auto* Found = FindEqualValue(Value);
            rstd_Assert(Found);
            return *Found;
        }
        
        value* FindValueFromKeyComparison
        (auto ComparisonFunc)
        {
            for(u32 I = 0; I < Count; I++)
            {
                if(ComparisonFunc(Keys.Elements[I]))
                    return &Values.Elements[I];
            }
            return nullptr;
        }
        
        value& FindValueFromKeyComparisonWithAssert
        (auto ComparisonFunc)
        {
            auto* Found = FindValueKeyComparison(ComparisonFunc);
            rstd_Assert(Found);
            return *Found;
        }
        
        value* FindValueFromValueComparison
        (auto ComparisonFunc)
        {
            for(u32 I = 0; I < Count; I++)
            {
                if(ComparisonFunc(Values.Elements[I]))
                    return &Values.Elements[I];
            }
            return nullptr;
        }
        
        value& FindValueFromValueComparisonWithAssert
        (auto ComparisonFunc)
        {
            auto* Found = FindValueFromValueComparison(ComparisonFunc);
            rstd_Assert(Found);
            return *Found;
        }
        
        key* FindKeyFromValueComparison
        (auto ComparisonFunc)
        {
            for(u32 I = 0; I < Count; I++)
            {
                if(ComparisonFunc(Values.Elements[I]))
                    return &Keys.Elements[I];
            }
            return nullptr;
        }
        
        key& FindKeyFromValueComparisonWithAssert
        (auto ComparisonFunc)
        {
            auto* Found = FindKeyFromValueComparison(ComparisonFunc);
            rstd_Assert(Found);
            return *Found;
        }
    };
    
    ////////////////////////
    // DOUBLY LINKED LIST //
    ////////////////////////
    template<class type>
        struct doubly_linked_list
    {
        // NOTE: to be more precise, it's a doubly linked list with sentinel node
        
        struct node
        {
            type Data;
            node* Next;
            node* Prev;
        };
        
        static_assert(offsetof(node, Data) == 0, "we need Data to be first in node for Remove(type*)");
        
        arena_ref ArenaRef;
        node* Sentinel;
        node* Nodes;
        node* FreeNodes;
        
        type GetVariableOfElementType()
        { return Sentinel->Data; }
        
        rstd_bool Initialized()
        { return Nodes; }
        
        doubly_linked_list
        (arena_ref ArenaRef)
        {
            this->ArenaRef = ArenaRef;
            Sentinel = &rstd_PushStructUninitialized(*ArenaRef, doubly_linked_list<type>::node);
            Nodes = Sentinel;
            Sentinel->Next = Sentinel;
            Sentinel->Prev = Sentinel;
            FreeNodes = nullptr;
        }
        
#ifdef rstd_DefaultArena
        doubly_linked_list()
            :doubly_linked_list(ShareArena(DefaultArena)) {}
#else
        doubly_linked_list()
        { Nodes = nullptr; }
#endif
        
        struct iterator
        {
            node* Node;
            rstd_DebugOnly(node* DebugSentinel;)
                
                iterator& operator++() 
            {
                Node = Node->Next;
                return *this;
            }
            
            iterator operator++(int)
            {
                auto Res = *this;
                Node = Node->Next;
                return Res;
            }
            
            iterator& operator--() 
            { 
                Node = Node->Prev;
                return *this;
            }
            
            iterator operator--(int)
            {
                auto Res = *this;
                Node = Node->Prev;
                return Res;
            }
            
            iterator& operator+=
            (u32 Offset) 
            {
                while(Offset--)
                    Node = Node->Next;
                return *this;
            }
            
            iterator& operator-=
            (u32 Offset)
            {
                while(Offset--)
                    Node = Node->Prev;
                return *this;
            }
            
            iterator operator+
            (u32 Offset)
            {
                auto Res = *this;
                return Res += Offset;
            }
            
            iterator operator-
            (u32 Offset)
            {
                auto Res = *this;
                return Res -= Offset;
            }
            
            u32 operator-
            (iterator Rhs)
            {
                u32 Diff = 0;
                while(Node != Rhs.Node)    
                {
                    Rhs = Rhs.Prev;
                    ++Diff;
                    rstd_Assert(Rhs.Node != DebugSentinel);
                }
                return Diff;
            }
            
            type* Ptr()
            { 
                rstd_Assert(Node != DebugSentinel);
                return &Node->Data;
            }
            
            type* operator->()
            { return Ptr(); }
            
            type& operator*()
            { return *Ptr(); }
            
            rstd_bool operator==(iterator Rhs)
            { return Node == Rhs.Node; }
            
            rstd_bool operator!=(iterator Rhs)
            { return Node != Rhs.Node; }
        };
        
        iterator Begin()
        {
            iterator It = {Sentinel->Next};
            rstd_DebugOnly(It.DebugSentinel = Sentinel);
            return It;
        }
        
        iterator End()
        {
            iterator It = {Sentinel};
            rstd_DebugOnly(It.DebugSentinel = Sentinel);
            return It;
        }
        
        internal_rstd_RestOfIteratorFunctions;
        
        rstd_bool Empty()
        { return Sentinel->Next == Sentinel; }
        
        type& operator[]
        (u32 DesiredIndex)
        {
            rstd_Assert(!Empty());
            
            u32 Index = 0;
            for(auto& E : *this)
            {
                if(Index == DesiredIndex)
                    return E;
                ++Index;
            }
            
            rstd_InvalidCodePathM(Format("You tried to get element [%], but doubly_linked_list has only % elements", DesiredIndex, Index));
            return *(type*)(nullptr);
        }
        
        type& GetFirst()
        {
            rstd_Assert(!Empty());
            return Sentinel->Next->Data;
        }
        
        type& GetLast()
        {
            rstd_Assert(!Empty());
            return Sentinel->Prev->Data;
        }
        
        void SanityCheck()
        {
#if rstd_Debug
            rstd_AssertM(Nodes, "You have to initialize list before using it!");
            rstd_Assert(Nodes == Sentinel);
            rstd_Assert(FreeNodes != Sentinel);
            rstd_Assert(Sentinel->Next && Sentinel->Prev);
            
#if rstd_DoublyLinkedListAdvancedSanityCheckEnabled
            auto* Prev = Sentinel;
            auto* Current = Sentinel->Next;
            while(Current != Sentinel)
            {
                rstd_Assert(Current->Next);
                rstd_Assert(Current->Prev);
                rstd_Assert(Prev->Next == Current);
                rstd_Assert(Current->Prev == Prev);
                Prev = Current;
                Current = Current->Next;
            }
#endif
#endif
        }
        
#ifndef rstd_DisableWarningForUsingGetCountInListsWithoutCounter
        [[deprecated("\nConsider using doubly_linked_list_with_counter instead of doubly_linked_list\n"
                     "if you want to call GetCount(); in most cases that will result in better performance.\n"
                     "If you want to get rid of this warning you can #define rstd_DisableWarningForUsingGetCountInListsWithoutCounter.")]]
#endif
        u32 GetCount()
        {
            SanityCheck();
            u32 Count = 0;
            for(auto& E : *this)
                ++Count;
            return Count;
        }
        
        void PushNode
        (node* Node)
        {
            auto* LastNode = Sentinel->Prev;
            Sentinel->Prev = Node;
            LastNode->Next = Node;
            Node->Prev = LastNode;
            Node->Next = Sentinel;
            
            SanityCheck();
        }
        
        void PushNodeFront
        (node* Node)
        {
            auto* FirstNode = Sentinel->Next;
            Sentinel->Next = Node;
            FirstNode->Prev = Node;
            Node->Next = FirstNode;
            Node->Prev = Sentinel;
            
            SanityCheck();
        }
        
        void PushNodeBetween
        (node* Prev, node* NewNode, node* Next)
        {
            NewNode->Next = Next;
            NewNode->Prev = Prev;
            Prev->Next = NewNode;
            Next->Prev = NewNode;
            
            SanityCheck();
        }
        
        node* PopFreeNode()
        {
            rstd_Assert(FreeNodes);
            node* Node = FreeNodes;
            FreeNodes = FreeNodes->Next;
            return Node;
        }
        
        node* AllocateZeroNode()
        {
            node* Node;
            if(FreeNodes)
            {
                Node = PopFreeNode();
                ZeroOutStruct(Node->Data);
            }
            else
            {
                Node = &rstd_PushStructZero(*ArenaRef, node);
            }
            return Node;
        }
        
        node* AllocateUninitializedNode()
        { return FreeNodes ? PopFreeNode() : &rstd_PushStructUninitialized(*ArenaRef, node); }
        
        type& PushZero()
        {
            node* Node = AllocateZeroNode();
            PushNode(Node);
            return Node->Data;
        }
        
        type& PushUninitialized()
        {
            node* Node = AllocateUninitializedNode();
            PushNode(Node);
            return Node->Data;
        }
        
        type& PushFrontZero()
        {
            node* Node = AllocateZeroNode();
            PushNodeFront(Node);
            return Node->Data;
        }
        
        type& PushFrontUninitialized()
        {
            node* Node = AllocateUninitializedNode();
            PushNodeFront(Node);
            return Node->Data;
        }
        
        type& PushBeforeZero
        (iterator It)
        {
            node* Node = AllocateZeroNode();
            PushNodeBetween(It.Node->Prev, Node, It.Node);
            return Node->Data;
        }
        
        type& PushBeforeUninitialized
        (iterator It)
        {
            node* Node = AllocateUninitializedNode();
            PushNodeBetween(It.Node->Prev, Node, It.Node);
            return Node->Data;
        }
        
        type& PushAfterZero
        (iterator It)
        {
            node* Node = AllocateZeroNode();
            PushNodeBetween(It.Node, Node, It.Node->Next);
            return Node->Data;
        }
        
        type& PushAfterUninitialized
        (iterator It)
        {
            node* Node = AllocateUninitializedNode();
            PushNodeBetween(It.Node, Node, It.Node->Next);
            return Node->Data;
        }
        
        type& PushBetweenZero
        (iterator A, iterator B)
        {
            node* Node = AllocateZeroNode();
            PushNodeBetween(A, Node, B);
            return Node->Data;
        }
        
        type& PushBetweenUninitialized
        (iterator A, iterator B)
        {
            node* Node = AllocateUninitializedNode();
            PushNodeBetween(A, Node, B);
            return Node->Data;
        }
        
        void AddFreeNode
        (node* Node)
        {
            Node->Next = FreeNodes;
            FreeNodes = Node;
        }
        
        void Clear()
        {
            SanityCheck();
            
            iterator Next;
            for(auto It = Begin(); It != End(); It = Next)
            {
                Next = It + 1;
                AddFreeNode(It.Node);
            }
            
            Sentinel->Next = Sentinel;
            Sentinel->Prev = Sentinel;
        }
        
        void Remove
        (node* Node)
        {
            rstd_Assert(!Empty());
            
            Node->Prev->Next = Node->Next;
            Node->Next->Prev = Node->Prev;
            AddFreeNode(Node);
            
            SanityCheck();
        }
        
        iterator Remove
        (iterator It)
        {
            auto ElementToDeleteIt = It++;
            Remove(ElementToDeleteIt.Node);
            return It;
        }
        
        void Remove(type& E)
        { Remove((node*)&E); }
        
        void PopFirst()
        { Remove(GetFirst()); }
        
        void PopLast()
        { Remove(GetLast()); }
        
        type* FindEqual(const auto& ThingToComare)        
        { return ::FindEqual(*this, ThingToComare); }
        
        type* Find(auto ComparisonFunc)           
        { return ::Find(*this, ComparisonFunc); }                                    
        
        type& FindWithAssert(auto ComparisonFunc)
        { return ::FindWithAssert(*this, ComparisonFunc); }
        
        rstd_bool HasEqual(const auto& ThingToCompare) 
        { return ::HasEqual(*this, ThingToCompare); } 
        
        rstd_bool Has(auto ComparisonFunc) 
        { return ::Find(*this, ComparisonFunc); }
        
        u32 FindIndexOfFirstEqual(const auto& ThingToCompare)
        {  return ::FindIndexOfFirstEqual(*this, ThingToCompare); } 
        
        u32 FindIndexOfFirst(auto ComparisonFunc) 
        { return ::FindIndexOfFirst(*this, ComparisonFunc); } 
        
        u32 HowManyEqualHas(const auto& ThingToCompare) 
        { return ::HowManyEqualHas(*this, ThingToCompare); } 
        
        u32 HowManyHas(auto ComparisonFunc) 
        { return ::HowManyHas(*this, ComparisonFunc); } 
        
        get_value_and_index_result<type> FindValueAndIndex 
        (auto ComparisonFunc) 
        { return ::FindValueAndIndex<type>(*this, ComparisonFunc); }
        
        type& Push 
        (const type& InitialData) 
        { 
            auto& Data = PushUninitialized(); 
            Data = InitialData; 
            return Data; 
        } 
        
        type& PushBefore
        (iterator It, const type& InitialData) 
        { 
            auto& Data = PushBeforeUninitialized(It); 
            Data = InitialData; 
            return Data;
        } 
        
        type& PushDefault() 
        { 
            type E; 
            return Push(E); 
        } 
        
        type* PushIfNotFull 
        (const type& InitialData) 
        { 
            return &Push(InitialData); 
        } 
        
        type* PushIfUnique 
        (const type& InitialData) 
        { 
            if(!HasEqual(InitialData)) 
                return &Push(InitialData); 
            return nullptr; 
        } 
        
        template<class comparison_fn> 
            type* PushIfUnique
        (const type& InitialData, comparison_fn Comparison) 
        { 
            if(!Has(Comparison)) 
                return &Push(InitialData); 
            return nullptr; 
        } 
        
        type& GetIfExistsOrPushIfUnique 
        (const type& ThingToCompareOrInitialData) 
        { 
            if(auto* Found = Find(ThingToCompareOrInitialData)) 
                return *Found; 
            return Push(ThingToCompareOrInitialData); 
        } 
        
        template<class... args> void Push 
        (const type& CurrentPushElement, const args&&... NextPushElements) 
        { 
            Push(CurrentPushElement); 
            Push(NextPushElements...); 
        }
        
        type& PushFront 
        (const type& InitialData) 
        { 
            auto& Data = PushFrontUninitialized(); 
            Data = InitialData; 
            return Data; 
        }
        
        template<class comparison_fn> 
            type* PushFrontIfUnique 
        (const type& InitialData, comparison_fn Comparison) 
        { 
            if(!Has(Comparison)) 
                return &PushFront(InitialData); 
            return nullptr; 
        } 
        
        template<class compare_type> 
            rstd_bool RemoveFirstEqualTo 
        (const compare_type& ThingToCompare) 
        { 
            if(auto* Found = FindEqual(ThingToCompare)) 
            { 
                Remove(*Found); 
                return true; 
            } 
            return false; 
        } 
        
        template<class compare_type> 
            void RemoveFirstEqualToWithAssert 
        (const compare_type& ThingToCompare) 
        { 
            bool ManagedToRemove = RemoveFirstEqualTo(ThingToCompare); 
            rstd_Assert(ManagedToRemove); 
        } 
        
        template<class comparison_fn> 
            rstd_bool RemoveFirstIf 
        (comparison_fn Comparison) 
        { 
            if(auto* Found = Find(Comparison)) 
            { 
                Remove(*Found); 
                return true; 
            } 
            return false; 
        } 
        
        template<class comparison_fn> 
            void RemoveFirstIfWithAssert 
        (comparison_fn Comparison) 
        { 
            bool ManagedToRemove = RemoveFirstIf(Comparison); 
            rstd_Assert(ManagedToRemove); 
        } 
        
        template<class comparison_fn> 
            optional<type> RemoveFirstIfReturnCopy 
        (comparison_fn Comparison) 
        { 
            if(auto* Found = Find(Comparison)) 
            { 
                Remove(*Found); 
                return {*Found}; 
            } 
            return {}; 
        } 
        
        template<class comparison_fn> 
            u32 RemoveIf 
        (comparison_fn Comparison) 
        { 
            u32 RemovedCount = 0; 
            for(auto It = Begin(); It != End(); ++It) 
            { 
                if(Comparison(*It)) 
                { 
                    Remove(It); 
                    ++RemovedCount; 
                } 
            } 
            return RemovedCount; 
        } 
        
        type GetAndPopFirst() 
        { 
            auto FirstCopy = GetFirst(); 
            PopFirst(); 
            return FirstCopy; 
        } 
        
        rstd_bool PopFirstIfNotEmpty() 
        { 
            if(Empty()) 
            { 
                return false; 
            } 
            else 
            { 
                PopFirst(); 
                return true; 
            } 
        } 
        
        optional<type> GetAndPopFirstIfNotEmpty() 
        { 
            if(Empty()) 
            { 
                return {}; 
            } 
            else 
            { 
                auto* First = &GetFirst(); 
                type FirstCopy = *First; 
                Remove(First); 
                return FirstCopy; 
            } 
        } 
        
        rstd_bool PopLastIfNotEmpty() 
        { 
            if(Empty()) 
            { 
                return false; 
            } 
            else 
            { 
                PopLast(); 
                return true; 
            } 
        } 
        
        type GetAndPopLast() 
        { 
            type LastCopy = GetLast(); 
            PopLast(); 
            return LastCopy; 
        } 
        
        optional<type> GetAndPopLastIfNotEmpty() 
        { 
            if(Empty()) 
            { 
                return {}; 
            } 
            else 
            { 
                auto* Last = &GetLast(); 
                type LastCopy = *Last; 
                Remove(Last); 
                return LastCopy; 
            } 
        }
        
    };
    
    ////////////////////////
    // SINGLY LINKED LIST //
    ////////////////////////
    template<class type>
        struct singly_linked_list
    {
        // TODO: Add memory debugging for singly_linked_list
        
        struct node
        {
            type Data;
            node* Next;
        };
        
        struct iterator
        {
            node* Node;
            
            iterator& operator++() 
            { 
                Node = Node->Next;
                return *this;
            }
            
            iterator operator++(int)
            {
                auto Res = *this;
                Node = Node->Next;
                return Res;
            }
            
            type& operator*() 
            { return Node->Data; }
            
            type* Ptr()
            { return &Node->Data; }
            
            type* operator->() 
            { return Ptr(); }
            
            rstd_bool operator==(iterator Rhs)
            { return Node == Rhs.Node; }
            
            rstd_bool operator!=(iterator Rhs)
            { return Node != Rhs.Node; }
        };
        
        node* FirstNode;
        node* LastNode;
        arena_ref ArenaRef;
        
        type GetVariableOfElementType()
        { return FirstNode->Data; }
        
        void Clear()
        { FirstNode = LastNode = nullptr; }
        
        void Init
        (arena_ref ArenaRef)
        {    
            rstd_AssertM(!this->ArenaRef, "It's already initialized!");
            this->ArenaRef = ArenaRef;
            Clear();
        }
        
        singly_linked_list(arena_ref ArenaRef) 
        { Init(ArenaRef); }
        
#ifdef rstd_DefaultArena
        singly_linked_list()
        { Init(ShareArena(DefaultArena)); }
        
        void Init()
        { Init(ShareArena(DefaultArena)); }
#else
        singly_linked_list()
            :ArenaRef() { Clear(); }
#endif
        
        iterator Begin()
        { return {FirstNode}; }
        
        iterator End()
        { return {nullptr}; }
        
        internal_rstd_RestOfIteratorFunctions;
        
        rstd_bool Initialized()
        { return ArenaRef; }
        
        rstd_bool Empty()
        { return !FirstNode; }
        
        type& GetFirst()
        {
            rstd_Assert(!Empty());
            return FirstNode->Data;
        }
        
        type& GetLast()
        {
            rstd_Assert(!Empty());
            return LastNode->Data;
        }
        
#ifndef rstd_DisableWarningForUsingGetCountInListsWithoutCounter
        [[deprecated("\nConsider using singly_linked_list_with_counter instead of singly_linked_list\n"
                     "if you want to call GetCount(); in most cases that will result in better performance.\n"
                     "If you want to get rid of this warning you can #define rstd_DisableWarningForUsingGetCountInListsWithoutCounter.")]]
#endif
        u32 GetCount()
        { 
            u32 Count = 0;
            for(auto& E : *this)
                ++Count;
            return Count;
        }
        
        void PushNode
        (node& Node)
        {
            if(LastNode)
                LastNode->Next = &Node;
            LastNode = &Node;
            if(!FirstNode)
                FirstNode = &Node;
        }
        
        void PushNodeFront
        (node& Node)
        {
            Node.Next = FirstNode;
            FirstNode = Node;
            if(!LastNode)
                LastNode = FirstNode;
        }
        
        type& PushZero()
        {
            rstd_Assert(ArenaRef);
            auto* Node = PushStructZero(ArenaRef.Ptr(), node);
            PushNode(Node);
            return Node->Data;
        }
        
        type& PushUninitialized()
        {
            rstd_Assert(ArenaRef);
            auto& Node = rstd_PushStructUninitialized(*ArenaRef, node);
            Node.Next = nullptr;
            PushNode(Node);
            return Node.Data;
        }
        
        type& PushFrontZero()
        {
            rstd_Assert(ArenaRef);
            auto* Node = PushStructZero(ArenaRef.Ptr(), node);
            PushNodeFront(Node);
            return Node->Data;
        }
        
        type& PushFrontUninitialized()
        {
            rstd_Assert(ArenaRef);
            auto* Node = PushStructUninitialized(ArenaRef.Ptr(), node);
            PushNodeFront(Node);
            return Node->Data;
        }
        
        type& PushAfter
        (node* NodeWhichWillBeBeforeThePushedOne)
        {
            rstd_Assert(ArenaRef);
            auto* PushedNode = PushStructUninitialized(ArenaRef.Ptr(), node);
            PushedNode->Next = NodeWhichWillBeBeforeThePushedOne->Next;
            if(!PushedNode->Next)
                LastNode = PushedNode;
            NodeWhichWillBeBeforeThePushedOne->Next = PushedNode;
            return PushedNode->Data;
        }
        
        type& PushAfter
        (node* NodeWhichWillBeBeforeThePushedOne, const type& InitialData)
        {
            auto& Data = PushAfter(NodeWhichWillBeBeforeThePushedOne);
            Data = InitialData;
            return Data;
        }
        
        void PopFront()
        {
            rstd_Assert(!Empty());
            FirstNode = FirstNode->Next;
        }
        
        void Pop()
        {
            rstd_Assert(!Empty());
            if(FirstNode->Next)
            {
                auto* Node = FirstNode;
                while(Node->Next != LastNode)
                    ++Node;
                Node->Next = nullptr;    
                LastNode = Node;
            }
            else
            {
                FirstNode = nullptr;
            }
        }
        
        void Remove
        (type* ElementToRemove)
        {
            node* NodeToRemove = (node*)ElementToRemove;    
            
            if(NodeToRemove == FirstNode)
            {
                FirstNode = FirstNode->Next;
            }
            else
            {
                node* PrevNode = FirstNode;
                for(auto* Node = FirstNode->Next;; Node = Node->Next)
                {
                    rstd_AssertM(Node, "You're trying to remove thing that is not in this container");
                    if(Node == NodeToRemove)
                    {
                        PrevNode->Next = Node->Next;
                        break;
                    }
                    PrevNode = Node;
                }
            }
        }
        
        // NOTE: returns true if found element to remove
        template<class comparison_fn>
            rstd_bool RemoveFirstIf
        (comparison_fn Comparison)
        {
            node* PrevNode = nullptr;
            for(auto* Node = FirstNode; Node; Node = Node->Next)
            {
                if(Comparison(Node->Data))
                {
                    if(PrevNode)
                    {
                        PrevNode->Next = Node->Next;    
                        if(Node == LastNode)
                            LastNode = PrevNode;
                    }
                    else
                    {
                        FirstNode = Node->Next;
                    }
                    return true;
                }
                else
                {
                    PrevNode = Node;
                }
            }
            return false;
        }
        
        template<class comparison_fn>
            optional<type> RemoveFirstIfReturnCopy
        (comparison_fn Comparison)
        {
            node* PrevNode = nullptr;
            for(auto* Node = FirstNode; Node; Node = Node->Next)
            {
                if(Comparison(Node->Data))
                {
                    if(PrevNode)
                    {
                        PrevNode->Next = Node->Next;    
                        if(Node == LastNode)
                            LastNode = PrevNode;
                    }
                    else
                    {
                        FirstNode = Node->Next;
                    }
                    return {Node->Data};
                }
                else
                {
                    PrevNode = Node;
                }
            }
            return {};
        }
        
        rstd_bool RemoveFirstEqualTo
        (type& E)
        {
            node* PrevNode = nullptr;
            for(auto* Node = FirstNode; Node; Node = Node->Next)
            {
                if(Node->Data == E)
                {
                    if(PrevNode)
                    {
                        PrevNode->Next = Node->Next;    
                        if(Node == LastNode)
                            LastNode = PrevNode;
                    }
                    else
                    {
                        FirstNode = Node->Next;
                    }
                    return true;
                }
                else
                {
                    PrevNode = Node;
                }
            }
            return false;
        }
        
        type* FindEqual(const auto& ThingToComare)        
        { return ::FindEqual(*this, ThingToComare); }
        
        type* Find(auto ComparisonFunc)           
        { return ::Find(*this, ComparisonFunc); }                                    
        
        type& FindWithAssert(auto ComparisonFunc)
        { return ::FindWithAssert(*this, ComparisonFunc); }
        
        rstd_bool HasEqual(const auto& ThingToCompare) 
        { return ::HasEqual(*this, ThingToCompare); } 
        
        rstd_bool Has(auto ComparisonFunc) 
        { return ::Find(ComparisonFunc); }
        
        u32 FindIndexOfFirstEqual(const auto& ThingToCompare)
        {  return ::FindIndexOfFirstEqual(*this, ThingToCompare); } 
        
        u32 FindIndexOfFirst(auto ComparisonFunc) 
        { return ::FindIndexOfFirst(*this, ComparisonFunc); } 
        
        u32 HowManyEqualHas(const auto& ThingToCompare) 
        { return ::HowManyEqualHas(*this, ThingToCompare); } 
        
        u32 HowManyHas(auto ComparisonFunc) 
        { return ::HowManyHas(*this, ComparisonFunc); } 
        
        get_value_and_index_result<type> FindValueAndIndex 
        (auto ComparisonFunc) 
        { return ::FindValueAndIndex<type>(*this, ComparisonFunc); }
        
        type& Push 
        (const type& InitialData)
        { 
            auto& Data = PushUninitialized(); 
            Data = InitialData; 
            return Data; 
        } 
        
        type& PushDefault() 
        { 
            type E; 
            return Push(E); 
        } 
        
        type* PushIfNotFull 
        (const type& InitialData) 
        { 
            return &Push(InitialData); 
        } 
        
        type* PushIfUnique 
        (const type& InitialData) 
        { 
            if(!HasEqual(InitialData)) 
                return &Push(InitialData); 
            return nullptr; 
        } 
        
        template<class comparison_fn> 
            type* PushIfUnique
        (const type& InitialData, comparison_fn Comparison) 
        { 
            if(!Has(Comparison)) 
                return &Push(InitialData); 
            return nullptr; 
        } 
        
        type& GetIfExistsOrPushIfUnique 
        (const type& ThingToCompareOrInitialData) 
        { 
            if(auto* Found = Find(ThingToCompareOrInitialData)) 
                return *Found; 
            return Push(ThingToCompareOrInitialData); 
        } 
        
        template<class... args> void Push 
        (const type& CurrentPushElement, const args&&... NextPushElements) 
        { 
            Push(CurrentPushElement); 
            Push(NextPushElements...); 
        }
        
        type& PushFront 
        (const type& InitialData) 
        { 
            auto& Data = PushFrontUninitialized(); 
            Data = InitialData; 
            return Data; 
        }
        
        template<class comparison_fn> 
            type* PushFrontIfUnique 
        (const type& InitialData, comparison_fn Comparison) 
        { 
            if(!Has(Comparison)) 
                return &PushFront(InitialData); 
            return nullptr; 
        } 
        
        optional<type> GetValueOfIndexIfExists
        (u32 RequestedIndex)
        {
            u32 I = 0;
            for(auto* Node = FirstNode; Node; Node = Node->Next)
            {
                if(I == RequestedIndex)
                    return Node->Data;
                I++;
            }
            return NullOpt;
        }
    };
    
    /////////////////////////////////
    // BACKWARD SINGLY LINKED LIST //
    /////////////////////////////////
    template<class type>
        struct backward_singly_linked_list
    {
        // TODO: Add memory debugging for backward_singly_linked_list
        
        struct node
        {
            type Data;
            node* Next;
        };
        
        struct iterator
        {
            node* Node;
            
            iterator& operator++() 
            { 
                Node = Node->Next;
                return *this;
            }
            
            iterator operator++(int)
            {
                auto Res = *this;
                Node = Node->Next;
                return Res;
            }
            
            type& operator*() 
            { return Node->Data; }
            
            type* Ptr()
            { return &Node->Data; }
            
            type* operator->() 
            { return Ptr(); }
            
            rstd_bool operator==(iterator Rhs)
            { return Node == Rhs.Node; }
            
            rstd_bool operator!=(iterator Rhs)
            { return Node != Rhs.Node; }
        };
        
        node* Nodes;
        arena_ref ArenaRef;
        
        rstd_bool Initialized()
        { return ArenaRef; }
        
        backward_singly_linked_list(arena_ref ArenaRef) 
            :ArenaRef(ArenaRef), Nodes(nullptr) {}
        
        void Init
        (arena_ref _ArenaRef)
        {
            ArenaRef = _ArenaRef;
            Nodes = nullptr;
        }
        
        void InitIfNotInitialized
        (arena_ref ArenaRef)
        {
            if(!Initialized())
                Init(ArenaRef);
        }
        
#ifdef rstd_DefaultArena
        backward_singly_linked_list()
            :backward_singly_linked_list(ShareArena(DefaultArena)) {}
#else
        backward_singly_linked_list()
            :ArenaRef(), Nodes(nullptr) {}
#endif
        
        type GetVariableOfElementType()
        { return Nodes->Data; }
        
        iterator Begin()
        { return {Nodes}; }
        
        iterator End()
        { return {nullptr}; }
        
        internal_rstd_RestOfIteratorFunctions;
        
        type& GetFirst()
        {
            rstd_Assert(!Empty());
            return Nodes->Data;
        }
        
        type& GetLast()
        {
            rstd_Assert(!Empty());
            node* Node = Nodes;
            while(Node && Node->Next)
                Node = Node->Next;
            return Node->Data;
        }
        
        type& operator[]
        (u32 DesiredIndex)
        {
            rstd_Assert(!Empty());
            
            u32 Index = 0;
            for(auto& E : *this)
            {
                if(Index == DesiredIndex)
                    return E;
                ++Index;
            }
            
            rstd_InvalidCodePathM(Format("You tried to get element [%], but this backward_singly_linked_list has only % elements", DesiredIndex, Index));
            return *(type*)(nullptr);
        }
        
        void Clear()
        { Nodes = nullptr; }
        
        rstd_bool Empty()
        { return !Nodes; }
        
#ifndef rstd_DisableWarningForUsingGetCountInListsWithoutCounter
        [[deprecated("\nConsider using backward_singly_linked_list_with_counter instead of backward_singly_linked_list\n"
                     "if you want to call GetCount(); in most cases that will result in better performance.\n"
                     "If you want to get rid of this warning you can #define rstd_DisableWarningForUsingGetCountInListsWithoutCounter.")]]
#endif
        u32 GetCount()
        { 
            u32 Count = 0;
            for(auto& E : *this)
                ++Count;
            return Count;
        }
        
        void PushNode
        (node& Node)
        {
            Node.Next = Nodes;
            Nodes = &Node;
        }
        
        type& PushZero()
        {
            rstd_Assert(ArenaRef);
            auto& Node = rstd_PushStructZero(*ArenaRef, node);
            PushNode(Node);
            return Node.Data;
        }
        
        type& PushUninitialized()
        {
            rstd_Assert(ArenaRef);
            auto& Node = rstd_PushStructUninitialized(*ArenaRef, node);
            PushNode(Node);
            return Node.Data;
        }
        
        type& PushAfter
        (node& NodeWhichWillBeBeforeThePushedOne, const type& InitialData)
        {
            rstd_Assert(ArenaRef);
            auto& PushedNode = rstd_PushStructUninitialized(*ArenaRef, node);
            PushedNode.Next = NodeWhichWillBeBeforeThePushedOne.Next;
            NodeWhichWillBeBeforeThePushedOne.Next = &PushedNode;
            PushedNode.Data = InitialData;
            return PushedNode.Data;
        }
        
        void Pop()
        {
            rstd_Assert(!Empty());
            Nodes = Nodes->Next; 
        }
        
        node* PopLastNode()
        {
            rstd_Assert(!Empty());
            node* Node = Nodes;
            while(Node && Node->Next->Next)
                Node = Node->Next;
            auto* NodeBeingPopped = Node->Next;
            Node->Next = nullptr;
            return NodeBeingPopped;
        }
        
        void PopLast()
        { PopLastNode(); }
        
        type* PopLastReturnPtr()
        { return &PopLastNode()->Data; }
        
        void Remove
        (type* ElementToRemove)
        {
            node* NodeToRemove = (node*)ElementToRemove;    
            
            if(NodeToRemove == Nodes)
            {
                Nodes = NodeToRemove->Next;
            }
            else
            {
                node* PrevNode = Nodes;
                for(auto* Node = Nodes->Next;; Node = Node->Next)
                {
                    rstd_AssertM(Node, "You're trying to remove thing that is not in this container");
                    if(Node == NodeToRemove)
                    {
                        PrevNode->Next = Node->Next;
                        break;
                    }
                    PrevNode = Node;
                }
            }
        }
        
        // NOTE: returns true if found element to remove
        template<class comparison_fn>
            rstd_bool RemoveFirstIf
        (comparison_fn Comparison)
        {
            node* PrevNode = nullptr;
            for(auto* Node = Nodes; Node; Node = Node->Next)
            {
                if(Comparison(Node->Data))
                {
                    if(PrevNode)
                        PrevNode->Next = Node->Next;    
                    else
                        Nodes = Node->Next;
                    return true;
                }
                else
                {
                    PrevNode = Node;
                }
            }
            return false;
        }
        
        template<class comparison_fn>
            void RemoveFirstIfWithAssert
        (comparison_fn Comparison)
        {
            rstd_bool ManagedToRemove = RemoveFirstIf(Comparison);    
            rstd_AssertM(ManagedToRemove, "Element which meets the condition of removal wasn't found!");
        }
        
        template<class comparison_fn>
            optional<type> RemoveFirstIfReturnCopy
        (comparison_fn Comparison)
        {
            node* PrevNode = nullptr;
            for(auto* Node = Nodes; Node; Node = Node->Next)
            {
                if(Comparison(Node->Data))
                {
                    if(PrevNode)
                        PrevNode->Next = Node->Next;    
                    else
                        Nodes = Node->Next;
                    return {Node->Data};
                }
                else
                {
                    PrevNode = Node;
                }
            }
            return {};
        }
        
        template<class comparison_fn>
            type RemoveFirstIfWithAssertReturnCopy
        (comparison_fn Comparison)
        {
            node* PrevNode = nullptr;
            for(auto* Node = Nodes; Node; Node = Node->Next)
            {
                if(Comparison(Node->Data))
                {
                    if(PrevNode)
                        PrevNode->Next = Node->Next;    
                    else
                        Nodes = Node->Next;
                    return Node->Data;
                }
                else
                {
                    PrevNode = Node;
                }
            }
            rstd_InvalidCodePathM("Element which meets the condition of removal wasn't found!");
            return {};
        }
        
        rstd_bool RemoveFirstEqualTo
        (type& E)
        {
            node* PrevNode = nullptr;
            for(auto* Node = Nodes; Node; Node = Node->Next)
            {
                if(Node->Data == E)
                {
                    if(PrevNode)
                        PrevNode->Next = Node->Next;    
                    else
                        Nodes = Node->Next;
                    return true;
                }
                else
                {
                    PrevNode = Node;
                }
            }
            return false;
        }
        
        type* FindEqual(const auto& ThingToComare)        
        { return ::FindEqual(*this, ThingToComare); }
        
        type* Find(auto ComparisonFunc)           
        { return ::Find(*this, ComparisonFunc); }                                    
        
        type& FindWithAssert(auto ComparisonFunc)
        { return ::FindWithAssert(*this, ComparisonFunc); }
        
        rstd_bool HasEqual(const auto& ThingToCompare) 
        { return ::HasEqual(*this, ThingToCompare); } 
        
        rstd_bool Has(auto ComparisonFunc) 
        { return ::Find(*this, ComparisonFunc); }
        
        u32 FindIndexOfFirstEqual(const auto& ThingToCompare)
        {  return ::FindIndexOfFirstEqual(*this, ThingToCompare); } 
        
        u32 FindIndexOfFirst(auto ComparisonFunc) 
        { return ::FindIndexOfFirst(*this, ComparisonFunc); } 
        
        u32 HowManyEqualHas(const auto& ThingToCompare) 
        { return ::HowManyEqualHas(*this, ThingToCompare); } 
        
        u32 HowManyHas(auto ComparisonFunc) 
        { return ::HowManyHas(*this, ComparisonFunc); } 
        
        get_value_and_index_result<type> FindValueAndIndex 
        (auto ComparisonFunc) 
        { return ::FindValueAndIndex<type>(*this, ComparisonFunc); }
        
        type& Push 
        (const type& InitialData) 
        { 
            auto& Data = PushUninitialized(); 
            Data = InitialData; 
            return Data; 
        } 
        
        type& PushDefault() 
        { 
            type E; 
            return Push(E); 
        } 
        
        type* PushIfNotFull 
        (const type& InitialData) 
        { 
            return &Push(InitialData); 
        } 
        
        type* PushIfUnique 
        (const type& InitialData) 
        { 
            if(!HasEqual(InitialData)) 
                return &Push(InitialData); 
            return nullptr; 
        } 
        
        template<class comparison_fn> 
            type* PushIfUnique
        (const type& InitialData, comparison_fn Comparison) 
        { 
            if(!Has(Comparison)) 
                return &Push(InitialData); 
            return nullptr; 
        } 
        
        type& GetIfExistsOrPushIfUnique 
        (const type& ThingToCompareOrInitialData) 
        { 
            if(auto* Found = Find(ThingToCompareOrInitialData)) 
                return *Found; 
            return Push(ThingToCompareOrInitialData); 
        } 
        
        // TODO: Do args need to be templated?
        template<class... args> 
            void Push 
        (const type& CurrentPushElement, const args&&... NextPushElements) 
        { 
            Push(CurrentPushElement); 
            Push(NextPushElements...); 
        }
    };
    
    /////////////////////////////////////
    // DOUBLY LINKED LIST WITH COUNTER //
    /////////////////////////////////////
    template<class type>
        struct doubly_linked_list_with_counter : doubly_linked_list<type>
    {
        using typename doubly_linked_list<type>::node;
        using typename doubly_linked_list<type>::iterator;
        
        doubly_linked_list_with_counter(arena_ref ArenaRef)
            :doubly_linked_list<type>(ArenaRef), Count(0) {}
        
#ifdef rstd_DefaultArena
        doubly_linked_list_with_counter()
            :doubly_linked_list_with_counter(ShareArena(DefaultArena)) {}
#else
        doubly_linked_list_with_counter()
            :doubly_linked_list<type>(), Count(0) {}
#endif
        
        u32 Count;
        
        u32 GetCount()
        { return Count; }
        
        using doubly_linked_list<type>::Find;
        
        type GetVariableOfElementType()
        { return doubly_linked_list<type>::GetVariableOfElementType(); }
        
        void Clear()
        {
            doubly_linked_list<type>::Clear();
            Count = 0;
        }
        
        void PushNode
        (node* Node)
        {
            ++Count;
            doubly_linked_list<type>::PushNode(Node);
        }
        
        void PushNodeFront
        (node* Node)
        {
            ++Count;
            doubly_linked_list<type>::PushNodeFront(Node);
        }
        
        void PopFreeNode
        (node* Node)
        {
            --Count;
            doubly_linked_list<type>::PopFreeNode(Node);
        }
        
        type& PushZero()
        {
            ++Count;
            return doubly_linked_list<type>::PushZero();
        }
        
        type& PushUninitialized()
        {
            ++Count;
            return doubly_linked_list<type>::PushUninitialized();
        }
        
        type& Push
        (const type& InitialData)
        {
            ++Count;
            return doubly_linked_list<type>::Push(InitialData);
        }
        
        type& PushFrontZero()
        {
            ++Count;
            return doubly_linked_list<type>::PushFrontZero();
        }
        
        type& PushFrontUninitialized()
        {
            ++Count;
            return doubly_linked_list<type>::PushFrontUninitialized();
        }
        
        type& PushFront
        (const type& InitialData)
        {
            ++Count;
            return doubly_linked_list<type>::PushFront(InitialData);
        }
        
        type* PushIfUnique
        (const type& InitialData)
        {
            ++Count;
            return doubly_linked_list<type>::PushIfUnique(InitialData);
        }
        
        type& GetIfExistsOrPushIfUnique
        (const type& E)
        {
            auto* Found = Find(*this, E);
            if(Found)
                return *Found;
            return Push(E);
        }
        
        void Remove
        (node* Node)
        {
            doubly_linked_list<type>::Remove(Node);
            --Count;
        }
        
        iterator Remove
        (iterator It)
        {
            iterator Res = doubly_linked_list<type>::Remove(It);
            --Count;
            return Res;
        }
        
        void Remove
        (type& ElementToRemove)
        {
            --Count;
            doubly_linked_list<type>::Remove(ElementToRemove);
        }
        
        void PopFirst()
        {
            doubly_linked_list<type>::PopFirst();
            --Count;
        }
        
        rstd_bool PopFirstIfNotEmpty()
        {
            rstd_bool Res = doubly_linked_list<type>::Empty();
            if(Res)
            {
                PopFirst();
                --Count;
            }
            return Res;
        }
        
        type GetAndPopFirst()
        {
            --Count;
            return doubly_linked_list<type>::GetAndPopFirst();
        }
        
        optional<type> GetAndPopFirstIfNotEmpty()
        {
            if(doubly_linked_list<type>::Empty())
            {
                return {};
            }
            else
            {
                auto* First = &doubly_linked_list<type>::GetFirst();
                type FirstCopy = *First;
                Remove(First);
                return FirstCopy;
            }
        }
        
        void PopLast()
        {
            doubly_linked_list<type>::PopLast();
            --Count;
        }
        
        rstd_bool PopLastIfNotEmpty()
        {
            rstd_bool Res = doubly_linked_list<type>::Empty();
            if(Res)
            {
                PopLast();
                --Count;
            }
            return Res;
        }
        
        type GetAndPopLast()
        {
            --Count;
            return doubly_linked_list<type>::GetAndPopLast();
        }
        
        optional<type> GetAndPopLastIfNotEmpty()
        {
            if(doubly_linked_list<type>::Empty())
            {
                return {};
            }
            else
            {
                auto* Last = &doubly_linked_list<type>::GetLast();
                type LastCopy = *Last;
                Remove(Last);
                return LastCopy;
            }
        }
        
        // NOTE: returns true if found element to remove
        template<class comparison_fn>
            rstd_bool RemoveFirstIf
        (comparison_fn Comparison)
        {
            if(auto* Found = Find(Comparison))
            {
                Remove(*Found);
                return true;
            }
            return false;
        }
        
        template<class comparison_fn>
            optional<type> RemoveFirstIfReturnCopy
        (comparison_fn Comparison)
        {
            if(auto* Found = Find(Comparison))
            {
                Remove(*Found);
                return {*Found};
            }
            return {};
        }
        
        rstd_bool RemoveFirstEqualTo
        (type& E)
        {
            if(auto* Found = FindEqual(E))
            {
                Remove(*Found);
                return true;
            }
            return false;
        }
    };
    
    /////////////////////////////////////
    // SINGLY LINKED LIST WITH COUNTER //
    /////////////////////////////////////
    template<class type>
        struct singly_linked_list_with_counter : singly_linked_list<type>
    {
        using typename singly_linked_list<type>::node;
        
        singly_linked_list_with_counter(arena_ref ArenaRef)
            :singly_linked_list<type>(ArenaRef), Count(0) {}
        
#ifdef rstd_DefaultArena
        singly_linked_list_with_counter()
            :singly_linked_list_with_counter(ShareArena(DefaultArena)) {}
#else
        singly_linked_list_with_counter()
            :singly_linked_list<type>(), Count(0) {}
#endif
        
        u32 Count;
        
        type GetVariableOfElementType()
        { return singly_linked_list<type>::GetVariableOfElementType(); }
        
        u32 GetCount()
        { return Count; }
        
        void Clear()
        {
            singly_linked_list<type>::Clear();
            Count = 0;
        }
        
        void PushNodeFront(node* Node)
        {
            ++Count;
            singly_linked_list<type>::PushNodeFront(Node);
        }
        
        type& PushFrontZero()
        {
            ++Count;
            return singly_linked_list<type>::PushFrontZero();
        }
        
        type& PushFrontUninitialized()
        {
            ++Count;
            return singly_linked_list<type>::PushFrontUninitialized();
        }
        
        type& PushFront
        (const type& InitialData)
        {
            ++Count;
            return singly_linked_list<type>::PushFront(InitialData);
        }
        
        void PushNode(node* Node)
        {
            ++Count;
            singly_linked_list<type>::PushNode(Node);
        }
        
        type& PushZero()
        {
            ++Count;
            return singly_linked_list<type>::PushZero();
        }
        
        type& PushUninitialized()
        {
            ++Count;
            return singly_linked_list<type>::PushUninitialized();
        }
        
        type& Push
        (const type& InitialData)
        {
            ++Count;
            return singly_linked_list<type>::Push(InitialData);
        }
        
        type& PushAfter
        (node* NodeWhichWillBeBeforeThePushedOne, const type& InitialData)
        {
            ++Count;
            return singly_linked_list<type>::PushAfter(NodeWhichWillBeBeforeThePushedOne, InitialData);
        }
        
        type* PushIfUnique
        (const type& InitialData)
        {
            ++Count;
            return singly_linked_list<type>::PushIfUnique(InitialData);
        }
        
        type& GetIfExistsOrPushIfUnique
        (const type& E)
        {
            auto* Found = Find(this, E);
            if(Found)
                return *Found;
            return Push(E);
        }
        
        void PopFront()
        {
            --Count;
            singly_linked_list<type>::PopFront();
        }
        
        void Pop()
        {
            --Count;
            singly_linked_list<type>::Pop();
        }
        
        void Remove
        (type& ElementToRemove)
        {
            --Count;
            singly_linked_list<type>::Remove(ElementToRemove);
        }
        
        template<class comparison_fn>
            rstd_bool RemoveFirstIf
        (comparison_fn Comparison)
        {
            rstd_bool ManagedToRemove = singly_linked_list<type>::RemoveFirstIf(Comparison);
            if(ManagedToRemove)
                --Count;
            return ManagedToRemove;
        }
        
        template<class comparison_fn>
            optional<type> RemoveFirstIfReturnCopy
        (comparison_fn Comparison)
        {
            auto Res = singly_linked_list<type>::RemoveFirstIfReturnCopy(Comparison);
            if(Res)
                --Count;
            return Res;
        }
        
        rstd_bool RemoveFirstEqualTo
        (type& E)
        {
            rstd_bool ManagedToRemove = singly_linked_list<type>::RemoveFirstEqualTo(E);
            if(ManagedToRemove)
                --Count;
            return ManagedToRemove;
        }
    };
    
    //////////////////////////////////////////////
    // BACKWARD SINGLY LINKED LIST WITH COUNTER //
    //////////////////////////////////////////////
    template<class type>
        struct backward_singly_linked_list_with_counter : backward_singly_linked_list<type>
    {
        using typename backward_singly_linked_list<type>::node;
        using typename backward_singly_linked_list<type>::iterator;
        
        backward_singly_linked_list_with_counter(arena_ref ArenaRef)
            :backward_singly_linked_list<type>(ArenaRef), Count(0) {}
        
#ifdef rstd_DefaultArena
        backward_singly_linked_list_with_counter()
            :backward_singly_linked_list_with_counter(ShareArena(DefaultArena)){}
#else
        backward_singly_linked_list_with_counter()
            :backward_singly_linked_list<type>(), Count(0) {}
#endif
        
        u32 Count;
        
        type GetVariableOfElementType()
        { return backward_singly_linked_list<type>::GetVariableOfElementType(); }
        
        u32 GetCount()
        { return Count; }
        
        void Clear()
        {
            backward_singly_linked_list<type>::Clear();
            Count = 0;
        }
        
        void PushNode
        (node& Node)
        {
            ++Count;
            backward_singly_linked_list<type>::PushNode(Node);
        }
        
        type& PushZero()
        {
            ++Count;
            return backward_singly_linked_list<type>::PushZero();
        }
        
        type& PushUninitialized()
        {
            ++Count;
            return backward_singly_linked_list<type>::PushUninitialized();
        }
        
        type& Push
        (const type& InitialData)
        {
            ++Count;
            return backward_singly_linked_list<type>::Push(InitialData);
        }
        
        type& PushAfter
        (node& NodeWhichWillBeBeforeThePushedOne, const type& InitialData)
        {
            ++Count;
            return backward_singly_linked_list<type>::PushAfter(NodeWhichWillBeBeforeThePushedOne, InitialData);
        }
        
        type* PushIfUnique
        (const type& InitialData)
        {
            ++Count;
            return backward_singly_linked_list<type>::PushIfUnique(InitialData);
        }
        
        type& GetIfExistsOrPushIfUnique
        (const type& E)
        {
            auto* Found = Find(*this, E);
            if(Found)
                return *Found;
            return Push(E);
        }
        
        void Pop()
        {
            --Count;
            backward_singly_linked_list<type>::Pop();
        }
        
        void PopLast()
        {
            --Count;
            backward_singly_linked_list<type>::PopLast();
        }
        
        node* PopLastNode()
        {
            --Count;
            return backward_singly_linked_list<type>::PopLastNode();
        }
        
        type* PopLastReturnPtr()
        {
            --Count;
            return backward_singly_linked_list<type>::PopLastReturnPtr();
        }
        
        void Remove
        (type* ElementToRemove)
        {
            --Count;
            backward_singly_linked_list<type>::Remove(ElementToRemove);
        }
        
        template<class comparison_fn>
            rstd_bool RemoveFirstIf
        (comparison_fn Comparison)
        {
            rstd_bool ManagedToRemove = backward_singly_linked_list<type>::RemoveFirstIf(Comparison);
            if(ManagedToRemove)
                --Count;
            return ManagedToRemove;
        }
        
        template<class comparison_fn>
            void RemoveFirstIfWithAssert
        (comparison_fn Comparison)
        {
            backward_singly_linked_list<type>::RemoveFirstIfWithAssert(Comparison);    
            --Count;
        }
        
        template<class comparison_fn>
            optional<type> RemoveFirstIfReturnCopy
        (comparison_fn Comparison)
        {
            auto Res = backward_singly_linked_list<type>::RemoveFirstIfReturnCopy(Comparison);
            if(Res)
                --Count;
            return Res;
        }
        
        template<class comparison_fn>
            type RemoveFirstIfWithAssertReturnCopy
        (comparison_fn Comparison)
        {
            auto Res = backward_singly_linked_list<type>::RemoveFirstIfWithAssertReturnCopy(Comparison);
            --Count;
            return Res;
        }
        
        rstd_bool RemoveFirstEqualTo
        (type& E)
        {
            rstd_bool ManagedToRemove = backward_singly_linked_list<type>::RemoveFirstEqualTo(E);
            if(ManagedToRemove)
                --Count;
            return ManagedToRemove;
        }
    };
    
    /////////////////////
    // MULTI-THREADING //
    /////////////////////
#ifdef _WIN32
    static void WriteFence()
    {
#if rstd_MultiThreadingEnabled
        _WriteBarrier();
#endif
    }
    
    static void ReadFence()
    {
#if rstd_MultiThreadingEnabled
        _ReadBarrier();
#endif
    }
    
    static void ReadWriteFence()
    {
#if rstd_MultiThreadingEnabled
        _ReadWriteBarrier();
#endif
    }
    
    u32 AtomicIncrement(volatile u32&);
    u32 AtomicDecrement(volatile u32&);
    
    u64 AtomicIncrement(volatile u64&);
    u64 AtomicDecrement(volatile u64&);
    
    i32 AtomicSet(volatile i32& Destination, i32 NewValue);
    
    u32 AtomicSet(volatile u32& Destination, u32 NewValue)
    { return AtomicSet((volatile i32&)Destination, (i32)NewValue);}
    
    i32 AtomicCompareAndSet
    (volatile i32& Destination, i32 NewValue, i32 ValueThatShouldBeInDestination);
    
    i64 AtomicCompareAndSet
    (volatile i64& Destination, i64 NewValue, i64 ValueThatShouldBeInDestination);
    
    static u32 AtomicCompareAndSet(volatile u32& Destination, u32 NewValue, u32 ValueThatShouldBeInDestination)
    { return (u32)AtomicCompareAndSet((volatile i32&)Destination, (i32)NewValue, (i32)ValueThatShouldBeInDestination); }
    
    // TODO: Do cast with assetion or use different intrinsic
    static u64 AtomicCompareAndSet(volatile u64& Destination, u64 NewValue, u64 ValueThatShouldBeInDestination)
    { return (u64)AtomicCompareAndSet((volatile i64&)Destination, (i64)NewValue, (i64)ValueThatShouldBeInDestination); }
    
    static u32 GetThreadID()
    {
        u8 *ThreadLocalStorage = (u8 *)__readgsqword(0x30);
        return *(u32*)(ThreadLocalStorage + 0x48);
    }
    
#endif
    
    struct mutex
    { volatile i32 Locked = 0; };
    
    static void Lock
    (volatile i32& Locked)
    {
#if rstd_MultiThreadingEnabled
        while(Locked || AtomicCompareAndSet(Locked, 1, 0) != 0);
#endif
    }
    
    static void Lock(mutex& Mutex)
    { Lock(Mutex.Locked); }
    
    static rstd_bool TryLock
    (volatile i32& Locked)
    {
#if rstd_MultiThreadingEnabled
        return !Locked && AtomicCompareAndSet(Locked, 1, 0) == 0;
#else
        return true;
#endif
    }
    
    static rstd_bool TryLock(mutex& Mutex)
    { return TryLock(Mutex.Locked); }
    
    static void Unlock
    (volatile i32& Locked)
    {
#if rstd_MultiThreadingEnabled
        Locked = 0;
#endif
    }
    
    static void Unlock(mutex& Mutex)
    { Unlock(Mutex.Locked); }
    
#define rstd_ScopeLock(_Mutex) \
rstd::Lock(_Mutex); \
rstd_defer(rstd::Unlock(_Mutex)); \
    
    struct thread_pool;
    
    typedef void thread_pool_job_callback(void* Data);
    
    struct thread_pool_job
    {
        void* CallbackUserData;
        thread_pool_job_callback* Callback;
    };
    
    struct thread_pool_job_node : thread_pool_job
    { thread_pool_job_node* Next; };
    
    struct thread_pool_job_list
    {
        thread_pool_job_node* NextJobToTake;
        thread_pool_job_node* LastJobToTake;
        thread_pool_job_node* JobFreeList;
        arena Arena;
        mutex Mutex;
    };
    
    struct thread_pool
    {
        thread_pool_job_list JobList;
        volatile u32 RunningJobCount;
        void* SemaphoreHandle;
        u32 ThreadCount;
    };
    
    void Init(thread_pool& Pool, u32 ThreadCount, arena ArenaResponsibleOnlyForAllocatingJobs);
    void PushJob(thread_pool&, void* JobUserData, thread_pool_job_callback* JobCallback);
    template<class job_container> void PushJobs(thread_pool& Pool, job_container Jobs);
    
    
    ///////////
    // FILES // 
    ///////////
    constexpr char FilePathSlash = '\\';
    
    struct file_info // TODO: What else should I put in file_info?
    { char* Name; };
    
    struct file
    {
        void* PlatformFileHandle;
        
        operator rstd_bool()
        { return PlatformFileHandle; }
    };
    
    struct file_stream
    {
        file File;
        u64 Pos;
        
        operator rstd_bool()
        { return File; }
    };
    
    enum class io_mode
    {
        Read,
        Write,
        ReadWrite,
    };
    
    enum class file_error
    {
        NoError,
        UnknownError,
        FileIsMissing,
        DirectoryOnPathIsMissing,
        AccessToFileWasDenied,
    };
    
    backward_singly_linked_list_with_counter<file_info> GetFileInfos(arena& Arena, const char* DirectoryPath);
    i32 RemoveFile(const char* FilePath);
    i32 RenameFile(const char* FilePath, const char* NewFilePath);
    rstd_bool FileExists(const char* FilePath);
    file OpenFile(const char* FilePath, io_mode);
    rstd_bool Close(file&);
    u32 Write(file File, u64 Pos, void* Data, u32 Size);
    u32 Read(void* Dest, file File, u64 Pos, u32 Size);
    rstd_bool SetFileSize(file File, u32 Size);
    u32 GetFileSize(file File);
    char* ReadWholeFile(arena& Arena, const char* FilePath);
    rstd_bool CreateDirectory(const char* Path);
    rstd_bool CreateDirectory(wchar_t* Path);
    rstd_bool DeleteDirectory(const char* Path);
    rstd_bool DeleteDirectory(wchar_t* Path);
    rstd_bool DeleteDirectoryWithAllContents(const char* Path);
    
    static void SetPosToEndOfFile(file_stream& Stream)
    { Stream.Pos = GetFileSize(Stream.File); }
    
    static backward_singly_linked_list_with_counter<file_info> GetFileInfos(arena& Arena, const rstd_stringlike& DirectoryPath)
    { return GetFileInfos(Arena, DirectoryPath.GetCString()); }
    
    static i32 RemoveFile(const rstd_stringlike& FilePath)
    { return RemoveFile(FilePath.GetCString()); }
    
    static i32 RenameFile(const rstd_stringlike& FilePath, const rstd_stringlike& NewFilePath)
    { return RenameFile(FilePath.GetCString(), NewFilePath.GetCString()); }
    
    static rstd_bool FileExists(const rstd_stringlike& FilePath)
    { return FileExists(FilePath.GetCString()); }
    
    static rstd_bool DirectoryExists(const rstd_stringlike& DirectoryPath)
    { return FileExists(DirectoryPath.GetCString()); }
    
    static file OpenFile(const rstd_stringlike& FilePath, io_mode Mode)
    { return OpenFile(FilePath.GetCString(), Mode); }
    
    static char* ReadWholeFile(arena& Arena, const rstd_stringlike& FilePath)
    { return ReadWholeFile(Arena, FilePath.GetCString()); }
    
#ifdef rstd_DefaultArena
    static char* ReadWholeFile(const char* FilePath)
    { return ReadWholeFile(DefaultArena, FilePath); }
    
    static char* ReadWholeFile(const rstd_stringlike& FilePath)
    { return ReadWholeFile(DefaultArena, FilePath.GetCString()); }
    
    static backward_singly_linked_list_with_counter<file_info> GetFileInfos(const char* DirectoryPath)
    { return GetFileInfos(DefaultArena, DirectoryPath); }
    
    static backward_singly_linked_list_with_counter<file_info> GetFileInfos(const rstd_stringlike& DirectoryPath)
    { return GetFileInfos(DefaultArena, DirectoryPath.GetCString()); }
#endif
    
    static rstd_bool CreateDirectory(const rstd_stringlike& Path)
    { return CreateDirectory(Path.GetCString()); }
    
    static rstd_bool DeleteDirectory(const rstd_stringlike& Path)
    { return DeleteDirectory(Path.GetCString()); }
    
    static rstd_bool DeleteDirectoryWithAllContents(const rstd_stringlike& Path)
    { return DeleteDirectoryWithAllContents(Path.GetCString()); }
    
    static void CreateFile
    (const char* FilePath)
    {
        auto File = OpenFile(FilePath, io_mode::Write);
        Close(File);
    }
    
    static void CreateFile(const rstd_stringlike& FilePath)
    { CreateFile(FilePath.GetCString()); }
    
    static char* GetRelativePath
    (char* FilePath, const char* RootDirectory)
    {
        char* C = FilePath;
        while(*C)
        {
            if(*C == '\\')
            {
                ++C;
                // TODO: This could be faster if you didn't use StringsMatch
                //       You can create AdvanceIfStringDontMatch
                if(StringsMatchUntilRightStringTerminates(C, RootDirectory))
                    return C;
            }
            ++C;
        }
        return nullptr;
    }
    
    static char* GetRelativePath(rstd_stringlike& FilePath, rstd_stringlike& RootDirectory)
    { return GetRelativePath(FilePath.GetCString(), RootDirectory.GetCString()); }
    
    static file_stream OpenFileStream
    (const char* FilePath, io_mode Mode)
    {
        auto File = OpenFile(FilePath, Mode);
        if(Mode == io_mode::Write)
            SetFileSize(File, 0);
        return {File};
    }
    
    static file_stream OpenFileStream(rstd_stringlike& FilePath, io_mode Mode)
    { return OpenFileStream(FilePath.GetCString(), Mode); }
    
    static rstd_bool Close(file_stream& Stream)
    { return Close(Stream.File); }
    
    template<class type> static u32 WriteStruct(file File, u64 Pos, type& Data, u32 Count = 1)
    { return Write(File, Pos, (void*)&Data, (u32)sizeof(type) * Count); }
    
    template<class... args> static u32 WriteString
    (file File, u64 Pos, const char* Fmt, args... Args)
    {
        // TODO: Probably we should let the user change the size of the string (sometimes 1024 characters might not be enough)
        auto String = Format<string<1024>>(Fmt, Args...);
        return Write(File, Pos, (void*)String.GetCString(), (u32)String.GetCount());
    }
    
    static u32 WriteString(file File, u64 Pos, const rstd_stringlike& String)
    { return Write(File, Pos, (void*)String.GetCString(), (u32)(String.GetCount() * sizeof(*String.GetCString()))); }
    
    static u32 WriteString(file File, u64 Pos, const char* String)
    { return Write(File, Pos, (void*)String, (u32)strlen(String)); }
    
    template<class type> static type Read
    (file File, u64 Pos)
    {
        type Res;
        Read((void*)&Res, File, Pos, (u32)sizeof(Res));
        return Res;
    }
    
    static u32 Write
    (file_stream& Stream, void* Data, u32 Size)
    {
        u32 WrittenBytes = Write(Stream.File, Stream.Pos, Data, Size);
        rstd_Assert(WrittenBytes == Size);
        Stream.Pos += Size;
        return WrittenBytes;
    }
    
    template<class type> static u32 WriteStruct(file_stream& Stream, type& Data, u32 Count = 1)
    { return Write(Stream, &Data, (u32)sizeof(type) * Count); }
    
    template<class... args> static u32 WriteString
    (file_stream& Stream, const char* Fmt, args... Args)
    {
        u32 WrittenBytes = WriteString(Stream.File, Stream.Pos, Fmt, Args...);
        Stream.Pos += WrittenBytes;
        return WrittenBytes;
    }
    
    static u32 WriteString
    (file_stream& Stream, const rstd_stringlike& String)
    { 
        u32 WrittenBytes = WriteString(Stream.File, Stream.Pos, String); 
        Stream.Pos += WrittenBytes;
        return WrittenBytes;
    }
    
    static u32 WriteString
    (file_stream& Stream, const char* String)
    { 
        u32 WrittenBytes = WriteString(Stream.File, Stream.Pos, String); 
        Stream.Pos += WrittenBytes;
        return WrittenBytes;
    }
    
    static u32 Read
    (void* Dest, file_stream& Stream, u32 Size)
    {
        u32 ReadBytes = Read(Dest, Stream.File, Stream.Pos, Size);
        rstd_Assert(ReadBytes == Size); // TODO: Should this assertion be here? Maybe there should be second function which just returns failure?
        Stream.Pos += Size;
        return ReadBytes;
    }
    
    template<class type> static type Read
    (file_stream& Stream)
    {
        auto Res = Read<type>(Stream.File, Stream.Pos);
        Stream.Pos += sizeof(Res);
        return Res;
    }
    
    template<class type> static type& GetAndAdvance
    (char** DataPtr)
    {
        auto& Res = *(type*)(*DataPtr);
        *DataPtr += sizeof(type);
        return Res;
    }
}

#ifdef rstd_Implementation

#ifdef _WIN32

#pragma comment(lib, "shell32.lib")

#ifndef rstd_ExcludeDebugPrintingFunctions
#pragma comment(lib, "user32.lib")
#endif

#include <windows.h>

namespace rstd
{
#ifndef rstd_ExcludeDebugPrintingFunctions
    ////////////////////
    // DEBUG PRINTING //
    ////////////////////
    u32 GetSystemErrorCode()
    { return GetLastError(); }
    
    void InternalPrintInDebugger(const char* Message)
    { OutputDebugStringA(Message); }
    
    void InternalWarningMessageBox(const char* Message)
    { MessageBoxA(0, Message, "Warning!", MB_ICONWARNING|MB_OK); }
    
    void ShowErrorMessageBox(const char* Message)
    { MessageBoxA(0, Message, "Error!", MB_ICONERROR|MB_OK); }
    
    void ShowErrorMessageBoxAndExitProcess
    (const char* Message)
    {
        ShowErrorMessageBox(Message);
        ExitProcess(0);
    }
#endif
    
    ///////////////////////
    // MEMORY ALLOCATION //
    ///////////////////////
    void* PageAlloc(size Bytes)
    { return VirtualAlloc(nullptr, Bytes, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE); }
    
    void PageFree(void* Memory)
    { VirtualFree(Memory, 0, MEM_RELEASE); }
    
    /////////////////////
    // MULTI-THREADING //
    /////////////////////
#ifdef rstd_ThreadPoolLogging
#if rstd_ThreadPoolLogging
    namespace ThreadPoolLogging
    {
        map<u32, char, 64> ThreadLetterMap;
        char NextLetter = 'A';
    }
    
#define ThreadPoolLog(_Str, ...) InternalPrintInDebugger(Format(_Str, __VA_ARGS__).GetCString())
    
#else
#define ThreadPoolLog(_Str, ...)
#endif
#else
#define ThreadPoolLog(_Str, ...)
#endif
    
    u32 AtomicIncrement
    (volatile u32& A)
    {
#if rstd_MultiThreadingEnabled
        return InterlockedIncrement(&A);
#else
        return ++A;
#endif
    }
    
    u32 AtomicDecrement
    (volatile u32& A)
    {
#if rstd_MultiThreadingEnabled
        return InterlockedDecrement(&A);
#else
        return --A;
#endif
    }
    
    u64 AtomicIncrement
    (volatile u64& A)
    {
#if rstd_MultiThreadingEnabled
        return InterlockedIncrement64((LONG64*)&A);
#else
        return ++A;
#endif
    }
    
    u64 AtomicDecrement
    (volatile u64& A)
    {
#if rstd_MultiThreadingEnabled
        return InterlockedDecrement64((LONG64*)&A);
#else
        return --A;
#endif
    }
    
    i32 AtomicSet
    (volatile i32& Destination, i32 NewValue)
    {
#if rstd_MultiThreadingEnabled
        i32 InitialValueInDestination = InterlockedExchange((LONG*)&Destination, NewValue);
#else
        i32 InitialValueInDestination = Destination;
        Destination = NewValue;
#endif
        return InitialValueInDestination;
    }
    
    i32 AtomicCompareAndSet
    (volatile i32& Destination, i32 NewValue, i32 ValueThatShouldBeInDestination)
    {
#if rstd_MultiThreadingEnabled
        i32 ValueThatReallyIsInDestination = InterlockedCompareExchange((volatile LONG*)&Destination,
                                                                        NewValue, ValueThatShouldBeInDestination);
        return ValueThatReallyIsInDestination;
#else
        Destination = NewValue;
        return Destination;
#endif
    }
    
    i64 AtomicCompareAndSet
    (volatile i64& Destination, i64 NewValue, i64 ValueThatShouldBeInDestination)
    {
#if rstd_MultiThreadingEnabled
        i64 ValueThatReallyIsInDestination = InterlockedCompareExchange64(&Destination, NewValue, ValueThatShouldBeInDestination);
        return ValueThatReallyIsInDestination;
#else
        Destination = NewValue;
        return Destination;
#endif
    }
    
#if rstd_MultiThreadingEnabled
    thread_pool_job* PopJob
    (thread_pool_job_list& List)
    {
        rstd_Assert(List.Mutex.Locked);
        auto* JobToDo = List.NextJobToTake;
        if(List.NextJobToTake == List.LastJobToTake)
            List.NextJobToTake = List.LastJobToTake = nullptr;
        else
            List.NextJobToTake = List.NextJobToTake->Next;
        return JobToDo;
    }
#endif
    
#if rstd_MultiThreadingEnabled
    DWORD WINAPI ThreadProc
    (LPVOID ThreadPoolVoidPtr)
    {
#if rstd_ThreadPoolLogging
        u32 ThreadId = GetThreadID();
        char ThreadLetter = ThreadPoolLogging::ThreadLetterMap.FindEqualValueWithAssert(ThreadId);
#endif
        ThreadPoolLog("Thread % starts\n", ThreadId);
        
        thread_pool& ThreadPool = *(thread_pool*)ThreadPoolVoidPtr;
        auto& List = ThreadPool.JobList;
        for(;;)
        {
            ThreadPoolLog("Thread % at the begining of loop\n", ThreadLetter);
            while(List.NextJobToTake)
            {
                if(TryLock(List.Mutex))
                {
                    ThreadPoolLog("Mutex is locked by thread: %\n", ThreadLetter);
                    
                    auto* CurrentJob = PopJob(List);
                    
                    AtomicIncrement(ThreadPool.RunningJobCount);
                    Unlock(List.Mutex);
                    ThreadPoolLog("Mutex is unlocked by thread: %\n", ThreadLetter);
                    
                    ThreadPoolLog("Thread % About to call Callback\n", ThreadLetter);
                    CurrentJob->Callback(CurrentJob->CallbackUserData);
                    AtomicDecrement(ThreadPool.RunningJobCount);
                }
            }
            ThreadPoolLog("Thread % going to sleep\n", ThreadLetter);
            WaitForSingleObjectEx(ThreadPool.SemaphoreHandle, INFINITE, FALSE);
            ThreadPoolLog("Thread % awakes\n", ThreadLetter);
        }
    }
#endif
    
    void Init
    (thread_pool& Pool, u32 ThreadCount, arena Arena)
    {
#if rstd_MultiThreadingEnabled
        Pool = {};
        
        Pool.JobList.Arena = Arena;
        Pool.SemaphoreHandle = CreateSemaphore(0, 0, ThreadCount, nullptr);
        Pool.ThreadCount = ThreadCount;
        
        for(u32 ThreadIndex = 0; ThreadIndex < ThreadCount; ++ThreadIndex)
        {
            DWORD ThreadId;
            auto ThreadHandle = CreateThread(0, 0, ThreadProc, &Pool, 0, &ThreadId);
            
#if rstd_ThreadPoolLogging
            ThreadPoolLogging::ThreadLetterMap.Push(ThreadId, ThreadPoolLogging::NextLetter++);
#endif
            
            CloseHandle(ThreadHandle); // TODO: Support changing number of threads in runtime?
        }
#endif
    }
    
    void PushJob
    (thread_pool& Pool, void* JobUserData, thread_pool_job_callback* JobCallback)
    {
#if rstd_MultiThreadingEnabled
        auto& List = Pool.JobList;
        auto* JobNode = &rstd_PushStructZero(List.Arena, thread_pool_job_node);
        JobNode->CallbackUserData = JobUserData;
        JobNode->Callback = JobCallback;
        
        Lock(List.Mutex);
        ThreadPoolLog("Mutex is locked by PushJob\n");
        if(List.NextJobToTake)
        {
            List.LastJobToTake->Next = JobNode;
            List.LastJobToTake = JobNode;
        }
        else
        {
            List.NextJobToTake = List.LastJobToTake = JobNode;
        }
        Unlock(List.Mutex);
        ThreadPoolLog("Mutex is unlocked by PushJob\n");
        
        ReleaseSemaphore(Pool.SemaphoreHandle, 1, 0);
        ThreadPoolLog("Thread should be awaken now\n");
#else
        JobCallback(JobUserData);
#endif
    }
    
    template<class job_container>
        void PushJobs
    (thread_pool& Pool, job_container Jobs)
    {
#if rstd_MultiThreadingEnabled
        auto& List = Pool.JobList;
        
        u32 PushedJobCount = Jobs.GetCount();
        
        Lock(List.Mutex);
        ThreadPoolLog("Mutex is locked by PushJob\n");
        
        if(!List.NextJobToTake)
        {
            auto LastJob = Jobs.GetAndPopLast();
            auto* JobNode = rstd_PushStruct(&List.Arena, thread_pool_job_node{LastJob});
            List.NextJobToTake = List.LastJobToTake = JobNode;
        }
        
        for(auto Job : Jobs)
        {
            auto* JobNode = rstd_PushStruct(&List.Arena, thread_pool_job_node{Job});
            List.LastJobToTake->Next = JobNode;
            List.LastJobToTake = JobNode;
        }
        
        Unlock(List.Mutex);
        ThreadPoolLog("Mutex is unlocked by PushJob\n");
        
        u32 ThreadsToAwakeCount = PushedJobCount > Pool.ThreadCount ? Pool.ThreadCount : PushedJobCount;
        LONG WorkingThreadCount = 0;
        ThreadPoolLog("ReleaseSemaphore 1 - ThreadsToAwakeCount: %\n", ThreadsToAwakeCount);
        if(!ReleaseSemaphore(Pool.SemaphoreHandle, ThreadsToAwakeCount, &WorkingThreadCount))
        {
            ThreadsToAwakeCount = Pool.ThreadCount - WorkingThreadCount;
            ThreadPoolLog("ReleaseSemaphore 2 - ThreadsToAwakeCount: %, WorkingThreadCount: %\n", ThreadsToAwakeCount, WorkingThreadCount);
            ReleaseSemaphore(Pool.SemaphoreHandle, ThreadsToAwakeCount, nullptr);
        }
#else
        for(auto Job : Jobs)
            Job.Callback(Job.CallbackUserData);
#endif
    }
    
    void CompleteAllJobs
    (thread_pool& Pool)
    {
#if rstd_MultiThreadingEnabled
        auto& List = Pool.JobList;
        while(List.NextJobToTake || Pool.RunningJobCount)
        {
            Lock(List.Mutex);
            auto* Job = PopJob(List);
            Unlock(List.Mutex);
            if(Job)
                Job->Callback(Job->CallbackUserData);
        }
#endif
    }
    
    ///////////
    // FILES //
    ///////////
#if rstd_FileDebugEnabled
    
    struct file_debug
    {
        struct open_file
        {
            string<> FilePath;
            void* PlatformFileHandle;
        };
        
        backward_singly_linked_list_with_counter<open_file> OpenFiles;
        mutex Mutex;
        
        file_debug()
        { OpenFiles = {OwnArena(rstd_AllocateArenaZero(MegabytesToBytes(1)))}; }
        
        void OnOpenFile
        (const char* FilePath, void* PlatformFileHandle)
        {
            rstd_ScopeLock(Mutex);
            OpenFiles.Push({FilePath, PlatformFileHandle});
        }
        
        void OnCloseFile
        (void* PlatformFileHandle)
        {
            rstd_ScopeLock(Mutex);
            OpenFiles.RemoveFirstIfWithAssert(
                                              [=](auto& File){ return File.PlatformFileHandle == PlatformFileHandle; });
        }
    };
    
#else
    
    struct file_debug
    {
        void OnOpenFile(const char* FilePath, void* PlatformFileHandle) {}
        void OnCloseFile(void* PlatformFileHandle) {}
    };
    
#endif
    
    static file_debug FileDebug;
    
    
    file_error GetLastFileError()
    {
        auto Error = GetLastError();
        switch(Error)
        {
            case ERROR_SUCCESS: return file_error::NoError;
            case ERROR_FILE_NOT_FOUND: return file_error::FileIsMissing;
            case ERROR_PATH_NOT_FOUND: return file_error::DirectoryOnPathIsMissing;
            case ERROR_INVALID_HANDLE: return file_error::DirectoryOnPathIsMissing; // TODO: Can't ERROR_INVALID_HANDLE be produced in other cases?
            case ERROR_ACCESS_DENIED: return file_error::AccessToFileWasDenied;
            default: return file_error::UnknownError;
        }
    }
    
    backward_singly_linked_list_with_counter<file_info> GetFileInfos
    (arena& Arena, const char* DirectoryPath)
    {
        backward_singly_linked_list_with_counter<file_info> FileInfos = ShareArena(Arena);
        
        // TODO: Support unicode file names
        
        string<258> Path = DirectoryPath; // TODO: Add support for 32767 character paths! (I guess in a separate function)
        Path += "/*";
        
        WIN32_FIND_DATA FindData;
        HANDLE FindHandle = FindFirstFileA(Path.GetCString(), &FindData); // name of first found thing is "." TODO: Make sure it's true
        if(FindHandle != INVALID_HANDLE_VALUE)
        {
            FindNextFileA(FindHandle, &FindData); // name of second found thing is ".." TODO: Make sure it's true
            
            while(FindNextFileA(FindHandle, &FindData))
            {
                file_info FileInfo;
                FileInfo.Name = rstd_PushStringCopy(Arena, FindData.cFileName);
                FileInfos.Push(FileInfo);
            }
            FindClose(FindHandle);
        }
        
        return FileInfos;
    }
    
    // TODO: Try to rename those to DeleteFile and MoveFile,
    //       You can use :: operator after you wrap this library in rlsr namespace
    //       Or you can do:
    //       namespace win32
    //       {
    //            #include <windows.h>
    //       }
    
#undef CopyFile // TODO: What to do about those stupid macros and repeated names (same name in rstd and win32)
#undef CreateDirectory
    
    // TODO: Make unicode version of those functions in which path can be longer the MAX_PATH
    
    i32 RemoveFile(const char* FilePath)
    { return DeleteFileA(FilePath); }
    
    i32 RenameFile(const char* FilePath, const char* NewFilePath)
    { return MoveFile(FilePath, NewFilePath); }
    
    const rstd_bool FailIfFileWithNewPathExists = true;
    const rstd_bool OverrideFileIfFileWithNewPathExists = false;
    
    rstd_bool CopyFile(const char* ExistingFilePath, const char* NewFilePath, rstd_bool FailOrOverride)
    { return (rstd_bool)CopyFileA(ExistingFilePath, NewFilePath, (BOOL)FailOrOverride); }
    
    rstd_bool FileExists
    (const char* Path)
    {
        u32 Attrib = GetFileAttributes(Path);
        return Attrib != INVALID_FILE_ATTRIBUTES && !(Attrib & FILE_ATTRIBUTE_DIRECTORY);
    }
    
    rstd_bool DirectoryExists
    (const char* Path)
    {
        u32 Attrib = GetFileAttributes(Path);
        return Attrib != INVALID_FILE_ATTRIBUTES && (Attrib & FILE_ATTRIBUTE_DIRECTORY);
    }
    
    // TODO: Make OpenFile api support control over dwShareMode, dwCreationDisposition, dwFlagsAndAttributes
    file OpenFile
    (const char* FilePath, io_mode Mode)
    {
        file File = {};
        
        u32 DesiredAccess = 0;
        u32 CreationDisposition = 0;
        switch(Mode)
        {
            case io_mode::Read:
            {
                DesiredAccess = GENERIC_READ;
                CreationDisposition = OPEN_EXISTING;
            } break;
            
            case io_mode::Write:
            {
                DesiredAccess = GENERIC_WRITE;
                CreationDisposition = OPEN_ALWAYS;
            } break;
            
            case io_mode::ReadWrite:
            {
                DesiredAccess = GENERIC_READ | GENERIC_WRITE;
                CreationDisposition = OPEN_ALWAYS;
            } break;
            
            rstd_InvalidDefaultCase;
        }
        
        File.PlatformFileHandle = ::CreateFileA(FilePath, DesiredAccess,
                                                FILE_SHARE_READ, 0, CreationDisposition, 0, 0);
        if(File.PlatformFileHandle == INVALID_HANDLE_VALUE)
            File.PlatformFileHandle = nullptr;
        else
            FileDebug.OnOpenFile(FilePath, File.PlatformFileHandle);
        
        return File;
    }
    
    rstd_bool Close
    (file& File)
    { 
        rstd_bool Succeded = (rstd_bool)CloseHandle(File.PlatformFileHandle);
        if(Succeded)
            FileDebug.OnCloseFile(File.PlatformFileHandle);
        File.PlatformFileHandle = nullptr;
        return Succeded;
    }
    
    OVERLAPPED MakeOverlapped
    (u64 Pos)
    {
        OVERLAPPED Overlapped = {};
        Overlapped.Offset = (u32)Pos;
        Overlapped.OffsetHigh = (u32)(Pos >> 32);
        return Overlapped;
    }
    
    u32 Write
    (file File, u64 Pos, void* Data, u32 Size)
    {
        rstd_Assert(File);
        DWORD WrittenBytes;
        auto Overlapped = MakeOverlapped(Pos);
        WriteFile(File.PlatformFileHandle, Data, Size, &WrittenBytes, &Overlapped);
        // TODO: Check if GetLastError returned ERROR_IO_PENDING
        return WrittenBytes;
    }
    
    u32 Read
    (void* Dest, file File, u64 Pos, u32 Size)
    {
        rstd_Assert(File);
        DWORD ReadBytes;
        auto Overlapped = MakeOverlapped(Pos);
        ReadFile(File.PlatformFileHandle, Dest, Size, &ReadBytes, &Overlapped);
        // TODO: Check if GetLastError returned ERROR_IO_PENDING
        return ReadBytes;
    }
    
    // TODO: Add u64 version of this function
    rstd_bool SetFileSize
    (file File, u32 Size)
    {
        LARGE_INTEGER FilePointer = {};
        FilePointer.QuadPart = Size;
        auto FileHandle = File.PlatformFileHandle;
        auto Success = SetFilePointerEx(FileHandle, FilePointer, 0, FILE_BEGIN) && SetEndOfFile(FileHandle);
        return (rstd_bool)Success;
    }
    
    // TODO: Make this function work for size_t
    u32 GetFileSize
    (file File)
    { 
        LARGE_INTEGER Int;
        if(GetFileSizeEx(File.PlatformFileHandle, &Int))
            return (u32)Int.QuadPart;
        else
            return InvalidU32;
    }
    
    char* ReadWholeFile
    (arena& Arena, const char* FilePath)
    {
        auto File = OpenFile(FilePath, io_mode::Read);
        rstd_defer(Close(File));
        
        u32 FileSize = GetFileSize(File);
        if(FileSize == 0 || FileSize == InvalidU32)
        {
            return nullptr;
        }
        else
        {
            void* Content = rstd_PushSizeUninitialized(Arena, FileSize + 1);
            Read(Content, File, (u32)0, FileSize);
            char* Res = (char*)Content;
            Res[FileSize] = 0;
            return Res;
        }
    }
    
    rstd_bool CreateDirectory(const char* Path)
    { return ::CreateDirectoryA(Path, nullptr); }
    
    rstd_bool CreateDirectory(const wchar_t* Path)
    { return CreateDirectoryW(Path, nullptr); }
    
    rstd_bool DeleteDirectory(const char* Path)
    { return RemoveDirectoryA(Path); }
    
    // TODO: What's up with prepend "\\?\". Check the docs and test it!
    rstd_bool DeleteDirectory(const wchar_t* Path)
    { return RemoveDirectoryW(Path); }
    
    // TODO: Paths longer then MAX_PATH could be a problem here. Should I use unicode versions of DeleteFile and DeleteDirectory?
    rstd_bool DeleteAllContentsOfDirectory
    (const char* Path)
    {
        string<> SearchPath = Path;
        SearchPath += "/*";
        
        WIN32_FIND_DATA FindData;
        HANDLE FindHandle = FindFirstFileA(SearchPath.GetCString(), &FindData); // name of first found thing is "." TODO: Make sure it's true
        if(FindHandle == INVALID_HANDLE_VALUE)
        {
            return false;
        }
        else
        {
            FindNextFileA(FindHandle, &FindData); // name of second found thing is ".." TODO: Make sure it's true
            
            while(FindNextFileA(FindHandle, &FindData))
            {
                string<> FilePath = Path;
                FilePath += "/";
                FilePath += FindData.cFileName;
                if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    DeleteAllContentsOfDirectory(FilePath.GetCString());
                    DeleteDirectory(FilePath.GetCString());
                }
                else
                {
                    DeleteFile(FilePath.GetCString());
                }
            }
            FindClose(FindHandle);
            return true;
        }
    }
    
    rstd_bool DeleteDirectoryWithAllContents
    (const char* Path)
    {
        rstd_bool Success = DeleteAllContentsOfDirectory(Path);
        if(Success)
            DeleteDirectory(Path);
        return Success;
    }
    
    //////////
    // TIME //
    //////////
    static time ConvertToTime
    (SYSTEMTIME& St)
    {
        time Time;
        Time.Year = (u32)St.wYear;
        Time.Month = (month)St.wMonth;
        Time.Day = (u8)St.wDay;
        Time.DayOfWeek = St.wDayOfWeek == 0 ? day_of_week::Sunday : (day_of_week)St.wDayOfWeek;
        Time.Hour = (u8)St.wHour;
        Time.Minute = (u8)St.wMinute;
        Time.Second = (u8)St.wSecond;
        Time.Millisecond = St.wMilliseconds;
        return Time;
    }
    
    time GetUtcTime()
    {
        SYSTEMTIME St;
        GetSystemTime(&St);
        return ConvertToTime(St);
    }
    
    time GetLocalTime()
    {
        SYSTEMTIME St;
        GetLocalTime(&St);
        return ConvertToTime(St);
    }
    
    u64 GetSystemTimeAsUnixEpoch()
    {
        union file_time 
        {
            FILETIME FileTime;
            u64 U64;
        };
        file_time Res;
        GetSystemTimeAsFileTime(&Res.FileTime);
        u64 DifferenceBetweenWindowsEpochAndUnixEpoch = 116444736000000000;
        Res.U64 -= DifferenceBetweenWindowsEpochAndUnixEpoch;
        return Res.U64;
    }
    
#endif // _WIN32
    
#if rstd_MemoryProfilerEnabled
    //////////////////
    // MEMORY DEBUG //
    //////////////////
    namespace MemoryDebug
    {
        struct statistics
        {
            u32
                Used, UsedPeak,
            Size, SizePeak,
            Wasted, Unused;
        };
        
        struct arena_debug_data
        {
            doubly_linked_list<memory_block> MemoryBlocks;
            const char* Name;
            const char* MasterArenaName;
            statistics Stats;
            calling_info CreationCallingInfo;
            rstd_bool HasNamelessName;
            rstd_bool TemporaryMemory;
        };
        
        struct allocation
        {
            calling_info CallingInfo;
            const char* AllocatorName;
            void* Memory;
            u32 Size;
            allocation_type Type;
        };
        
        struct deallocation
        {
            void* Memory;
            allocation_type Type;
        };
        
        struct memory_group
        {
            doubly_linked_list_with_counter<arena_debug_data> Arenas;
            doubly_linked_list<allocation> Allocations;
            doubly_linked_list<deallocation> Deallocations;
            const char* Name;
        };
        
        struct state
        {
            arena Arena;
            doubly_linked_list<memory_group> MemoryGroups;
            doubly_linked_list<allocator_name> NamelessAllocatorNames;
            statistics Stats;
            memory_group* OthersMemoryGroup;
            memory_group* CurrentMemoryGroup;
            u32 NextArenaDebugId;
            u32 NamelessAllocatorIndex;
            rstd_bool JustCalledNextAllocationMemoryGroup;
            rstd_bool RegisterAllocations;
        };
        
        global state State;
        
        struct scope_no_alloc_registration
        {
            scope_no_alloc_registration()
            { State.RegisterAllocations = false; }
            
            ~scope_no_alloc_registration()
            { State.RegisterAllocations = true; }
        };
        
#define ScopeNoAllocRegistration scope_no_alloc_registration ScopeNoAllocRegis
        
        memory_group* AddMemoryGroup
        (const char* Name)
        {
            ScopeNoAllocRegistration;
            auto& MemoryGroup = State.MemoryGroups.PushUninitialized();
            MemoryGroup.Name = Name;
            MemoryGroup.Arenas = ShareArena(State.Arena);
            MemoryGroup.Allocations = ShareArena(State.Arena);
            MemoryGroup.Deallocations = ShareArena(State.Arena);
            return &MemoryGroup;
        }
        
        void Init()
        {
            ScopeNoAllocRegistration;
            State.Arena = AllocateArenaZero(MegabytesToBytes(2), "Memory debug");
            State.MemoryGroups = ShareArena(State.Arena);
            State.NamelessAllocatorNames = ShareArena(State.Arena);
            State.OthersMemoryGroup = AddMemoryGroup("Others");
            State.CurrentMemoryGroup = State.OthersMemoryGroup;
            State.RegisterAllocations = true;
        }
        
        void BeginMemoryGroup
        (const char* Name)
        {
            auto* MemoryGroup = Find(State.MemoryGroups, [=]
                                     (auto Group){ return strcmp(Group.Name, Name) == 0; });
            
            if(!MemoryGroup)
                MemoryGroup = AddMemoryGroup(Name);
            
            State.CurrentMemoryGroup = MemoryGroup;
        }
        
        void EndMemoryGroup()
        { State.CurrentMemoryGroup = State.OthersMemoryGroup; }
        
        void NextCallMemoryGroup
        (const char* Name)
        {
            BeginMemoryGroup(Name);
            State.JustCalledNextAllocationMemoryGroup = true;
        }
        
        struct scope_memory_group
        {
            scope_memory_group(char* Name)
            { BeginMemoryGroup(Name); }
            
            ~scope_memory_group()
            { EndMemoryGroup(); }
        };
        
#define ScopeMemoryGroup(_Name) scope_memory_group ScopeMemGroup##__LINE__(_Name)
        
        static void EndNextCallMemoryGroup()
        {
            if(State.JustCalledNextAllocationMemoryGroup)
            {
                State.JustCalledNextAllocationMemoryGroup = false;
                State.CurrentMemoryGroup = State.OthersMemoryGroup;
            }
        }
        
        static void AddStatistic
        (u32* Statistic, u32* MaxStatistic, u32 Delta)
        {
            *Statistic += Delta;
            if(*Statistic > *MaxStatistic)
                *MaxStatistic = *Statistic;
        }
        
        static void AddUsedAndSubtractUnused
        (statistics* Stats, u32 Delta)
        {
            AddStatistic(&Stats->Used, &Stats->UsedPeak, Delta);
            Stats->Unused -= Delta;
        }
        
        static void AddSizeAndUnused
        (statistics* Stats, u32 Delta)
        { 
            AddStatistic(&Stats->Size, &Stats->SizePeak, Delta);
            Stats->Unused += Delta;
        }
        
        static void SubtractUsedAndAddUnused
        (statistics* Stats, u32 Delta)
        { 
            Stats->Used -= Delta;
            Stats->Unused += Delta;
        }
        
        static void SubtractSizeAndUnused
        (statistics* Stats, u32 SizeDelta, u32 UnusedDelta)
        { 
            Stats->Size -= SizeDelta; 
            Stats->Unused -= UnusedDelta;
        }
        
        static void AlterStatsOnMemoryBlockAllocation
        (statistics* Stats, u32 NewMemoryBlockSize, u32 PrevMemoryBlockUnusedBytes)
        {
            AddSizeAndUnused(Stats, NewMemoryBlockSize);
            Stats->Unused -= PrevMemoryBlockUnusedBytes;
            Stats->Wasted += PrevMemoryBlockUnusedBytes;
        }
        
        static void AlterStatsOnMemoryBlockDeallocation
        (statistics* Stats, u32 MemoryBlockToDeallocateSize, u32 PrevMemoryBlockUnusedBytes)
        {
            SubtractSizeAndUnused(Stats, MemoryBlockToDeallocateSize, PrevMemoryBlockUnusedBytes);
            Stats->Unused += PrevMemoryBlockUnusedBytes;
            Stats->Wasted -= PrevMemoryBlockUnusedBytes;
        }
        
        void RegisterCreateArena
        (arena& Arena, const char* ArenaName, const char* MasterArenaName, calling_info CallingInfo)
        {
            if(!State.RegisterAllocations)
                return;
            
            ScopeNoAllocRegistration;
            
            auto& MemGroup = *State.CurrentMemoryGroup;
            auto& ArenaDebug = MemGroup.Arenas.PushZero();
            
            ArenaDebug.MemoryBlocks = ShareArena(State.Arena);
            ArenaDebug.MemoryBlocks.Push(*Arena.MemoryBlock);
            
            u32 ArenaSize = Arena.MemoryBlock->Size;
            AddSizeAndUnused(&ArenaDebug.Stats, ArenaSize);
            AddSizeAndUnused(&State.Stats, ArenaSize);
            
            ArenaDebug.CreationCallingInfo = CallingInfo;
            
            if(ArenaName)
            {
                Arena.DebugName = ArenaDebug.Name = ArenaName;
            }
            else
            {
                u32 MemGroupArenaCount = MemGroup.Arenas.GetCount();
                auto& Name = State.NamelessAllocatorNames.PushUninitialized();
                Name = Format<allocator_name>("nameless %", MemGroupArenaCount);
                Arena.DebugName = ArenaDebug.Name = Name;
                ArenaDebug.HasNamelessName = true;
            }
            
            ArenaDebug.MasterArenaName = MasterArenaName;
            
            EndNextCallMemoryGroup();
        }
        
        static arena_debug_data& GetArenaDebug
        (arena Arena)
        {
            arena_debug_data* ArenaDebug = nullptr;
            
            for(auto& MemGroup : State.MemoryGroups)
            {
                ArenaDebug = Find(MemGroup.Arenas,
                                  [=](auto Ad){ return Ad.Name == Arena.DebugName; });
                
                if(ArenaDebug)
                    break;
            }
            
            rstd_Assert(ArenaDebug);
            return *ArenaDebug;
        }
        
        void RegisterArenaAllocateNextMemoryBlock
        (arena Arena)
        {
            if(!State.RegisterAllocations)
                return;
            
            ScopeNoAllocRegistration;
            
            auto* NewMemBlock = Arena.MemoryBlock;
            auto& ArenaDebug = GetArenaDebug(Arena);
            ArenaDebug.MemoryBlocks.Push(*NewMemBlock);
            
            rstd_AssertM(NewMemBlock->Prev, "Pay attention to word 'Next' in function name");
            
            u32 PrevMemBlockUnused = GetUnusedBytes(*NewMemBlock->Prev);
            AlterStatsOnMemoryBlockAllocation(&ArenaDebug.Stats, NewMemBlock->Size, PrevMemBlockUnused);
            AlterStatsOnMemoryBlockAllocation(&State.Stats, NewMemBlock->Size, PrevMemBlockUnused);
        }
        
        void RegisterArenaDeallocateMemoryBlock
        (arena Arena)
        {
            if(!State.RegisterAllocations)
                return;
            
            ScopeNoAllocRegistration;
            
            auto& MemBlockToDeallocate = *Arena.MemoryBlock;
            auto& ArenaDebug = GetArenaDebug(Arena);
            ArenaDebug.MemoryBlocks.PopLast();
            
            auto* PrevMemBlock = MemBlockToDeallocate.Prev;
            u32 PrevMemBlockUnused = PrevMemBlock ? GetUnusedBytes(*PrevMemBlock) : 0;
            AlterStatsOnMemoryBlockDeallocation(&ArenaDebug.Stats, MemBlockToDeallocate.Size, PrevMemBlockUnused);
            AlterStatsOnMemoryBlockDeallocation(&State.Stats, MemBlockToDeallocate.Size, PrevMemBlockUnused);
        }
        
        void SetDebugTemporaryMemory
        (temporary_memory TempMem, rstd_bool Flag)
        {
            auto& ArenaDebug = GetArenaDebug(*TempMem.Arena);
            ArenaDebug.TemporaryMemory = Flag;
        }
        
        void RegisterBeginTemporaryMemory(temporary_memory TempMem)
        { SetDebugTemporaryMemory(TempMem, true); }
        
        void RegisterEndTemporaryMemory(temporary_memory TempMem)
        { SetDebugTemporaryMemory(TempMem, false); }
        
        void RegisterArenaPush
        (arena Arena, push_size_uninitialized_ex_res Res, size Size,
         allocation_type AllocationType, calling_info CallingInfo)
        {
            if(!State.RegisterAllocations)
                return;
            
            ScopeNoAllocRegistration;
            
            auto& DebugArena = GetArenaDebug(Arena);
            
            if(DebugArena.TemporaryMemory)
                return;
            
            allocation Alloc = {CallingInfo, DebugArena.Name, Res.Memory, Size, AllocationType};
            auto& MemGroup = *State.CurrentMemoryGroup;
            MemGroup.Allocations.PushFront(Alloc);
            
            auto& DebugMemBlock = DebugArena.MemoryBlocks.GetLast();
            DebugMemBlock = *Arena.MemoryBlock;
            AddUsedAndSubtractUnused(&DebugArena.Stats, Size);
            AddUsedAndSubtractUnused(&State.Stats, Size);
            
            EndNextCallMemoryGroup();
        }
        
        void RegisterGenAlloc
        (void* Memory, size Size, calling_info CallingInfo)
        {
            if(!State.RegisterAllocations)
                return;
            
            ScopeNoAllocRegistration;
            
            allocation Alloc = {CallingInfo, "malloc", Memory, Size, allocation_type::GenAlloc};
            auto& MemGroup = *State.CurrentMemoryGroup;
            MemGroup.Allocations.PushFront(Alloc);
            AddUsedAndSubtractUnused(&State.Stats, Size);
            AddSizeAndUnused(&State.Stats, Size);
            EndNextCallMemoryGroup();
        }
        
        void RegisterGenFree
        (void* Memory, calling_info CallingInfo)
        {
            if(!State.RegisterAllocations)
                return;
            
            ScopeNoAllocRegistration;
            
            auto& MemGroup = *State.CurrentMemoryGroup;
            
            auto AllocComparison = [=](auto Alloc){ return Alloc.Memory == Memory; };
            auto* FoundAlloc = Find(MemGroup.Allocations, AllocComparison);
            if(!FoundAlloc)
            {
                for(auto& Mg : State.MemoryGroups)
                {
                    if(&Mg != &MemGroup)    
                    {
                        FoundAlloc = Find(Mg.Allocations, AllocComparison);
                        if(FoundAlloc)
                            break;
                    }    
                }
            }
            
            if(FoundAlloc)
            {
                MemGroup.Allocations.Remove(FoundAlloc);
                MemGroup.Deallocations.Push({FoundAlloc->Memory, allocation_type::GenAlloc});
                SubtractUsedAndAddUnused(&State.Stats, FoundAlloc->Size);
                SubtractSizeAndUnused(&State.Stats, FoundAlloc->Size, 0);
            }
            else
            {
                auto DeallocComparison = [=](auto Dealloc){ return Dealloc.Memory == Memory; };
                auto* FoundFreedMemory = Find(MemGroup.Deallocations, DeallocComparison);
                if(!FoundFreedMemory)
                {
                    for(auto& Mg : State.MemoryGroups)
                    {
                        if(&Mg != &MemGroup)    
                        {
                            FoundFreedMemory = Find(Mg.Deallocations, DeallocComparison);
                            if(FoundFreedMemory)
                                break;
                        }    
                    }
                }
                
                rstd_InvalidCodePathM(FoundFreedMemory ?
                                      "Memory was already freed!" : "Memory was never allocated!");
            }
        }
    }
    
#endif // rstd_MemoryProfilerEnabled
    
}

#endif // rstd_Implementation
