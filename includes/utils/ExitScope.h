#pragma once

namespace utils {

/* nice RAII for lambda-expr
 *
 * put any function that should be called
 * leavning current scope
 */
template<typename Act>
inline auto ExitScope(Act fn) {
    struct ExitScope_impl {
        Act act;
        explicit ExitScope_impl(Act fn) : act(std::move(fn)) {}
        ~ExitScope_impl() { act(); }
    };
    return ExitScope_impl(std::move(fn));
}

} // namespace utils