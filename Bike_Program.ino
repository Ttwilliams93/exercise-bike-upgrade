/* YourDuino.com Example Software Sketch
 20 character 4 line I2C Display
 Backpack Interface labelled "LCM1602 IIC  A0 A1 A2"
 terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>

/*-----( Declare Constants )-----*/
//none
/*-----( Declare objects )-----*/
// set the LCD address to 0x20 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address


/*-----( Declare Variables )-----*/
//Light Sensor Variables
const int LightSensorPin = 0; //Connected to analog 0 
int LightSensorValue;

//Push Button Variables
int TimerResetPin=2; //connected to pin 7
volatile int TimerButtonState = 0;  //Variable reading push button status


//Calculated Variables Based of light sensor pin
int RPM = 0;
int RPMAVG=0;

//Time Variables
long SumOfTime=0;
int Seconds = 0;
int Minutes = 0;
int Hours = 0;
int SecondsCounter = 0;
int MinutesCounter = 0;
long ResetTime=0;
int OldSeconds=0;
long UpdateScreenTime=0;




void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  
  //Establishing Pin Types
  pinMode(TimerResetPin, INPUT);
  
  
  Serial.begin(9600);  // Used to type in characters
  lcd.begin(20,4);         
// ------- Quick 3 blinks of backlight  -------------
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(10);
    lcd.noBacklight();
    delay(10);
  }
  lcd.backlight(); // finish with backlight on  
  
//-------- Write characters on the display ----------------
// NOTE: Cursor Position: CHAR, LINE) start at 0  
  lcd.setCursor(1,0); //Start at character 4 on line 0
  lcd.print("Hello, welcome to your bike!");
  delay(200);
  lcd.setCursor(2,1);
  lcd.print("From Troy");
  delay(200);  
  lcd.setCursor(0,2);
  lcd.clear();
  
  //attaching interrupts 
  attachInterrupt(0, PushButton, CHANGE);
}


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
 SumOfTime = millis()-ResetTime;
 
 ReadLightSensor();
 CalculateTime();
 
 if ((SumOfTime - UpdateScreenTime) > 1000 ){
 UpdateScreen();
}
}

void ReadLightSensor(){
}

void PushButton(){
  TimerButtonState = digitalRead(PushButton);
  if(TimerButtonState = HIGH){
    RPMAVG = 0;
    RPM = 0;
    ResetTime = millis();
  }
  }
  
void CalculateTime(){
   Seconds = (SumOfTime /1000) - 60*(SecondsCounter) ;
   
    if (Seconds > 59 ){ 
      lcd.clear();
      SecondsCounter = SecondsCounter+1;
      Minutes = Minutes + 1;
      
      if (Minutes > 59) {
        Minutes = 0;
        Hours = Hours + 1;
        if(Hours >=24) { 
          Hours = 0;
        }
      }
    }
  }
  

void UpdateScreen(){
     UpdateScreenTime = millis();
   

    //Printing Time on Top Row
    lcd.setCursor(0,0); 
    lcd.print("Time:");
    lcd.setCursor(5,0);
    lcd.print(Hours); // Print the time
    lcd.setCursor(7,0);
    lcd.print(":");
    lcd.setCursor(8,0);
    lcd.print(Minutes);
    lcd.setCursor(10,0);
    lcd.print(":");
    lcd.setCursor(11,0);
    lcd.print(Seconds);

    //Printing RPM on Second Row
    lcd.setCursor(0,1);
    lcd.print("RPM:");
    lcd.setCursor(4,1);
    lcd.print(RPM); //PRINT CURRENT RPM

    //Printing Average RPM Over Time interval 
    lcd.setCursor(0,2);
    lcd.print("Average RPM:");
    lcd.setCursor(12,2);
    lcd.print(RPMAVG);
    }





