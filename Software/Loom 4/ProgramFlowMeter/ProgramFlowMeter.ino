/**
 * This Script is for a feather we keep in the lab to only power the USFM so it can be programmed without the Hypnos putting it to sleep
 * 
 * Does not log or measure data. Only enables Hypnos power rails, specfically the 12V rail.
 */
 
#include <Loom_Manager.h>

#include <Hardware/Loom_Hypnos/Loom_Hypnos.h>

Manager manager("Device", 1);

// Create a new Hypnos object setting the version to determine the SD Chip select pin, and starting without the SD card functionality
Loom_Hypnos hypnos(manager, HYPNOS_VERSION::V3_3, TIME_ZONE::PST);

void isrTrigger(){
  hypnos.wakeup();
}

void setup() {

  // Enable the hypnos rails
  hypnos.enable();

  // Called after enable
  manager.initialize();

  // Register the ISR and attach to the interrupt
  hypnos.registerInterrupt(isrTrigger);
}

void loop() {

}
