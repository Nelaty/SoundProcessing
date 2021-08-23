/* 
 * Created by Matthias "Nelaty" Gensheimer on 25.06.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once
#include <filesystem>
#include <string_view>
#include <optional>
#include <vector>

namespace sp
{
    class Configuration
    {
    public:
        static constexpr std::string_view resourceBasePath = "res/";
        static constexpr std::string_view soundDirName = "sound/";

        static std::filesystem::path getResourcePath()
        {
            return {resourceBasePath};
        }

        static std::filesystem::path getSoundDir()
        {
            return getResourcePath() / soundDirName;
        }

        static std::filesystem::path workingDir()
        {
            return std::filesystem::absolute(".");
        }

        static std::vector<std::filesystem::path> listFiles(const std::filesystem::path& filePath,
                                                            std::optional<std::string> extension = {},
                                                            bool recursive=false)
        {
            // Add point to extension if non-existent
            if(extension.has_value() && !extension.value().empty()){
                if(extension.value().at(0) != '.'){
                    extension.value() = std::string(".") + extension.value();
                }
            }

            auto iterateDirectory = [&](auto directoryIterator)
            {
                auto result = std::vector<std::filesystem::path>{};
                for(const auto& visited : directoryIterator)
                {
                    if(visited.is_regular_file())
                    {
                        if(!extension
                        || (extension && visited.path().extension() == extension.value()))
                        {
                            result.emplace_back(visited.path());
                        }
                    }
                }
                return result;
            };

            auto result = std::vector<std::filesystem::path>{};
            if(recursive)
                result = iterateDirectory(std::filesystem::recursive_directory_iterator(filePath));
            else
                result = iterateDirectory(std::filesystem::directory_iterator(filePath));
            return result;
        }

        static std::filesystem::path getSoundPath(std::string_view fileName)
        {
            return getSoundDir() / fileName;
        }
    };
}