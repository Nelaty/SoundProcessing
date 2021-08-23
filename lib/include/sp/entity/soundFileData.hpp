/* 
 * Created by Matthias "Nelaty" Gensheimer on 25.06.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once
#include "sp/util/rawData.hpp"

#include <AL/al.h>

#include <fmt/format.h>

#include <array>
#include <cstdint>
#include <string.h>
#include <filesystem>

namespace sp
{
    namespace channel
    {
        enum class Type
        {
            mono,
            stereo,
            count
        };

        struct Relation
        {
            Type type;
            uint32_t count;
            std::string_view name;
        };

        namespace detail
        {
            static constexpr auto relation = std::array{
                Relation{Type::mono, 1, "mono"},
                Relation{Type::stereo, 2, "stereo"}
            };
        }

        static constexpr Type type(uint16_t count)
        {
            for(const auto& visited : detail::relation){
                if(visited.count == count) return visited.type;
            }
            throw std::runtime_error(fmt::format("Unknown channel count: {}", count));
        }

        static constexpr std::string_view name(Type type){
            for(const auto& visited : detail::relation){
                if(visited.type == type) return visited.name;
            }
            throw std::runtime_error(fmt::format("Unknown channel type: {}", static_cast<int>(type)));
        }
    }

    namespace format
    {
        enum class Type : uint16_t
        {
            mono8 = AL_FORMAT_MONO8,
            mono16 = AL_FORMAT_MONO16,
            stereo8 = AL_FORMAT_STEREO8,
            stereo16 = AL_FORMAT_STEREO16
        };

        struct Relation
        {
            Type type;
            uint16_t channels;
            uint16_t bits;
            std::string_view name;
        };

        namespace detail
        {
            static constexpr auto relation = std::array{
                Relation{Type::mono8, 1, 8, "mono8"},
                Relation{Type::mono16, 1, 16, "mono16"},
                Relation{Type::stereo8, 2, 8, "stereo8"},
                Relation{Type::stereo16, 2, 16, "stereo16"}
            };
        }

        static constexpr Type type(uint16_t channels, uint16_t bits){
            for(const auto& visited : detail::relation){
                if(visited.channels == channels && visited.bits == bits) return visited.type;
            }
            throw std::runtime_error(fmt::format("Unknown channel-bits combination: {}-{}", channels, bits));
        }

        static constexpr std::string_view name(Type type){
            for(const auto& visited : detail::relation){
                if(visited.type == type) return visited.name;
            }
            throw std::runtime_error(fmt::format("Unknown format type: {}", static_cast<int>(type)));
        }
    }

    enum class AudioFormatType
    {
        pcm,
        count
    };



    struct SoundFileData
    {
        std::filesystem::path path;

        channel::Type channel;
        format::Type format;
        uint32_t frequency;
        size_t durationMillis;

        util::RawData data;
    };
}