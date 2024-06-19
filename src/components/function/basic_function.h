//
// Created by konstantin on 19.06.24.
//

#pragma once

#include <memory>

namespace NComponents {

template <class U>
class BasicFunction;

template <typename R, typename... Args>
class BasicFunction <R(Args...)> {
public:
    BasicFunction() {}

    template <typename F>
    BasicFunction(F f) : invoker(new free_function_holder(f)) {}

    R operator()(Args... args) {
        return invoker->invoke(std::forward<Args...>(args)...);
    }

private:
    class function_holder_base {
    public:
        function_holder_base() {}
        virtual ~function_holder_base() {};
        virtual R invoke(Args...) = 0;
    };

    using function_holder_base_ptr = std::shared_ptr<function_holder_base>;

    template <typename F>
    class free_function_holder : public function_holder_base {
    public:
        free_function_holder(F f) : function_holder_base(), func(f) {}
        ~free_function_holder() override = default;

        R invoke(Args... args) override {
            return func(std::forward<Args...>(args)...);
        }

    private:
        F func;
    };

    function_holder_base_ptr invoker;
};
}  // namespace NComponents
