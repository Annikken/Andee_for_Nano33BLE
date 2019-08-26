/************************************************************
  [Annikken Andee]
  ================
  Lesson 7b
  Using Sliders to Control RGB LEDs
  
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

// We'll need 6 buttons to increase/decrease the red, green
// and blue channels of our RGB LED
AndeeHelper displayCurrentColour;
AndeeHelper redSlider;
AndeeHelper greenSlider;
AndeeHelper blueSlider;

// To get various colours from the RGB LED, we'll need to connect 
// the LED to pins capable of analog output. On the Arduino Uno,
// pins capable of analog output are Pins 3, 5, 6, 9, 10.
// Do not use Pins 8, 11, 12, 13 (Arduino Uno layout) as Andee 
// is using them. Strange things will happen if you do.
const int pinR = 6; // Red is connected to pin 6
const int pinG = 5; // Green is connected to pin 5
const int pinB = 3; // Blue is connected to pin 3

// We'll need to store the intensity levels of each channel
// Note: Analog output is only capable of producing a range from 0 to 255
int r = 0; // Red channel
int g = 0; // Green channel
int b = 0; // Blue channel

char colour[9] = "00000000"; // The initial colour of our display

// The setup() function is meant to tell Arduino what to do 
// only when it starts up.
void setup()
{
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.clear();  // Clear the screen of any previous displays
  setInitialData(); // Define object types and their appearance
  
  pinMode(pinR, OUTPUT); // Set pins to output mode
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
}

// This is the function meant to define the types and the appearance of
// all the objects on your smartphone
void setInitialData()
{
  displayCurrentColour.setId(0); // Our display box
  displayCurrentColour.setType(DATA_OUT);
  displayCurrentColour.setLocation(0,0,FULL);
  displayCurrentColour.setTitle("Current Colour");
  displayCurrentColour.setTitleColor("00000000");  
  displayCurrentColour.setTitleTextColor("FF000000");  
  displayCurrentColour.setBaseColor("FFFFFFFF");
  displayCurrentColour.setTextColor("FF000000");
  displayCurrentColour.setData(colour); // Show the RGB colour code.
  displayCurrentColour.setUnit("Values are in hexadecimal format");

  // Draw red slider
  redSlider.setId(1);  
  redSlider.setType(SLIDER_IN); // Set object as a slider
  redSlider.setLocation(1, 0, FULL); // Sliders can only be full size
  redSlider.setTitle("Red Channel");
  redSlider.setSliderMinMax(0, 255);
  redSlider.setSliderInitialValue(r);  
  redSlider.setSliderNumIntervals(256); // Set as discrete slider
  redSlider.setSliderReportMode(ON_FINGER_UP); // Update values as you're moving
  redSlider.setActiveColor(THEME_RED); // Slider Active and Font colour
  redSlider.setBaseColor(THEME_RED_DARK); // Slider Base colour  
  
  // Draw green slider
  greenSlider.setId(2);  
  greenSlider.setType(SLIDER_IN); // Set object as a slider
  greenSlider.setLocation(2, 0, FULL); // Sliders can only be full size
  greenSlider.setTitle("Green Channel");
  greenSlider.setSliderMinMax(0, 255);
  greenSlider.setSliderInitialValue(g);  
  greenSlider.setSliderNumIntervals(256); // Set as discrete slider
  greenSlider.setSliderReportMode(ON_FINGER_UP); // Update values as you're moving
  greenSlider.setActiveColor(THEME_GREEN); // Slider Active and Font colour
  greenSlider.setBaseColor(THEME_GREEN_DARK); // Slider Base colour  

  // Draw blue slider
  blueSlider.setId(3);  
  blueSlider.setType(SLIDER_IN); // Set object as a slider
  blueSlider.setLocation(3, 0, FULL); // Sliders can only be full size
  blueSlider.setTitle("Blue Channel");
  blueSlider.setSliderMinMax(0, 255);
  blueSlider.setSliderInitialValue(b);  
  blueSlider.setSliderNumIntervals(256); // Set as discrete slider
  blueSlider.setSliderReportMode(ON_FINGER_UP); // Update values as you're moving
  blueSlider.setActiveColor(THEME_BLUE); // Slider Active and Font colour
  blueSlider.setBaseColor(THEME_BLUE_DARK); // Slider Base colour   
  }

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{  
  // Get the slider values of each channel colour
  redSlider.getSliderValue(&r); 
  greenSlider.getSliderValue(&g);
  blueSlider.getSliderValue(&b);
  
  // Output each channel value
  analogWrite(pinR, r); 
  analogWrite(pinG, g); 
  analogWrite(pinB, b);     
  
  if ( (r>=0 && r<=255) || (g>=0 && g<=255) || (b>=0 && b<=255) )
  {
    sprintf(colour, "FF%02X%02X%02X", r,g,b); // Convert to an ARGB colour code string
  }
  
  displayCurrentColour.setData(colour); // Show user the ARGB colour code on the smartphone
  displayCurrentColour.setColor(colour); // Show the actual color on screen
  displayCurrentColour.update(); // Update screen
  redSlider.update();
  greenSlider.update();
  blueSlider.update();
  
  delay(100); // Always leave a short delay for Bluetooth communication
}
