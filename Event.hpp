#pragma once
#include <unordered_map>
#include <functional>


namespace Core{
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
    //TDOD can change to work with vec need to chang how to add function
    //auto myCallback = [this]() { move_right(); }; //make fuction
    //event.AddListener(this, myCallback); //add to event
    //event.RemoveListener(this, myCallback); //remove the same object

    //right.AddListener(this, [this]() {move_right();}); //make fuction and add to event, but dont store object or piotner
}
