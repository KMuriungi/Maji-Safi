
//Turbidity = 100 - (Vout/Vclear)*100

#include <EEPROM.h> // to store last calibration value (blank, Vclear)
#include <Wire.h>  // for LCD display (with I2S interphase)
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

int sensor = 0; // variable for averaging
int n = 10; // number of samples to average 
int sensorValue = 0;
float voltage = 0.00;
float turbidity = 0.00;
float Vclear = 2.85; // Output voltage to calibrate (with clear water).

int readingButton = 6;  

int resetButton = 7;  
 
void setup()
{
  Serial.begin(9600);
  pinMode(resetButton, INPUT_PULLUP); //initializes digital pin 7 as an input with pull-up resistance.
  pinMode(readingButton, INPUT);
 
 // LCD display
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Turbidity");
  lcd.setCursor(1,3);
  lcd.print("Meter");

//Serial display
  Serial.println("Hi... welcome to your turbidity sensor");
  
  EEPROM.get(0, Vclear);  // recovers the last Vclear calibration value stored in ROM.
  delay(5000);  // Pause for 3 sec
}

void loop()
{
readingButton = digitalRead(6);  // push button status

if (readingButton == HIGH) {
// If the push button is not pushed, do the normal sensing routine:
      for (int i=0; i < n; i++){
      sensor += analogRead(A0); // read the input on analog pin 0 (turbidity sensor analog output)      
  delay(10);
      }       
  sensorValue = sensor / n;    // average the n values  
  voltage = sensorValue * (5.000 / 1023.000); // Convert analog (0-1023) to voltage (0 - 5V)
  
  turbidity = 100.00 - (voltage / Vclear) * 100.00; // as relative percentage; 0% = clear water; 
  
  EEPROM.put(0, Vclear); // guarda el voltaje de calibración actualmente en uso.
          
          // Serial display
          Serial.print(sensorValue);
          Serial.print(", ");
          Serial.print(voltage,3);
          Serial.print(", ");
          Serial.print(turbidity,3);
          Serial.print(", ");
          Serial.println(Vclear,3);
   
  // Display LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Volts = ");
  lcd.print(voltage,2);
  lcd.setCursor(0,1);
  lcd.print("Turbidity = ");
  lcd.print(turbidity,2);
  //lcd.print("%");

  delay(6000);

  sensor = 0;   // resets for averaging
    
  } else {

// Calibration routine, when push button is pushed:
  
  Serial.println("Put the sensor in clear water to calibrate...");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibrating...");
  delay(2000); 
  
      for (int i=0; i < n; i++){
      sensor += analogRead(A1); // read the input on analog pin 1:
      delay(10);
      }
  sensorValue = sensor / n;
  Vclear = sensorValue * (5.000 / 1023.000); // Converts analog (0-1023) to voltage (0-5V):
  sensor = 0;
  EEPROM.put(0, Vclear);  // stores Vclear in ROM
  delay(1000);
  lcd.clear();  
  }  
  delay(1000);  // Pause for 1 seconds.  // sampling rate
}
