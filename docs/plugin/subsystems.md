Thunder provides "Subsystems" which are abstract categories of functionality (Network, Graphics, Internet) that can be marked as up/down by plugins.

- Plugins can then have preconditions on these subsystems (e.g. prevent starting the browser plugin until internet is available).
- Support for susbsytems tends to works better on a platform where everything is implemented in Thunder.


<h3>Supported SubSystems</h3>

Thunder supports the following subsystems (enumerated in *ISubSystem.h*):

| **Subsystem**    | **Description**                                                                    |
|------------------|------------------------------------------------------------------------------------|
| PLATFORM         | Platform is available                                                              |
| SECURITY         | A security system can validate external requests (JSONRPC/WebRequest)              |
| NETWORK          | Network connectivity has been established.                                         |
| IDENTIFIER       | System identification has been accomplished.                                       |
| GRAPHICS         | Graphics screen EGL is available.                                                  |
| INTERNET         | Network connectivity to the outside world has been established.                    |
| LOCATION         | Location of the device has been set.                                               |
| TIME             | Time has been synchronized.                                                        |
| PROVISIONING     | Provisioning information is available.                                             |
| DECRYPTION       | Decryption functionality is available.                                             |
| WEBSOURCE        | Content exposed via a local web server is available.                               |
| STREAMING        | Content can be streamed.                                                           |
| BLUETOOTH        | The Bluetooth subsystem is up and running.                                         |
| NOT_PLATFORM     | The platform is NOT available.                                                     |
| NOT_SECURITY     | No security system is available to validate external requests (JSONRPC/WebRequest) |
| NOT_NETWORK      | Network connectivity has NOT been established.                                     |
| NOT_IDENTIFIER   | System identification has NOT been accomplished.                                   |
| NOT_GRAPHICS     | Graphics screen EGL is NOT available.                                              |
| NOT_INTERNET     | Network connectivity to the outside world has not been established.                |
| NOT_LOCATION     | Location of the device has NOT been set.                                           |
| NOT_TIME         | Time has been NOT synchronized.                                                    |
| NOT_PROVISIONING | Provisioning information is NOT available.                                         |
| NOT_DECRYPTION   | Decryption functionality is NOT available.                                         |
| NOT_WEBSOURCE    | Content exposed via a local web server is NOT available.                           |
| NOT_STREAMING    | Content can NOT be streamed.                                                       |
| NOT_BLUETOOTH    | The Bluetooth communication system is NOT available.                               |


<h3>Checking and Marking Subsystems</h3>

The plugin supports both JSON and COM RPC methods.

There are 3 COM-RPC methods that can be used to check and mark subsystems, these are:

```cpp
        // Events setter and getters.
        virtual void Set(const subsystem type, Core::IUnknown* information) = 0;
        virtual const Core::IUnknown* Get(const subsystem type) const = 0;
        virtual bool IsActive(const subsystem type) const = 0;
```

There are 2 JSON-RPC methods which are:

```
activate
deactivate
```

<h4>Marking a Subsytem as Available/Unavailable</h4>

In order to mark a subsystem the Set() method may be used in the following manner:
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

<h4>Checking Subsytem State</h4>

Checking availability is achieved using the IsAvailable() API call. The IsActive() call simply returns true if the susbsytem exists in a list of active subsystems.

Determining the operational status of a subsystem is then achieved via the Get() method. The Get() method will return an object consistent with the subsystem type passed as a parameter. 

For example if attempting to obtain Internet status:

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

<h3>Plugin Startup Dependencies</h3>
Every plugin config can add dependencies on subsytems being available before starting. This may be achieved by adding the following sort of entry to each plugin config file:

```json
        "precondition":[
            "Platform"
        ]
```

See [config](config.md) for more details on plugin config options.

Preconditions can also be set in the plugin metadata...

The Susbsystem string format and supported susbsytems to add as preconditions:

| **Subsystem Type** | **String**                |
|--------------------|---------------------------|
| PLATFORM           | "Platform"                |
| NETWORK            | "Network"                 |
| SECURITY           | "Security"                |
| IDENTIFIER         | "Identifier"              |
| INTERNET           | "Internet"                |
| LOCATION           | "Location"                |
| TIME               | "Time"                    |
| PROVISIONING       | "Provisioning"            |
| DECRYPTION         | "Decryption"              |
| GRAPHICS           | "Graphics"                |
| WEBSOURCE          | "WebSource"               |
| STREAMING          | "Streaming"               |
| BLUETOOTH          | "Bluetooth"               |
|                    |                           |


<h3>Event Notification</h3>

Changes to subsystems are notified to interested parties which have registered using:

```cpp
        virtual void Register(ISubSystem::INotification* notification) = 0;
        virtual void Unregister(ISubSystem::INotification* notification) = 0;
```
An implementation (or implementations) of the Updated() method should then be called for that subsystem, where appropriate handler code can process the updates.
