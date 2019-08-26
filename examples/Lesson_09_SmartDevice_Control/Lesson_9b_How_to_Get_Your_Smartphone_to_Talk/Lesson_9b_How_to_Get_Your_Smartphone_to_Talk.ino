/************************************************************
  [Annikken Andee]
  ================
  Lesson 9b
  How to Get Your Smartphone to Talk
  
  Check out our Resources section for more information and
  ideas on what you can do with the Annikken Andee!
  https://annikken.gitbooks.io/annikken-andee/content/

  Contact us at andee@annikken.com if there are
  bugs in this sketch or if you need help with the
  Annikken Andee

************************************************************/

#include <Andee_for_Nano.h>

AndeeHelper displaybox;
AndeeHelper button;

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
  displaybox.setId(0);
  displaybox.setType(DATA_OUT);
  displaybox.setLocation(0,0,FULL);
  displaybox.setTitle("Text to Speech");
  displaybox.setData("Be sure to unmute your phone to hear your phone talk!");  
  displaybox.setTitleColor(DARK_BLUE);
  displaybox.setTitleTextColor(WHITE);
  displaybox.setColor(BLUE);
  displaybox.setTextColor(WHITE);
  
  button.setId(1); // Don't forget to assign a unique ID number
  button.setType(BUTTON_IN); // Defines object as a button
  button.setLocation(1,0,FULL);
  button.setTitle("Say something!");
  button.setColor(GREEN);
  button.setTextColor(WHITE);
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  if( button.isPressed() )
  {
    button.ack();
    Andee.textToSpeech("Hello World!", 1.0, 1.0, GREAT_BRITON);  
  }
  
  displaybox.update();
  button.update();
  
  delay(100); // Always leave a short delay for Bluetooth communication
}
