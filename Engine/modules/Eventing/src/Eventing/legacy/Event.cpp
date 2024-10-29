#if 0

	#include "Event.hpp"

namespace SW
{

	struct RegisteredEvent final
	{
		void* listener         = nullptr;
		EventCallback callback = nullptr;
	};

	struct EventCodeEntry final
	{
		std::vector<RegisteredEvent> events;
	};

// To be expanded if necessary
	#define MAX_EVENT_CODES 24

	struct EventSystemState final
	{
		EventCodeEntry registered[MAX_EVENT_CODES];
	};

	static EventSystemState* state = nullptr;

	bool AreEventCallbacksEqual(const EventCallback& a, const EventCallback& b)
	{
		return a.target_type() == b.target_type() && a.target<void (*)(Event)>() == b.target<void (*)(Event)>();
	}

	bool EventSystem::Initialize()
	{
		state = new EventSystemState();

		SYSTEM_INFO("Event system has been properly initialized");

		return true;
	}

	bool EventSystem::Shutdown()
	{
		delete state;

		state = nullptr;

		SYSTEM_INFO("Event system has been properly shut down");

		return true;
	}

	bool EventSystem::Register(EventCode code, EventCallback handler)
	{
		ASSERT(state, "Event system must be initialized before registering a listener!");

		for (u64 i = 0; i < state->registered[code].events.size(); ++i)
		{
			if (AreEventCallbacksEqual(state->registered[code].events[i].callback, handler))
			{
				SYSTEM_WARN("Event has already been registered with the code {} and the same callback!", (int)code);
				return false;
			}
		}

		RegisteredEvent toRegister = {.callback = handler};

		state->registered[code].events.emplace_back(toRegister);

		return true;
	}

	bool EventSystem::UnRegister(EventCode code, EventCallback handler)
	{
		ASSERT(state, "Event system must be initialized before unregistering a listener!");

		if (state->registered[code].events.empty())
		{
			SYSTEM_WARN("No event is registered for the code {}", (int)code);

			return false;
		}

		// Find the exact event handler for exact event listener and remove it.
		for (u64 i = 0; i < state->registered[code].events.size(); ++i)
		{
			if (AreEventCallbacksEqual(state->registered[code].events[i].callback, handler))
			{
				std::vector<RegisteredEvent>::iterator it = state->registered[code].events.begin();
				std::advance(it, i);

				state->registered[code].events.erase(it);
			}
		}

		return false;
	}

	bool EventSystem::Emit(Event event)
	{
		ASSERT(state, "Event system must be initialized before emitting an event!");

		if (state->registered[event.Code].events.empty())
		{
			SYSTEM_WARN("No event listener is registered for the code {}", (int)event.Code);

			return false;
		}

		for (u64 i = 0; i < state->registered[event.Code].events.size(); ++i)
		{
			const RegisteredEvent e = state->registered[event.Code].events[i];

			/*if (e.callback(event, sender, e.listener)) {
			    return true;
			}*/
			if (e.callback(event))
			{
				return true;
			}
		}

		SYSTEM_DEBUG("Event with the code {} has not been fully handled!", (int)event.Code);

		return false;
	}

	bool EventSystem::Emit(EventCode code)
	{
		return Emit({.Code = code});
	}

} // namespace SW

#endif
