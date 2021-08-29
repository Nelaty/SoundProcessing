#pragma once

#incldue <memory>
#include <vector>

namespace sp {
namespace filter {

class Sink;
class Source : public PipelineElement
{
public:
    explicit Source() = default;

private:

};
} // namespace filter
} // namespace sp