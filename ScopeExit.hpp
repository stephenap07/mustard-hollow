#ifndef H_SCOPE_EXIT_H_
#define H_SCOPE_EXIT_H_

/**
 * Scope Exit code from http://the-witness.net/news/2012/11/scopeexit-in-c11/
 * TODO: This v.s. unique_ptr
 */

template <typename F>
struct ScopeExit {
    ScopeExit(F f) : f(f) {}
    ~ScopeExit() { f(); }
    F f;
};

template <typename F>
ScopeExit<F> MakeScopeExit(F f) {
    return ScopeExit<F>(f);
};

#define STRING_JOIN2(arg1, arg2) DO_STRING_JOIN2(arg1, arg2)
#define DO_STRING_JOIN2(arg1, arg2) arg1 ## arg2
#define SCOPE_EXIT(code) \
    auto STRING_JOIN2(scope_exit_, __LINE__) = MakeScopeExit([=](){ code })

#endif
