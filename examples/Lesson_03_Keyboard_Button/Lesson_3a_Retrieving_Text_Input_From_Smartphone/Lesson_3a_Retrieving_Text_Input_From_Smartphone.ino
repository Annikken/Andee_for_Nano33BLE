/************************************************************
  [Annikken Andee]
  ================
  Lesson 3a
  Retrieving Text Input from Smartphone
  
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

// We'll create a text input button and a display box to display
// the user input
AndeeHelper displaybox;
AndeeHelper textInputButton;
char userInput[32];

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
  displaybox.setId(0);
  displaybox.setType(DATA_OUT);
  displaybox.setLocation(0,0,FULL);
  displaybox.setTitle("Here's what you wrote");
  
  textInputButton.setId(1);
  textInputButton.setType(KEYBOARD_IN); // Sets object as a text input button
  textInputButton.setLocation(1,0,FULL);
  textInputButton.setTitle("Type something!");
  textInputButton.setInputMode(ALPHA_NUMERIC); 
  /* Sets the type of keyboard to show to the user
  Options available are:
    - ALPHA_NUMERIC - Alphanumeric keyboard.
    - ALPHA_NUMERIC_PW - Alphanumeric keyboard, input as password.
    - NUMERIC - Android OS numeric keyboard.
    - NUMERIC_PW - Android OS numeric keyboard, input as password.    
  */
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  if( textInputButton.isPressed() )
  {
    memset(userInput, 0x00, 32); // Empty the contents of the string before receiving user input
    textInputButton.ack(); // Acknowledge button press or else phone will be left waiting
    textInputButton.getKeyboardMessage(userInput); // Display keyboard and store input into userInput
    displaybox.setData(userInput); // Display user input on the display box
  }
  
  displaybox.update(); // Always remember to update so that new content will be displayed
  textInputButton.update();
  
  delay(200); // Always leave a short delay for Bluetooth communication
}
