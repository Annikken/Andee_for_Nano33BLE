/************************************************************
  [Annikken Andee]
  ================
  Lesson 1a
  Creating Your First Display Boxes!

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
AndeeHelper objectA;
AndeeHelper objectB;
// We're creating two objects

// The setup() function is meant to tell Arduino what to do 
// only when it starts up.
void setup()
{
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
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
     Col: From 0 (left-most) to 3. There is no more scrolling
     Size: The following sizes are available for you to choose:
     FULL, HALF, ONE_THIRD, ONE_QUART, TWO_THIRD, THREE_QUART */
  objectA.setTitle("This goes to the title bar");
  objectA.setData("This goes to the data field");
  objectA.setUnit("This goes to the units field"); // Optional
    
  
  //// Let's draw the second object! ////////////////////////////////////////
  
  objectB.setId(1); // Don't forget to give it a unique ID number
  objectB.setType(DATA_OUT); // Another display box
  objectB.setLocation(1,0,FULL); // Second row, left-most, full size
  objectB.setTitle("Hello");
  objectB.setData("World!");
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{  
  objectA.update(); // Call update() to refresh the display on your screen
  objectB.update(); // If you forgot to call update(), your object won't appear

  // A short delay is necessary to give Andee time to communicate with the smartphone
  delay(500); 
}
