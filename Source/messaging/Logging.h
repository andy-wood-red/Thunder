/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2022 Metrological
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

#pragma once

#include "Module.h"
#include "Control.h"
#include "TextMessage.h"
#include "BaseCategory.h"
#include "MessageUnit.h"

namespace WPEFramework {

namespace Logging {

    void EXTERNAL DumpException(const string& exceptionType);
    void EXTERNAL DumpSystemFiles(const Core::process_t pid);

    template <typename CATEGORY>
    class BaseLoggingType : public Messaging::BaseCategoryType<Core::Messaging::Metadata::type::LOGGING> {
    public:
        using BaseClass = Messaging::BaseCategoryType<Core::Messaging::Metadata::type::LOGGING>;
        using Control = Messaging::ControlType<CATEGORY, &Core::Messaging::MODULE_LOGGING, Core::Messaging::Metadata::type::LOGGING>;

        BaseLoggingType(const BaseLoggingType&) = delete;
        BaseLoggingType& operator=(const BaseLoggingType&) = delete;

        BaseLoggingType() = default;
        ~BaseLoggingType() = default;

        using BaseClass::BaseClass;

    public:
        inline static void Announce() {
            IsEnabled();
        }

        inline static bool IsEnabled() {
            return (_control.IsEnabled());
        }

        inline static void Enable(const bool enable) {
            _control.Enable(enable);
        }
        
        inline static const Core::Messaging::Metadata& Metadata() {
            return (_control.Metadata());
        }

    private:
        static Control _control;
    };

} // namespace Logging
}

#ifdef __WINDOWS__

#define DEFINE_LOGGING_CATEGORY(CATEGORY)                                                                                                         \
    DEFINE_MESSAGING_CATEGORY(WPEFramework::Logging::BaseLoggingType<CATEGORY>, CATEGORY)

#else

#define DEFINE_LOGGING_CATEGORY(CATEGORY)                                                                                                         \
    DEFINE_MESSAGING_CATEGORY(WPEFramework::Logging::BaseLoggingType<CATEGORY>, CATEGORY)                                                         \
    template<>                                                                                                                                    \
    EXTERNAL typename WPEFramework::Logging::BaseLoggingType<CATEGORY>::Control WPEFramework::Logging::BaseLoggingType<CATEGORY>::_control;

#endif

#define SYSLOG_ANNOUNCE(CATEGORY) template<> WPEFramework::Logging::BaseLoggingType<CATEGORY>::Control WPEFramework::Logging::BaseLoggingType<CATEGORY>::_control(true)

#define SYSLOG(CATEGORY, PARAMETERS)                                                                                                              \
    do {                                                                                                                                          \
        static_assert(std::is_base_of<WPEFramework::Logging::BaseLoggingType<CATEGORY>, CATEGORY>::value, "SYSLOG() only for Logging controls");  \
        if (CATEGORY::IsEnabled() == true) {                                                                                                      \
            CATEGORY __data__ PARAMETERS;                                                                                                         \
            WPEFramework::Core::Messaging::MessageInfo __info__(                                                                                  \
                CATEGORY::Metadata(),                                                                                                             \
                WPEFramework::Core::Time::Now().Ticks()                                                                                           \
            );                                                                                                                                    \
            WPEFramework::Core::Messaging::IStore::Logging __log__(__info__);                                                                     \
            WPEFramework::Messaging::TextMessage __message__(__data__.Data());                                                                    \
            WPEFramework::Messaging::MessageUnit::Instance().Push(__log__, &__message__);                                                         \
        }                                                                                                                                         \
    } while(false)

#define SYSLOG_GLOBAL(CATEGORY, PARAMETERS)                                                                                                       \
    do {                                                                                                                                          \
        static_assert(std::is_base_of<WPEFramework::Logging::BaseLoggingType<CATEGORY>, CATEGORY>::value, "SYSLOG() only for Logging controls");  \
        if (CATEGORY::IsEnabled() == true) {                                                                                                      \
            CATEGORY __data__ PARAMETERS;                                                                                                         \
            WPEFramework::Core::Messaging::MessageInfo __info__(                                                                                  \
                CATEGORY::Metadata(),                                                                                                             \
                WPEFramework::Core::Time::Now().Ticks()                                                                                           \
            );                                                                                                                                    \
            WPEFramework::Core::Messaging::IStore::Logging __log__(__info__);                                                                     \
            WPEFramework::Messaging::TextMessage __message__(__data__.Data());                                                                    \
            WPEFramework::Messaging::MessageUnit::Instance().Push(__log__, &__message__);                                                         \
        }                                                                                                                                         \
    } while(false)

