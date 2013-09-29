// Screen1.ino

// include the library code:
#include <LiquidCrystal.h>
// #include <Wire.h>


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);



// you can change the overall brightness by range 0 -> 255
int brightness = 255;
int Temp=0;

int blRed=3;  //Red backlight pin
int blGrn=5;  //Green backlight pin
int blBlu=6;  //Blue backlight pin

int TempPin=A0; //The pin into which the temp sensor output is plugged
int Temp1=0;   //Temperature reading directly from sensor

int r=1;
int g=1;
int b=1;

String IncomingMessage;

void setup() 
{

  pinMode(TempPin,INPUT);
 	analogReference(EXTERNAL);  //Use 3.3 volt supply as AREF

  // Set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);

  	//Set up the backlight controls
    pinMode(blRed, OUTPUT);
    pinMode(blGrn, OUTPUT);
    pinMode(blBlu, OUTPUT);

    lcd.print("Temperature ");

    brightness = 255;
    Serial.begin(9600);
    Serial.println("1050");
  }


  void loop() 
  {
   if ( Serial.available() > 0)
   {
    while(Serial.available() >0)
    {
     char recieved=Serial.read();
     if (recieved==char(003))
     {
      Serial.println("Message Received");
      Serial.println(IncomingMessage);
      lcd.clear();
      lcd.print(IncomingMessage);

      delay(10000);
      lcd.clear();
      lcd.print("Temperature "); 
      IncomingMessage="";   //Clear String for next message
    }
    else{
      IncomingMessage += recieved;
    }



  }
  
}
Temp=random(32,100);
lcd.setCursor(12,0);
lcd.print(Temp);
lcd.print(char(223));
	// if (tempmode==0)
	// {
	// 	lcd.print("F");
	// }
  lcd.setCursor(0,1);
  Temp1=analogRead(TempPin);

  lcd.print(Temp1);
  Serial.println(Temp1);

  delay(1000);
  r=0;
  b=0;
  g=255;
  setBacklight(r,g,b);
}



void setBacklight(uint8_t r, uint8_t g, uint8_t b) {
  // normalize the red LED - its brighter than the rest!
  r = map(r, 0, 255, 0, 100);
  g = map(g, 0, 255, 0, 150);

  r = map(r, 0, 255, 0, brightness);
  g = map(g, 0, 255, 0, brightness);
  b = map(b, 0, 255, 0, brightness);

  // common anode so invert!
  r = map(r, 0, 255, 255, 0);
  g = map(g, 0, 255, 255, 0);
  b = map(b, 0, 255, 255, 0);
  analogWrite(blRed, r);
  analogWrite(blGrn, g);
  analogWrite(blBlu, b);
}