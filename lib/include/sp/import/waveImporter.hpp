/* 
 * Created by Matthias "Nelaty" Gensheimer on 30.06.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once
#include "soundFileImporterAccess.h"
#include "sp/util/pathUtils.hpp"
#include "sp/util/fileUtils.hpp"
#include "sp/entity/soundFileData.hpp"
#include "sp/entity/soundFileType.hpp"

#include <spdlog/spdlog.h>

#include <fstream>
#include <string>
#include <sstream>

namespace sp
{
    class WaveImporter  : public SoundFileImporterAccess
    {
    public:
        WaveImporter() = default;
        ~WaveImporter() = default;

        SoundFileData import(const std::filesystem::path& path) const override
        {
            spdlog::info("Import started for wav file: {}", path.c_str());

            std::fstream in(path, std::ios::in | std::ios::binary);
            if(!in.is_open()){
                spdlog::error("Failed to open file: {}", path.c_str());
                return {};
            }

            auto result = SoundFileData{};
            result.path = path;


            const auto riffHeader = util::readBinary<RiffHeader>(in);
            const auto fmtSection = util::readBinary<FmtSection>(in);
            const auto dataSection = util::readBinary<DataSection>(in);

            spdlog::info("{} | Size parsed: {}", toString(riffHeader), sizeof(RiffHeader));
            spdlog::info("{} | Size parsed: {}", toString(fmtSection), sizeof(FmtSection));
            spdlog::info("{} | Size parsed: {}", toString(dataSection), sizeof(DataSection));

            auto wavData = std::vector<char>(dataSection.subchunk2Size);
            in.read(wavData.data(), dataSection.subchunk2Size);
            size_t offset = 44;

            // Build result
            result.channel = channel::type(fmtSection.numChannels);
            result.format = format::type(fmtSection.numChannels, fmtSection.bitsPerSample);
            result.frequency = fmtSection.sampleRate;
            result.data.allocate(wavData.data()+offset, wavData.size()-offset);

            const auto numSamples = dataSection.subchunk2Size / fmtSection.blockAlign;
            result.durationMillis = static_cast<size_t>(1000.0f * static_cast<float>(numSamples) /
                static_cast<float>(fmtSection.numChannels * fmtSection.sampleRate));

            return result;
        }

    private:
        struct RiffHeader
        {
            alignas(4) std::array<char, 4> chunkId;
            alignas(4) std::uint32_t chunkSize;
            alignas(4) std::array<char, 4> format;
        };

        struct FmtSection
        {
            alignas(4) std::array<char, 4> subchunk1Id;
            alignas(4) uint32_t subchunk1Size;
            alignas(2) uint16_t audioFormat;
            alignas(2) uint16_t numChannels;
            alignas(4) uint32_t sampleRate;
            alignas(4) uint32_t byteRate;
            alignas(2) uint16_t blockAlign;
            alignas(2) uint16_t bitsPerSample;
        };

        struct DataSection
        {
            alignas(4) std::array<char, 4> subchunk2Id;
            alignas(4) uint32_t subchunk2Size;
        };

        template<class ArrayType>
        static std::string toString(ArrayType a)
        {
            return std::string(a.data(), a.size());
        }

        static std::string toString(const RiffHeader& val){
            std::ostringstream oss;
            oss << "RIFF Header: "
                << toString(val.chunkId) << " "
                << val.chunkSize << " "
                << toString(val.format);
            return oss.str();
        }

        static std::string toString(const FmtSection& val){
            std::ostringstream oss;
            oss << "Fmt Section: "
                << toString(val.subchunk1Id) << " "
                << val.subchunk1Size << " "
                << val.audioFormat << " "
                << val.numChannels << " "
                << val.sampleRate << " "
                << val.byteRate << " "
                << val.blockAlign << " "
                << val.bitsPerSample;
            return oss.str();
        }

        static std::string toString(const DataSection& val){
            std::ostringstream oss;
            oss << "Data Section: "
                << toString(val.subchunk2Id) << " "
                << val.subchunk2Size;
            return oss.str();
        }
    };
}