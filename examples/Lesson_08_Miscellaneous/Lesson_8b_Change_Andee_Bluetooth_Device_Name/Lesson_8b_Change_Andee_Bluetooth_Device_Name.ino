/************************************************************
  [Annikken Andee]
  ================
  Lesson 8b
  Change Andee Bluetooth Device Name
  
  Check out our Resources section for more information and 
  ideas on what you can do with the Annikken Andee!
  https://annikken.gitbooks.io/annikken-andee/content/

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/

// You only need to run this code once.
#include <Andee_for_Nano.h>

void setup()
{
  Andee.setName("My New Name");// Use this function and type the name you want here
  Andee.begin();
  Andee.clear();
}

void loop()
{
  // Disconnect user - there's nothing to do here anyway
  if(Andee.isConnected()) 
    Andee.disconnect();
}
