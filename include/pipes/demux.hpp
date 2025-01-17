#ifndef PIPES_DEMUX_HPP
#define PIPES_DEMUX_HPP

#include "pipes/operator.hpp"

#include "pipes/helpers/meta.hpp"
#include "pipes/helpers/warnings.hpp"
#include "pipes/pipeline_base.hpp"

PIPES_DISABLE_WARNING_PUSH
PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

namespace pipes
{

template<typename... OutputPipes>
class demux_pipe : public pipeline_base<demux_pipe<OutputPipes...>>
{
public:
    template<typename T>
    void onReceive(T&& value)
    {
        detail::for_each([&value](auto&& outputPipe){ send(outputPipe, FWD(value)); }, outputPipes_);
    }

    explicit demux_pipe(OutputPipes const&... outputPipes) : outputPipes_(outputPipes...) {}
    
private:
    std::tuple<OutputPipes...> outputPipes_;
    
public: // but technical
    using base = pipeline_base<demux_pipe<OutputPipes...>>;
    using base::operator=;
    demux_pipe& operator=(demux_pipe const& other)
    {
        outputPipes_ = other.outputPipes_;
        return *this;
    }
    demux_pipe& operator=(demux_pipe& other) { *this = const_cast<demux_pipe const&>(other); return *this; }
};

template<typename... OutputPipes>
demux_pipe<OutputPipes...> demux(OutputPipes const&... outputPipes)
{
    return demux_pipe<OutputPipes...>(outputPipes...);
}

} // namespace pipes

PIPES_DISABLE_WARNING_POP

#endif /* PIPES_DEMUX_HPP */
