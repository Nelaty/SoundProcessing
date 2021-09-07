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

#include <array>
#include <fstream>
#include <string>
#include <sstream>

namespace sp
{
    class OggImporter : public SoundFileImporterAccess
    {
    public:
        static constexpr auto magicNumber = "OggS";

        enum class PageType
        {
            invalid = 0x00,
            continuation = 0x01,
            bos = 0x02,
            eos = 0x04
        };
        static PageType toPageType(uint8_t typeVal){
            const auto type = static_cast<PageType>(typeVal);
            if(type != PageType::continuation
            && type != PageType::bos
            && type != PageType::eos)
                return PageType::invalid;

            return type;
        }


        struct OggMetaData
        {
            alignas(4) std::array<char, 4> capturePattern;
            alignas(1) std::uint8_t version;
            alignas(1) std::uint8_t headerType;
            alignas(8) std::uint64_t granulePosition;
            alignas(4) std::uint32_t bitstreamSerialNumber;
            alignas(4) std::uint32_t pageSequenceNumber;
            alignas(4) std::uint32_t checksum;
            alignas(1) std::uint8_t pageSegments;
        };

        static std::string toString(const OggMetaData& data){
            std::ostringstream oss;
            oss << data.capturePattern.data() << " "
                << data.version << " "
                << data.headerType << " "
                << data.granulePosition << " "
                << data.bitstreamSerialNumber << " "
                << data.pageSequenceNumber << " "
                << data.checksum << " "
                << data.pageSegments << " ";
            return oss.str();
        }

        SoundFileData import(const std::filesystem::path& path) const override
        {
            //spdlog::info("Import started for ogg file: {}", path.c_str());
            std::fstream in(path, std::ios::in | std::ios::binary);
            if(!in.is_open()){
                //spdlog::error("Failed to open file: {}", path.c_str());
                return {};
            }

            auto result = SoundFileData{};
            result.path = path;

            const auto oggHeader = util::readBinary<OggMetaData>(in);
            //spdlog::debug("Ogg Header: {}", toString(oggHeader));

            // Check for corruption
            if(oggHeader.capturePattern.data() != magicNumber){
                throw std::runtime_error(fmt::format("Ogg magic number is incorrect: {} != {}",
                                                     oggHeader.capturePattern.data(), magicNumber));
            }
            if(oggHeader.version != 0){
                throw std::runtime_error(fmt::format("Ogg version not supported: {}",
                                                     oggHeader.version));
            }
            if(toPageType(oggHeader.headerType) == PageType::invalid){
                throw std::runtime_error(fmt::format("Ogg header type is invalid {}",
                                                     static_cast<int>(oggHeader.headerType)));
            }

            struct Page
            {
                size_t size_;
            };
            std::vector<Page> pages;


            const auto start = in.tellg();
            in.seekg(0, in.end);
            const auto dataCount = in.tellg() - start;
            auto oggData = std::vector<char>(dataCount);



            size_t offset = 44;


            // Read pages




            return result;
        }

    };
}