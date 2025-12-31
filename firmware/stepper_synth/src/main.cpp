#include <Arduino.h>

#include <TMCStepper.h>
#include <CD74HC4067.h>
#include <FastLED.h>
#include <ESP32Encoder.h>

// --------------------- analog mux 2 hall sensors ---------------------

CD74HC4067 mux(14, 9, 10, 11); //mux control pins shared between 3 muxes

const int g_common_pin_1 = 6; //mux 1 out
const int g_common_pin_2 = 7; //mux 2 out
const int g_common_pin_3 = 8; //mux 3 out



// --------------------- neopixels ---------------------

#define NUM_LEDS  8
#define DATA_PIN 12
CRGB leds[NUM_LEDS];



// --------------------- buttons ---------------------

#define BTN_1 47
#define BTN_2 48
#define BTN_3  0 //rotary


// --------------------- rotary encoders ---------------------

#define ROTARY_A1  1
#define ROTARY_B1  3
#define ROTARY_A2 38
#define ROTARY_B2 39
#define ROTARY_A3 45
#define ROTARY_B3 46

ESP32Encoder encoder1;
ESP32Encoder encoder2;
ESP32Encoder encoder3;



// --------------------- tmc2209 nema 17 motors ---------------------

// ty <3 https://forum.arduino.cc/t/tmcstepper-arduino-tmc2209/956036

#define STEP_PIN_1     2
#define DIR_PIN_1     13
#define PD_UART_PIN_1  4

#define STEP_PIN_2     5
#define DIR_PIN_2     15
#define PD_UART_PIN_2 16

#define STEP_PIN_3    17
#define DIR_PIN_3     18
#define PD_UART_PIN_3 21

#define STEP_PIN_4    40
#define DIR_PIN_4     41
#define PD_UART_PIN_4 42

#define R_SENSE 0.11f
#define DRIVER_1_ADDRESS 0b00
#define DRIVER_2_ADDRESS 0b01
#define DRIVER_3_ADDRESS 0b10
#define DRIVER_4_ADDRESS 0b11

HardwareSerial serial_1(1);
HardwareSerial serial_2(2);
HardwareSerial serial_3(0);
HardwareSerial serial_4(1);

TMC2209Stepper driver_1(&serial_1, R_SENSE, DRIVER_1_ADDRESS);
TMC2209Stepper driver_2(&serial_2, R_SENSE, DRIVER_2_ADDRESS);
TMC2209Stepper driver_3(&serial_3, R_SENSE, DRIVER_3_ADDRESS);
TMC2209Stepper driver_4(&serial_4, R_SENSE, DRIVER_4_ADDRESS);

void setupDriver(TMC2209Stepper &driver) {
  driver.begin();
  driver.toff(5);
  driver.blank_time(24);

  driver.rms_current(800);
  driver.microsteps(16);
  driver.en_spreadCycle(false);
  driver.pwm_autoscale(true);

  driver.TCOOLTHRS(0xFFFFF);
  driver.SGTHRS(0xFF);
}

void setup() {

  Serial.begin(115200);

  // CD74HC4067 mux setup
  
  pinMode(g_common_pin_1, OUTPUT);
  pinMode(g_common_pin_2, OUTPUT);
  pinMode(g_common_pin_3, OUTPUT);

  // ws2812 neopixel setup

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  // button setup

  pinMode(BTN_1, INPUT_PULLUP);
  pinMode(BTN_2, INPUT_PULLUP);
  pinMode(BTN_3, INPUT_PULLUP);

  // encoder setup

  encoder1.attachSingleEdge(ROTARY_A1, ROTARY_B1);
  encoder1.setCount(0);

  encoder2.attachSingleEdge(ROTARY_A2, ROTARY_B2);
  encoder2.setCount(0);

  encoder3.attachSingleEdge(ROTARY_A3, ROTARY_B3);
  encoder3.setCount(0);

  // tmc2209 driver setup

  serial_1.begin(115200, SERIAL_8N1, -1, PD_UART_PIN_1);
  serial_2.begin(115200, SERIAL_8N1, -1, PD_UART_PIN_2);
  serial_3.begin(115200, SERIAL_8N1, -1, PD_UART_PIN_3);
  serial_4.begin(115200, SERIAL_8N1, -1, PD_UART_PIN_4);

  setupDriver(driver_1);
  setupDriver(driver_2);
  setupDriver(driver_3);
  setupDriver(driver_4);

  pinMode(STEP_PIN_1, OUTPUT);
  pinMode(DIR_PIN_1, OUTPUT);
  pinMode(STEP_PIN_2, OUTPUT);
  pinMode(DIR_PIN_2, OUTPUT);
  pinMode(STEP_PIN_3, OUTPUT);
  pinMode(DIR_PIN_3, OUTPUT);
  pinMode(STEP_PIN_4, OUTPUT);
  pinMode(DIR_PIN_4, OUTPUT);
}

void loop() {
  leds[0] = CRGB::Red;
  FastLED.show();
  
  digitalWrite(STEP_PIN_1, HIGH);

  delayMicroseconds(500);

  leds[0] = CRGB::Black;
  FastLED.show();

  digitalWrite(STEP_PIN_1, LOW);

  delayMicroseconds(500);
}