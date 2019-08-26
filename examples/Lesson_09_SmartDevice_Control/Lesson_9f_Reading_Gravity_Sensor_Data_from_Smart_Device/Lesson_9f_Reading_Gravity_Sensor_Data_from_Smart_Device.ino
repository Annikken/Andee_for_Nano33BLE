/************************************************************
  [Annikken Andee]
  ================
  Lesson 9f
  Reading Gravity Sensor Data from Smart devices!

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
AndeeHelper grav;
AndeeHelper xDisp;
AndeeHelper yDisp;
AndeeHelper zDisp;

//Initialising 3 variables to store the values from the smartdevice
float x;
float y;
float z;

//Initialising 2 more variables to check if the UI has been updated yet
int setgrav = 0;

void setup() {
  // put your setup code here, to run once:
  Andee.begin();// Setup communication between Annikken Andee and Arduino
  Andee.clear();// Clear the screen of any previous displays
  setInitialData();// Define object types and their appearance
}

void setInitialData()
{
  xDisp.setId(0); // Each object must have a unique ID number
  xDisp.setType(DATA_OUT);// Defines object as a display box
  xDisp.setLocation(0, 0, ONE_THIRD); // Sets the location and size of your object
  xDisp.setTitle("X-Axis");// Sets the title of the object
  xDisp.setData("");//Leave this empty since this will be filled up with data later on
  xDisp.setColor(LIGHT_BLUE);//Sets the colour of the UI object

  yDisp.setId(1);
  yDisp.setType(DATA_OUT);
  yDisp.setLocation(0, 1, ONE_THIRD);
  yDisp.setTitle("Y-Axis");
  yDisp.setData("");
  yDisp.setColor(BLUE);

  zDisp.setId(2);
  zDisp.setType(DATA_OUT);
  zDisp.setLocation(0, 2, ONE_THIRD);
  zDisp.setTitle("Z-Axis");
  zDisp.setData("");
  zDisp.setColor(DARK_BLUE);
  
  grav.setId(3);
  grav.setType(BUTTON_IN);
  grav.setLocation(1, 1, FULL);
  grav.setTitle("Grav Sensor On");
  grav.setColor(GREEN);
}

void loop() {
  // put your main code here, to run repeatedly: 
  if(grav.isPressed())
  {
    grav.ack();
    if(setgrav == 0)
    {
      Andee.gravInit(400, -1);//Initialise the gyro sensor output. 
      //400 is the interval between sensor readings and -1 is the amount of times. -1 means infinite
      setgrav = 1;
    }
    else
    {
      Andee.gravStop();//Stop the phone from sending Grav readings
      setgrav = 0;
    }     
  }
 
  
  Andee.getGravReading(&x, &y, &z); //this function saves the sensor reading into the variables x,y and z
  grav.update(2);//Each UI object has to be updated to appear in the smartdevice
  xDisp.setData(x,4);
  yDisp.setData(y,4);
  zDisp.setData(z,4);
  
  xDisp.update();
  yDisp.update();
  zDisp.update();
  //This will help from sending the Andee updates every loop to not flood the Andee with information
  delay(500);
}
