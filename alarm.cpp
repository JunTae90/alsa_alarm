#include "alarm.hpp"

Alarm::Alarm()
{
    fp = fopen(filename.c_str(), "rb");
    wav_header_info = (wav_header *)malloc(sizeof(wav_header));
    fread(wav_header_info, 1, sizeof(wav_header), fp);
    channels = wav_header_info->number_of_channels;
    rate = wav_header_info->sample_rate;
    periods_per_buffer = 2; 

    if ((rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) 
    {
        cout << "ERROR: Cannot open pcm device. " << snd_strerror(rc) << endl;
    }

    if ((rc = snd_pcm_hw_params_malloc(&params)) < 0)
    {
        cout << "ERROR: Cannot allocate hardware parameters. " << snd_strerror(rc) << endl;
    }

    if ((rc = snd_pcm_hw_params_any(handle, params)) < 0)
    {
        cout << "ERROR: Cannot initialize hardware parameters. " << snd_strerror(rc) << endl;
    }

    if ((rc = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
    {
        cout << "ERROR: Cannot set interleaved mode. " << snd_strerror(rc) << endl;
    }

    if ((rc = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE)) < 0)
    {
        cout << "ERROR: Cannot set PCM format. " << snd_strerror(rc) << endl;
    }

    if ((rc = snd_pcm_hw_params_set_channels_near(handle, params, &channels)) < 0)
    {
        cout << "ERROR: Cannot set number of channels. " << snd_strerror(rc) << endl;
    }

    if ((rc = snd_pcm_hw_params_set_rate_near(handle, params, &rate, 0)) < 0)
    {
        cout << "ERROR: Cannot set plyabck rate. " << snd_strerror(rc) << endl;
    }

    if ((rc = snd_pcm_hw_params(handle, params)) < 0)
    {
        cout << "ERROR: Cannot set hardware parameters. " << snd_strerror(rc) << endl;
    }

    if ((rc = snd_pcm_hw_params_get_period_size(params, &frames, 0)) < 0)
    {
        cout << "Playback ERROR: Can't get period size. " << snd_strerror(rc) << endl;
    }

    if ((rc = snd_pcm_hw_params_get_channels(params, &channels)) < 0)
    {
        cout << "Playback ERROR: Can't get channel number. %s\n" << snd_strerror(rc) << endl;
    }

    if ((rc = snd_pcm_hw_params_get_rate(params, &rate, 0)) < 0)
    {
        cout << "ERROR: Cannot get rate. %s\n" << snd_strerror(rc) << endl;
    }

    snd_pcm_hw_params_free(params);
    frames /= 2;
    buffer_size = frames * periods_per_buffer * channels * sizeof(int16_t);
    buffer = (char *) malloc(buffer_size);

    fread(buffer, 1, periods_per_buffer * frames * channels * sizeof(int16_t), fp);

}

Alarm::~Alarm()
{
    cout << "Info set: Device is now draining..." << endl;
    snd_pcm_drain(handle);
    cout << "Done playing, closing connections." << endl;
    snd_pcm_close(handle);

    free(wav_header_info);
    free(buffer);
    fclose(fp);
}

void Alarm::ring()
{
    rc = snd_pcm_writei(handle, buffer, frames * periods_per_buffer);
    if (rc == -EPIPE) 
    {
        snd_pcm_prepare(handle);
    } 
    else if (rc < 0) 
    {
        cout << "ERROR: Cannot write to playback device. " << strerror(rc) << endl;
    }
    usleep(100000);
    
}
