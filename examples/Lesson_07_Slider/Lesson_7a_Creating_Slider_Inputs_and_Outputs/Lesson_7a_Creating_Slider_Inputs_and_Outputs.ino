/************************************************************
  [Annikken Andee]
  ================
  Lesson 7a
  Creating Slider Inputs and Outputs
  
  Check out our Resources section for more information and 
  ideas on what you can do with the Annikken Andee!
  https://annikken.gitbooks.io/annikken-andee/content/

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/

// There are two types of slider inputs:
// - Discrete slider with fixed intervals (works best for integer values)
// - Continuous sliders (works best for float values)

// You can also use a slider input as a display!

// Always include these libraries. Annikken Andee needs them
// to work with the Arduino!
#include <Andee_for_Nano.h>

// We'll create a display and a button to show you how to
// program a button to do something
AndeeHelper buttonResetPosition;
AndeeHelper sliderDiscrete; // Discrete input slider
AndeeHelper sliderContinuous; // Continuous input slider
AndeeHelper sliderDisplay; // Input slider used as an output display

int sliderPosA;
float sliderPosB;
float sliderTotal;

// The setup() function is meant to tell Arduino what to do 
// only when it starts up.
void setup()
{
  sliderPosA = 128; // Just some arbitrary values for demo purposes
  sliderPosB = 50.0;
  sliderTotal = (float)sliderPosA + sliderPosB;
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.clear();  // Clear the screen of any previous displays
  setInitialData(); // Define object types and their appearance
}

// This is the function meant to define the types and the appearance of
// all the objects on your smartphone
void setInitialData()
{  
  // Let's draw a discrete slider! ///////////////////////////////////////
  sliderDiscrete.setId(0);
  sliderDiscrete.setType(SLIDER_IN); // Set object as a slider
  sliderDiscrete.setLocation(0, 0, FULL); // Sliders can only be full size
  sliderDiscrete.setTitle("Discrete Slider");
  
  // Slider settings:
  sliderDiscrete.setSliderMinMax(0, 200);
  // - setSliderMinMax(minimumValue, maximumValue, numberOfDecimalPlaces)
  // Set numberOfDecimalPlaces to 0 for integer values only.
 
  sliderDiscrete.setSliderInitialValue(sliderPosA);
  // Set slider to position 128
  
  sliderDiscrete.setSliderNumIntervals(200);
  // Set number of intervals in your slider.
  // Set 0 for continuous slider.
  // Set any number greater than 0 for discrete slider

  sliderDiscrete.setSliderReportMode(ON_VALUE_CHANGE);
  // Set when you want the smartphone to update Arduino
  // Options available:
  // - ON_VALUE_CHANGE : Updates Arduino every time your finger moves the slider
  // - ON_FINGER_UP    : Updates Arduino only after you lift up your finger from the slider
  
  sliderDiscrete.setActiveColor(RED); // set the Slider active color and font color
  sliderDiscrete.setBaseColor(DARK_RED); // Slider base colour
  
  // Let's draw a continuous slider! ///////////////////////////////////////
  sliderContinuous.setId(1);
  sliderContinuous.setType(SLIDER_IN);
  sliderContinuous.setLocation(1,0,FULL);
  sliderContinuous.setTitle("Continuous Slider");
  sliderContinuous.setSliderMinMax(0.0, 100.0, 2); // Display 2 decimal places
  sliderContinuous.setSliderInitialValue(sliderPosB);  // Set slider position to 50
  sliderContinuous.setSliderNumIntervals(0); // Set to 0 for continuous slider
  sliderContinuous.setSliderReportMode(ON_FINGER_UP);
  sliderContinuous.setActiveColor(BLUE);
  sliderContinuous.setBaseColor(DARK_BLUE);
  
  // Button to reset slider positions 
  buttonResetPosition.setId(2); // Don't forget to assign a unique ID number
  buttonResetPosition.setType(BUTTON_IN); // Defines object as a button
  buttonResetPosition.setLocation(2,0,FULL);
  buttonResetPosition.setTitle("Reset Position");
  buttonResetPosition.setBaseColor(THEME_TURQUOISE);
  buttonResetPosition.setTitleColor(THEME_TURQUOISE_DARK);  
  
  // You can also use sliders to display output ////////////////////////////
  sliderDisplay.setId(3);
  sliderDisplay.setType(SLIDER_IN);
  sliderDisplay.setLocation(3,0,FULL);
  sliderDisplay.setTitle("Sum of the Two Sliders");
  sliderDisplay.setSliderMinMax(0.0, 300.0, 2); // Display 2 decimal places
  sliderDisplay.setSliderInitialValue(sliderTotal);  // Set slider position to 178
  sliderDisplay.setSliderNumIntervals(0); // Set to 0 for continuous slider
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  if(sliderDiscrete.getSliderValue(&sliderPosA))//only update sliderTotal if there is new data at getSliderValue
  {
    sliderTotal = (float)(sliderPosA) + sliderPosB;
  }
  
  if(sliderContinuous.getSliderValue(&sliderPosB,2))//only update sliderTotal if there is new data at getSliderValue
  {
    sliderTotal = (float)(sliderPosA) + sliderPosB;
  } 

  sliderTotal = sliderPosA + sliderPosB;  

  if(sliderTotal >= 200) // You can even get it to update its colour
  {
    sliderDisplay.setActiveColor(THEME_GREEN_DARK);
    sliderDisplay.setBaseColor(THEME_GREEN);    
  }
  else if(sliderTotal < 100)
  {
    sliderDisplay.setActiveColor(THEME_RED_DARK);
    sliderDisplay.setBaseColor(THEME_RED);      
  }
  else if(sliderTotal < 200 && sliderTotal >= 100)
  {
    sliderDisplay.setActiveColor(THEME_PURPLE_DARK);
    sliderDisplay.setBaseColor(THEME_PURPLE);
  }      

  if( buttonResetPosition.isPressed() )
  {
    sliderPosA = 128;
    sliderPosB = 50;
    sliderTotal = sliderPosA + sliderPosB;
    buttonResetPosition.ack();
    
    sliderDiscrete.moveSliderToValue(sliderPosA);
    sliderContinuous.moveSliderToValue(sliderPosB);
  }

  sliderDisplay.moveSliderToValue(sliderTotal,2);//2 represents the number of decimal place needed

  sliderDisplay.update();
  sliderDiscrete.update();
  sliderContinuous.update();
  buttonResetPosition.update();
  delay(50); // Always leave a short delay for Bluetooth communication
}
