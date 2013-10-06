// LCD_Control.ino

#include <LiquidCrystal.h>
LiquidCrystal lcd(7,8,9,10,11,12); //Initialize LCD. Give data pin locations
int brightness = 255; //Enables changing the overall backlight brightness
int blRed=3;  //Red backlight pin
int blGrn=5;  //Green backlight pin
int blBlu=6;  //Blue backlight pin
int r=1; //Used to set 0-255 value for backlight red LED
int g=1; //Used to set 0-255 value for backlight green LED
int b=1; //Used to set 0-255 value for backlight blue LED

const int tempPin = A0;  //For temperature reading and modes
long tempC1 = 0; //Tens and ones place of Celsius temperature
long tempC2 = 0; //Decimal of Celsius temperature
long tempF1 = 0;//Tens and ones place of Celsius temperature
long tempF2 = 0;//Decimal of Celsius temperature
int i = 0; //Used to determine whether to display C or F
int TempRunOnce=1; //Used to prevent rapid refreshing and flickering of the screen

int buttonPin = 13;   //Pushbutton Pin
long reading = 0; //Used for reading the pushbutton state
int buttonState = LOW;//Pushbutton state variable
int lastButtonState = HIGH; //Pushbutton memory state

long lastDebounceTime = 0; //Used for debouncing
long debounceDelay = 40; //Debounce delay in milliseconds

String IncomingMessage; //Used to store the incoming serial message from a PC.

//Custom Character setup
byte box1 [8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00011,
  B00100,
  B00000,
  B00000
};

byte box2 [8] = {
  B01110,
  B01110,
  B01110,
  B00100,
  B11111,
  B01110,
  B01110,
  B01110
};

byte box3 [8] = {
  B00000,
  B00000,
  B00100,
  B01000,
  B10000,
  B00000,
  B00000,
  B00000
};

byte box4 [8] = {
  B01110,
  B01010,
  B01010,
  B01010,
  B01010,
  B11011,
  B00000,
  B00000
};


void setup() 
  {
  pinMode (buttonPin, INPUT); 
  pinMode(tempPin,INPUT);
 	analogReference(EXTERNAL);  //Use 3.3 volt supply as AREF

  // Set up the LCD's number of rows and columns and clear it.
  lcd.begin(16, 2);
  lcd.clear();

	//Set up the backlight controls
  pinMode(blRed, OUTPUT);
  pinMode(blGrn, OUTPUT);
  pinMode(blBlu, OUTPUT);

  lcd.createChar (1, box1); //create custom characters
  lcd.createChar (2, box2);
  lcd.createChar (3, box3);    
  lcd.createChar (4, box4);

  lcd.setCursor(0,0);       //Sets cursor at the origin
  lcd.print("Temperature: "); //Prints "Temperature :" label to first line

  brightness = 255; //Sets screen to maximum brightness
  Serial.begin(9600); //Start the serial port
  Serial.println("1050"); //The Arduino prints to the serial port to indicate that it has loaded and is ready to recieve information. Used to alert Python code that Arduino has loaded after Pyserial forces an Arduino restart.
  }


