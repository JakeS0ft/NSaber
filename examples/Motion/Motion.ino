#include "Arduino.h"
#include <NSaber.h>

#define SMALL_THRESHOLD   110
#define MEDIUM_THRESHOLD  200
#define LARGE_THRESHOLD   350
#define CLASH_THRESHOLD   32

//Tolerance threshold data for MPU6050 motion manager
MPU6050AdvancedTolData gToleranceData;

unsigned long gLastSwingTime;
unsigned long gLastClashTime;

/**
 * This function will test a motion manager. Run it in a loop and use
 * serial monitoring to see when motion events are triggered.
 */
void TestMotion(AMotionManager* apMotion)
{
  //Note: The Init() function must be called for changes
  //to clash tolerance to take effect. Must always
  //be called prior to calling Update() or the MPU6050
  //will remain in sleep mode.
  apMotion->Init();

  //Run the test
  while(true)
  {
    unsigned long lNow = millis();
    //Read the MPU6050 for data
    apMotion->Update();

    //Check for clashes, debounced a little
    if(apMotion->IsClash() && lNow - gLastClashTime > 100)
    {
      gLastClashTime = millis();
      Serial.print("CLASH detected:");

      //Note: This is generic motion manager test code
      //The class under test will only ever report
      //"Large" clashes.
      switch(apMotion->GetClashMagnitude())
      {
      case eeSmall:
        Serial.println("Small");
        break;
      case eeMedium:
        Serial.println("Medium");
        break;
      case eeLarge:
        Serial.println("Large");
        break;
      default:
        Serial.println("Unknown");
        break;
      }
    }
    //Check for swings. Debounce it a little so we don't get a ton of swing reports
    else if(apMotion->IsSwing() && lNow - gLastSwingTime > 100)
    {
      gLastSwingTime = millis();
      Serial.print("Swing detected:");
      switch(apMotion->GetSwingMagnitude())
      {
      case eeSmall:
        Serial.println("Small");
        break;
      case eeMedium:
        Serial.println("Medium");
        break;
      case eeLarge:
        Serial.println("Large");
        break;
      default:
        Serial.println("Unknown");
        break;
      }
    }
    else if(apMotion->IsTwist() && lNow - gLastSwingTime > 100)
    {
      gLastSwingTime = millis();
      Serial.println("Twist detected.");
    }
  }

}

void setup()
{
  /**
   * Set MPU6050 swing tolerance thresholds here.
   * Tweak as needed to adjust sensitivity
   */
  //Recommended swing tolerance values are between 512 and 50
  //The higher the number, the less sensitive
  gToleranceData.mSwingLarge  = LARGE_THRESHOLD;
  gToleranceData.mSwingMedium = MEDIUM_THRESHOLD;
  gToleranceData.mSwingSmall  = SMALL_THRESHOLD;
  gToleranceData.mTwist = LARGE_THRESHOLD;
  //Recommended the clash threshold be no higher than 64 and no lower than 10
  //The higher the number, the less sensitive
  gToleranceData.mClash = CLASH_THRESHOLD;

  Serial.begin(115200);
}

void loop()
{
  Serial.println("Testing MPU6050AdvancedMotionManager");
  AMotionManager* lpMotion = new Mpu6050AdvancedMotionManager(&gToleranceData);

  TestMotion(lpMotion);
  Serial.println("Test ends.");

  delete lpMotion;
  delay(2000);
}
