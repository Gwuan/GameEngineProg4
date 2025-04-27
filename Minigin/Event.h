#pragma once
#include <functional>
#include <unordered_set>

template <typename... Args>
class Event {
public:
    // TODO: Make sure a simple function ptr or something like that is enough instead of calling std::bind
    void operator+=(std::function<void(Args...)> func) {
        m_Functions.push_back(func);
    }

    void Broadcast(Args... args) {
        for (auto& func : m_Functions) {
            func(args...);
        }
    }

private:
    std::vector<std::function<void(Args...)>> m_Functions;
};