#include <scheduleManager.hpp>
#include <iostream>
ScheduleManager::ScheduleManager()
{
    _prev_time = std::chrono::high_resolution_clock::now();
}

void ScheduleManager::update()
{
    if (_is_first_run)
    {
        _prev_time = std::chrono::high_resolution_clock::now();
        _is_first_run = false;
    }

    auto now = std::chrono::high_resolution_clock::now();
    _delta = now - _prev_time;
    _prev_time = now;

    for (auto& handler : _handlers)
    {
        bool need_run = true;
        
        if (handler.delay.count() > 0.0f)
        {
            auto delta = now - handler.last_time;
            if (delta >= handler.delay)
            {
                handler.delay = 0s;
            }
            else
            {
                need_run = false;
            }
        }

        if (handler.delta.count() > 0 && (now - handler.last_time) < handler.delta)
        {
            need_run = false;
        }

        if (need_run)
        {
            handler.last_time = now;
            handler.fun(_delta);
        }

        if (handler.count != 0)
        {
            --handler.count;

            if (handler.count == 0)
            {
                _handlers.erase(std::remove_if(_handlers.begin(), _handlers.end(), [&](const HandlerStorage& cur_handler)
                    {
                        return cur_handler.internal_tag == handler.internal_tag;
                    }));
            }
        }
    }
}

void ScheduleManager::schedule_update(handler_fun handler, size_t tag) const
{
    this->schedule(std::move(handler), 0s, 0s, 0, tag);
}

void ScheduleManager::stop(size_t tag) const
{
    std::cout << _handlers.size() << std::endl;
    auto help = _handlers.size();
    _handlers.erase(std::remove_if(_handlers.begin(), _handlers.end(), [&](const HandlerStorage& cur_handler)
        {
            return cur_handler.tag == tag;
        }));
    if (_handlers.size() < help)
        return;
}

fseconds ScheduleManager::get_delta() const
{
    return _delta;
}

void ScheduleManager::schedule(ScheduleManager::handler_fun fun, fseconds delay, fseconds delta, size_t count, size_t tag) const
{
    HandlerStorage handler;
    handler.fun = std::move(fun);
    handler.delay = delay;
    handler.delta = delta;
    handler.count = count;
    handler.tag = tag;
    handler.internal_tag = utils::gen_unique_object_ID();
    handler.last_time = std::chrono::high_resolution_clock::now();
    _handlers.push_back(std::move(handler));
}