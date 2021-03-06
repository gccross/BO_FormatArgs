// BO_FormatArgs.prv

// BO_FormatArgs_t implementation. 
// Kept in this private source file. No need to expose the implementation details

template <class T>
inline size_t BO_FormatArgs_t<T>::size() const
{ 
    return mFmtable.size(); 
}

template <class T>
inline BO_FormatArgs_t<T>::operator T const*() const
{
    // vector elements are contiguous, according to standard
    // so, passing this to MessageFormat::format should work... 
    return &(mFmtable[0]);
}

template<class T>
template<class U>
inline BO_FormatArgs_t<T>& BO_FormatArgs_t<T>::operator<< (U const& u)
{   
    mFmtable.push_back(T(u));
    return *this;
}

// This overload helps since ICU's Formattable only accepts signed 
// ints, longs etc.
// Add more as you might need. 
template <>
template <>
inline 
BO_FormatArgs_t<Formattable>& BO_FormatArgs_t<Formattable>::operator<< (unsigned64 const& t)
{   
    if (t>static_cast<unsigned64>(std::numeric_limits<int64>::max()))
    {
        LOG_I18N(LOG_WARNING, 
                 "BO_FormatArgs_t::operator<< : argument %llu overflows underlying capability",  
                 t);
    }

    return *this << static_cast<int64>(t);
}

// Specialization for operator<< (BO_S)
// Creates a UnicodeString from an BO_S, using platform default codepage
// You might want/need to provide another implementation 
// (say, in the derived class) which more intelligently maps
// the codepage of the BO_S with a codepage name which the 
// platform (currently ICU on linux) can understand.
template <>
template <>
inline
BO_FormatArgs_t<Formattable>& BO_FormatArgs_t<Formattable>::operator << (BO_S const&  lxs)
{
    UnicodeString us(lxs, lxs.size());
    return  *this << us;
}

// Templatize it on ICU's Formattable class
class BO_FormatArgs : public BO_FormatArgs_t<Formattable>
{};

