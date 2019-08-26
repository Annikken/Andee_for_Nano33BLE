/************************************************************
  [Annikken Andee]
  ================
  Lesson 9d
  Take photos using the Andee as a remote control!

  Check out our Resources section for more information and
  ideas on what you can do with the Annikken Andee!
  https://annikken.gitbooks.io/annikken-andee/content/

  Contact us at andee@annikken.com if there are
  bugs in this sketch or if you need help with the
  Annikken Andee

************************************************************/

#include <Andee_for_Nano.h>

// We'll just create a display box to give us updates, and a button
// to trigger the camera.
AndeeHelper displaybox;
AndeeHelper button;
AndeeHelper camera; // This object will handle the smartphone camera

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
  // Let's draw the button!
  displaybox.setId(0); // Don't forget to assign a unique ID number
  displaybox.setType(DATA_OUT); // Defines object as a display box
  displaybox.setLocation(0,0,FULL);
  displaybox.setTitle("Andee Controlled Camera");
  displaybox.setData("Camera Ready");

  button.setId(1);
  button.setType(BUTTON_IN);
  button.setLocation(1,0,FULL);
  button.setTitle("Take Photo Now");
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  // You can substitute the button press with a sensor condition instead
  if( button.isPressed() )
  {
    button.ack();
    displaybox.setData("Taking Photo");
    Andee.takePhoto(CAM_DEFAULT, AUTO, OFF); // Call smartphone camera to take photo
    
    // Give the smartphone time to complete action. It will take
    // the camera at least 2 seconds to complete the entire
    // photo-taking process. This process may take even longer
    // on slower phones.
    delay(3000);
  }
  displaybox.update();
  button.update();
  
  delay(100); // Always leave a short delay for Bluetooth communication
  
  displaybox.setData("Camera Ready"); // Reset displaybox message
}
