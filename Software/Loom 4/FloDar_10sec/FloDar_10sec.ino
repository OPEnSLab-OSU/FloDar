/**
 * This is an example use case for the FloDar project
 * 
 * NOTE: THIS EXAMPLE DOESN'T WAIT FOR SERIAL AFTER SLEEPING
 * MANAGER MUST BE INCLUDED FIRST IN ALL CODE
 * 
 * MAX31856 is currently unused, so disabled to save compile time
 * Thermocouple unused as well, however is attached to ADS1115
 */

#include <Loom_Manager.h>

#include <Hardware/Loom_Hypnos/Loom_Hypnos.h>
#include <Sensors/I2C/Loom_ADS1115/Loom_ADS1115.h>
#include <Sensors/Loom_Analog/Loom_Analog.h>
// Currently unused
//#include <Sensors/SPI/Loom_MAX318XX/Loom_MAX31856.h>

// Sleep Duration
#define DAY    0
#define HOUR   0
#define MINUTE 0
#define SECOND 10

// Wait to measure (milliseconds)
#define DELAY 10000

Manager manager("Device", 1);

// Create a new Hypnos object setting the version to determine the SD Chip select pin, and starting without the SD card functionality
Loom_Hypnos hypnos(manager, HYPNOS_VERSION::V3_3, TIME_ZONE::PST);
Loom_Analog analog(manager);
Loom_ADS1115 ads(manager);
// Currently unused
//Loom_MAX31856 max56(manager);
// Called when the interrupt is triggered 
void isrTrigger(){
  hypnos.wakeup();
}

void setup() {

  // Start the serial interface
  manager.beginSerial();

  // Enable the hypnos rails
  hypnos.enable();

  // Called after enable
  manager.initialize();

  // Register the ISR and attach to the interrupt
  hypnos.registerInterrupt(isrTrigger);
}

void loop() {

  // Wait for USFM to boot up before taking data
  delay(DELAY);
  
  // Measure and package data
  manager.measure();
  manager.package();
  
  // Print the current JSON packet
  manager.display_data();            

  // Log the data to the SD card              
  hypnos.logToSD();

  // Set the RTC interrupt alarm to wake the device in given time
  hypnos.setInterruptDuration(TimeSpan(DAY, HOUR, MINUTE, SECOND));

  // Reattach to the interrupt after we have set the alarm so we can have repeat triggers
  hypnos.reattachRTCInterrupt();
  
  // Put the device into a deep sleep, operation HALTS here until the interrupt is triggered
  hypnos.sleep(false);
}
