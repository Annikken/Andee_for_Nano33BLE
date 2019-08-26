/************************************************************
  [Annikken Andee]
  ================
  Lesson 2g
  Different Types of Buttons and Color Features
  
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
/*
We will be creating 4 different buttons. 
One will be a normal rectangular button with 
a solid background while th other will be a 
normal rectangular button with a transparent background.
The other 2 will be a circle button with solid background
while the other will be a circle button with transparent background
*/
AndeeHelper buttonRectSolid;
AndeeHelper buttonRect;
AndeeHelper buttonCircleSolid;
AndeeHelper buttonCircle;

void setup(){
  Andee.begin();
  Andee.clear();
  setInitialData();
}

void setInitialData(){ 
  buttonRectSolid.setId(0);
  buttonRectSolid.setType(BUTTON_IN);//The normal button type
  buttonRectSolid.setLocation(0,0, FULL);
  buttonRectSolid.setTitle("Rectangular Solid");
  buttonRectSolid.setColor(RED);//Set colour as usual
  buttonRectSolid.setTextColor(WHITE);

  buttonRect.setId(1);
  buttonRect.setType(BUTTON_IN);
  buttonRect.setLocation(1,0, FULL);
  buttonRect.setTitle("Rectangular Transparent");
  //For a transparent background, set Alpha to 00 as shown below
  buttonRect.setTextColor(BLUE);
  buttonRect.setColor("000000FF");//ARGB Hex
  /*
   You can generate your own colour hexcode by converting the various 
   Red, Green and Blue(RGB) values to HEX format (AARRGGBB).
   0 to 255 converts to 00 to FF. 
   You can use online calculators for that. 
   Once you have the decided color, you can change the Alpha to 00 for a transparent background
   */
  
  
  buttonCircleSolid.setId(2);
  buttonCircleSolid.setType(CIRCLE_BUTTON);//The circle button type
  buttonCircleSolid.setLocation(2,0, FULL);
  buttonCircleSolid.setTitle("Circle Solid");
  buttonCircleSolid.setColor(GREEN);
  buttonCircleSolid.setTextColor(PURPLE);

  buttonCircle.setId(3);
  buttonCircle.setType(CIRCLE_BUTTON);
  buttonCircle.setLocation(3,0, FULL);
  buttonCircle.setTitle("Circle Transparent");
  buttonCircle.setColor("00FF00FF");//Background colour works the same way for circle button
  buttonCircle.setTextColor("FFFF00FF");
}

void loop(){
  //Show buttons on screen
  buttonRect.update();
  buttonRectSolid.update();
  buttonCircle.update();
  buttonCircleSolid.update();
  
 //Handles if buttons are pressed.
  if(buttonRectSolid.isPressed()){
    buttonRectSolid.ack();
  }
  if(buttonRect.isPressed()){
    buttonRect.ack();
  }
  
  //Handle if buttonCircle is pressed.
  if(buttonCircleSolid.isPressed()){
    buttonCircleSolid.ack();
  }
  if(buttonCircle.isPressed()){
    buttonCircle.ack();
  }
  delay(200);
}
