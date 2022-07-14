///////////////////////////////////////////////////////////////////////////////

// FloDar
// This code is intended to be used to keep the TUF-2000M on
// to allow the user to manually enter data

// January 2022

///////////////////////////////////////////////////////////////////////////////

void setup() 
{
  // Needs to be done for Hypno Board
  pinMode(5, OUTPUT);   // Enable control of 3.3V rail 
  pinMode(6, OUTPUT);   // Enable control of 5V rail 
  pinMode(12, INPUT_PULLUP);    // Enable waiting for RTC interrupt, MUST use a pullup since signal is active low
  pinMode(13, OUTPUT);

  //See Above
  digitalWrite(5, LOW); // Enable 3.3V rail
  digitalWrite(6, HIGH);  // Enable 5V rail
  digitalWrite(13, LOW);

  Serial.flush();

  digitalWrite(5, LOW); // Disable 3.3V rail
  digitalWrite(6, HIGH);  // Disable 5V rail
  digitalWrite(13, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
}


void loop() 
{
  
}
