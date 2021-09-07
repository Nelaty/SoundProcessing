/*
 * Created by Matthias "Nelaty" Gensheimer on 07.09.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */
#include "sp/filter/pipeline.hpp"
#include "catch2/catch.hpp"

#include <memory>
#include <vector>

using namespace sp::filter;



TEST_CASE("Pipeline test", "[sp][filter]")
{
    auto source = std::make_shared<ConstantSource>(Sample{5});
    auto lowPass = std::make_shared<LowPassFilter>();
    lowPass->setThreashold(4);
    auto console = std::make_shared<ConsolePrinterSink>();

    lowPass->connect(source, lowPass->m_inputSignal);
    console->connect(lowPass, console->m_printValue);

    Pipeline pipeline;
    pipeline.add(source);
    pipeline.add(lowPass);
    pipeline.add(console);
    pipeline.execute();

    CHECK(console->m_printValue->m_value == 5);
    lowPass->setThreashold(6);
    CHECK(console->m_printValue->m_value == 0);
}