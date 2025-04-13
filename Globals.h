#pragma once

#include <unordered_map>
#include "template.h"

#define FPSCOUNT

class Globals {
private:


public:
	static float DeltaTime;
	static int screen_width;
	static int screen_height;
    static float gravity;
    static float jump_inpuls;
    static float move_acceleration;
    static float air_control;
    static float max_move_speed;
    static float Time;

    static void ExitApp();
	
};

namespace Util {
    enum Key;
    enum class MouseButton;

    class Input {
    private:
        enum Status {
            KeyUp,
            KeyDown,
            KeyHeld
        };

        static std::unordered_map<Key, Status> keyStatus;
        static std::unordered_map<MouseButton, Status> mouseStatus;

    public:
        static Tmpl8::vec2int mousePos();
        static bool GetMouseHeld(MouseButton key);
        static bool GetMouseDown(MouseButton key);
        static bool GetMouseUp(MouseButton key);
        static bool GetMouse(MouseButton key);

        static void MouseDownEvent(MouseButton key);
        static void MouseUpEvent(MouseButton key);
   
        static bool GetKeyHeld(Key key);
        static bool GetKeyDown(Key key);
        static bool GetKeyUp(Key key);
        static bool GetKey(Key key);

        static void TickList();
        static void KeyDownEvent(Key key);
        static void KeyUpEvent(Key key);
    };

    enum class MouseButton {
		Left = 1,
		Middle = 2,
		Right = 3,
		Back = 4,
		Forwards = 5
	};

    enum Key {
        Unknown = 0,
        A = 4,
        B = 5,
        C = 6,
        D = 7,
        E = 8,
        F = 9,
        G = 10,
        H = 11,
        I = 12,
        J = 13,
        K = 14,
        L = 15,
        M = 16,
        N = 17,
        O = 18,
        P = 19,
        Q = 20,
        R = 21,
        S = 22,
        T = 23,
        U = 24,
        V = 25,
        W = 26,
        X = 27,
        Y = 28,
        Z = 29,
        NUM1 = 30,
        NUM2 = 31,
        NUM3 = 32,
        NUM4 = 33,
        NUM5 = 34,
        NUM6 = 35,
        NUM7 = 36,
        NUM8 = 37,
        NUM9 = 38,
        NUM0 = 39,
        Enter = 40,
        Escape = 41,
        Backspace = 42,
        Tab = 43,
        Space = 44,
        Minus = 45,
        Equal = 46,
        OpenBracket = 47,
        CloseBracket = 48,
        Backslash = 49,
        // NonUSHash = 50, ??
        Semicolon = 51,
        Quote = 52,
        Backtick = 53,
        Comma = 54,
        Period = 55,
        Slash = 56,
        CapsLock = 57,
        F1 = 58,
        F2 = 59,
        F3 = 60,
        F4 = 61,
        F5 = 62,
        F6 = 63,
        F7 = 64,
        F8 = 65,
        F9 = 66,
        F10 = 67,
        F11 = 68,
        F12 = 69,
        PrintScreen = 70, // I guess
        ScrollLock = 71,
        Pause = 72,
        Insert = 73,
        Home = 74,
        PageUp = 75,
        Delete = 76,
        End = 77,
        PageDown = 78,
        Right = 79,
        Left = 80,
        Down = 81,
        Up = 82,
        NumLockClear = 83,
        KeypadDivide = 84,
        KeypadMultiply = 85,
        KeypadMinus = 86,
        KeypadPlus = 87,
        KeypadEnter = 88,
        Keypad1 = 89,
        Keypad2 = 90,
        Keypad3 = 91,
        Keypad4 = 92,
        Keypad5 = 93,
        Keypad6 = 94,
        Keypad7 = 95,
        Keypad8 = 96,
        Keypad9 = 97,
        Keypad0 = 98,
        KeypadPeriod = 99,
        // NonUSBackslash = 100, IDK what this is
        Application = 101,
        // Power = 102,
        KeypadEqual = 103,
        F13 = 104,
        F14 = 105,
        F15 = 106,
        F16 = 107,
        F17 = 108,
        F18 = 109,
        F19 = 110,
        F20 = 111,
        F21 = 112,
        F22 = 113,
        F23 = 114,
        F24 = 115,
        //  Execute = 116,
        //  Help = 117,
        Menu = 118,
        // Select = 119,
        // Stop = 120,
        // Again = 121,
        // Undo = 122,
        // Cut = 123,
        // Copy = 124,
        // Paste = 125,
        // Find = 126,
         //Mute = 127,
       //  VolumeUp = 128,
        // VolumeDown = 129,
        CTRL = 224,
        SHIFT = 225,
        ALT = 226,
        //GUI = 227
    };
};