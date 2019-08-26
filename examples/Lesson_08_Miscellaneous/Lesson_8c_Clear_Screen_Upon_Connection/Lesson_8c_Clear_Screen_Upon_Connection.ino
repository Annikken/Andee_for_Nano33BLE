/************************************************************
  [Annikken Andee]
  ================
  Lesson 8
  Clearing Device Screen Upon Connection  
  
  A connection may be made when your sketch is running in the
  loop function. It may then be desirable to clear the screen
  when a new user connects to your program. This lesson uses
  Andee.isConnected() and some flags to instruct Andee to 
  clear the users screen when it connects.
    
  Check out our Resources section for more information and 
  ideas on what you can do with the Annikken Andee!
  https://annikken.gitbooks.io/annikken-andee/content/

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/
#include <Andee_for_Nano.h>

AndeeHelper ghost;

bool wasConnectedFlag; //boolean flag to keep track of previous connection status
int connection; //return value for Andee.isConnected().

//The Andee.isConnected() function returns CONNECTED, NOT_CONNECTED or TIMEOUT
//TIMEOUT = the function ended prematurely. You should call the function again

void setup(){
  Andee.begin();
  Andee.clear();
  
  ghost.setId(0);
  ghost.setType(DATA_OUT);
  ghost.setLocation(0,0,FULL);
  ghost.setTitle("Clear Screen Demo");
  ghost.setData("Now you see me");
}

void loop(){

  if(Andee.isConnected()){
    Andee.clear(); //Clear user's screen of any previous sketches from another Andee
    delay(1500);
   
  }
  ghost.update(); //show display on screen
  delay(1000);  
}
