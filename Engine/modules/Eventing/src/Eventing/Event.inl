/**
 * @file Event.inl
 * @author SW
 * @version 0.2.0
 * @date 2024-10-28
 *
 * @copyright Copyright (c) 2024 SW
 */
#pragma once

namespace SW::Eventing
{

	template <typename... CallbackArgTypes>
	ListenerID Event<CallbackArgTypes...>::AddListener(Callback callback)
	{
		m_Callbacks.emplace(m_LastListenerID, callback);

		return m_LastListenerID++;
	}

	template <typename... CallbackArgTypes>
	ListenerID Event<CallbackArgTypes...>::operator+=(Callback callback)
	{
		return AddListener(callback);
	}

	template <typename... CallbackArgTypes>
	bool Event<CallbackArgTypes...>::RemoveListener(ListenerID listenerID)
	{
		return m_Callbacks.erase(listenerID) != 0;
	}

	template <typename... CallbackArgTypes>
	bool Event<CallbackArgTypes...>::operator-=(ListenerID listenerID)
	{
		return RemoveListener(listenerID);
	}

	template <typename... CallbackArgTypes>
	void Event<CallbackArgTypes...>::RemoveAllListeners()
	{
		m_Callbacks.clear();
	}

	template <typename... CallbackArgTypes>
	u32 Event<CallbackArgTypes...>::GetListenerCount()
	{
		return m_Callbacks.size();
	}

	template <typename... CallbackArgTypes>
	void Event<CallbackArgTypes...>::Invoke(CallbackArgTypes... args)
	{
		for (auto const& [key, value] : m_Callbacks) value(args...);
	}

} // namespace SW::Eventing
