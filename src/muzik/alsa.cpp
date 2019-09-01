//#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <cmath>
#include <climits>
      
int main (int argc, char *argv[])
{
    int i;
    int err;
    constexpr float FREQ = 44100;
    constexpr float PERIOD = 1.0f/FREQ;
    constexpr size_t SEQ = 128;
    short buf[SEQ];
    snd_pcm_t *playback_handle;
    snd_pcm_hw_params_t *hw_params;

    if ((err = snd_pcm_open (&playback_handle, argv[1], SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        fprintf (stderr, "cannot open audio device %s (%s)\n", 
             argv[1],
             snd_strerror (err));
        exit (1);
    }
       
    if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
        fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
             snd_strerror (err));
        exit (1);
    }
             
    if ((err = snd_pcm_hw_params_any (playback_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
             snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_set_access (playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        fprintf (stderr, "cannot set access type (%s)\n",
             snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_set_format (playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
        fprintf (stderr, "cannot set sample format (%s)\n",
             snd_strerror (err));
        exit (1);
    }

    unsigned int sample_rate = 44100;
    if ((err = snd_pcm_hw_params_set_rate_near (playback_handle, hw_params, &sample_rate, 0)) < 0) {
        fprintf (stderr, "cannot set sample rate (%s)\n",
             snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_set_channels (playback_handle, hw_params, 1)) < 0) {
        fprintf (stderr, "cannot set channel count (%s)\n",
             snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params (playback_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot set parameters (%s)\n",
             snd_strerror (err));
        exit (1);
    }

    snd_pcm_hw_params_free (hw_params);

    if ((err = snd_pcm_prepare (playback_handle)) < 0) {
        fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
             snd_strerror (err));
        exit (1);
    }

    for (i = 0; i < (1ll<<16); i++) {
        for(size_t sample = 0; sample < SEQ; sample++)
        {
            buf[sample] = short(SHRT_MAX*sin(700.0*PERIOD*float(i*SEQ+sample)*2.0f*3.1415f));
            //std::cout << "Sample: " << sin(PERIOD*float(i*SEQ+sample)*2.0f*3.1415f) << std::endl;
            //std::cout << static_cast<short>(buf[sample]) << std::endl;
        }

        if ((err = snd_pcm_writei (playback_handle, buf, SEQ)) != SEQ) {
            fprintf (stderr, "write to audio interface failed (%s)\n",
                 snd_strerror (err));
            exit (1);
        }
        
    }

    snd_pcm_close (playback_handle);
    exit (0);
}
