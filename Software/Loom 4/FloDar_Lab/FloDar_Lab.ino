/**
 * This is an example use case for the FloDar project for lab use with no sleeping.
 * 
 * MANAGER MUST BE INCLUDED FIRST IN ALL CODE
 * 
 */

#include <Loom_Manager.h>

#include <Hardware/Loom_Hypnos/Loom_Hypnos.h>
#include <Sensors/I2C/Loom_ADS1115/Loom_ADS1115.h>
#include <Sensors/Loom_Analog/Loom_Analog.h>

// Wait to measure (milliseconds)
#define DELAY 10000

// Analog to Flowrate Conversion Formula Constants
#define MAXBITS     16384
#define MAXFLOWRATE 150

Manager manager("FloDar", 1);

// Create a new Hypnos object setting the version to determine the SD Chip select pin, and starting without the SD card functionality
Loom_Hypnos hypnos(manager, HYPNOS_VERSION::V3_3, TIME_ZONE::PST, false);
Loom_Analog analog(manager);
Loom_ADS1115 ads(manager, 0x48, true, false, adsGain_t::GAIN_TWOTHIRDS);

// Analog to Flowrate Conversion Formula
float calcFlowRate(){
    return (((float)ads.getAnalog(3) / MAXBITS) * MAXFLOWRATE);
}

void setup() {

  // Start the serial interface
  manager.beginSerial();

  // Enable the hypnos rails
  hypnos.enable();

  // Called after enable
  manager.initialize();

}

void loop() {
  // Wait for USFM to boot up before taking data
  delay(DELAY);

  // Measure and package data
  manager.measure();
  manager.package();

  // Add the flow rate to the package
  manager.addData("Flow Meter", "Flow Rate", calcFlowRate());
  
  // Print the current JSON packet
  manager.display_data();            

  // Log the data to the SD card              
  hypnos.logToSD();
}
