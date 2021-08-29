#pragma once

namespace sp {
namespace filter {

struct Sample
{
    using Frequency = float;
    std::vector<Frequency> m_data = {};
};

struct Chunk
{
    std::vector<Sample> m_samples = {};
};

class Sink
{
public:
    using Ptr = std::shared_ptr<Sink>;

    explicit Sink() = default;
    virtual ~Sink() = default;

private:

};

}; // namespace filter
} // namespace sp