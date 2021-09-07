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

#include <vector>

namespace sp::api
{

struct SP_EXPORT Version
{
    static constexpr const char* m_version = "0";
};

struct SP_EXPORT Sample
{
    using Frequency = float;

    std::vector<Frequency> m_data = {};
};

struct SP_EXPORT Chunk
{
    std::vector<Sample> m_samples = {};
};

}