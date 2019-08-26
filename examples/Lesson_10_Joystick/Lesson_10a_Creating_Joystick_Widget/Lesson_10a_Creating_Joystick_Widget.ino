/************************************************************
  [Annikken Andee]
  ================
  Lesson 10a
  Creating a Joystick Widget!

  Check out our Resources section for more information and 
  ideas on what you can do with the Annikken Andee!
  https://annikken.gitbooks.io/annikken-andee/content/

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
  
************************************************************/

// Always include these libraries. Annikken Andee needs them
// to work with the Arduino!
#include <Andee_for_Nano.h>

// Every object that appears on your smartphone's screen
// needs to be declared like this:
AndeeHelper titleDisplay;
AndeeHelper joystick;
// We're creating two objects

int xAxis,yAxis; //Need 2 INT numbers to store the joystick values

// The setup() function is meant to tell Arduino what to do 
// only when it starts up.
void setup()
{
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.clear();  // Clear the screen of any previous displays
  setInitialData(); // Define object types and their appearance
}

// This is the function meant to define the types and the appearance of
// all the objects on your smartphone
void setInitialData()
{
  //// Let's draw the first object! //////////////////////////////////////////

  titleDisplay.setId(0);  // Each object must have a unique ID number
  titleDisplay.setType(DATA_OUT_HEADER);  // This defines your object as a display box
  titleDisplay.setLocation(0, 0, FULL); // Sets the location and size of your object
  titleDisplay.setTitle("Joystick Example");
  titleDisplay.setTextColor(WHITE);
  titleDisplay.setColor(DARK_BLUE);
    
  
  //// Let's draw the second object! ////////////////////////////////////////
  
  joystick.setId(1); // Don't forget to give it a unique ID number
  joystick.setType(JOYSTICK); // Another display box
  joystick.setLocation(1,0,FULL); // Second row, left-most, full size
  joystick.setYAxisMax(100);
  joystick.setXAxisMax(100);
  joystick.setActiveColor(GREEN);// set the color of the "thumbstick"
  joystick.setBaseColor(RED);// set the color of the joystick base
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  joystick.getJoystick(&xAxis,&yAxis);// this function stores the value of the joystick 

  Serial.print("xAxis:");Serial.println(xAxis);
  Serial.print("yAxis:");Serial.println(yAxis);
  
  titleDisplay.update(); // Call update() to refresh the display on your screen
  joystick.update(); // If you forgot to call update(), your object won't appear
  
  // A short delay is necessary to give Andee time to communicate with the smartphone
  delay(50); 
}
