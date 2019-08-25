#ifndef LIB_MUZIK_HPP
#define LIB_MUZIK_HPP

#include "constexprutils.hpp"
#include <cmath>

namespace muzik
{

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

/**
 * @brief Returns /-|/- signal with frequency FREQ
 */
template<CE_FLOAT_PARAM(FREQ)>
struct SawTooth
{
    static inline float value(float time)
    {
        constexpr auto frequency = CE_FLOAT_VAL(FREQ);
        constexpr auto period = 1.0f/frequency;
        return fmod(time, period)/period;
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


template<CE_FLOAT_PARAM(FREQ), typename T>
struct Sin: public T
{
    static inline float value(float time)
    {
        constexpr float frequency = CE_FLOAT_VAL(FREQ);
        constexpr auto phase = 2.0f*M_PI*frequency/44100;
        return sin(time*phase)*T::value(time);
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


}
#endif
