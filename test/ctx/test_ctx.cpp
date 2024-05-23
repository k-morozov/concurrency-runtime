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
        TestCoro* coro_;
        void Suspend() {}
    };

public:
    explicit TestCoro(T&& body) : body_(std::move(body)) {
        caller_ctx_.Setup(buffer_, this);
    };

    void Resume();
    void Suspend();

    bool IsDone() const { return done_; }

private:
    T body_;
    std::byte buffer_[1024];
    Ctx caller_ctx_;
    bool done_{false};

    void Run() override {}
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
