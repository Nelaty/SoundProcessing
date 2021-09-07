/* 
 * Created by Matthias "Nelaty" Gensheimer on 25.06.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once
#include "sp/import/soundFileImporter.hpp"

#include <spdlog/spdlog.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>
#include <string>
#include <thread>

namespace sp
{
    class Player
    {
    public:
        Player(const std::filesystem::path& dir)
            : soundDir(dir)
        {
            if(const auto result = alIsExtensionPresent("EAX2.0"); result){
                spdlog::info("EAX2.0 extension is present");
            }
        }

        ~Player()
        {
            cleanup();
        }

        void start()
        {

        }


        void playSound(const std::filesystem::path& path)
        {
            init();

            const auto wav = sp::SoundFileImporter::import(path);

            // ... and fill it with data
            //std::cout << wav.frequency << "\n";
            //std::cout << sp::format::name(wav.format) << "\n";

            alBufferData(buffer, static_cast<uint16_t>(wav.format), wav.data.data(), wav.data.size(), wav.frequency);
            if(const auto error = alGetError(); error != AL_NO_ERROR){
                spdlog::error("{} {}: buffer initialization.", static_cast<int>(error), alErrorConversion(error));
            } else {
                spdlog::info("Buffer created and initialized");
            }

            // Play source
            alSourcePlay(source);
            if(const auto error = alGetError(); error != AL_NO_ERROR){
                spdlog::error("{} {}: playing the sound file.", static_cast<int>(error), alErrorConversion(error));
            }

            // Wait for audio to finish playing
            std::this_thread::sleep_for(std::chrono::milliseconds(wav.durationMillis));

            cleanup();
        }

        void initPlayAndCleanup(const std::filesystem::path& path)
        {
            const auto wav = sp::SoundFileImporter::import(path);
            initPlayAndCleanup(wav);
        }

        void initPlayAndCleanup(const SoundFileData& data)
        {
            if(data.data.empty())
            {
                spdlog::warn("Sound file data is empty: {}", data.path.c_str());
                return;
            }

            // Create device
            device = alcOpenDevice(nullptr);
            if(device){
                spdlog::info("Device was created");
                context = alcCreateContext(device, nullptr);
                alcMakeContextCurrent(context);
            }
            alGetError();

            // Create buffer
            alGenBuffers(1, &buffer);
            if(const auto error = alGetError(); error != AL_NO_ERROR){
                spdlog::error("{} {}: buffer creation.", error, alErrorConversion(error));
            }

            // ... and fill it with data
            std::cout << data.frequency << "\n";
            std::cout << sp::format::name(data.format) << "\n";

            alBufferData(buffer, AL_FORMAT_MONO16, data.data.data(), data.data.size(), data.frequency);
            if(const auto error = alGetError(); error != AL_NO_ERROR){
                spdlog::error("{} {}: buffer initialization.", error, alErrorConversion(error));
            } else {
                spdlog::info("Buffer created and initialized");
            }

            // Create source
            alGenSources(1, &source);
            if(const auto error = alGetError(); error != AL_NO_ERROR){
                spdlog::error("{} {}: source creation.", error, alErrorConversion(error));
            }

            // Attach buffer to sources
            alSourcei(source, AL_BUFFER, buffer);
            if(const auto error = alGetError(); error != AL_NO_ERROR){
                spdlog::error("{} {}: attachment of buffer to source.", error, alErrorConversion(error));
            }

            alListener3f(AL_POSITION, 0.f, 0.f, 0.f);
            alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f);
            alListenerf(AL_GAIN, 1.f);

            // Play source
            alSourcePlay(source);
            if(const auto error = alGetError(); error != AL_NO_ERROR){
                spdlog::error("{} {}: playing the sound file.", error, alErrorConversion(error));
            }

            // Wait for audio to finish playing
            std::this_thread::sleep_for(std::chrono::milliseconds(data.durationMillis));

            cleanup();
        }

        void init()
        {
            // Create device
            device = alcOpenDevice(nullptr);
            if(device){
                spdlog::info("Device was created");
                context = alcCreateContext(device, nullptr);
                alcMakeContextCurrent(context);
            }
            alGetError();

            // Create buffer
            alGenBuffers(1, &buffer);
            if(const auto error = alGetError(); error != AL_NO_ERROR){
                spdlog::error("{} {}: buffer creation.", error, alErrorConversion(error));
            }

            // Create source
            alGenSources(1, &source);
            if(const auto error = alGetError(); error != AL_NO_ERROR){
                spdlog::error("{} {}: source creation.", error, alErrorConversion(error));
            }

            // Attach buffer to sources
            alSourcei(source, AL_BUFFER, buffer);
            if(const auto error = alGetError(); error != AL_NO_ERROR){
                spdlog::error("{} {}: attachment of buffer to source.", error, alErrorConversion(error));
            }

            alListener3f(AL_POSITION, 0.f, 0.f, 0.f);
            alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f);
            alListenerf(AL_GAIN, 1.f);
        }

        void cleanup()
        {
            alcMakeContextCurrent(nullptr);
            alcDestroyContext(context);
            alcCloseDevice(device);
        }

        std::string alErrorConversion(ALuint alerror)
        {
            switch (alerror) {
                case AL_NO_ERROR:
                    return "AL_NO_ERROR";
                case ALC_INVALID_DEVICE:
                    return "AL_INVALID_DEVICE";
                case ALC_INVALID_CONTEXT:
                    return "AL_INVALID_CONTEXT";
                case AL_INVALID_OPERATION:
                    return "AL_INVALID_OPERATION";
                case AL_INVALID_VALUE:
                    return "AL_INVALID_VALUE";
                case AL_OUT_OF_MEMORY:
                    return "AL_OUT_OF_MEMORY";
                    /* ... */
                default:
                    return "Unknown error code";
            }
        }

    private:
        std::filesystem::path soundDir;

        ALuint buffer;
        ALuint source;

        ALCcontext* context;
        ALCdevice* device;
    };
}