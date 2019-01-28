#include <sgpc3.h>

//  Create an instance of SGPC3
#ifdef ARDUINO_ARCH_ESP32
  SGPC3 SGCP3Sensor(22,21,200000);
#else
  SGPC3 SGCP3Sensor();
#endif

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  // First step is to initialize the sensors; this should only fail if
  // the board is defect, or the connection isn't working. Since there's nothing
  // we can do if this fails, the code will loop forever if an error is detected
  SGCP3Sensor.ultraLowPower();
  if (SGCP3Sensor.initSGPC3(LT_FOREVER) != 0) {
    Serial.print("Error while initializing sensors: ");
    Serial.print(SGCP3Sensor.getError());
    Serial.print("\n");
    while (1) { // loop forever
      delay(1000);
    }
  }
  // The SGP sensor has product type information and feature set stored
  // the following code reads it out, and prints it to the serial console.
  // This is purely to demo the function calls, and is not necessary to operate
  // the sensor
  int type = SGCP3Sensor.getProductType();
  int p_version = SGCP3Sensor.getVersion();
  Serial.print("\n");
  Serial.print("Type: ");
  Serial.println(type);
  Serial.print("Version: ");
  Serial.println(p_version);
}

void loop() {
  uint16_t tvoc = -1;

  // we'll start by triggering a measurement of the VOC/CO2 sensor;
  // it's important to do this first to make sure sleep timing is
  // correct. If the command succeeds, the local variables will
  // be set to the values we just read; if it fails, they'll be -1
  if (SGCP3Sensor.measureIAQ() != 0) {
    Serial.print("Error while measuring IAQ: ");
    Serial.print(SGCP3Sensor.getError());
    Serial.print("\n");
  } else {
    tvoc = SGCP3Sensor.getTVOC();
  }
  if (SGCP3Sensor.getBaseline() != 0) {
    Serial.print("Error while getting Baseline: ");
    Serial.print(SGCP3Sensor.getError());
    Serial.print("\n");
  } else {
    Serial.print("Baseline value: ");
    Serial.println(SGCP3Sensor.getBaselineValue());
  }
  

  // finally, let's print those to the serial console
  Serial.print("TVOC: ");
  Serial.println(tvoc);
  // and then, we'll use remainingWaitTimeMS() to ensure the correct
  // Measurement rate
  delay(SGCP3Sensor.remainingWaitTimeMS());
}
