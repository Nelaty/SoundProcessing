/*
 * Created by Matthias "Nelaty" Gensheimer on 06.09.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once

#include "spdlog/spdlog.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace sp::filter {

using Sample = float;

class PipelineElement;
class Port;

using PortPtr = std::shared_ptr<Port>;
using PipelineElementPtr = std::shared_ptr<PipelineElement>;

class Port
{
public:
    bool operator==(const Port &port) const
    {
        return port.m_id == m_id && port.m_name == m_name &&
            port.m_source == m_source && port.m_target == m_target;
    }

    bool bound() const
    {
        return m_source != nullptr;
    }

    int         m_id;
    std::string m_name;
    Sample      m_value;

    PipelineElementPtr m_source;
    PipelineElement    *m_target;
};

class PipelineElement
{
public:
    bool connect(const PortPtr& targetPort)
    {
        if(!targetPort)
        {
            std::cout << "ERROR: target port is null\n";
            return false;
        }
        m_targets.emplace_back(targetPort);
    }

    void deleteTargetConnection(const PortPtr& port)
    {
        m_targets.erase(std::remove(m_targets.begin(),
                                     m_targets.end(),
                                     port));
    }

    void removeSourceConnection(const PortPtr& ptr)
    {
        auto found = std::find(m_targets.begin(),
                               m_targets.end(),
                               ptr);
        if(found == m_targets.end()) return;

        auto source = (*found)->m_source;
        if(!source) return;

        source->deleteTargetConnection(ptr);
    }

    bool findSource(const Port &port)
    {
        return std::any_of(
            m_sources.cbegin(), m_sources.cend(),
            [&](const PortPtr &it) -> bool { return *it == port; });
    }

    bool findTarget(const Port &port)
    {
        return std::any_of(
            m_targets.cbegin(), m_targets.cend(),
            [&](const PortPtr &it) -> bool { return *it == port; });
    }

    PortPtr getSource(const std::string &name)
    {
        const auto found = std::find_if(
            m_sources.cbegin(), m_sources.cend(),
            [&](const PortPtr port) { return port->m_name == name; });
        return found != m_sources.cend() ? *found : nullptr;
    }

    PortPtr getSource(size_t id)
    {
        const auto found =
            std::find_if(m_sources.cbegin(), m_sources.cend(),
                         [&](const PortPtr port) { return port->m_id == id; });
        return found != m_sources.cend() ? *found : nullptr;
    }

    PortPtr getTarget(const std::string &name)
    {
        const auto found = std::find_if(
            m_targets.cbegin(), m_targets.cend(),
            [&](const PortPtr port) { return port->m_name == name; });
        return found != m_sources.cend() ? *found : nullptr;
    }

    PortPtr getTarget(size_t id)
    {
        const auto found =
            std::find_if(m_targets.cbegin(), m_targets.cend(),
                         [&](const PortPtr port) { return port->m_id == id; });
        return found != m_sources.cend() ? *found : nullptr;
    }

    const std::vector<std::shared_ptr<Port>> &sources() { return m_sources; }
    const std::vector<std::shared_ptr<Port>> &targets() { return m_targets; }

    virtual void onInit(){}
    virtual void onExecute(){}
    virtual void onEnd(){}

protected:
    void sendSignal(const Sample& sample)
    {
        for(auto& it : m_targets)
        {
            it->m_value = sample;
        }
    }

    std::shared_ptr<Port> createPort(std::string name)
    {
        auto port = std::make_shared<Port>();
        {
            std::lock_guard lck(m_sourceIdMutex);
            port->m_id = m_sourceId++;
        }
        port->m_name   = name;
        port->m_target = this;
        port->m_source = nullptr;
        port->m_value  = {};

        m_sources.emplace_back(port);
        return port;
    }

private:
    std::mutex  m_sourceIdMutex;
    std::size_t m_sourceId{0};

    /// Input sources can be different entities. The
    /// values can differ between ports.
    std::vector<std::shared_ptr<Port>> m_sources;
    /// Output targets can be different entites but
    /// they will all receive the same value.
    std::vector<std::shared_ptr<Port>> m_targets;
};

class LowPassFilter : public PipelineElement
{
public:
    LowPassFilter()
    {
        m_inputSignal = createPort("input");
    }

    void setThreshold(float threshold)
    {
        m_threshold = threshold;
    }

    void onExecute() override
    {
        Sample outSignal = m_inputSignal->m_value <= m_threshold ? 0 : m_inputSignal->m_value;
        sendSignal(outSignal);
    }

    PortPtr m_inputSignal;

private:
    float m_threshold;
};

class AverageMixer : public PipelineElement
{
public:
    void onExecute() override
    {
        Sample average{0};
        for(const auto& it : sources())
        {
            average += it->m_value / sources().size();
        }
        sendSignal(average);
    }

    void addSource(PipelineElementPtr item, std::string name)
    {
        auto port = createPort(name);
        item->connect(port);
    }
};

class ConstantSource : public PipelineElement
{
public:
    ConstantSource(Sample sample)
    {
        m_sample = sample;
    }

    void onExecute() override
    {
        sendSignal(m_sample);
    }

    Sample m_sample;
};

class ConsolePrinterSink : public PipelineElement
{
public:
    ConsolePrinterSink()
    {
        m_printValue = createPort("printValue");
    }
    void onExecute() override
    {
        std::cout << "Value: " << m_printValue->m_value << std::endl;
    }

    PortPtr m_printValue;
};

class Pipeline
{
public:
    using SampleCallback = std::function<void(const Sample &sample)>;

    [[nodiscard]] Sample filterSingle(const Sample &sample) { return sample; }

    void execute()
    {
        evaluateExecutionOrder();
        for(const auto& it : m_executionOrder)
        {
            m_elements[it]->onExecute();
        }
    }

    void add(const std::shared_ptr<PipelineElement>& item)
    {
        m_elements.emplace_back(item);
        m_executionOrderDirty = true;
    }

private:
    std::vector<PipelineElementPtr> m_elements;

    bool evaluateExecutionOrder()
    {
        if(!m_executionOrderDirty) return true;

        m_executionOrder.clear();
        for(size_t i=0; i < m_elements.size(); ++i)
        {
            m_executionOrder.emplace_back(i);
        }


        m_executionOrderDirty = false;
        return true;
    }

    bool m_executionOrderDirty{true};
    std::vector<int> m_executionOrder;
};
} // namespace sp::filter