/* 
 * Created by Matthias "Nelaty" Gensheimer on 25.06.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#include "catch2/catch.hpp"

#include <sp/entity/soundFileType.hpp>

TEST_CASE("soundFileTypeTest")
{
    CHECK(sp::SoundFileTypeMapper::type("wav") == sp::SoundFileType::wav);
    CHECK(sp::SoundFileTypeMapper::name(sp::SoundFileType::wav) == "wav");

    CHECK_THROWS(sp::SoundFileTypeMapper::type("bababap"));
    CHECK_THROWS(sp::SoundFileTypeMapper::name(sp::SoundFileType::count));
}