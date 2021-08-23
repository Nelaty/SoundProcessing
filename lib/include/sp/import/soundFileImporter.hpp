/* 
 * Created by Matthias "Nelaty" Gensheimer on 25.06.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once
#include "sp/entity/soundFileData.hpp"

#include <filesystem>

namespace sp
{
    class SoundFileImporter
    {
    public:
        static SoundFileData import(const std::filesystem::path& path);
    };
}