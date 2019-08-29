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
#define INTERVAL(x,y)\
    Range<F(x),F(y)>

using namespace muzik;

template<unsigned a, unsigned b, unsigned c>
using simpleSin = Sin<a,b,c>;

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
    using result = Sin<F(1600), trainWithHumming,INTERVAL(0.6,0.9)>;
    using spikes = SawTooth<F(80)>;
    //using result = Sin<F(5),Sin<F(800)>>;
    //
    //using result = Harmonics<100,1, SawTooth>;
    //using result = AbsDifference<SawTooth<F(100)>, SawTooth<F(100)>>;

    result synth;

    Track a;
    for(size_t i = 0; i < 8; i++)
    {
        a.play<result>(16000-i*2000); 
        a.play<spikes>(4000); 
    }
    a.play<synA>(10000);
    a.play<synB>(50000);
    a.play<synA>(10000);

    //-------------------------------------------------------------------------
    //---- OUTPUT TO FILE -----------------------------------------------------
    //-------------------------------------------------------------------------
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
        //auto value = synth.value(i);
        auto value = a.value(i);
        output << encode<int8_t>(value);
    }
}


