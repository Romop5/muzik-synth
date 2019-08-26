#include "constexprutils.hpp"
#include "muzik.hpp"
//#include <iostream>
//#include <fstream>
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
#define INTERVAL(x,y)\
    Range<F(x),F(y)>

using namespace muzik;

template<unsigned a, unsigned b, unsigned c>
using simpleSin = Sin<a,b,c>;

/*
int main()
{
    //Sin<F(50),Const<F(1)>> synth;
    //using synA = Sin<F(700),Const<F(1)>>;
    using synA = Sin<F(1), Sin<F(50),Const<F(1)>>>;
    using synB = Sin<F(5), Sin<F(80),Const<F(1)>>>;
    using synC = Noise;
    //using result = MixNormalize<synA, synB>;
    using train = Sin<F(5),Noise>;
    using trainWithHumming = MixNormalize<synA, synB,train>;
    //using result = Sin<F(1600), trainWithHumming,INTERVAL(0.6,0.9)>;
    using spikes = SawTooth<F(40)>;
    //using result = Sin<F(5),Sin<F(800)>>;
    //
    //using result = Harmonics<100,1, SawTooth>;
    using result = AbsDifference<SawTooth<F(100)>, SawTooth<F(100)>>;

    result synth;

    std::ofstream output;
    output.open("audio.pcm", std::ofstream::out | std::ofstream::binary);
    if(!output.is_open())
    {
        std::cout << "Failed to open file for output" << std::endl;
        return 1;
    }

    size_t length = 10;
    for(size_t i = 0; i < muzik::SAMPLING_RATE*length; i += 1)
    {
        output << encode<int8_t>(synth.value(i));
    }

    static_assert(getFloat(11.5) == 115, "LOL");
    constexpr float number = -10.567;
    std::cout << getFloat(number) << "/" << getFloatRemainer(number) << std::endl;
    std::cout << (getFloat(number)/getFloatRemainer(number)) << std::endl;

    std::cout << RANGE_NORMAL::getMin() << " to " << RANGE_NORMAL::getMax() << std::endl;
}
*/

int main()
{
    using synA = Sin<F(1), Sin<F(50),Const<F(1)>>>;
    using synB = Sin<F(5), Sin<F(80),Const<F(1)>>>;
    using synC = Noise;
    //using result = MixNormalize<synA, synB>;
    using train = Sin<F(5),Noise>;
    using trainWithHumming = MixNormalize<synA, synB,train>;
    //using result = Sin<F(1600), trainWithHumming,INTERVAL(0.6,0.9)>;
    using spikes = SawTooth<F(40)>;
    //using result = Sin<F(5),Sin<F(800)>>;
    //
    //using result = Harmonics<100,1, SawTooth>;
    using result = AbsDifference<SawTooth<F(100)>, SawTooth<F(100)>>;

    result synth;

    unsigned char* buff = new unsigned char[44100];

    size_t length = 1;
    for(size_t i = 0; i < muzik::SAMPLING_RATE*length; i += 1)
    {
        buff[i] = encode<int8_t>(synth.value(i));
    }
    delete[] buff;
}


