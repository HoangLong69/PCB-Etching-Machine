#include "Countimer.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0X27,16,2); //SCL A5 SDA A4
Countimer tdown;
#include <EEPROM.h>
#include <Wire.h> 

#define bt_set    A3
#define bt_up     A2
#define bt_down   A1
#define bt_start  A0

int time_s = 0;
int time_m = 0;
int time_h = 0;

int set = 0;
int flag1=0, flag2=0;

int relay = 8;
int buzzer = 6;

// Define LEDs
int leds[] = {7, 9, 10, 11, 12, 13};
int total_leds = 6;

void setup() {
  Serial.begin(9600);
  
  pinMode(bt_set,   INPUT_PULLUP);
  pinMode(bt_up,    INPUT_PULLUP);
  pinMode(bt_down,  INPUT_PULLUP);
  pinMode(bt_start, INPUT_PULLUP);
  
  pinMode(relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  // Set LED pins to output mode
  for (int i = 0; i < total_leds; i++) {
    pinMode(leds[i], OUTPUT);
  }
  
  lcd.init();                    
  lcd.backlight(); 
  lcd.setCursor(2,0);
  lcd.print("Hi Hoang Long");
  lcd.setCursor(0,1);
  lcd.print(" Countdown  Timer");
  tdown.setInterval(print_time, 999);
  eeprom_read();
  delay(1000);
  for (int i = 0; i < 2; i++) {
    digitalWrite(buzzer, HIGH);
    delay(60);
    digitalWrite(buzzer, LOW);
    delay(10);
    effect_9();
  }
  lcd.clear();
}

void print_time(){
  time_s = time_s-1;
  if(time_s<0){time_s=59; time_m = time_m-1;}
  if(time_m<0){time_m=59; time_h = time_h-1;}
}

void tdownComplete(){Serial.print("ok");}

//tdown.stop(); 

void loop(){
  tdown.run();
  if(digitalRead (bt_set) == 0){
    digitalWrite(leds[0], HIGH);
    digitalWrite(buzzer, HIGH);
    if(flag1==0 && flag2==0){
      flag1=1;
      set = set+1;
      if(set>3){set=0;}
      delay(100);
    }
  }else{flag1=0; digitalWrite(leds[0], LOW); digitalWrite(buzzer, LOW);}
  
  if(digitalRead (bt_up) == 0){
    if(set==0){tdown.start(); flag2=1;}
    if(set==1){time_s++;}
    if(set==2){time_m++;}
    if(set==3){time_h++;}
    if(time_s>59){time_s=0;}
    if(time_m>59){time_m=0;}
    if(time_h>99){time_h=0;}
    if(set>0){eeprom_write();}
    digitalWrite(leds[1], HIGH);
    digitalWrite(buzzer, HIGH);
    delay(200);
  }else{digitalWrite(leds[1], LOW); digitalWrite(buzzer, LOW);}
  
  if(digitalRead (bt_down) == 0){
    if(set==0){tdown.stop(); flag2=0;}
    if(set==1){time_s--;}
    if(set==2){time_m--;}
    if(set==3){time_h--;}
    if(time_s<0){time_s=59;}
    if(time_m<0){time_m=59;}
    if(time_h<0){time_h=99;}
    if(set>0){eeprom_write();}
    digitalWrite(leds[2], HIGH);
    digitalWrite(buzzer, HIGH);
    delay(200); 
  }else{digitalWrite(leds[2], LOW); digitalWrite(buzzer, LOW);}
  
  if(digitalRead (bt_start) == 0){ 
    digitalWrite(leds[3], HIGH);
    digitalWrite(buzzer, HIGH);
    flag2=1; 
    eeprom_read(); 
    digitalWrite(relay, HIGH); 
    tdown.restart(); 
    tdown.start();
  }else{digitalWrite(leds[3], LOW); digitalWrite(buzzer, LOW);}
  
  lcd.setCursor(0,0);
  if(set==0){lcd.print("      Timer     ");}
  if(set==1){lcd.print("  Set Timer SS  ");}
  if(set==2){lcd.print("  Set Timer MM  ");}
  if(set==3){lcd.print("  Set Timer HH  ");}
  
  lcd.setCursor(4,1);
  if(time_h<=9){lcd.print("0");}
  lcd.print(time_h);
  lcd.print(":");
  if(time_m<=9){lcd.print("0");}
  lcd.print(time_m);
  lcd.print(":");
  if(time_s<=9){lcd.print("0");}
  lcd.print(time_s);
  lcd.print("   ");
  
  if(time_s==0 && time_m==0 && time_h==0 && flag2==1){
    flag2=0;
    tdown.stop(); 
    digitalWrite(relay, LOW);
    for (int i = 0; i < 4; i++) {
      digitalWrite(buzzer, HIGH);
      delay(90);
      digitalWrite(buzzer, LOW);
      delay(50);
      effect_5();
    }
  }
  
  if(flag2==1){digitalWrite(relay, HIGH);}
  else{digitalWrite(relay, LOW);}
  
  delay(1);
}

void eeprom_write(){
  EEPROM.write(1, time_s);  
  EEPROM.write(2, time_m);  
  EEPROM.write(3, time_h);  
}

void eeprom_read(){
  time_s =  EEPROM.read(1);
  time_m =  EEPROM.read(2);
  time_h =  EEPROM.read(3);
}

//TURN ON ALL LEDs
void ledsOn() {
  for(int i=0; i<total_leds; i++) {
    digitalWrite(leds[i], HIGH);
  }
}

//TURN OFF ALL LEDs
void ledsOff() {
  for(int i=0; i<total_leds; i++) {
    digitalWrite(leds[i], LOW);
  }
}

void effect_5() {
  for(int i=(total_leds/2)-1; i>=0; i--){
    digitalWrite(leds[i], HIGH);
    digitalWrite(leds[total_leds -1 -i], HIGH);
    delay(200);
    digitalWrite(leds[i], LOW);
    digitalWrite(leds[total_leds -1 -i], LOW);
  }
}

void effect_9() {
  int i=0;
  for(i=0; i<=total_leds/2 ; i++){
    digitalWrite(leds[i],HIGH);
    digitalWrite(leds[total_leds-i],HIGH);
    delay(80);
  }
  
  for(i=0; i<=total_leds/2 ; i++){
    digitalWrite(leds[i],LOW);
    digitalWrite(leds[total_leds-i],LOW);
    delay(80);
  }
  
  delay(80);
  
  for(i=0; i<=total_leds/2 ; i++){
    digitalWrite(leds[i],HIGH);
    digitalWrite(leds[total_leds-i],HIGH);
    delay(80);
  }
  for(i=total_leds/2; i>=0 ; i--){
    digitalWrite(leds[i],LOW);
    digitalWrite(leds[total_leds-i],LOW);
    delay(80);
  }
}
