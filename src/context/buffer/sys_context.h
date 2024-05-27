//
// Created by konstantin on 16.05.24.
//

#pragma once

extern "C" {

extern void* PrepareContextInBuffer(void* buffer, void* runner, void* arg);

extern void SwitchBufferContext(void** source, void** destination);
}