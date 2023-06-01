/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 Metrological
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../IPTestAdministrator.h"

#include <gtest/gtest.h>
#include <core/core.h>

#include <cstring>

using namespace WPEFramework;

TEST(Core_DataBuffer, simpleSet)
{
    Core::ScopedStorage<10u> buffer;

    EXPECT_EQ(buffer.Size(), 10u);
    EXPECT_NE(buffer.Buffer(), nullptr);
    EXPECT_EQ(*buffer.Buffer(), '\0');

    const std::string data = "abcdefghi";
    EXPECT_STREQ(reinterpret_cast<char*>(std::memcpy(buffer.Buffer(), data.c_str(), data.size())), data.c_str());
}
