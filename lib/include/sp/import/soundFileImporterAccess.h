/* 
 * Created by Matthias "Nelaty" Gensheimer on 30.06.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once
#include "sp/entity/soundFileData.hpp"

#include <filesystem>

namespace sp
{
    class SoundFileImporterAccess
    {
    public:
        virtual ~SoundFileImporterAccess() = default;
        virtual SoundFileData import(const std::filesystem::path& path) const = 0;
    };
}