//
// Created by konstantin on 19.07.24.
//

#pragma once

namespace NComponents {

struct Event;

struct MutexAwaiter final {
    Event& event;

    explicit MutexAwaiter(Event&);
};

}  // namespace NComponents