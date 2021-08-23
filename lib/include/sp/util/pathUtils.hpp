/* 
 * Created by Matthias "Nelaty" Gensheimer on 25.06.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once

#include <filesystem>

namespace sp
{
    namespace util
    {
        struct FilenameExtensionPair
        {
            std::string name;
            std::string extension;
        };

        static FilenameExtensionPair splitFilename(const std::filesystem::path& path)
        {
            const auto& extension = path.extension().string();
            return {
                path.stem(),
                !extension.empty()
                        ? extension.substr(extension.find_first_of(".") + 1)
                        : ""
            };
        }
    }
}