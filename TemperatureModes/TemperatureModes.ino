//initialize tempurature sensor
const int tempPin = A0;
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

void setup () {
  analogReference (EXTERNAL);
  Serial.begin (9600);
  pinMode (buttonPin, INPUT);
  pinMode (tempPin, INPUT);
}

void loop () {
  long tempReading = analogRead (tempPin);
  Serial.println(tempReading);
  long voltage = (tempReading*3300)/1023;
  Serial.print (voltage); 
  Serial.println ("volts");

  int reading = digitalRead (buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis ();
  }
  if ((millis () - lastDebounceTime > debounceDelay )) {
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        i++;
        Serial.print("i=");
        Serial.println(i);
      }
    }

  }
  Serial.print (voltage); 
  Serial.println ("volts");
  if (( i % 2)== 0) {
    // now print out the temperature
    tempC1 = ((voltage - 500)/10);//converting from 10 mv per degree wit 500 mV offset
    //to degrees ((voltage - 500mV) times 100)
    tempC2 =((voltage - 500)%10) ;                                     
    Serial.print(tempC1); 
    Serial.print ("."); 
    Serial.print(tempC2); 
    Serial.print (char (176)); 
    Serial.println ("C"); 
  }
  if( i%2 !=0) {
    // now convert to Fahrenheit
    tempF1 = (((((voltage-500)/10)*9)/5)+32);
    tempF2 = (((((voltage-500)%10)*9)%5)+32);
    //tempF2 = (((tempC1 * 9 )/ 5) + 32);

    Serial.print(tempF1); 
    Serial.print ("."); 
    Serial.print (tempF2); 
    Serial.print (char (176)); 
    Serial.println ("F");
  }
  lastButtonState = reading;
}



