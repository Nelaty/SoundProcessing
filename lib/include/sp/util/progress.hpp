/* 
 * Created by Matthias "Nelaty" Gensheimer on 25.06.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once

#include "catch2/catch.hpp"

#include <functional>

namespace sp{
    namespace util{
        class ProgressFunction
        {
        public:
            using Func = std::function<void(float current)>;

            void setCallback(Func cb){
                callback = cb;
            }

            void setProgress(float current)
            {
                progress = std::clamp(current, minVal, maxVal);
                if(callback) callback();
            }
            float getProgress() const
            {
                return progress;
            }

            float getProgressRatio() const
            {
                return progress / (maxVal - minVal);
            }

            void setMaxVal(float val)
            {
                maxVal = val;
                if(callback) callback();
            }

            void setMinVal(float val)
            {
                minVal = val;
                if(callback) callback();
            }

            bool done() const
            {
                return Approx(progress) >= maxVal;
            }

        private:
            float minVal{0.0};
            float maxVal{1.0f};

            float progress{0.0};

            Func callback;
        };
    }
}