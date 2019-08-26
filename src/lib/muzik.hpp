#ifndef LIB_MUZIK_HPP
#define LIB_MUZIK_HPP

#include "constexprutils.hpp"
#include <cmath>

namespace muzik
{

/// Global sampling rate
constexpr size_t SAMPLING_RATE = 44100;

///////////////////////////////////////////////////////////////////////////////
// HELPERS
///////////////////////////////////////////////////////////////////////////////

template<CE_FLOAT_PARAM(MIN), CE_FLOAT_PARAM(MAX)>
struct Range
{
    public:
    static constexpr float getMin()
    {
        return CE_FLOAT_VAL(MIN);
    }
    static constexpr float getMax()
    {
        return CE_FLOAT_VAL(MAX);
    }
};

using RANGE_NORMAL = Range<CE_FLOAT(-1), CE_FLOAT(1)>;

///////////////////////////////////////////////////////////////////////////////
// BASIC SIGNAL GENERATORS/MODULATORS
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Returns a constant signal, determined by VALUE
 */
template<CE_FLOAT_PARAM(VALUE)>
struct Const
{
    static inline float value(float time)
    {
        return CE_FLOAT_VAL(VALUE);
    }
};

using ZERO = Const<CE_FLOAT(0)>;
using ONE = Const<CE_FLOAT(1)>;

/**
 * @brief Returns /-|/- signal with frequency FREQ
 */
template<CE_FLOAT_PARAM(FREQ)>
struct SawTooth
{
    static inline float value(float time)
    {
        constexpr auto frequency = CE_FLOAT_VAL(FREQ);
        constexpr auto period = SAMPLING_RATE/frequency;
        return fmod(time, period)/period;
    }
};

/**
 * @brief Returns _|-|_ signal
 */
template<CE_FLOAT_PARAM(FREQ),CE_FLOAT_PARAM(RATIO)>
struct Clock 
{
    static inline float value(float time)
    {
        constexpr auto frequency = CE_FLOAT_VAL(FREQ);
        constexpr auto period = SAMPLING_RATE/frequency;
        return (fmod(time,period) > (period*CE_FLOAT_VAL(RATIO))) ? 1.0: -1.0;
    }
};



/**
 * @brief Returns random signal <-1,1>
 */
struct Noise
{
    static inline float value(float time)
    {
        return (random()/static_cast<float>(RAND_MAX))*2-1.0;
    }
};


template<CE_FLOAT_PARAM(FREQ), typename T = ONE, typename RANGE = RANGE_NORMAL>
struct Sin: public T
{
    static inline float value(float time)
    {
        constexpr float frequency = CE_FLOAT_VAL(FREQ);
        constexpr auto phase = 2.0f*M_PI*frequency/SAMPLING_RATE;
        auto sinusValue = sin(time*phase)*T::value(time);
        auto normalizedValue = ((1.0f+sinusValue)/2.0f);
        return RANGE::getMin() + normalizedValue*(RANGE::getMax()-RANGE::getMin());
    }
};

///////////////////////////////////////////////////////////////////////////////
// MIX 
///////////////////////////////////////////////////////////////////////////////

typedef void end_t;

template<typename T,typename ...ARGS>
struct MixImpl: public T
{
    static inline float value(float time)
    {
        return T::value(time)+MixImpl<ARGS...>::value(time);
    }
};

template<>
struct MixImpl<void>
{
    static inline float value(float time)
    {
        return 0.0f;
    }
};

template<typename T,typename ...ARGS>
struct Mix: public T
{
    static inline float value(float time)
    {
        return (MixImpl<ARGS...,void>::value(time));
    }
};

template<typename T,typename ...ARGS>
struct MixNormalize: public T
{
    static inline float value(float time)
    {
        // Normalize amplitude to fit to <-1,1>
        constexpr auto normalization = 1.0f/(sizeof...(ARGS));
        return (MixImpl<ARGS...,void>::value(time))*normalization;
    }
};

template<unsigned START, unsigned END, typename T>
struct WindowSharp: public T
{
    static inline float value(float time)
    {
        if(time < START || time > END)
            return 0.0f;
        return T::value(time);
    }
};

template<unsigned LENGTH, typename T, unsigned START = 0>
struct Repeater: public T
{
    static inline float value(float time)
    {
        auto forcedTime = fmod(time, LENGTH)+START;
        return T::value(forcedTime);
    }
};


/**
 * @brief Mix instrument T with COUNT multiples of frequency BASE
 */
template<unsigned BASE, unsigned COUNT, template<unsigned, unsigned, unsigned> class T>
struct HarmonicsImpl
{
    static inline float value(float time)
    {
        return T<BASE,1,0>::value(time)+0.5*HarmonicsImpl<BASE*2,COUNT-1, T>::value(time);
    }
};

template<unsigned BASE, template<unsigned, unsigned,unsigned> class T>
struct HarmonicsImpl<BASE,0,T>
{
    static inline float value(float time)
    {
        return 0.0f;
    }
};

template<unsigned BASE, unsigned COUNT, template<unsigned, unsigned, unsigned> class T>
struct Harmonics 
{
    static inline float value(float time)
    {
        float norm = 1.0f/(pow(1.5,COUNT));
        return norm*(T<BASE,1,0>::value(time)+HarmonicsImpl<BASE*2,COUNT-1, T>::value(time));
    }
};

template<typename T1, typename T2>
struct AbsDifference 
{
    static inline float value(float time)
    {
        return ceAbs(T1::value(time)-T2::value(time));
    }
};

}
#endif
