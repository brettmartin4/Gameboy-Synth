// Original Author: Tom Van den Bon
// Edited by: Brett Martin
// 2012/12/21
// Send Data to Gameboy via Link Cable
// Updated to send packets via 

// Link Cable     Arduino      Desc
// 6              GND          GND
// 5              8            SC
// 2              7            SI

int volatile CLOCK_PIN = 8;
int volatile SO_PIN = 7;

void setup() {                
  pinMode(SO_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println(0x80);
  Serial.println(byte(1)<<3);
  Serial.println(byte(7));
  Serial.println(0x80 | (byte(1)<<3) | byte(7));
}

// the loop routine runs over and over again forever:
void loop() {
  byte dataToSend = 0x05;
  byte counter = 0;
  
  for ( counter = 0; counter < 8; counter++ )
  {
    if ( dataToSend & 0x80 )
    {
      //Serial.println( "1" );
      digitalWrite( SO_PIN, HIGH );
      digitalWrite( CLOCK_PIN, LOW );
      delayMicroseconds( 50);
      digitalWrite( CLOCK_PIN, HIGH );
      delayMicroseconds( 50);
    }
    else
    {
      //Serial.println( "0" );
      digitalWrite( SO_PIN, LOW );
      digitalWrite( CLOCK_PIN, LOW );
      delayMicroseconds( 50);
      digitalWrite( CLOCK_PIN, HIGH );
      delayMicroseconds( 50);
    }
    
    dataToSend = dataToSend << 1;
  }
  delay(25); // This is the fastest possible setting. Anything lower will cause misreadings from the CGB CPU
}
