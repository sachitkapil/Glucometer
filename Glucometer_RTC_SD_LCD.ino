int current = 0;
float concentration=0.0;
float current_voltage=0;
// measurement starts if transimpedance amp output voltage > threshold
float threshold = 0.800; //open circuit strip voltage in V

String line1="Glucose concentration:";
int stringStart=0;
int stringStop=0;
int scrollCursor=16; //16 being the screen width for 16x2 LCD screen

#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); /// REGISTER SELECT PIN,ENABLE PIN,D4 PIN,D5 PIN, D6 PIN, D7 PIN

#include <SPI.h> //for the SD card module
#include <SD.h> // for the SD card
#include <RTClib.h>
#include<Wire.h>
RTC_DS1307 rtc;
int chipSelect=2;
File myFile; 

void setup() {

 lcd.begin(16, 2);
 Serial.begin(9600);
 Wire.begin();
 rtc.begin(); 
 rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

 Serial.println("Initializing SD card.");

  if(!SD.begin(chipSelect)) {
    Serial.println("Initialization failed.");
    return;
  }
  Serial.println("Initialization done.");

  myFile=SD.open("DATA.txt", FILE_WRITE);
  if (myFile) 
  {
   Serial.println("File opened ok.");
  }
 
  myFile.close();
}

void loop() 
{
   lcd.setCursor(0, 0);
   lcd.print("Apply Sample");//set the cursor to column 0, line 1
   delay(1000);
   lcd.clear();
   delay(1000); //for blinking
  // while(1) 
  //{
   current_voltage=analogRead(0)*(5.0 / 1023.0);
 //lcd.print(current_voltage);// delay(1000);
 Serial.println(current_voltage);
   
  if(current_voltage > threshold) 
   {
    lcd.setCursor(0, 0); // set the cursor to column 0, line 1
    lcd.print("Calculating");
    delay(1000); //delay
    lcd.clear();

   
      for(int i=15; i>0; i--) //Countdown to 15 seconds
      {
        lcd.setCursor(0, 0);
        lcd.print(i);
        Serial.println(i);
        delay(1000);
        lcd.clear();
      }
  // compute concentration
  current_voltage = analogRead(0)*(5.0 / 1023.0);
 
  lcd.clear();
  //lcd.print(current_voltage);

  delay(1000);
  concentration = 378.95*current_voltage-210.56;
  delay(500);

 //For LCD display- Scrolling and non-scrolling line

 for (int i=0;i<50;i++) //Amount of time to keep the scrolling going
        {         lcd.setCursor(scrollCursor, 0);
                  lcd.print(line1.substring(stringStart,stringStop));
                  lcd.setCursor(0, 1); 
                  lcd.print(concentration);
                  lcd.print(" mg/dL");
                  delay(400);
                  lcd.clear();
                  if(stringStart == 0 && scrollCursor > 0)
                    {
                    scrollCursor--;
                    stringStop++;
                    } 
                    
                  else if (stringStart == stringStop)
                    {
                    stringStart = stringStop = 0;
                    scrollCursor = 16; //16 is the screen width
                  } else if (stringStop == line1.length() && scrollCursor == 0) 
                  {
                    stringStart++;
                  } 
                  else 
                  {
                    stringStart++;
                    stringStop++;
                  } //For continuing L to R after column 0 is filled

        }
        loggingData();
        delay(5000);
}


}

void loggingData() {
  DateTime now = rtc.now();
  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    myFile.print("Date:");
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.println(now.day(), DEC);
    myFile.print("Time:");
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.println(now.second(), DEC);
    myFile.print("Glucose Concentration:");
    myFile.print(concentration);
    myFile.print(" mg/dL");
  }
 
  myFile.close();
  delay(1000);  
}

   
 
