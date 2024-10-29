/**
 * @file Event.hpp
 * @author SW
 * @version 0.1.3
 * @date 2024-05-04
 *
 * @copyright Copyright (c) 2024 SW
 */
#pragma once

#if 0

namespace SW
{

	/**
	 * @brief EventCode is an enum representing the type of the event.
	 */
	enum EventCode : u8
	{
		/** @brief Shuts the application down on the next frame. */
		EVENT_CODE_APPLICATION_QUIT = 0x01,

		/**
		 * @brief Keyboard key pressed.
		 * @warning Context usage: u16 keyCode = event.Payload.u16[0];
		 */
		EVENT_CODE_KEY_PRESSED = 0x02,

		/**
		 * @brief Keyboard key released.
		 * @warning Context usage: u16 keyCode = event.Payload.u16[0];
		 */
		EVENT_CODE_KEY_RELEASED = 0x03,

		/**
		 * @brief Keyboard key repeat.
		 * @warning Context usage: u16 keyCode = event.Payload.u16[0];
		 */
		EVENT_CODE_KEY_REPEAT = 0x04,

		/**
		 * @brief Mouse wheel scrolled.
		 * @warning Context usage: f32 xOffset = event.Payload.f32[0]; f32 yOffset = event.Payload.f32[1];
		 */
		EVENT_CODE_MOUSE_WHEEL = 0x05,

		/**
		 * @brief OS Window resized/resolution changed.
		 * @warning Context usage: i32 width = event.Payload.i32[0]; i32 height = event.Payload.i32[1];
		 */
		EVENT_CODE_WINDOW_RESIZED = 0x06,

		/**
		 * @brief Mouse button pressed.
		 * @warning Context usage: u16 mouseCode = event.Payload.u16[0];
		 */
		EVENT_CODE_MOUSE_BUTTON_PRESSED = 0x07,

		/**
		 * @brief Mouse button released.
		 * @warning Context usage: u16 mouseCode = event.Payload.u16[0];
		 */
		EVENT_CODE_MOUSE_BUTTON_RELEASED = 0x08,

		/**
		 * @brief Engine project loaded.
		 * @warning Event without payload!
		 */
		EVENT_CODE_PROJECT_LOADED = 0x09,

		/**
		 * @brief A change of the project's assets directory was made by the sender.
		 * @warning Event without payload!
		 */
		EVENT_CODE_ASSET_DIR_CONTENT_CHANGED = 0x10,
	};

	/**
	 * @brief Event is a struct representing an event.
	 * @note Maximum event message size is 128 bytes.
	 */
	struct Event final
	{
		/** @brief The type of the event. */
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

	/** @brief EventCallback is a function pointer (interface) for event callbacks.*/
	using EventCallback = std::function<bool(Event event)>;

	/**
	 * @brief Compares two event callbacks.
	 *
	 * @param a The first event callback.
	 * @param b The second event callback.
	 * @return bool True if the callbacks are equal, false otherwise.
	 */
	bool AreEventCallbacksEqual(const EventCallback& a, const EventCallback& b);

	/**
	 * @brief EventSystem is a static class that exposes event system functionality.
	 */
	class EventSystem final
	{
	public:
		/**
		 * @brief Initializes the event system.
		 * @warning This function must be called before any other event system function.
		 *
		 * @return bool True if the event system was initialized successfully, false otherwise.
		 */
		static bool Initialize();

		/**
		 * @brief Shuts down the event system.
		 *
		 * @return bool True if the event system was shut down successfully, false otherwise.
		 */
		static bool Shutdown();

		/**
		 * @brief Registers an event callback. An event callback a function that is called when an event is emitted.
		 * @param code The event code to register the listener for.
		 * @param handler The event callback to register.
		 * @return bool True if the listener was registered successfully, false otherwise.
		 */
		static bool Register(EventCode code, EventCallback handler);

		/**
		 * @brief Unregisters an event callback. An event callback is a function that is called when an event is
		 * emitted.
		 *
		 * @param code The event code to unregister the listener for.
		 * @param handler The event callback to unregister.
		 * @return bool True if the listener was unregistered successfully, false otherwise.
		 */
		static bool UnRegister(EventCode code, EventCallback handler);

		/**
		 * @brief Emits an event. This function calls all registered event listeners for the specified event code
		 * 		  and passes the event with the data to them.
		 *
		 * @param event The event to emit.
		 * @return bool True if the event was emitted successfully, false otherwise.
		 */
		static bool Emit(Event event);

		/**
		 * @brief Emits an empty event based on passed event code.
		 * @note Especially useful for treating certain event just as an impulse, without any data.
		 *
		 * @param code The event to emit.
		 * @return bool True if the event was emitted successfully, false otherwise.
		 */
		static bool Emit(EventCode code);
	};

} // namespace SW

#endif
