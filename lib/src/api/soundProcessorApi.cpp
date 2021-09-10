/*
 * Created by Matthias "Nelaty" Gensheimer on 09.09.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */
#include "sp/api/soundProcessorApi.hpp"

#include <cassert>

namespace sp::api {

class SoundDataSource::Impl
{
public:
    explicit Impl(SoundDataSource::Source source) : m_source(source) {}

    [[nodiscard]] SoundDataSource::Source getSource() const { return m_source; }

private:
    SoundDataSource::Source m_source;
};

SoundDataSource::SoundDataSource(FileSource fileSource)
    : m_impl(std::make_unique<Impl>(Source{fileSource, SourceType::File}))
{}

SoundDataSource::SoundDataSource(RemoteFileSource remoteFileSource)
    : m_impl(std::make_unique<Impl>(Source{remoteFileSource, SourceType::RemoteFile}))
{}

SoundDataSource::SoundDataSource(RemoteStreamSource remoteStreamSource)
    : m_impl(std::make_unique<Impl>(Source{remoteStreamSource, SourceType::RemoteStream}))
{}

SoundDataSource::Source SoundDataSource::getSource() const
{
    assert(m_impl);
    return m_impl->getSource();
}

SoundDataSource::SourceType SoundDataSource::getSourceType() const
{
    assert(m_impl);
    return m_impl->getSource().type;
}
} // namespace sp::api