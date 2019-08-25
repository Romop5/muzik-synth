#ifndef LIB_CONSTEXPRUTILS_HPP
#define LIB_CONSTEXPRUTILS_HPP

#define CE_FLOAT_PARAM(p)\
    unsigned p, unsigned remainder##p

#define CE_FLOAT(v)\
    getFloat(v), getFloatRemainer(v)

#define CE_FLOAT_VAL(v)\
    static_cast<float>(v)/remainder##v

constexpr long getFloat(float val)
{
   if((val-static_cast<int>(val)) > 0.1f)
       return getFloat(val*10.0f);
   return val;
}

constexpr long getFloatRemainer(float val,float divisor = 1.0f)
{
   if((val-static_cast<int>(val)) > 0.1f)
       return getFloatRemainer(val*10.0f, divisor*10.0f);
   return divisor;
}

#endif
