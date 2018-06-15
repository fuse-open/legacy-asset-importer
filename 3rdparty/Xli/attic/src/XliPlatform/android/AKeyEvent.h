/* * Copyright (C) 2010-2014 Outracks Technologies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __XLI_KEYEVENT_ANDROID_H__
#define __XLI_KEYEVENT_ANDROID_H__

#include <XliPlatform/Window.h>
#include <XliPlatform/InputEventHandler.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        enum AKeyEvent
        {
            ACTION_DOWN = 0,
            ACTION_MULTIPLE = 2,
            ACTION_UP = 1,
            FLAG_CANCELED = 32,
            FLAG_CANCELED_LONG_PRESS = 256,
            FLAG_EDITOR_ACTION = 16,
            FLAG_FALLBACK = 1024,
            FLAG_FROM_SYSTEM = 8,
            FLAG_KEEP_TOUCH_MODE = 4,
            FLAG_LONG_PRESS = 128,
            FLAG_SOFT_KEYBOARD = 2,
            FLAG_TRACKING = 512,
            FLAG_VIRTUAL_HARD_KEY = 64,
            FLAG_WOKE_HERE = 1,
            KEYCODE_0 = 7,
            KEYCODE_1 = 8,
            KEYCODE_2 = 9,
            KEYCODE_3 = 10,
            KEYCODE_3D_MODE = 206,
            KEYCODE_4 = 11,
            KEYCODE_5 = 12,
            KEYCODE_6 = 13,
            KEYCODE_7 = 14,
            KEYCODE_8 = 15,
            KEYCODE_9 = 16,
            KEYCODE_A = 29,
            KEYCODE_ALT_LEFT = 57,
            KEYCODE_ALT_RIGHT = 58,
            KEYCODE_APOSTROPHE = 75,
            KEYCODE_APP_SWITCH = 187,
            KEYCODE_ASSIST = 219,
            KEYCODE_AT = 77,
            KEYCODE_AVR_INPUT = 182,
            KEYCODE_AVR_POWER = 181,
            KEYCODE_B = 30,
            KEYCODE_BACK = 4,
            KEYCODE_BACKSLASH = 73,
            KEYCODE_BOOKMARK = 174,
            KEYCODE_BREAK = 121,
            KEYCODE_BRIGHTNESS_DOWN = 220,
            KEYCODE_BRIGHTNESS_UP = 221,
            KEYCODE_BUTTON_1 = 188,
            KEYCODE_BUTTON_10 = 197,
            KEYCODE_BUTTON_11 = 198,
            KEYCODE_BUTTON_12 = 199,
            KEYCODE_BUTTON_13 = 200,
            KEYCODE_BUTTON_14 = 201,
            KEYCODE_BUTTON_15 = 202,
            KEYCODE_BUTTON_16 = 203,
            KEYCODE_BUTTON_2 = 189,
            KEYCODE_BUTTON_3 = 190,
            KEYCODE_BUTTON_4 = 191,
            KEYCODE_BUTTON_5 = 192,
            KEYCODE_BUTTON_6 = 193,
            KEYCODE_BUTTON_7 = 194,
            KEYCODE_BUTTON_8 = 195,
            KEYCODE_BUTTON_9 = 196,
            KEYCODE_BUTTON_A = 96,
            KEYCODE_BUTTON_B = 97,
            KEYCODE_BUTTON_C = 98,
            KEYCODE_BUTTON_L1 = 102,
            KEYCODE_BUTTON_L2 = 104,
            KEYCODE_BUTTON_MODE = 110,
            KEYCODE_BUTTON_R1 = 103,
            KEYCODE_BUTTON_R2 = 105,
            KEYCODE_BUTTON_SELECT = 109,
            KEYCODE_BUTTON_START = 108,
            KEYCODE_BUTTON_THUMBL = 106,
            KEYCODE_BUTTON_THUMBR = 107,
            KEYCODE_BUTTON_X = 99,
            KEYCODE_BUTTON_Y = 100,
            KEYCODE_BUTTON_Z = 101,
            KEYCODE_C = 31,
            KEYCODE_CALCULATOR = 210,
            KEYCODE_CALENDAR = 208,
            KEYCODE_CALL = 5,
            KEYCODE_CAMERA = 27,
            KEYCODE_CAPS_LOCK = 115,
            KEYCODE_CAPTIONS = 175,
            KEYCODE_CHANNEL_DOWN = 167,
            KEYCODE_CHANNEL_UP = 166,
            KEYCODE_CLEAR = 28,
            KEYCODE_COMMA = 55,
            KEYCODE_CONTACTS = 207,
            KEYCODE_CTRL_LEFT = 113,
            KEYCODE_CTRL_RIGHT = 114,
            KEYCODE_D = 32,
            KEYCODE_DEL = 67,
            KEYCODE_DPAD_CENTER = 23,
            KEYCODE_DPAD_DOWN = 20,
            KEYCODE_DPAD_LEFT = 21,
            KEYCODE_DPAD_RIGHT = 22,
            KEYCODE_DPAD_UP = 19,
            KEYCODE_DVR = 173,
            KEYCODE_E = 33,
            KEYCODE_EISU = 212,
            KEYCODE_ENDCALL = 6,
            KEYCODE_ENTER = 66,
            KEYCODE_ENVELOPE = 65,
            KEYCODE_EQUALS = 70,
            KEYCODE_ESCAPE = 111,
            KEYCODE_EXPLORER = 64,
            KEYCODE_F = 34,
            KEYCODE_F1 = 131,
            KEYCODE_F10 = 140,
            KEYCODE_F11 = 141,
            KEYCODE_F12 = 142,
            KEYCODE_F2 = 132,
            KEYCODE_F3 = 133,
            KEYCODE_F4 = 134,
            KEYCODE_F5 = 135,
            KEYCODE_F6 = 136,
            KEYCODE_F7 = 137,
            KEYCODE_F8 = 138,
            KEYCODE_F9 = 139,
            KEYCODE_FOCUS = 80,
            KEYCODE_FORWARD = 125,
            KEYCODE_FORWARD_DEL = 112,
            KEYCODE_FUNCTION = 119,
            KEYCODE_G = 35,
            KEYCODE_GRAVE = 68,
            KEYCODE_GUIDE = 172,
            KEYCODE_H = 36,
            KEYCODE_HEADSETHOOK = 79,
            KEYCODE_HENKAN = 214,
            KEYCODE_HOME = 3,
            KEYCODE_I = 37,
            KEYCODE_INFO = 165,
            KEYCODE_INSERT = 124,
            KEYCODE_J = 38,
            KEYCODE_K = 39,
            KEYCODE_KANA = 218,
            KEYCODE_KATAKANA_HIRAGANA = 215,
            KEYCODE_L = 40,
            KEYCODE_LANGUAGE_SWITCH = 204,
            KEYCODE_LEFT_BRACKET = 71,
            KEYCODE_M = 41,
            KEYCODE_MANNER_MODE = 205,
            KEYCODE_MEDIA_CLOSE = 128,
            KEYCODE_MEDIA_EJECT = 129,
            KEYCODE_MEDIA_FAST_FORWARD = 90,
            KEYCODE_MEDIA_NEXT = 87,
            KEYCODE_MEDIA_PAUSE = 127,
            KEYCODE_MEDIA_PLAY = 126,
            KEYCODE_MEDIA_PLAY_PAUSE = 85,
            KEYCODE_MEDIA_PREVIOUS = 88,
            KEYCODE_MEDIA_RECORD = 130,
            KEYCODE_MEDIA_REWIND = 89,
            KEYCODE_MEDIA_STOP = 86,
            KEYCODE_MENU = 82,
            KEYCODE_META_LEFT = 117,
            KEYCODE_META_RIGHT = 118,
            KEYCODE_MINUS = 69,
            KEYCODE_MOVE_END = 123,
            KEYCODE_MOVE_HOME = 122,
            KEYCODE_MUHENKAN = 213,
            KEYCODE_MUSIC = 209,
            KEYCODE_MUTE = 91,
            KEYCODE_N = 42,
            KEYCODE_NOTIFICATION = 83,
            KEYCODE_NUM = 78,
            KEYCODE_NUMPAD_0 = 144,
            KEYCODE_NUMPAD_1 = 145,
            KEYCODE_NUMPAD_2 = 146,
            KEYCODE_NUMPAD_3 = 147,
            KEYCODE_NUMPAD_4 = 148,
            KEYCODE_NUMPAD_5 = 149,
            KEYCODE_NUMPAD_6 = 150,
            KEYCODE_NUMPAD_7 = 151,
            KEYCODE_NUMPAD_8 = 152,
            KEYCODE_NUMPAD_9 = 153,
            KEYCODE_NUMPAD_ADD = 157,
            KEYCODE_NUMPAD_COMMA = 159,
            KEYCODE_NUMPAD_DIVIDE = 154,
            KEYCODE_NUMPAD_DOT = 158,
            KEYCODE_NUMPAD_ENTER = 160,
            KEYCODE_NUMPAD_EQUALS = 161,
            KEYCODE_NUMPAD_LEFT_PAREN = 162,
            KEYCODE_NUMPAD_MULTIPLY = 155,
            KEYCODE_NUMPAD_RIGHT_PAREN = 163,
            KEYCODE_NUMPAD_SUBTRACT = 156,
            KEYCODE_NUM_LOCK = 143,
            KEYCODE_O = 43,
            KEYCODE_P = 44,
            KEYCODE_PAGE_DOWN = 93,
            KEYCODE_PAGE_UP = 92,
            KEYCODE_PERIOD = 56,
            KEYCODE_PICTSYMBOLS = 94,
            KEYCODE_PLUS = 81,
            KEYCODE_POUND = 18,
            KEYCODE_POWER = 26,
            KEYCODE_PROG_BLUE = 186,
            KEYCODE_PROG_GREEN = 184,
            KEYCODE_PROG_RED = 183,
            KEYCODE_PROG_YELLOW = 185,
            KEYCODE_Q = 45,
            KEYCODE_R = 46,
            KEYCODE_RIGHT_BRACKET = 72,
            KEYCODE_RO = 217,
            KEYCODE_S = 47,
            KEYCODE_SCROLL_LOCK = 116,
            KEYCODE_SEARCH = 84,
            KEYCODE_SEMICOLON = 74,
            KEYCODE_SETTINGS = 176,
            KEYCODE_SHIFT_LEFT = 59,
            KEYCODE_SHIFT_RIGHT = 60,
            KEYCODE_SLASH = 76,
            KEYCODE_SOFT_LEFT = 1,
            KEYCODE_SOFT_RIGHT = 2,
            KEYCODE_SPACE = 62,
            KEYCODE_STAR = 17,
            KEYCODE_STB_INPUT = 180,
            KEYCODE_STB_POWER = 179,
            KEYCODE_SWITCH_CHARSET = 95,
            KEYCODE_SYM = 63,
            KEYCODE_SYSRQ = 120,
            KEYCODE_T = 48,
            KEYCODE_TAB = 61,
            KEYCODE_TV = 170,
            KEYCODE_TV_INPUT = 178,
            KEYCODE_TV_POWER = 177,
            KEYCODE_U = 49,
            KEYCODE_UNKNOWN = 0,
            KEYCODE_V = 50,
            KEYCODE_VOLUME_DOWN = 25,
            KEYCODE_VOLUME_MUTE = 164,
            KEYCODE_VOLUME_UP = 24,
            KEYCODE_W = 51,
            KEYCODE_WINDOW = 171,
            KEYCODE_X = 52,
            KEYCODE_Y = 53,
            KEYCODE_YEN = 216,
            KEYCODE_Z = 54,
            KEYCODE_ZENKAKU_HANKAKU = 211,
            KEYCODE_ZOOM_IN = 168,
            KEYCODE_ZOOM_OUT = 169,
            MAX_KEYCODE = 84,
            META_ALT_LEFT_ON = 16,
            META_ALT_MASK = 50,
            META_ALT_ON = 2,
            META_ALT_RIGHT_ON = 32,
            META_CAPS_LOCK_ON = 1048576,
            META_CTRL_LEFT_ON = 8192,
            META_CTRL_MASK = 28672,
            META_CTRL_ON = 4096,
            META_CTRL_RIGHT_ON = 16384,
            META_FUNCTION_ON = 8,
            META_META_LEFT_ON = 131072,
            META_META_MASK = 458752,
            META_META_ON = 65536,
            META_META_RIGHT_ON = 262144,
            META_NUM_LOCK_ON = 2097152,
            META_SCROLL_LOCK_ON = 4194304,
            META_SHIFT_LEFT_ON = 64,
            META_SHIFT_MASK = 193,
            META_SHIFT_ON = 1,
            META_SHIFT_RIGHT_ON = 128,
            META_SYM_ON = 4
        };

        /**
           \ingroup XliCorePlatform
        */

        inline Xli::Key AndroidToXliKeyEvent(AKeyEvent androidKeyCode)
        {
            switch (androidKeyCode)
            {
            case KEYCODE_0:
                return Key0;
            case KEYCODE_1:
                return Key1;
            case KEYCODE_2:
                return Key2;
            case KEYCODE_3:
                return Key3;
            case KEYCODE_4:
                return Key4;
            case KEYCODE_5:
                return Key5;
            case KEYCODE_6:
                return Key6;
            case KEYCODE_7:
                return Key7;
            case KEYCODE_8:
                return Key8;
            case KEYCODE_9:
                return Key9;
            case KEYCODE_A:
                return KeyA;
            case KEYCODE_ALT_LEFT:
                return KeyAlt;
            case KEYCODE_B:
                return KeyB;
            case KEYCODE_BACK:
                return KeyUnknown;
            case KEYCODE_BREAK:
                return KeyBreak;
            case KEYCODE_C:
                return KeyC;
            case KEYCODE_CAPS_LOCK:
                return KeyCapsLock;
            case KEYCODE_CTRL_LEFT:
                return KeyCtrl;
            case KEYCODE_D:
                return KeyD;
            case KEYCODE_DEL:
                return KeyBackspace;
            case KEYCODE_DPAD_DOWN:
                return KeyDown;
            case KEYCODE_DPAD_LEFT:
                return KeyLeft;
            case KEYCODE_DPAD_RIGHT:
                return KeyRight;
            case KEYCODE_DPAD_UP:
                return KeyUp;
            case KEYCODE_E:
                return KeyE;
            case KEYCODE_ENDCALL:
                return KeyEnd;
            case KEYCODE_ENTER:
                return KeyEnter;
            case KEYCODE_ESCAPE:
                return KeyEscape;
            case KEYCODE_F:
                return KeyF;
            case KEYCODE_F1:
                return KeyF1;
            case KEYCODE_F10:
                return KeyF10;
            case KEYCODE_F11:
                return KeyF11;
            case KEYCODE_F12:
                return KeyF12;
            case KEYCODE_F2:
                return KeyF2;
            case KEYCODE_F3:
                return KeyF3;
            case KEYCODE_F4:
                return KeyF4;
            case KEYCODE_F5:
                return KeyF5;
            case KEYCODE_F6:
                return KeyF6;
            case KEYCODE_F7:
                return KeyF7;
            case KEYCODE_F8:
                return KeyF8;
            case KEYCODE_F9:
                return KeyF9;
            case KEYCODE_G:
                return KeyG;
            case KEYCODE_H:
                return KeyH;
            case KEYCODE_HOME:
                return KeyHome;
            case KEYCODE_I:
                return KeyI;
            case KEYCODE_INSERT:
                return KeyInsert;
            case KEYCODE_J:
                return KeyJ;
            case KEYCODE_K:
                return KeyK;
            case KEYCODE_L:
                return KeyL;
            case KEYCODE_M:
                return KeyM;
            case KEYCODE_MENU:
                return KeyMenu;
            case KEYCODE_N:
                return KeyN;
            case KEYCODE_NUMPAD_0:
                return KeyNumPad0;
            case KEYCODE_NUMPAD_1:
                return KeyNumPad1;
            case KEYCODE_NUMPAD_2:
                return KeyNumPad2;
            case KEYCODE_NUMPAD_3:
                return KeyNumPad3;
            case KEYCODE_NUMPAD_4:
                return KeyNumPad4;
            case KEYCODE_NUMPAD_5:
                return KeyNumPad5;
            case KEYCODE_NUMPAD_6:
                return KeyNumPad6;
            case KEYCODE_NUMPAD_7:
                return KeyNumPad7;
            case KEYCODE_NUMPAD_8:
                return KeyNumPad8;
            case KEYCODE_NUMPAD_9:
                return KeyNumPad9;
            case KEYCODE_O:
                return KeyO;
            case KEYCODE_P:
                return KeyP;
            case KEYCODE_PAGE_DOWN:
                return KeyPageDown;
            case KEYCODE_PAGE_UP:
                return KeyPageUp;
            case KEYCODE_Q:
                return KeyQ;
            case KEYCODE_R:
                return KeyR;
            case KEYCODE_S:
                return KeyS;
            case KEYCODE_SHIFT_LEFT:
                return KeyShift;
            case KEYCODE_SPACE:
                return KeySpace;
            case KEYCODE_T:
                return KeyT;
            case KEYCODE_TAB:
                return KeyTab;
            case KEYCODE_U:
                return KeyU;
            case KEYCODE_UNKNOWN:
                return KeyUnknown;
            case KEYCODE_V:
                return KeyV;
            case KEYCODE_W:
                return KeyW;
            case KEYCODE_X:
                return KeyX;
            case KEYCODE_Y:
                return KeyY;
            case KEYCODE_Z:
                return KeyZ;
            };
            return KeyUnknown;
        }
    };
};

#endif
