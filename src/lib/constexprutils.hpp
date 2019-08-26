#ifndef LIB_CONSTEXPRUTILS_HPP
#define LIB_CONSTEXPRUTILS_HPP

#define CE_FLOAT_PARAM(p)\
    unsigned p, unsigned remainder##p, unsigned sign##p

#define CE_FLOAT(v)\
    ceAbs(getFloat(v)), ceAbs(getFloatRemainer(v)), ((getFloat(v) > 0)?0:1)

#define CE_FLOAT_VAL(v)\
    ((static_cast<float>(v)/remainder##v)*((sign##v == 1)?-1.0f:1.0f))

template<typename T>
constexpr T ceAbs(T val)
{
    return (val > 0)?val:-val;
}
constexpr long getFloat(float val)
{
   if(ceAbs((val-static_cast<int>(val))) > 0.1f)
       return getFloat(val*10.0f);
   return val;
}

constexpr long getFloatRemainer(float val,float divisor = 1.0f)
{
   if(ceAbs((val-static_cast<int>(val))) > 0.1f)
       return getFloatRemainer(val*10.0f, divisor*10.0f);
   return divisor;
}

#endif