void loop() 
{
  long tempReading = analogRead (tempPin); //Read temperature from sensor
  long voltage = (tempReading*3300)/1023;  //Convert to voltage (in mV)

  //Debounce Button
  int reading = digitalRead (buttonPin); 

  if (reading != lastButtonState)//Looking for a change in the button's state
  {
    lastDebounceTime = millis (); //Saves the time at which the state changed
  }
  if ((millis () - lastDebounceTime > debounceDelay )) //Check whether its been debounceDelay milliseconds since the button's state initially changed
  {
    if (reading != buttonState) //Check whether the button is currently the opposite of what it began as
    {
      buttonState = reading; //Store current reading

      if (buttonState == HIGH) //Only switch if the button is HIGH
      {
        i++; //increments i, used to select between C and F
        TempRunOnce=1; //Sets TempRunOnce=0; this displays the temp once per button press to prevent screen flicker
        lcd.setCursor(0,0); //Sets LCD cursor at the origin
        lcd.print("Temperature: "); //Prints temperature text label
      }
    }
  }

  lastButtonState = reading; //Stores the latest reading from the button

  if (( i % 2)== 0) //checks whether i is an even number
  {
    if(TempRunOnce==1) //checks whether this is the first time through this if-statement since the button has been pressed
    {
      tempC1 = ((voltage - 500)/10);//converting from 10 mv per degree wit 500 mV offset
      //to degrees ((voltage - 500mV) times 100)
      tempC2 =((voltage - 500)%10) ;                                     
      lcd.setCursor(0,1);
      lcd.print(tempC1);
      lcd.print ("."); 
      lcd.print(tempC2); 
      lcd.print (char (223)); 
      lcd.println ("C          ");
      Serial.print(tempC1); Serial.print(".");Serial.print(tempC2);Serial.println("C");
      g=map(tempC1,20,30,0,255); //map green backlight LED to display 0 when temp is 20C, 255 when temp is 
      b=map(tempC1,20,30,255,0); //map green backlight LED to display 0 when temp is 20C, 255 when temp is  
      r=0;
      setBacklight(r,g,b);//Use the setBacklight function to change backlight
      ShowCustomChar();
      TempRunOnce++ ;
    }
  }
  if( i%2 !=0) //checks whether i is an odd number
  {
    if (TempRunOnce==1)
    {
     // now convert to Fahrenheit
     tempF1 = (((((voltage-500)/10)*9)/5)+32);
     tempF2 = (((((voltage-500)%10)*9)%5)+32);
     lcd.setCursor(0,1);
     lcd.print(tempF1);
     lcd.print ("."); 
     lcd.print(tempF2); 
     lcd.print (char (223)); //Print the degree sign to the LCD
     lcd.println ("F         "); 
     Serial.print(tempF1); Serial.print(".");Serial.print(tempF2);Serial.println("F");
     g=map(tempC1,65,80,0,255);
     b=map(tempC1,65,80,255,0);
     r=0;
     setBacklight(r,g,b);
     ShowCustomChar();
     TempRunOnce++;
   }
  }
  if ( Serial.available() > 0) //Checks for incoming data from the serial port
  {
    while(Serial.available() >0) //Loops as long as there is incoming data
    {
     char recieved=Serial.read(); //Stores the recieved character
     
     if (recieved==char(003)) //Check to see whether  character has been recieved. Python appends this to the end of each custom message it sends.
     {
      lcd.clear(); //Clear the LCD
      lcd.print(IncomingMessage); //Print everything recieved to the serial port, excluding the  character to the LCD
      delay(10000); //Display the message for 10 seconds
      lcd.clear(); //Clear the message from the screen
      lcd.print("Press button to"); //Prompt the user to press the button to 
      lcd.setCursor(0,1);           //resume temperature display
      lcd.print("continue.");
        IncomingMessage="";   //Clear IncomingMessage string for next message
      }
      else{
        IncomingMessage += recieved; //if the message isn't over yet, append the latest recieved character to a string
      }
    }
  }
}

void ShowCustomChar() //A function which will print four custom characters in their appropriate places at the far right side of the LCD.
{
  lcd.setCursor(13,0); //Set cursor to print boxes 1-3 on the first line
  lcd.write (1); //Print box 1
  lcd.write (2); //Print box 2
  lcd.write (3); //Print box 3
  lcd.setCursor (14,1); //Set cursor to print box four on the bottom line below box 2
  lcd.write (4); //Print box 4
}


void setBacklight(uint8_t r, uint8_t g, uint8_t b) {
  //Normalize the red LED--its brighter than the rest.
  r = map(r, 0, 255, 0, 100);
  g = map(g, 0, 255, 0, 150);

  //Map rgb values to reflect brightness variable.
  r = map(r, 0, 255, 0, brightness);
  g = map(g, 0, 255, 0, brightness);
  b = map(b, 0, 255, 0, brightness);

  //Common anode so invert!
  r = map(r, 0, 255, 255, 0);
  g = map(g, 0, 255, 255, 0);
  b = map(b, 0, 255, 255, 0);\

  //Set backlight LEDs to mapped values
  analogWrite(blRed, r); 
  analogWrite(blGrn, g);
  analogWrite(blBlu, b);
}