/**
 * @file Event.hpp
 * @author SW
 * @version 0.2.0
 * @date 2024-10-28
 *
 * @copyright Copyright (c) 2024 SW
 */
#pragma once

#include <functional>
#include <unordered_map>

namespace SW::Eventing
{

	using u32        = unsigned int;
	using ListenerID = u32;

	template <typename... CallbackArgTypes>
	class Event
	{
	public:
		// Callback of such type to be called when the event is invoked.
		using Callback = std::function<void(CallbackArgTypes...)>;

		Event()  = default;
		~Event() = default;

		// Adds a listener to the event.
		ListenerID AddListener(Callback callback);
		ListenerID operator+=(Callback callback);

		// Removes a listener from the event.
		bool RemoveListener(ListenerID listenerID);
		bool operator-=(ListenerID listenerID);

		// Removes all listeners from the event.
		void RemoveAllListeners();

		// Returns the number of listeners.
		u32 GetListenerCount();

		// Invokes the event with the specified arguments.
		void Invoke(CallbackArgTypes... args);

	private:
		std::unordered_map<ListenerID, Callback> m_Callbacks; // Map of listener IDs to their callbacks.

		ListenerID m_LastListenerID = 0u; // Last listener ID used.
	};

} // namespace SW::Eventing

#include "Eventing/Event.inl"
