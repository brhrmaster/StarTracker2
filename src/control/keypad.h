#ifndef KEYPAD_H
#define KEYPAD_H

#include <IRremote.h>

#define KP_KEY_CONTEXT       0x807F32CD
#define KP_KEY_MUTE          0x807F827D
#define KP_KEY_SETUP         0x807FC23D
#define KP_KEY_BACKSPACE     0x807F42BD  
#define KP_KEY_BACK          0x807F9867
#define KP_KEY_1             0x807F728D 
#define KP_KEY_2             0x807FB04F
#define KP_KEY_3             0x807F30CF
#define KP_KEY_4             0x807F52AD  
#define KP_KEY_5             0x807F906F 
#define KP_KEY_6             0x807F10EF  
#define KP_KEY_7             0x807F629D  
#define KP_KEY_8             0x807FA05F 
#define KP_KEY_9             0x807F20DF 
#define KP_KEY_0             0x807F807F
#define KP_KEY_HOME          0x807F8877 
#define KP_KEY_UP_ARROW      0x807F6897  
#define KP_KEY_DOWN_ARROW    0x807F58A7 
#define KP_KEY_RIGHT_ARROW   0x807F0AF5
#define KP_KEY_LEFT_ARROW    0x807F8A75
#define KP_KEY_OK            0x807FC837
#define KP_KEY_VOL_PLUS      0x807F18E7

#define KP_UPDATE_MS         200     

class Keypad {

    public:

        Keypad() : _recv(IRrecv(KEYPAD_IR_PIN)) {}

        void initialize() {
            _recv.enableIRIn();
            _recv.blink13(true);
        }

        // returns true if the given key was pressed for longer than SHORT_HOLD_TIME_MS
        inline boolean pushed(uint32_t key_code) { return pressed_internal(key_code, SHORT_HOLD_TIME_MS); }

        // returns true if the given key was pressed for longer than LONG_HOLD_TIME_MS
        inline boolean pressed(uint32_t key_code) { return pressed_internal(key_code, LONG_HOLD_TIME_MS); }

        void update() {

            _hold_time = 0;

            if (_last_used_key == 0 && (millis() - _last_update) < KP_UPDATE_MS) return;
            _last_update = millis();

            uint32_t new_key = get_key();
          
            if (new_key == _used_key) return;

            #ifdef DEBUG_KEYS
                Serial.print(F("Keypad change: 0x"));
                Serial.print(_used_key, HEX);
                Serial.print(F("-->"));
                Serial.println(new_key, HEX);
                Serial.println(getButtonName(new_key));
            #endif

            _last_used_key = _used_key;
            _used_key = new_key;
            _hold_time = millis() - _press_time;
            _press_time = millis();
        }

    private:

        inline bool pressed_internal(uint32_t key_code, unsigned long duration) {
            if (key_code != _last_used_key) return false; 
            if (_hold_time >= duration) {
                _hold_time = 0;
                return true;
            } else return false;
        }

        uint32_t get_key() {

            decode_results results;
            if (!_recv.decode(&results)) return 0;

            _recv.resume();

            return (_recv.results.value == 0xFFFFFFFF ? _used_key : _recv.results.value) ;
        }

        IRrecv _recv;

        uint32_t _used_key = 0;         // currently pressed key
        uint32_t _last_used_key = 0;    // key pressed before previous key change

        unsigned long _hold_time = 0;   // millis since last key change
        unsigned long _press_time = 0;  // millis of last key change

        unsigned long _last_update = 0; // millis since last key check

        char* getButtonName(uint32_t new_key) {
            switch(new_key) {
                case KP_KEY_SETUP: return "SETUP";
                case KP_KEY_HOME: return "HOME";
                case KP_KEY_BACK: return "BACK";
                case KP_KEY_UP_ARROW: return "UP";
                case KP_KEY_DOWN_ARROW: return "DOWN";
                case KP_KEY_LEFT_ARROW: return "LEFT";
                case KP_KEY_RIGHT_ARROW: return "RIGHT";
                case KP_KEY_OK: return "OK";
                case KP_KEY_VOL_PLUS: return "Vol+";
                case KP_KEY_CONTEXT: return "Context Menu";
                case KP_KEY_1: return "1";
                case KP_KEY_2: return "2";
                case KP_KEY_3: return "3";
                case KP_KEY_4: return "4";
                case KP_KEY_5: return "5";
                case KP_KEY_6: return "6";
                case KP_KEY_7: return "7";
                case KP_KEY_8: return "8";
                case KP_KEY_9: return "9";
                case KP_KEY_0: return "0";
                case KP_KEY_MUTE: return "MUTE";
                case KP_KEY_BACKSPACE: return "Backspace";
                default: return "";
            }
        }
};

#endif