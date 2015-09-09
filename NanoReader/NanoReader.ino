/*
  NanoReader
  Reads value from analog inputs and sends the values to Serial output if they change
 
  This example code is in the public domain.
 */
int digitalPins[] = {2,4,7,8,10,11,12,13};
int numDigitalPins = 8;
int numAnalogPins = 4;

void sendVal( int ch, int val ) {
//*  
  int usdelay = 71;
  Serial.write(ch);
  delayMicroseconds(usdelay);
  Serial.write(val>>8);
  delayMicroseconds(usdelay);
  Serial.write(val&0xff);
  delayMicroseconds(usdelay);
  Serial.println();
/*/  
  Serial.print(ch);
  Serial.print(" ");
  Serial.println(val);
/**/
}
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  for( int i=0; i<numDigitalPins; i++ ) {
    int pin = digitalPins[i];
    pinMode(pin, INPUT_PULLUP);
  }
}

int prevprevvals[15];
int prevvals[15];
int prevdigitalvals[15];
unsigned long lastSends[15];
unsigned long lastSendsDigital[15];
int digiMap[] = {1,0,5,4,7,3,2,6,8};
int ledMap[] = {3,1,2,0};
// the loop routine runs over and over again forever:
int incval;

void loop() {
  // read the input on analog pin 0:
  
  for( int i=0; i<numAnalogPins; i++ ) {
    int sensorValue = analogRead(A0+i);
//    sensorValue /= 4;
    if( sensorValue != prevvals[i] ) 
    {
      if( lastSends[i] > millis() ) lastSends[i] = 1;//rollover yo
      int diff = sensorValue-prevvals[i];
      
//      if( abs(diff)>1 || ((millis()-lastSends[i])>50) ) 
      if( sensorValue != prevprevvals[i])
      {
        prevprevvals[i] = prevvals[i];
        lastSends[i] = millis();
        prevvals[i] = sensorValue;
        sendVal(3-i, sensorValue);
      }
    }
  }
  
//*  
  for( int i=0; i<numDigitalPins; i++ ) {
    int pin = digitalPins[i];
    int sensorValue = digitalRead(pin);
    if( sensorValue != prevdigitalvals[i] ) 
    {
      prevdigitalvals[i] = sensorValue;
      if( lastSendsDigital[i] > millis() ) lastSendsDigital[i] = 1;//rollover yo
      
      //10ms debounce
//      if( sensorValue == LOW && ((millis()-lastSendsDigital[i])>5) ) {
      if( ((millis()-lastSendsDigital[i])>5) ) {
          //TODO check if sinceLow > 2000 and send longpress to serial
        lastSendsDigital[i] = millis();
        sendVal(digiMap[i]+20, 1-sensorValue);
      }
    }
  }
/**/  
//  incval++; if( incval>255 ) incval = 0;

  analogWrite(3, prevvals[ledMap[0]]/6);
  analogWrite(5, prevvals[ledMap[1]]/6);
  analogWrite(6, prevvals[ledMap[2]]/6);
  analogWrite(9, prevvals[ledMap[3]]/6);
  
  delay( 1 ); //1ms is enough to not saturate the serial line
}
