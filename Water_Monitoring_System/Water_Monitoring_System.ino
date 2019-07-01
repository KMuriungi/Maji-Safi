#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int buttonPinSelect = 51;    // the number of the pushbutton pin
const int buttonPinReset = 53;

int redPin = 23;      // the number of the LED pin
int greenPin =25;
int bluePin =27;

int buttonSelCounter = 0;   // counter for the number of button presses
int buttonSelState = 0;         // current state of the button
int lastButtonSelState = 0;     // previous state of the button

int buttonResCounter = 0;   // counter for the number of button presses
int buttonResState = 0;         // current state of the button
int lastButtonResState = 0;     // previous state of the button

#define SENSOR       A7 //Turbidity Sensor 
float voltage, turbidity, finalturbidity, phValue;

#define SensorPin A0          //pH meter Analog output to Arduino Analog Input 0
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;

void setup() {
  pinMode(buttonPinSelect, INPUT);
  pinMode(buttonPinReset, INPUT);
  Serial.begin(9600);
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(SENSOR,INPUT); //Turbidity

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   MAJI SAFI   ");
  lcd.setCursor(0,1);
  lcd.print("*Health Living*");
  delay(5000);
}

 /*
 ************************************************************
 *----------------- MAIN FUNCTION -------------------*
 ************************************************************
*/

void loop() {
 
/*
STATE SELECT BUTTON
*/

  buttonSelState = digitalRead(buttonPinSelect);
  if (buttonSelState != lastButtonSelState) {
    if (buttonSelState == HIGH) {
      buttonSelCounter++;
      Serial.println("on");
      Serial.print("number of button pushes:  ");
      Serial.println(buttonSelCounter);
    } else {
      Serial.println("off");
    }
    delay(50);
  }
  lastButtonSelState = buttonSelState;
  if (buttonSelCounter % 2 == 0) {
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);
    turbid();
  } else if(buttonSelCounter % 3 == 0) {
    digitalWrite(bluePin, HIGH);
    digitalWrite(greenPin, LOW);
    pHSensor();
  }else {
    digitalWrite(bluePin, HIGH);
    digitalWrite(greenPin, LOW);

    if(finalturbidity < 1.4 && phValue > 6.2 && phValue < 7.8){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   MAJI SAFI   ");
  lcd.setCursor(0, 1);
  lcd.print("Fit for Drinking");
  }else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  MAJI CHAFU   ");
    lcd.setCursor(0, 1);
    lcd.print("Not Fit 4 Drink");
    }
  }
  
 /*
 RESET BUTTON
 */
 
  buttonResState = digitalRead(buttonPinReset);
  if (buttonResState != lastButtonResState) {
    if (buttonResState == LOW) {
      buttonResCounter++;
      Serial.println("on");
      Serial.print("number of button pushes:  ");
      Serial.println(buttonResCounter);
    } else {
      Serial.println("off");
    }
    delay(50);
  }
  lastButtonResState = buttonResState;
  if (buttonResCounter % 2 == 0) {
    // NOTHING HERE
    //Addition of Anything Affects the Normal Working of the Entire System
    //Leave it as it is!!! 
  } else {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("   MAJI SAFI   ");
    lcd.setCursor(0,1);
    lcd.print("*Health Living*");
  }

} //End Loop()

/*
 ************************************************************
 *                                                          *
 **********************CUSTOM FUNCTIONS**********************
 *                                                          *
 ************************************************************
*/

  /*
 ************************************************************
 *----------------- TURBIDITY (PHYSICAL) -------------------*
 ************************************************************
*/

void turbid(){
  voltage=0.004888*analogRead(SENSOR);  //in V
  turbidity=-1120.4*voltage*voltage+5742.3*voltage-4352.9;  //in NTU fron 0-5000
  finalturbidity = (turbidity/1000); // in NTU from 0-5
  if((voltage>=2.5)&(turbidity>=0))
  {
    Serial.println(" Turbidity = "+String(turbidity)+" NTU");  
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   MAJI SAFI   ");
    lcd.setCursor(2, 1);
    lcd.print("Turbid: "+String(finalturbidity));
  }
}

/*
 ************************************************************
 *----------------- pH Sensor (CHEMICAL)  --------------------*
 ************************************************************
*/
void pHSensor(){
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=2.90*phValue;                      //convert the millivolt into pH value
  Serial.print("    pH:");  
  Serial.print(phValue,2);
  Serial.println(" ");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   MAJI SAFI   ");
  lcd.setCursor(3, 1);
  lcd.print("pH: "+String(phValue));
  }

