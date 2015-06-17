#include <OneWire.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include <SPI.h>
//#include "nRF24L01.h"
//#include "RF24.h"

//  DS18B20 PIN
#define  DS_PIN  5
#define  DS_DELAY  1000

// I2C Display configuration
#define  I2C_DISPLAY_ADDRESS  0x27
#define  DISPLAY_CHARS  16
#define  DISPLAY_LINE  2  

// HR-SCR02 configuration
#define  TRIG  4
#define  ECHO  2

// NRF24L01 radio module
#define  CE  9
#define  CSN  10

// DEV ID
#define  DEVID  2

// command define
#define  SEND  255
#define  COMMIT  0
#define  SET  1

#define  DEBUG true

// Sensors ids
//  DISTANCE
#define  DISTANCE  10

LiquidCrystal_I2C  lcd(I2C_DISPLAY_ADDRESS, DISPLAY_CHARS, DISPLAY_LINE);
OneWire ds(DS_PIN);

volatile  unsigned int  _distance  =  0;
volatile  unsigned long  echo_time;
unsigned int  distance = 0;
unsigned int  previouse_millis = 0;
bool  flag_update = false;
bool  flag_search_ds = true;
volatile bool  flag_update_distance = false;


void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("DISTANCE    0 cm");
  
  if(flag_search_ds){
    flag_search_ds = false;
  }
  
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(TRIG, LOW);
  previouse_millis = millis();
  attachInterrupt(0, check_echo, CHANGE);
  interrupts();
}

void check_echo(void){
  int echo_state = digitalRead(ECHO);
  if(echo_state == HIGH){
    echo_time = micros();
  }
  else{
    echo_time = micros() - echo_time;
    flag_update_distance = true;
  }
}

void loop() {
  unsigned int current_millis = millis();
  if(current_millis - previouse_millis > 500){
    digitalWrite(TRIG,HIGH);
    delay(10);
    digitalWrite(TRIG,LOW);
    previouse_millis = current_millis;
  }
  
  if(flag_update){
    lcd.setCursor(9,0);
    if(distance>350){
      lcd.print("  NaN");
    }
    else{
      if(distance >= 100){
        lcd.print(" ");
      }
      else{
        if(distance >= 10){
          lcd.print("  ");
        }
        else{
          lcd.print("   ");
        }  
      }
      lcd.print(distance);
    }

    flag_update = false;
  }
  if(flag_update_distance){
    _distance = echo_time/58;
    if(distance != _distance){
      flag_update = true;
      distance = _distance;
    }
    flag_update_distance = false;
  }
}
