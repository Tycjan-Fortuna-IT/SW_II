#pragma once
#include <functional>

namespace SW {

    enum EventCode : u8
    {
        /** @brief Shuts the application down on the next frame. */
        EVENT_CODE_APPLICATION_QUIT = 0x01,
    };

    struct Event final
    {
        /** @brief In total 128 bytes of data. */
        union {
            /** @brief An array of 2 64-bit signed integers. */
            i64 i64[2];
            /** @brief An array of 2 64-bit unsigned integers. */
            u64 u64[2];
        } Payload;

        EventCode Code;
    };

    // typedef bool(*EventCallback)(Event event, void* sender, void* listener);
    using EventCallback = std::function<bool(Event event, void* sender, void* listener)>;

    bool AreEventCallbacksEqual(const EventCallback& a, const EventCallback& b);

    class EventSystem final
    {
    public:

        static bool Initialize();
        static bool Shutdown();

        static bool Register(EventCode code, void* listener, EventCallback handler);
        static bool UnRegister(EventCode code, void* listener, EventCallback handler);

        static bool Emit(Event event, void* sender);
    };

}
