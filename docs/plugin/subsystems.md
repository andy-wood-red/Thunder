Thunder provides "Subsystems" which are abstract categories of functionality (Network, Graphics, Internet) that can be marked as up/down by plugins.

- Plugins can then have preconditions on these subsystems (e.g. prevent starting the browser plugin until internet is available).
- Support for susbsytems tends to works better on a platform where everything is implemented in Thunder, less so in RDK where there is a need to contend with platform/system dependencies.
- Subsystems have been not widely used in RDK as a result.


<h3>Supported SubSystems</h3>

In the header file *ISubSystem.h* the following enumerated types can be found which represent the various subsystems supported by Thunder:

```cpp
        enum subsystem : uint32_t {
            PLATFORM = 0, // platform is available.
            SECURITY, // A security system can validate external requests (JSONRPC/WebRequest)
            NETWORK, // Network connectivity has been established.
            IDENTIFIER, // System identification has been accomplished.
            GRAPHICS, // Graphics screen EGL is available.
            INTERNET, // Network connectivity to the outside world has been established.
            LOCATION, // Location of the device has been set.
            TIME, // Time has been synchronized.
            PROVISIONING, // Provisioning information is available.
            DECRYPTION, // Decryption functionality is available.
            WEBSOURCE, // Content exposed via a local web server is available.
            STREAMING, // Content can be streamed.
            BLUETOOTH, // The bluetooth subsystem is up and running.
            END_LIST,

            // Also define a "negative" value.
            NEGATIVE_START = 0x80000000,
            NOT_PLATFORM = NEGATIVE_START, // platform is NOT available.
            NOT_SECURITY, // A security system can validate external requests (JSONRPC/WebRequest)
            NOT_NETWORK, // Network connectivity has NOT been established.
            NOT_IDENTIFIER, // System identification has NOT been accomplished.
            NOT_GRAPHICS, // Graphics screen EGL is NOT available.
            NOT_INTERNET, // Network connectivity to the outside world has been established.
            NOT_LOCATION, // Location of the device has NOT been set.
            NOT_TIME, // Time has been NOT synchronized.
            NOT_PROVISIONING, // Provisioning information is NOT available.
            NOT_DECRYPTION, // Decryption functionality is NOT available.
            NOT_WEBSOURCE, // Content exposed via a local web server is NOT available.
            NOT_STREAMING, // Content can NOT be streamed.
            NOT_BLUETOOTH // The Bluetooth communication system is NOT available.
        };
```


<h3>SubSystem Plugin Interface - to set and get</h3>

In order to indicate and check for changes to subsystems, the plugin supports the following COM interface:

```cpp
        // Events setter and getters.
        virtual void Set(const subsystem type, Core::IUnknown* information) = 0;
        virtual const Core::IUnknown* Get(const subsystem type) const = 0;
        virtual bool IsActive(const subsystem type) const = 0;
```

In order to use these methods they should be used in a way similar to the following:
```cpp
void TestPlugin::Initialize(PluginHost::IShell* service) {
    _service = service;
    // Other init code...
    PluginHost::ISubSystem* subSystems = _service->SubSystems();
    if (subSystems != nullptr) {
        subSystems->Set(PluginHost::ISubSystem::GRAPHICS, nullptr);
    }
}
```

The Get() API call will return a type consistent with the subsystem type passed as a parameter for example if the type was INTERNET:

```cpp
// returns pointer to Internet class
Internet* _internet;

// where
        class Internet : public PluginHost::ISubSystem::IInternet {
        public:
            Internet(const Internet&) = delete;
            Internet& operator=(const Internet&) = delete;

            Internet()
                : _ipAddress()
            {
            }
            ~Internet() override = default;

        public:
            BEGIN_INTERFACE_MAP(Internet)
            INTERFACE_ENTRY(PluginHost::ISubSystem::IInternet)
            END_INTERFACE_MAP

        public:
            string PublicIPAddress() const override;
            PluginHost::ISubSystem::IInternet::network_type NetworkType() const override;

            bool Set(const PluginHost::ISubSystem::IInternet* info);
            inline bool Set(const string& ip)
            {
                bool result(false);
                if (_ipAddress != ip) {
                    _ipAddress = ip;

                    result = true;
                }
                return result;
            }

        private:
            string _ipAddress;
        };
// All types defined in Thunder/Source/WPEFramework/SystemInfo.h

```


<h3>Plugin Startup</h3>
Each plugin config can add dependencies on subsytems being available before starting. This is achieved with the following sort of entry in the plugin config file:

```json
        "precondition":[
            "Platform"
        ]
```

The Susbsystem string format and supported susbsytems to add as preconditions:

 ```
       PLATFORM, _TXT("Platform")
        NETWORK, _TXT("Network")
        SECURITY, _TXT("Security")
        IDENTIFIER, _TXT("Identifier")
        INTERNET, _TXT("Internet")
        LOCATION, _TXT("Location")
        TIME, _TXT("Time")
        PROVISIONING, _TXT("Provisioning")
        DECRYPTION, _TXT("Decryption")
        GRAPHICS, _TXT("Graphics")
        WEBSOURCE, _TXT("WebSource")
        STREAMING, _TXT("Streaming")
        BLUETOOTH, _TXT("Bluetooth")       
```


