/************************************************************
  [Annikken Andee]
  ================
  Lesson 5a
  Create Date Input Button
  
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

// We'll need a date input button and a display box to provide feedback
AndeeHelper displaybox;
AndeeHelper dateInputButton;

int dd, mm, yyyy; // Variables to store user date input
char tempString[20]; // Used to store date as a string

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
  displaybox.setTitle("Your Selected Date");
  displaybox.setData("No Date Selected");
  
  dateInputButton.setId(1);
  dateInputButton.setType(DATE_IN); // Sets object as a date input button
  dateInputButton.setLocation(1,0,FULL);
  dateInputButton.setTitle("Select Date");
  
  // Optional Setting. When the user opens up the date picker, Andee will display this
  // date as the initial date to choose.
  dateInputButton.setDefaultDate(21, Aug, 2019); // Format: day, month, year
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  if( dateInputButton.isPressed() )
  { 
    dateInputButton.ack();
    
    // Display date picker and store it in the three variables - dd, mm, and yyyy
    dateInputButton.getDateInput(&dd, &mm, &yyyy); 
    
    // String the three variables into a single date string
    sprintf(tempString, "%02d/%02d/%02d", dd, mm, yyyy);
    displaybox.setData(tempString);
  }
  
  displaybox.update();
  dateInputButton.update();
  
  delay(100); // Always leave a short delay for Bluetooth communication
}
