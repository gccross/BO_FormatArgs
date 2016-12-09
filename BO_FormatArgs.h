// BO_FormatArgs.h
//
// This class holds arguments for a formattable Unicode string.  
// The class type to use is BO_FormatArgs, with the interface 
// presented in the template BO_FormatArgs_t. 
// 
// A contrived example of how to use this class:
//
//     const char* username = "Joe" 
//     size_t mbox_size = 96;
//     size_t mbox_limit = 128;
//     BO_FormatArgs fa;
//     fa << username << mbox_size << mbox_limit;
//     BO_S quotaLimitResourceStr = BO_TEXT("QuotaOverLimit",NULL, &msgArgs); // L10N string
//
// Here are some internal details, if you are interested. If you need a pointer 
// to an array of type T holding the values you have inserted as above, 
// BO_FormatArgs_t::operator T const*() can help.  For example, in 
// BO_LocaleResource::getText() you'll see:
//
//     
//     UnicodeString formattedstr;
//
//     UErrorCode status = U_ZERO_ERROR;
//     FieldPosition pos;
//     MessageFormat msg(pattern, status);
//           
//     msg.format(*format_args,   // MessageFormat::format() takes an icu::Formattable[]
//                 format_args->size(),
//                 formattedstr, 
//                 pos, status); 
// 
// In passing argument type BO_FormatArgs to a function taking argument 
// with type Formattable[], the coversion operator 
// BO_FormatArgs_t<>::operator T const*() satisfies the type match.
//

#include <vector>

template <class T>
class BO_FormatArgs_t 
{
private:
    std::vector<T> mFmtable;
public:

    size_t size() const;

    operator T const*() const;

    template<class U> 
    BO_FormatArgs_t<T>& operator << (U const&  u);

};
class BO_FormatArgs;

#include <BO_FormatArgs.prv>

---------------------------------------------------------------------------------------------------
