#pragma once

namespace SW {

    enum EventCode : u8
    {
        /** @brief Shuts the application down on the next frame. */
        EVENT_CODE_APPLICATION_QUIT = 0x01,

        /** @brief Keyboard key pressed.
         * Context usage:
         * u16 keyCode = data.data.u16[0];
         */
        EVENT_CODE_KEY_PRESSED = 0x02,

        /** @brief Not implemented */
        EVENT_CODE_KEY_RELEASED = 0x03,

        /** @brief Not implemented */
        EVENT_CODE_KEY_REPEAT = 0x04,
    };

    struct Event final
    {
        EventCode Code;

        /** @brief In total 128 bytes of data. */
        union {
            /** @brief An array of 2 64-bit signed integers. */
            i64 i64[2];
            /** @brief An array of 2 64-bit unsigned integers. */
            u64 u64[2];
            /** @brief An array of 2 64-bit floating-point numbers. */
            f64 f64[2];

            /** @brief An array of 4 32-bit signed integers. */
            i32 i32[4];
            /** @brief An array of 4 32-bit unsigned integers. */
            u32 u32[4];
            /** @brief An array of 4 32-bit floating-point numbers. */
            f32 f32[4];

            /** @brief An array of 8 16-bit signed integers. */
            i16 i16[8];

            /** @brief An array of 8 16-bit unsigned integers. */
            u16 u16[8];

            /** @brief An array of 16 8-bit signed integers. */
            i8 i8[16];
            /** @brief An array of 16 8-bit unsigned integers. */
            u8 u8[16];

            /** @brief An array of 16 characters. */
            char c[16];
        } Payload;
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
