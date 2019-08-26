/************************************************************
  [Annikken Andee]
  ================
  Lesson 9c
  Different variations for TTS
  
  Check out our Resources section for more information and 
  ideas on what you can do with the Annikken Andee!
  https://annikken.gitbooks.io/annikken-andee/content/

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/
#include <Andee_for_Nano.h>

AndeeHelper tts;

void setup(){
  Andee.begin();
  Andee.clear();

  tts.setId(0);
  tts.setLocation(0,0,FULL);
  tts.setType(DATA_OUT);
  tts.setTitle("TTS Demo");
  tts.setData("Different Ways of using TTS");
}

void loop(){  

  if(Andee.isConnected())
  {
    Andee.textToSpeech("A text to speech demonstration using Annikken Andee.",1.0,0.4,GREAT_BRITON);
    delay(4000);
      
    Andee.textToSpeech("Hello! I am from the states!",1.0,0.4,US);
    delay(4000);
    
    Andee.textToSpeech("Hello! I am from Great Britain!",1.0,0.4,GREAT_BRITON);
    delay(4000);
    
    Andee.textToSpeech("Hello! I am from Australia!",1.0,0.4,AUSTRALIA);
    delay(4000);  
    
    Andee.textToSpeech("Hello! I am from Ireland!",1.0,0.4,IRELAND);
    delay(4000);
    
    Andee.textToSpeech("Hello! I am from South Africa!",1.0,0.4,SOUTH_AFRICA);
    delay(4000);
    
    Andee.textToSpeech("Hello! I am speaking really slow.",0.0,1.0,GREAT_BRITON);
    delay(4000);
      
    Andee.textToSpeech("Hello! I am speaking really fast!",1.5,1.0,AUSTRALIA);
    delay(4000);
    
    Andee.textToSpeech("This is a high pitch voice!",1.0,2.0,US);
    delay(4000);
    
    Andee.textToSpeech("This is a low pitch voice!",1.0,0.5,GREAT_BRITON);
    delay(4000);
  }  
  
  tts.update();
  delay(100);
}
