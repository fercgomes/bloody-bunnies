#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>
#include <thread>

#define MUS_PATH "k";

class Audio
{

public:
    static void play_func(const char* path)
    {
        SDL_AudioSpec wavSpec;
        Uint32 wavLength;
        Uint8* wavBuffer;

        SDL_LoadWAV(path, &wavSpec, &wavBuffer, &wavLength);

        SDL_AudioDeviceID deviceID = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

        int sucess = SDL_QueueAudio(deviceID, wavBuffer, wavLength);
        SDL_PauseAudioDevice(deviceID, 0);

        SDL_Delay(1000 * 100);

        SDL_CloseAudioDevice(deviceID);
        SDL_FreeWAV(wavBuffer);
    }

    static void play(const char* path)
    {
        std::thread tst(play_func, path);
        tst.detach();
    }
};