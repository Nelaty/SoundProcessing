/*
 * Created by Matthias "Nelaty" Gensheimer on 29.08.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once

#if defined(SP_EXPORT_SHARED_LIBRARIES)
#define SP_EXPORT __dllexport
#else
#define SP_EXPORT
#endif

#include <cstdint>
#include <filesystem>
#include <memory>
#include <span>
#include <variant>
#include <vector>

namespace sp::api {
struct SP_EXPORT Version
{
    static constexpr const char *m_version = "0";
};

using SP_EXPORT Sample = std::uint32_t;
using SP_EXPORT Chunk  = std::span<Sample>;



enum class SP_EXPORT SampleSize
{
    b8,
    b12,
    b16,
    b24
};

struct Metadata
{
    SampleSize m_sampleSize;
};

class SP_EXPORT SoundDataSource
{
public:
    struct FileSource
    {
        std::filesystem::path m_path;
    };

    struct RemoteFileSource
    {
        std::string m_address;
    };
    struct RemoteStreamSource
    {
        std::string m_address;
    };

    enum class SourceType : std::uint8_t
    {
        File = 0,
        RemoteFile,
        RemoteStream
    };

    using SourceValue = std::variant<FileSource,
                                     RemoteFileSource,
                                     RemoteStreamSource>;

    struct Source
    {
        SourceValue value;
        SourceType type;
    };



    explicit SoundDataSource(FileSource source);
    explicit SoundDataSource(RemoteFileSource source);
    explicit SoundDataSource(RemoteStreamSource source);

    [[nodiscard]] Source getSource() const;
    [[nodiscard]] SourceType getSourceType() const;


private:
    SourceType    m_type;

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

class SP_EXPORT SoundData
{
public:
    explicit SoundData();

    bool load(SoundDataSource source);

    Metadata getMetadata() const;

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace sp::api