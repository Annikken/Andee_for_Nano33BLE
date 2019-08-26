/************************************************************
  [Annikken Andee]
  ================
  Lesson 8d
  Making the smartdevice vibrate using the Annikken Andee
  
  Check out our Resources section for more information and 
  ideas on what you can do with the Annikken Andee!
  https://annikken.gitbooks.io/annikken-andee/content/

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/

/*In this lesson, you will learn how to make the smartdevice vibrate.
Users can use the vibrate function to perform alerts*/

// Always include these libraries. Annikken Andee needs them
// to work with the Arduino!
#include <Andee_for_Nano.h>

// Every object that appears on your smartphone's screen
// needs to be declared like this:
AndeeHelper objectA;

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

  objectA.setId(0);  // Each object must have a unique ID number
  objectA.setType(DATA_OUT);  // This defines your object as a display box
  objectA.setLocation(0, 0, FULL); // Sets the location and size of your object
  /* setLocation (row, col, size)
     Row: From 0 (top-most) to 3
     Col: From 0 (left-most) to 9. If there are too many objects on that row, you can
          scroll from left to right.
     Size: The following sizes are available for you to choose:
     FULL, HALF, ONE_THIRD, ONE_QUART, TWO_THIRD, THREE_QUART */
  objectA.setTitle("Vibration");
  objectA.setData("This sketch is to test vibration on this device");  
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  if(Andee.isConnected())
  {
    Andee.vibrate();//Use this function to tell your connected device to vibrate
    delay(2000);
  }
    
  objectA.update(); // Call update() to refresh the display on your screen 
  delay(100);//delay for 2 seconds so that the vibration on the device occurs every 2 seconds
}
