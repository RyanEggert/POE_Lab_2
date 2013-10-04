// WorkingScriptYes.ino

// include the library code:
#include <LiquidCrystal.h>
// #include <Wire.h>


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);



// you can change the overall brightness by range 0 -> 255
int brightness = 255;

int blRed=3;  //Red backlight pin
int blGrn=5;  //Green backlight pin
int blBlu=6;  //Blue backlight pin

// int TempPin=A0; //The pin into which the temp sensor output is plugged
// int Temp1=0;   //Temperature reading directly from sensor

int r=1;
int g=1;
int b=1;

String IncomingMessage;

const int tempPin = A0;  //For temperature reading and modes
long tempC1 = 0;
long tempC2 = 0;
long tempF1 =0;
long tempF2 = 0;
long reading = 0;

int buttonPin = 13;
int buttonState = LOW;
int lastButtonState = HIGH;
int i = 0;

long lastDebounceTime = 0;
long debounceDelay = 40;

int  StartRefresh=0;
int RefreshTime=1000;

int TempMode=1;

void setup() 
{
  pinMode (buttonPin, INPUT);
  pinMode(tempPin,INPUT);
 	analogReference(EXTERNAL);  //Use 3.3 volt supply as AREF

  // Set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  lcd.clear();

  	//Set up the backlight controls
    pinMode(blRed, OUTPUT);
    pinMode(blGrn, OUTPUT);
    pinMode(blBlu, OUTPUT);

    lcd.print("Temperature: ");

    brightness = 255;
    Serial.begin(9600);
    Serial.println("1050");
    r=0;
    b=0;
    g=255;
    setBacklight(r,g,b);
  }


  void loop() 
  {

    long tempReading = analogRead (tempPin);
    Serial.println(tempReading);
    long voltage = (tempReading*3300)/1023;

    int reading = digitalRead (buttonPin);
    if (reading != lastButtonState) {
      lastDebounceTime = millis ();
    }
    if ((millis () - lastDebounceTime > debounceDelay )) 
    {
      if (reading != buttonState) {
        buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        i++;
        TempMode=1;
      }
    }

  }

  if (( i % 2)== 0) {
    if(TempMode==1)
    {
        // now print out the temperature
    tempC1 = ((voltage - 500)/10);//converting from 10 mv per degree wit 500 mV offset
    //to degrees ((voltage - 500mV) times 100)
    tempC2 =((voltage - 500)%10) ;                                     
    lcd.setCursor(0,1);
    lcd.print(tempC1);
    lcd.print ("."); 
    lcd.print(tempC2); 
    lcd.print (char (223)); 
    lcd.println ("C          ");
    g=map(tempC1,20,30,0,255);
    b=map(tempC1,20,30,255,0);
    r=0;
    setBacklight(r,g,b);
    TempMode++ ;
  }
  else
  {

  }

}
if( i%2 !=0) 
{
  if (TempMode==1)
  {
     // now convert to Fahrenheit
     tempF1 = (((((voltage-500)/10)*9)/5)+32);
     tempF2 = (((((voltage-500)%10)*9)%5)+32);
    //tempF2 = (((tempC1 * 9 )/ 5) + 32);

    lcd.setCursor(0,1);
    lcd.print(tempF1);
    lcd.print ("."); 
    lcd.print(tempF2); 
    lcd.print (char (223)); 
    lcd.println ("F         "); 
    TempMode++;
  }
  else
  {

  }
}






lastButtonState = reading;

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
    lcd.print("Temperature: "); 
      IncomingMessage="";   //Clear String for next message
    }
    else{
      IncomingMessage += recieved;
    }



  }
}

// lcd.setCursor(12,0);
// lcd.print(Temp);
// lcd.print(char(223));

// 	// if (tempmode==0)
// 	// {
// 	// 	lcd.print("F");
// 	// }
//   lcd.setCursor(0,1);

//   lcd.print(Temp1);


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