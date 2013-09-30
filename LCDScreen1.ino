// Screen1.ino

// include the library code:
#include <LiquidCrystal.h>
// #include <Wire.h>


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);



// you can change the overall brightness by range 0 -> 255
int brightness = 255;
int Temp=0; //Temp variable used for testing. Random values

int blRed=3;  //Red backlight pin
int blGrn=5;  //Green backlight pin
int blBlu=6;  //Blue backlight pin

int TempPin=A0; //The pin into which the temp sensor output is plugged
int Temp1=0;   //Temperature reading directly from sensor

int cutoff;   //For splitting long messages into two lines for the LCD

int r=1;
int g=1;
int b=1;

String IncomingMessage; //Custom message assembled from data coming from serial port
String FirstLine;
String SecondLine; //Strings used to write long messages to two lines
void setup() 
{

  pinMode(TempPin,INPUT);
 	analogReference(EXTERNAL);  //Use 3.3 volt supply as AREF

  // Set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  lcd.clear();

  	//Set up the backlight controls
    pinMode(blRed, OUTPUT);
    pinMode(blGrn, OUTPUT);
    pinMode(blBlu, OUTPUT);

    lcd.print("Temperature ");

    brightness = 255;
    r=0;
    b=10;
    g=100;
    setBacklight(r,g,b);
    Serial.begin(9600);
    Serial.println("1050");
  }


  void loop() 
  {

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
  if ( Serial.available() > 0)
  {
    while(Serial.available() >0)
    {
     char recieved=Serial.read();
     if (recieved==char(003))
     {
      Serial.println("Message Received");
      cutoff=IncomingMessage.indexOf(char(007));
      if (cutoff!=-1) //if the incomingmessage has the bell character--this means it will be displayed on two lines
      {
        FirstLine=IncomingMessage[0:cutoff];
        SecondLine=IncomingMessage[cutoff+1:length(IncomingMessage)];
        lcd.clear();
        lcd.print(FirstLine);
        lcd.setCursor(0,1);
        lcd.print(SecondLine);
        FirstLine="";  //clear string for next message/use
        SecondLine=""; //clear string for next message/use
      }
      else
      {
        Serial.println(IncomingMessage);
        lcd.clear();
        lcd.print(IncomingMessage);
        IncomingMessage="";   
      }

      delay(10000);
      lcd.clear();
      lcd.print("Temperature "); 
      

    }
    else{
      IncomingMessage += recieved;
    }



  }
  
}
delay(1000);

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