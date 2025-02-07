/*
 *   Copyright (c) 2021 Project CHIP Authors
 *   All rights reserved.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 */

#include <mutex>
#include <vector>

#include "pw_trace_chip/trace_chip.h"

#include "pw_trace/trace.h"

namespace chip {
namespace trace {
namespace {

std::mutex handlerLock;
std::vector<TraceHandlerCallback> traceHandlers;

} // namespace

void TraceEvent(const char * module, const char * label, TraceEventType eventType, const char * group, uint32_t traceId,
                uint8_t flags, const char * dataFormat, const void * dataBuffer, size_t dataSize)
{
    TraceEventFields fields = { module, label, eventType, group, traceId, flags, dataFormat, dataBuffer, dataSize };
    std::lock_guard<std::mutex> guard(handlerLock);
    for (const auto & handler : traceHandlers)
    {
        if (handler(fields))
        {
            return;
        }
    }
}

void RegisterTraceHandler(TraceHandlerCallback callback)
{
    std::lock_guard<std::mutex> guard(handlerLock);
    traceHandlers.push_back(callback);
}

void UnregisterAllTraceHandlers()
{
    std::lock_guard<std::mutex> guard(handlerLock);
    traceHandlers.clear();
}

} // namespace trace
} // namespace chip
