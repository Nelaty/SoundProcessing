/* 
 * Created by Matthias "Nelaty" Gensheimer on 30.06.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#include "sp/import/soundFileImporter.hpp"

#include "sp/util/pathUtils.hpp"
#include "sp/entity/soundFileData.hpp"
#include "sp/entity/soundFileType.hpp"
#include "sp/import/soundFileImporterAccess.h"

#include "sp/import/oggImporter.hpp"
#include "sp/import/waveImporter.hpp"

#include <spdlog/spdlog.h>

#include <array>
#include <filesystem>
#include <utility>

namespace sp
{
    namespace
    {
        using ImporterRelation = std::pair<SoundFileType, const SoundFileImporterAccess&>;

        static const auto oggImporter = OggImporter{};
        static const auto waveImporter = WaveImporter{};

        static const auto importerTable = std::array{
            ImporterRelation{SoundFileType::ogg, oggImporter},
            ImporterRelation{SoundFileType::wav, waveImporter}
        };
    }

    SoundFileData SoundFileImporter::import(const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path))
        {
            spdlog::error("Filepath doesn't exist: {}", path.c_str());
            return {};
        }

        const auto [stem, extension] = sp::util::splitFilename(path);
        try {
            const auto type = SoundFileTypeMapper::type(extension);
            for(const auto& importer : importerTable){
                if(importer.first == type) return importer.second.import(path);
            }
            spdlog::error("Unrecognized file type: {}", path.c_str());
        } catch (std::exception& e) {
            spdlog::error("Exception: {}", e.what());
            spdlog::error("Failed to import sound file: {}", path.c_str());
        }

        return {};
    }
}