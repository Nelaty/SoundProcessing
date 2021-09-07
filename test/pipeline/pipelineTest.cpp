/*
 * Created by Matthias "Nelaty" Gensheimer on 07.09.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */
#include "sp/filter/pipeline.hpp"
#include "catch2/catch.hpp"

#include <memory>
#include <vector>

using namespace sp::filter;



TEST_CASE("General pipeline test", "[sp][filter]")
{
    auto source = std::make_shared<ConstantSource>(Sample{5});
    auto lowPass = std::make_shared<LowPassFilter>();
    lowPass->setThreshold(4);
    auto console = std::make_shared<ConsolePrinterSink>();

    source->connect(lowPass->m_inputSignal);
    lowPass->connect(console->m_printValue);

    //lowPass->connect(source, lowPass->m_inputSignal);
    //console->connect(lowPass, console->m_printValue);

    Pipeline pipeline;
    pipeline.add(source);
    pipeline.add(lowPass);
    pipeline.add(console);
    pipeline.execute();

    CHECK(console->m_printValue->m_value == 5);
    lowPass->setThreshold(6);
    pipeline.execute();
    CHECK(console->m_printValue->m_value == 0);
}

TEST_CASE("Average filter test", "[sp][filter]")
{
    auto sources = std::array {
        std::make_shared<ConstantSource>(Sample(6)),
        std::make_shared<ConstantSource>(Sample(12)),
    };
    auto mixer = std::make_shared<AverageMixer>();
    mixer->addSource(sources[0], "1");
    mixer->addSource(sources[1], "2");

    auto console = std::make_shared<ConsolePrinterSink>();
    mixer->connect(console->m_printValue);

    Pipeline pipeline;
    pipeline.add(sources[0]);
    pipeline.add(sources[1]);
    pipeline.add(mixer);
    pipeline.add(console);
    pipeline.execute();
    CHECK(console->m_printValue->m_value == Approx(9.0f));

    mixer->addSource(sources[1], "3");
    pipeline.execute();
    CHECK(console->m_printValue->m_value == Approx(10.0f));
}