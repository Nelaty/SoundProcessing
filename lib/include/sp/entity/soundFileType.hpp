/* 
 * Created by Matthias "Nelaty" Gensheimer on 25.06.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once

#include <array>
#include <stdexcept>
#include <string_view>

namespace sp
{
    enum class SoundFileType
    {
        wav,
        ogg,
        count
    };

    struct SoundFileTypeRelation {
        const SoundFileType    type;
        const std::string_view name;
    };

    struct SoundFileTypeMapper
    {
        static constexpr auto relations = std::array {
            SoundFileTypeRelation{SoundFileType::wav, "wav"},
            SoundFileTypeRelation{SoundFileType::ogg, "ogg"}
        };

        static constexpr SoundFileType type(std::string_view name){
            for(const auto& visited : relations){
                if(visited.name == name) return visited.type;
            }
            throw std::runtime_error(std::string("Unknown sound file type string: ") + name.data());
        }

        static constexpr std::string_view name(SoundFileType type){
            for(const auto& visited : relations){
                if(visited.type == type) return visited.name;
            }
            throw std::runtime_error(std::string("Unknown sound file type: " + std::to_string(static_cast<int>(type))));
        }
    };

}