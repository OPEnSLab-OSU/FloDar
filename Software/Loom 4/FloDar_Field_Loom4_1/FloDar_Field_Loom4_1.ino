/**
 * This is an example use case for the FloDar project: Ultrasonic flow meter reading PVC pipe.
 * Flow meter is on channel A2 of the ADS1115.
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
#define MINUTE 15
#define SECOND 0

// Wait to measure (milliseconds)
#define DELAY 10000

// Analog to Flowrate Conversion Formula Constants
#define MAXBITS     16384
#define MAXFLOWRATE 150

Manager manager("FloDar", 1);

// Create a new Hypnos object setting the version to determine the SD Chip select pin, and starting without the SD card functionality
Loom_Hypnos hypnos(manager, HYPNOS_VERSION::V3_3, TIME_ZONE::PST, true);
Loom_Analog analog(manager);
Loom_ADS1115 ads(manager, 0x48, false, true, false, adsGain_t::GAIN_TWOTHIRDS);
// Currently unused
//Loom_MAX31856 max56(manager);

// Analog to Flowrate Conversion Formula
float calcFlowRate(){
    return (((float)ads.getAnalog(3) / MAXBITS) * MAXFLOWRATE);
}

// Called when the interrupt is triggered 
void isrTrigger(){
  hypnos.wakeup();
}

void setup() {

  // Start the serial interface
  manager.beginSerial();

  if ((DELAY/1000 >= SECOND + 1) && (MINUTE == 0) && (HOUR == 0) && (DAY == 0)) {
    Serial.println("You need more time between measurements!");
    return;
  }

  // Enable the hypnos rails
  hypnos.enable();

  // Called after enable
  manager.initialize();

  // Register the ISR and attach to the interrupt
  hypnos.registerInterrupt(isrTrigger);
}

void loop() {
  // Set the RTC interrupt alarm to wake the device in given time
  hypnos.setInterruptDuration(TimeSpan(DAY, HOUR, MINUTE, SECOND));

  // Wait for USFM to boot up before taking data
  manager.pause(DELAY);

  // Measure and package data
  manager.measure();
  manager.package();

  // Add the flow rate to the package
  manager.addData("Flow Meter", "Flow Rate", calcFlowRate());
  
  // Print the current JSON packet
  manager.display_data();            

  // Log the data to the SD card              
  hypnos.logToSD();

  // Reattach to the interrupt after we have set the alarm so we can have repeat triggers
  hypnos.reattachRTCInterrupt();

  // Put the device into a deep sleep, operation HALTS here until the interrupt is triggered
  hypnos.sleep(false);
}
