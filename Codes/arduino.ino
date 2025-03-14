#include <Servo.h>
#include <HX711_ADC.h> // need to install 
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // need to install

HX711_ADC LoadCell(4, 5); // parameters: dt pin 6, sck pin 7;
LiquidCrystal_I2C lcd(0x27, 16,2); // 0x27 is the i2c address might different;you can check with Scanner
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int relay=5;
int buzzer=2;
int gasflag=0;
int levelflag=0;
void setup() {
  LoadCell.begin(); // start connection to HX711
  LoadCell.start(2000); // load cells gets 2000ms of time to stabilize
  LoadCell.setCalFactor(115.68); // calibration factor for load cell => dependent on your individual setup
  lcd.init(); 
  lcd.backlight();
Serial.begin(115200);
 myservo.attach(3);
  myservo.write(pos);
pinMode(buzzer,OUTPUT);
  lcd.setCursor(0,0);
  lcd.print("GAS MONITORING");
   lcd.setCursor(0,1);
  lcd.print("   SYSTEM   ");
  delay(2000);
  lcd.clear();
   delay(6000);
     lcd.print("System Ready");
      delay(1000);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
   int sensorValue = analogRead(A0);
   LoadCell.update(); // retrieves data from the load cell
  float i = LoadCell.getData(); // get output value
   lcd.clear();
 lcd.setCursor(0,0);
  lcd.print("GAS:");
   lcd.setCursor(5,0);
  lcd.print(sensorValue);
  lcd.setCursor(0,1);
  lcd.print("WEIGHT:  ");
   lcd.setCursor(7,1);
  lcd.print(i);
  if(sensorValue>475)
  {
    gasflag=1;
     lcd.clear();
     lcd.setCursor(0,0);
  lcd.print("GAS DETECTED");
  digitalWrite(buzzer,HIGH);
  delay(300);
  myservo.write(90);
  }
  else{
    gasflag=0;
    digitalWrite(buzzer,LOW);
    myservo.write(pos);
  }
  if((i>20)&&(i<100))
  {
    levelflag=1;
    lcd.clear();
     lcd.setCursor(0,1);
  lcd.print("GAS LEVEL:LOW  ");
  digitalWrite(buzzer,HIGH);
  }
  else
  {
    levelflag=0;
    digitalWrite(buzzer,LOW);
  }
       String getStr;
  getStr +="&field1=";
  getStr += String(gasflag);
  getStr +="&field2=";
  getStr += String(levelflag);
  getStr +="&field3=";
  getStr += String(i);
  getStr += "\r\n\r\n";
  Serial.print(getStr);
  delay(300);
}

