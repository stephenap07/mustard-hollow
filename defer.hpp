#ifndef H_MUH_H
#define H_MUH_H

#include <functional>

/**
 * Defer a function until the end of a scope
 * Taken from http://blog.korfuri.fr/post/go-defer-in-cpp/
 */
class DeferredAction {
public:
    ~DeferredAction() {
        if (_f) {
            _f();
        }
    }
    DeferredAction(DeferredAction&& l) :
        _f(std::forward<std::function<void()>>(l._f)) {
            l._f = nullptr;
        }

    DeferredAction(DeferredAction const& l) = delete;
    DeferredAction& operator=(DeferredAction const&) = delete;
    DeferredAction& operator=(DeferredAction&&) = delete;

private:
    template<typename... Tpack>
    DeferredAction(Tpack&&... p) :
        _f(std::bind(std::forward<Tpack>(p)...)) {}

    std::function<void()> _f;

    template<typename... Tpack>
    friend DeferredAction defer(Tpack&&... p);
};

template<typename... Tpack>
DeferredAction defer(Tpack&&... p) {
    return DeferredAction(std::forward<Tpack>(p)...);
}

#endif
