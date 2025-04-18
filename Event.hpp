#pragma once
#include <unordered_map>
#include <functional>


namespace Core{
	/// <summary>
    /// A simple generic event system that supports adding/removing listeners
    /// based on an identifying key (typically the 'this' pointer of the object adding it).
    ///
    /// Usage:
    ///     event.AddListener(this, [](){ ... });
    ///     event.Invoke(...);
    ///     event.RemoveListener(this);
    ///
    /// Limitations:
    ///     - Only one listener per object (key) by default.
    ///     - If multiple listeners per object are needed, consider rewriting the mapping
    ///       or use the questionable way (see TODOs).
	/// </summary>
	/// <typeparam name="...Args"></typeparam>
	template<typename... Args>
	class Event
	{
	private:
		std::unordered_map<void*, std::function<void(Args...)>> m_Listeners;
    public:
        void AddListener(void* obj, std::function<void(Args...)> listener);

        void RemoveListener(void* obj);

        void operator-=(void* obj);

        void Invoke(Args... args);
        void Clear();
    };


    template<typename... Args>
    inline void Event<Args...>::AddListener(void* obj, std::function<void(Args...)> listener)
    {
        m_Listeners[obj] = listener;
    }


    template<typename... Args>
    inline void Event<Args...>::RemoveListener(void* obj)
    {
        auto it = m_Listeners.find(obj);
        if (it != m_Listeners.end()) {
            m_Listeners.erase(it);
        }
    }

    template<typename... Args>
    inline void Event<Args...>::operator-=(void* obj)
    {
        RemoveListener(obj);
    }

	template<typename ...Args>
	inline void Event<Args...>::Invoke(Args ...args)
	{
        // copy list so the callback can be completed even if the event gets destroyed in the proses
        auto listeners_copy = m_Listeners;  
        for (std::pair<void* const, std::function<void(Args...)>> listener : listeners_copy) {
            listener.second(args...);
        }
	}

    template<typename... Args>
    inline void Event<Args...>::Clear()
	{
        m_Listeners.clear();
	}

    //TODO can work with vec, make it one object that holds the key and the function,
    //TODO create a object that can compare its self, holds a void*/size_t/... to the owner object and a index and make a save system for holding more listener from one object, or do it the questionable way by { reinturpret_cast<void*>(this) + 1 }
}
