/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_HARDWARE_MEDIA_BUFFERPOOL_V1_0_BUFFERPOOLCLIENT_H
#define ANDROID_HARDWARE_MEDIA_BUFFERPOOL_V1_0_BUFFERPOOLCLIENT_H

#include <memory>
#include <cutils/native_handle.h>
#include <android/hardware/media/bufferpool/1.0/IAccessor.h>
#include <android/hardware/media/bufferpool/1.0/IConnection.h>
#include <BufferPoolTypes.h>
#include "Accessor.h"

namespace android {
namespace hardware {
namespace media {
namespace bufferpool {
namespace V1_0 {
namespace implementation {

using ::android::hardware::media::bufferpool::V1_0::IAccessor;
using ::android::hardware::media::bufferpool::V1_0::IConnection;
using ::android::hardware::media::bufferpool::V1_0::ResultStatus;
using ::android::sp;

/**
 * A buffer pool client for a buffer pool. For a specific buffer pool, at most
 * one buffer pool client exists per process. This class will not be exposed
 * outside. A buffer pool client will be used via ClientManager.
 */
class BufferPoolClient {
public:
    /**
     * Creates a buffer pool client from a local buffer pool
     * (via ClientManager#create).
     */
    explicit BufferPoolClient(const sp<Accessor> &accessor);

    /**
     * Creates a buffer pool client from a remote buffer pool
     * (via ClientManager#registerSender).
     * Note: A buffer pool client created with remote buffer pool cannot
     * allocate a buffer.
     */
    explicit BufferPoolClient(const sp<IAccessor> &accessor);

    /** Destructs a buffer pool client. */
    ~BufferPoolClient();

private:
    bool isValid();

    ConnectionId getConnectionId();

    ResultStatus getAccessor(sp<IAccessor> *accessor);

    ResultStatus allocate(const std::vector<uint8_t> &params,
                          std::shared_ptr<_C2BlockPoolData> *buffer);

    ResultStatus receive(TransactionId transactionId,
                         BufferId bufferId,
                         int64_t timestampUs,
                         std::shared_ptr<_C2BlockPoolData> *buffer);

    ResultStatus postSend(ConnectionId receiver,
                          const std::shared_ptr<_C2BlockPoolData> &buffer,
                          TransactionId *transactionId,
                          int64_t *timestampUs);

    class Impl;
    std::shared_ptr<Impl> mImpl;

    friend struct ClientManager;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace bufferpool
}  // namespace media
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_MEDIA_BUFFERPOOL_V1_0_BUFFERPOOLCLIENT_H