/* 
 * Created by Matthias "Nelaty" Gensheimer on 25.06.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#include "catch2/catch.hpp"

#include <sp/util/pathUtils.hpp>

TEST_CASE("splitFilenameTest")
{
    auto test = [](const std::string& path, const std::string& expectedStem, const std::string& expectedExtension){
        const auto&[stem, extension] = sp::util::splitFilename(path);
        CHECK(stem == expectedStem);
        CHECK(extension == expectedExtension);
    };

    test("asdf/ghjk/myCoolFile.xyz", "myCoolFile", "xyz");
    test("myotherfile.xas.zzz", "myotherfile.xas", "zzz");
}