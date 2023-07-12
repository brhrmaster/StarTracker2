#include "src/config.h"
#include "src/control/control.h"
#include "src/core/motor_controller.h"
#include "src/core/clock.h"
#include "src/core/rtc_ds3231.h"

RtcDS3231 ds3231;
Clock& persistent_clock = ds3231;

MountController mount(MotorController::instance());

Control control(mount, persistent_clock);

void setup() {

  Serial.begin(SERIAL_BAUD_RATE);
  delay(10);
  control.initialize();
  delay(100);

}

void loop() {

  control.update();
  delay(10);

}
