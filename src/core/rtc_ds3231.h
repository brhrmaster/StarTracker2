#ifndef RTC_DS3231_H
#define RTC_DS3231_H

#ifndef RTCLIB_H
#include <RTClib.h>
#define RTCLIB_H
#endif

#include <Time.h>

#include "../config.h"
#include "clock.h"

class RtcDS3231 : public Clock {

    private: 

        inline void set_time(const DateTime& dt) {
            Clock::_time.adjust(dt);
            Clock::_local_siderial_time_offset = Clock::compute_LST_offset();
        }

        RTC_DS3231 _rtc;
    
    public:

        void obtain_time() override { 

            if (!_rtc.begin()) {
                #ifdef DEBUG_TIME
                      Serial.println("Cannot find DS3231");
                #endif
                while(true);
            }

            if (_rtc.lostPower()) {
                Serial.println("RTC lost power, lets set the time!");
                _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
            }

            set_time(_rtc.now());
        }

        void sync(const DateTime& dt) override { 
              _rtc.adjust(dt);
              set_time(dt);
        };

};

#endif