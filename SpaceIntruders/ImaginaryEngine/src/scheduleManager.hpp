#pragma once

#include <chrono>
#include <vector>
#include <functional>

using fseconds = std::chrono::duration<float>;
using namespace std::chrono_literals;

class ScheduleManager final
{
public:
    explicit ScheduleManager();
    void update();
    fseconds get_delta() const;

    using handler_fun = std::function<void(fseconds)>;

    void schedule_update(handler_fun, size_t) const;
    void stop(size_t) const;

    void schedule(ScheduleManager::handler_fun, fseconds, fseconds, size_t, size_t) const;
    
    struct HandlerStorage
    {
        handler_fun fun;
        fseconds delay;
        fseconds delta;

        size_t count;

        size_t tag;
        size_t internal_tag;

        std::chrono::high_resolution_clock::time_point last_time;
    };

private:
    mutable std::vector<HandlerStorage> _handlers;
    std::chrono::high_resolution_clock::time_point _prev_time;
    fseconds _delta{};
    bool _is_first_run = true;
};