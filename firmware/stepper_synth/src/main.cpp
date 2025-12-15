#include <Arduino.h>

#include <TMCStepper.h>
#include <CD74HC4067.h>

CD74HC4067 mux(14, 9, 10, 11); //mux control pins shared between 3 muxes

const int g_common_pin_1 = 6; //mux 1 out
const int g_common_pin_2 = 7; //mux 2 out
const int g_common_pin_3 = 8; //mux 3 out

// ty <3 https://forum.arduino.cc/t/tmcstepper-arduino-tmc2209/956036

#define STEP_PIN_1 2
#define DIR_PIN_1 13
#define UART_PIN_1 4

#define R_SENSE 0.11f
#define DRIVER_ADDRESS 0b00

HardwareSerial serial_1(1);
TMC2209Stepper driver_1(&serial_1, R_SENSE, DRIVER_ADDRESS);

void setup() {

  Serial.begin(115200);

  // CD74HC4067 mux setup
  
  pinMode(g_common_pin_1, OUTPUT);
  pinMode(g_common_pin_2, OUTPUT);
  pinMode(g_common_pin_3, OUTPUT);

  // tmc2209 driver setup 1

  serial_1.begin(
    115200,
    SERIAL_8N1,
    UART_PIN_1,
    UART_PIN_1
  );

  driver_1.begin();
  driver_1.toff(5);
  driver_1.blank_time(24);

  driver_1.rms_current(800);
  driver_1.microsteps(16);
  driver_1.en_spreadCycle(false);
  driver_1.pwm_autoscale(true);

  driver_1.TCOOLTHRS(0xFFFFF);


  // tmc2209 driver setup 2


  // tmc2209 driver setup 3


  // tmc2209 driver setup 4


}

void loop() {
  
}
