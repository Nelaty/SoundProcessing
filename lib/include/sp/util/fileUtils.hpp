/* 
 * Created by Matthias "Nelaty" Gensheimer on 25.06.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once

#include <array>
#include <fstream>

namespace sp::util
    {
        template<class Result>
        static Result readBinary(std::fstream& in)
        {
            auto buf = std::array<char, sizeof(Result)>{};
            in.read(buf.data(), buf.size());
            return reinterpret_cast<const Result&>(*buf.data());
        }
    }