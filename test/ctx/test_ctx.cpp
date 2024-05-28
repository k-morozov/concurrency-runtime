//
// Created by konstantin on 10.05.24.
//

#include "gtest/gtest.h"

#include <context/buffer/buffer.h>
#include <context/context.h>
#include <context/runner.h>

using Buffer = go::impl::ctx::Buffer;
using Ctx = go::impl::ctx::Context;

template <class T>
class TestCoro : private go::impl::Runner {
    struct SuspendCtx {
        TestCoro* coro;
        void Suspend() {
            coro->Suspend();
        }
    };

public:
    explicit TestCoro(T&& body) : body_(std::move(body)) {
        ctx_.Setup(buffer_, this);
    };

    void Resume() {
        assert(!IsDone());
        caller_ctx_.SwitchTo(ctx_);
    }

    void Suspend() {
        ctx_.SwitchTo(caller_ctx_);
    }

    bool IsDone() const noexcept { return done_; }

private:
    T body_;
    std::byte buffer_[1024];
    Ctx ctx_;
    Ctx caller_ctx_;
    bool done_{false};

    void Run() noexcept override {
        try {
            body_(SuspendCtx{this});
        } catch (...) {
            std::abort();
        }
        done_ = true;

        // @todo change to ExitTo
        ctx_.ExitTo(caller_ctx_);
    }
};

TEST(TestCtxSimple, JustWorks1Coro) {
    TestCoro coro([](auto self) { self.Suspend(); });

    coro.Resume();
    EXPECT_FALSE(coro.IsDone());

    coro.Resume();
    EXPECT_TRUE(coro.IsDone());
}

TEST(TestCtxSimple, JustWorks2Coro) {
    TestCoro a([](auto self) { self.Suspend(); });

    TestCoro b([](auto self) { self.Suspend(); });

    a.Resume();
    b.Resume();

    a.Resume();
    b.Resume();

    EXPECT_TRUE(a.IsDone());
    EXPECT_TRUE(b.IsDone());
}
