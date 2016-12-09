// runtime_type_checks.h
//
// This template function is used to ensure at runtime the value you want to assign
// to a variable will not incur data loss due to implicit numeric conversion.
// One can detect such problems at compile time using warnings for implicit 
// conversions (gcc4.3+ -Wconversion)  or (SunCC5.9 +w). And if the types can't 
// be matched, (due to design criteria) then this can provide safety.
//
// This won't safeguard against signed/unsigned comparison when U is signed. 
// Although the compiler should warn. 
#include <limits>
#include <algorithm>
template <typename T, typename U>
bool BO_Check_WideEnough_and_Assign(T& t, U u)
{
    //see if T is wide enough for U's value
    if (u > std::numeric_limits<T>::max()) return false;
    t = static_cast<T>(u);
    return true;
};

// These templates merely ensures an addition,subtraction ('+','-') 
// operation will not overflow a result of the same type.
// According to ISO C such an operation must cause SIGFPE:FPE_INTOVF, with 
// default action to abort. But I couldn't find consistency in this.  
// On Linux/x86, the result just wraps around (eg. max+1=0).
//  
// Arithmetic Type Conversions, and Implicit Numeric Conversions are 
// a different story (which can bite you anytime, eg. -1/1U does not equal -1!).
//
// An example of using this mechanism is:
//
//    typedef unsigned long long itype;
//    itype s,t=numeric_limits<itype>::max()/2+1,u=t;
//    BO_Plus_With_Overflow_Check<itype, numeric_limits<itype>::is_signed > l;
//    if (l(t,u,s)==false)
//    {
//        cout << "overflow" << endl;
//    }

template <typename T, bool is_signed> 
struct BO_Plus_With_Overflow_Check;   // no default implemenation necessary, 
                                      // see following instantiations

template <typename T>
struct  BO_Plus_With_Overflow_Check<T, true>
{
    bool operator()(T const& left, T const& right, T& result)
    {
        result = left + right;
        return result >= std::max(left,right);
    }
};

template <typename T>
struct  BO_Plus_With_Overflow_Check<T, false>
{
    bool operator()(T const& left, T const& right, T& result)
    {
        result = left + right;
        if (left < static_cast<T>(0) && right < static_cast<T>(0))
        {
            // both negative, result should be less than operands
            return result <= std::min(left,right);
        }
        else if (left > static_cast<T>(0) && right > static_cast<T>(0))
        {
            // both positive, result should be greater than operands
            return   result >= std::max(left,right ); 

        }
        else
        {
            // mixed or zeros, can't overflow
            return true;
        }
    }

};

// This is a basic compile-time assertion mechanism. 
template <bool> class BO_Compile_Time_Check ;
template <> class BO_Compile_Time_Check<true>{} ;
#define BO_COMPILE_TIME_CHECK(x) namespace { BO_Compile_Time_Check<x> ERROR_compile_time_check; }
                                


