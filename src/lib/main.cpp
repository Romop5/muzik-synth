#include "constexprutils.hpp"
#include "muzik.hpp"
#include <iostream>
#include <fstream>
#include <limits>
#include <type_traits>

template<typename T>
T encode(float f)
{
    if(std::is_unsigned<T>::value == true)
    {
        return static_cast<T>((f*0.9*std::numeric_limits<T>::max()/2.0f) + (std::numeric_limits<T>::max()/2.0f));
    }
    return static_cast<T>(f*0.9*std::numeric_limits<T>::max());
}


#define F(x)\
    CE_FLOAT(x)
int main()
{
    using namespace muzik;
    //Sin<F(50),Const<F(1)>> synth;
    //using synA = Sin<F(700),Const<F(1)>>;
    using synA = Sin<F(1), Sin<F(50),Const<F(1)>>>;
    using synB = Sin<F(5), Sin<F(80),Const<F(1)>>>;
    using synC = Noise;
    //using result = MixNormalize<synA, synB>;
    using train = Sin<F(5),Noise>;
    using result = MixNormalize<synA, synB,train>;

    result synth;

    std::ofstream output;
    output.open("audio.pcm", std::ofstream::out | std::ofstream::binary);
    if(!output.is_open())
    {
        std::cout << "Failed to open file for output" << std::endl;
        return 1;
    }

    size_t fSample = 44100;
    size_t length = 10;
    for(size_t i = 0; i < fSample*length; i += 1)
    {
        std::cout << "VAL: " << synth.value(i) << std::endl;
        output << encode<int8_t>(synth.value(i));
    }

    static_assert(getFloat(11.5) == 115, "LOL");
    constexpr float number = 1.332323;
    std::cout << getFloat(number) << "/" << getFloatRemainer(number) << std::endl;
}
