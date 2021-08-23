/* 
 * Created by Matthias "Nelaty" Gensheimer on 26.06.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once

#include <mutex>

namespace sp::ui
    class Imgui
    {
    public:
        static void init()
        {
            std::mutex m;
            std::lock_guard lck(m);

            if(initialized) return;

        }

        static void cleanup()
        {
            std::mutex m;
            std::lock_guard lck(m);

            if(!initialized) return;
        }
    private:
        static bool initialized{false};
    };
}