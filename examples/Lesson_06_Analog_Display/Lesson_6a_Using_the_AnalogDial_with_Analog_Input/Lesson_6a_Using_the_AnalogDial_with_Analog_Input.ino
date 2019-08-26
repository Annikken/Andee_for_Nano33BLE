/************************************************************
  [Annikken Andee]
  ================
  Lesson 6a
  Displaying Raw Analog Input Using AnalogDial
  
  Check out our Resources section for more information and 
  ideas on what you can do with the Annikken Andee!
  https://annikken.gitbooks.io/annikken-andee/content/

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/
// Works with any sensor. You can use the Light Dependent Resistor
// to try this out!

// Always include these libraries. Annikken Andee needs them
// to work with the Arduino!
#include <Andee_for_Nano.h>

// We'll creating one object to display the analog input signal
AndeeHelper analogDial;

// We'll use Analog Input Pin A0 to read our analog input.
// Change the pin number if you are using another pin.
const int analogInputPin = A0;

//Setting the min and max values for the dial. This affects the look of the dial
const int minValue = 0; //dial's min value
const int maxValue = 255;//dial's max value

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
  // Only one display box this time
  analogDial.setId(0);
  analogDial.setType(ANALOG_DIAL_OUT);
  analogDial.setLocation(0,0,FULL);
  
  //Values can be double/float.
  //E.g. analogDial.setMinMax(0.1, 100.1);
  analogDial.setMinMax(minValue, maxValue);
  
  analogDial.setTitle("Analog dial");
  
  //These functions set different colors to the dial
  analogDial.setBaseColor(LIME_GREEN);//Set the base color
  analogDial.setActiveColor(GREEN);//Set both the moving dial and font color
  analogDial.setData(0);  
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  // Read value from analog pin and store it in an int variable
  int reading = analogRead(analogInputPin);  
   
  analogDial.setData(reading); //show dial and update value on screen
  analogDial.update();
  
  // A short delay is necessary to give Andee time to communicate with the smartphone
  delay(100); 
}
