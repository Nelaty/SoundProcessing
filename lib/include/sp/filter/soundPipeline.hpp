/* 
 * Created by Matthias "Nelaty" Gensheimer on 26.06.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once
#include "sp/entity/soundFileData.hpp"

#include <memory>
#include <vector>

namespace sp::filter
{
    /* Apply a filter on sound file data or a single sound sample */
    class SoundFilter
    {
    public:
        using Sample = uint32_t;

        virtual void apply(SoundFileData& data) const = 0;
        virtual Sample apply(const Sample& sample) const = 0;
    };

    class AmplitudeFilter : public SoundFilter
    {
    public:
        AmplitudeFilter(float factor)
        {
            mFactor = factor;
        }

        void apply(SoundFileData& dataIn) const override
        {
            const auto size = dataIn.data.size();
            void* data = dataIn.data.data();
            //todo: implement
        }

        Sample apply(const Sample& sample) const override
        {
            return {};
        }

    private:
        float mFactor;
    };

    class SoundPipeline
    {
    public:
        void add(const std::shared_ptr<SoundFilter>& filter)
        {
            filters.emplace_back(filter);
        }

        void apply(SoundFileData& data)
        {
            for(const auto& filter : filters){
                filter->apply(data);
            }
        }

        SoundFilter::Sample apply(SoundFilter::Sample sample) const
        {
            for(const auto& filter : filters){
                sample = filter->apply(sample);
            }
            return sample;
        }

    private:
        std::vector<std::shared_ptr<SoundFilter>> filters;
    };
}