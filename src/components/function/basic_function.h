//
// Created by konstantin on 19.06.24.
//

#pragma once

#include <memory>

namespace NComponents {

template <class U>
class BasicFunction;

template <typename R, typename... Args>
class BasicFunction<R(Args...)> {
    class function_holder_base;
    using function_holder_base_ptr = std::shared_ptr<function_holder_base>;

    function_holder_base_ptr invoker;

public:
    BasicFunction() = default;

    template <typename F>
    BasicFunction(F f) : invoker(new free_function_holder(std::move(f))) {}

    R operator()(Args... args) {
        return invoker->invoke(std::forward<Args...>(args)...);
    }

    explicit operator bool() const { return invoker.operator bool(); }

private:
    class function_holder_base {
    public:
        function_holder_base() = default;
        virtual ~function_holder_base() = default;
        virtual R invoke(Args...) = 0;
    };

    template <typename F>
    class free_function_holder : public function_holder_base {
    public:
        explicit free_function_holder(F f)
            : function_holder_base(), func(std::move(f)) {}
        ~free_function_holder() override = default;

        R invoke(Args... args) override {
            return func(std::forward<Args...>(args)...);
        }

    private:
        F func;
    };
};
}  // namespace NComponents
