#include <alsa/asoundlib.h>
#include <stdint.h>
#include <string>
#include <iostream>
#include <unistd.h>

using namespace std;

typedef struct WaveHeader 
{
    char RIFF_marker[4];            
    uint32_t file_size;             
    char filetype_header[4];        
    char format_marker[4];          
    uint32_t data_header_length;    
    uint16_t format_type;          
    uint16_t number_of_channels;    
    uint32_t sample_rate;           
    uint32_t bytes_per_second;      
    uint16_t bytes_per_frame;       
    uint16_t bits_per_sample;       
} wav_header;

class Alarm
{
private:
    int rc;
    char * buffer;
    int buffer_size;
    int periods_per_buffer;
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames;

    unsigned int channels;
    unsigned int rate;

    wav_header * wav_header_info;

    FILE * fp;

    const string filename = "./beep.wav";
    
public:
    Alarm();
    ~Alarm();
    void ring();
    

};
