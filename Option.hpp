#pragma once
#include "MACRO.hpp"
#include <string>
#include <utility>
#include <type_traits>

namespace Util {
    template<typename T, bool IsRef = std::is_reference<T>::value>
    class Option;

    // Specialization for non-reference types
    template<typename T>
    class Option<T, false> {
    private:
        bool is_some;
        alignas(T) char storage[sizeof(T)]; // Uninitialized storage for T

        T* get_value_ptr() {
            return reinterpret_cast<T*>(&storage);
        }

        const T* get_value_ptr() const {
            return reinterpret_cast<const T*>(&storage);
        }

    public:
        Option() : is_some(false) {} // None

        Option(const T& a_value) : is_some(true) {
            new (storage) T(a_value);
        }

        Option(T&& a_value) : is_some(true) {
            new (storage) T(std::move(a_value));
        }

        ~Option() {
            reset();
        }

        static Option<T> None() {
            return Option<T>();
        }

        void reset() {
            if (is_some) {
                get_value_ptr()->~T();
                is_some = false;
            }
        }

        T expect(const std::string& msg) {
            if (!is_some) Panic(msg);
            return *get_value_ptr();
        }

        T unwrap() {
            if (!is_some) Panic("Tried unwrapping None.");
            T temp = std::move(*get_value_ptr());
            reset();
            return temp;
        }

        T unwrap_or(T default_value) {
            return is_some ? *get_value_ptr() : default_value;
        }

        bool is_some_func() const {
            return is_some;
        }

        bool is_none() const {
            return !is_some;
        }

        operator bool() const {
            return is_some;
        }
    };

    // Specialization for reference types
    template<typename T>
    class Option<T, true> {
    private:
        typename std::remove_reference<T>::type* value; // Pointer to simulate reference behavior

    public:
        Option() : value(nullptr) {} // None
        Option(T& ref) : value(&ref) {} // Some(T&)

        static Option<T> None() {
            return Option<T>();
        }

        T& expect(const std::string& msg) {
            if (!value) Panic(msg);
            return *value;
        }

        T& unwrap() {
            if (!value) Panic("Tried unwrapping None.");
            return *value;
        }

        T& unwrap_or(T& default_value) {
            return value ? *value : default_value;
        }

        bool is_some() const {
            return value != nullptr;
        }

        bool is_none() const {
            return value == nullptr;
        }

        operator bool() const {
            return value != nullptr;
        }
    };
};