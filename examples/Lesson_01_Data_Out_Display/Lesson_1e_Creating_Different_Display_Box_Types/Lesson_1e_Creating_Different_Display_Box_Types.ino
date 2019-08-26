/************************************************************
  [Annikken Andee]
  ================
  Lesson 1e
  Changing the Display Box Types!

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

//We will be creating 3 different display box types
AndeeHelper displayBoxA;
AndeeHelper displayBoxB;
AndeeHelper displayBoxC;

// The setup() function is meant to tell Arduino what to do
// only when it starts up.
void setup()
{
  Andee.begin();
  Andee.clear();
  setInitialData();
}
void setInitialData()
{
  //You can have 3 different types of data displays
  //DATA_OUT, DATA_OUT_CIRCLE, DATA_OUT_TEXT

  displayBoxA.setId(0);
  displayBoxA.setType(DATA_OUT);
  displayBoxA.setLocation(0, 0, FULL);
  displayBoxA.setTitle("Title");
  displayBoxA.setData("Data");
  displayBoxA.setUnit("Unit");

  displayBoxB.setId(1);
  displayBoxB.setType(DATA_OUT_CIRCLE);//This creates a Circular Display
  displayBoxB.setLocation(1, 0, FULL);
  displayBoxB.setTitle("Title");
  displayBoxB.setData("Data-Circle Type");
  displayBoxB.setUnit("Unit");
  displayBoxB.setColor(RED);//Changing the colour of the Circle
  displayBoxB.setTextColor(WHITE);//Changing the colour of the text in the Body
  displayBoxB.setTitleTextColor(GREEN);//Changing the colour of the Title under the Body

  displayBoxC.setId(2);
  displayBoxC.setType(DATA_OUT_HEADER);
  //This creates a Title Display to only show title
  //This is useful for people who wants a title like widget
  displayBoxC.setLocation(2, 0, FULL);
  displayBoxC.setTitle("Title Type");//DATA_OUT_HEADER does not have Data or Units
  displayBoxC.setColor(RED);//Changing the colour of the Body
  displayBoxC.setTextColor(GREEN);//Changing the colour of the Text

}

void loop() {
    //Calling updates displays your boxes on screen.
  displayBoxA.update();
  displayBoxB.update();
  displayBoxC.update();
    
  delay(500);
}
