/************************************************************
  [Annikken Andee]
  ================
  Advanced Lesson
  Count Up&Down Timer Example

  Check out our Resources section for more information and 
  ideas on what you can do with the Annikken Andee!
  https://annikken.gitbooks.io/annikken-andee/content/

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
  
************************************************************/
#include <Andee_for_Nano.h>

AndeeHelper timeDisplay;
AndeeHelper minPlus;
AndeeHelper minMinus;
AndeeHelper secPlus;
AndeeHelper secMinus;
AndeeHelper startButton;
AndeeHelper resetButton;
AndeeHelper modeButton;

void setup() {
  // put your setup code here, to run once:
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.clear();  // Clear the screen of any previous displays
  setInitialData();
}

void setInitialData()
{  
  timeDisplay.setId(0);
  timeDisplay.setType(DATA_OUT);
  timeDisplay.setLocation(0, 0, FULL);
  timeDisplay.setTitle("Count Down Timer");  
  timeDisplay.setData("00:00");
  timeDisplay.setUnit("MM:SS");
  timeDisplay.setColor(THEME_BLUE_DARK);  
  timeDisplay.setTitleColor(HOT_PINK);
  timeDisplay.setTextColor(WHITE);
  timeDisplay.setTitleTextColor(WHITE);

  minPlus.setId(1);
  minPlus.setType(BUTTON_IN);
  minPlus.setLocation(1, 0, ONE_QUART);
  minPlus.setTitle("Min +");
  minPlus.setInputMode(NO_ACK);
  minPlus.setColor(ORANGE);
  minPlus.setTextColor(WHITE);

  minMinus.setId(2);
  minMinus.setType(BUTTON_IN);
  minMinus.setLocation(1, 1, ONE_QUART);
  minMinus.setTitle("Min -");
  minMinus.setInputMode(NO_ACK);
  minMinus.setColor(PURPLE);
  minMinus.setTextColor(WHITE);

  secPlus.setId(3);
  secPlus.setType(BUTTON_IN);
  secPlus.setLocation(1, 2, ONE_QUART);
  secPlus.setTitle("Sec +");
  secPlus.setInputMode(NO_ACK);
  secPlus.setColor(ORANGE);
  secPlus.setTextColor(WHITE);

  secMinus.setId(4);
  secMinus.setType(BUTTON_IN);
  secMinus.setLocation(1, 3, ONE_QUART);
  secMinus.setTitle("Sec -");
  secMinus.setInputMode(NO_ACK);
  secMinus.setColor(PURPLE);
  secMinus.setTextColor(WHITE);

  startButton.setId(5);
  startButton.setType(BUTTON_IN);
  startButton.setLocation(2, 0, FULL);
  startButton.setTitle("Start");
  startButton.setInputMode(NO_ACK);
  startButton.setColor(GREEN);
  startButton.setTextColor(WHITE);

  resetButton.setId(6);
  resetButton.setType(BUTTON_IN);
  resetButton.setLocation(3, 0, HALF);
  resetButton.setTitle("Reset");
  resetButton.setInputMode(NO_ACK);
  resetButton.setColor(BLUE);
  resetButton.setTextColor(WHITE);

  modeButton.setId(7);
  modeButton.setType(BUTTON_IN);
  modeButton.setLocation(3, 1, HALF);
  modeButton.setTitle("Change Mode to Count UP");
  modeButton.setInputMode(NO_ACK);
  modeButton.setColor(DEEP_PINK);
  modeButton.setTextColor(WHITE);
}

char buffer[6];
char buffer2[6];

bool intermediate = false;   //to check if connection is temporarily broken
bool connected = false; //Andee is connected or not
int uiState = 0;        //whether UI is loaded
int timerState = 0;     //whether timer function is on or not 
int startState = 0;     //whether start button is in Play or Pause mode
int mode = 0;           //Track whether Count Up or Down is chosen
int disconnectCounter = 0;//prevent Arduino from entering an unconnected state instantly

int secCount = 0; //amount of secs set by user
int minCount = 0; //amount of mins set by user
int minUP = 0;    //amount of mins during Count Up
int secUP = 0;    //amount of secs during Count Up
          
unsigned long  currentMillis;//current Millis
unsigned long previousMillis;//previous value of Millis

const long interval = 1000;//1000 milliseconds
const int CountDOWN = 0;
const int CountUP = 1;


void loop() {
  Andee.poll();
  intermediate = Andee.isConnected();//check to see if Andee is connected with timeout of 200
  if(intermediate == 0)//if Andee does not seem to be connected
  {
    disconnectCounter++;//add a counter. We will check if Andee is connected again to be sure it is not disconnected temporarily
    if(disconnectCounter == 5)//if this value reaches 5, the Andee should be disconnected and not a broken connection
    {
      uiState = 0;          //reset this value to refresh the UI upon the next connection
      connected = false;   //When this is 0, the main loop will not run
      disconnectCounter = 0;//Reset the value to not trigger it again in the next run
      intermediate = false;     //just to reset
    }
  }
  else if(intermediate == true)
  {
    connected = true;     //Andee is connected
    disconnectCounter = 0;  //reset this value again to prepare for next run
  }  
  
  if(connected == true && uiState == 0)//if Andee is connected and uiState is low(UI not drawn)
  {
    for(int x = 0; x < 2; x++)
    {
      //To draw the UI once and then refresh for another 1 time upon connection      
      timeDisplay.update();
      minPlus.update();
      minMinus.update();
      secPlus.update();
      secMinus.update();
      startButton.update();
      resetButton.update();
      modeButton.update();
      uiState = 1;//This shows that the UI has been drawn, so no need to draw it again
    }
  }

  if(connected == 1 && uiState == 1)//If Andee is connected and UI is drawn
  {
    if(minPlus.isPressed() && timerState == 0)//If minPlus button is pressed and timer is not running
    {
      minCount++;       //add 1 to minCount's value
      if(minCount == 60)//if minCount is 60 (since clocks do not display 60)
      {
        minCount = 0;   //return the value back to 0
      }
      printTime();      //This function is located at the bottom of the sketch. All this function 
                        //does is to print the time out in a buffer and display it on the smartphone
    }
    if(minMinus.isPressed() && timerState == 0)
    {
      minCount--;       //subtract 1 from minCount's value
      if(minCount == -1)//once again clocks do not display a neg number so bring it back up
      {
        minCount = 59;
      }
      printTime();

    }
    if(secPlus.isPressed() && timerState == 0)
    {
      secCount++;       //similar to minPlus, except with seconds
      if(secCount == 60)
      {
        secCount = 0;
      }
      printTime();

    }
    if(secMinus.isPressed() && timerState == 0)
    {
      secCount--;       //similar to minMinus, except with seconds
      if(secCount == -1)
      {
        secCount = 59;
      }
      printTime();

    }
    
    if(modeButton.isPressed() && timerState == 0)//If mode change button is pressed when timer is off
    {
      if(mode == CountDOWN)//if the mode is already in count down mode
      {
        mode = CountUP;   //change to count up
        modeButton.setTitle("Change Mode to Count DOWN");//Change the necessary UI 
        timeDisplay.setTitle("Count Up Timer");
        modeButton.setColor(DARK_CYAN);
        modeButton.update();
        timeDisplay.update();
        minUP = 0;
        secUP = 0;
      }
      else if(mode == CountUP)//if the mode is already in count up mode
      {
        mode = CountDOWN;     //change to count up
        modeButton.setTitle("Change Mode to Count UP");
        timeDisplay.setTitle("Count Down Timer");
        timeDisplay.setUnit("MM:SS");
        modeButton.setColor(DEEP_PINK);
        modeButton.update();
        timeDisplay.update();
      }
    }

    if(resetButton.isPressed() && timerState == 0)//If reset button is pressed 
    {
      //reset all counting values
      minCount = 0;
      secCount = 0;
      minUP = 0;
      secUP = 0;
      timeDisplay.setUnit("MM:SS");
      printTime();//display 00:00      
    }

    if(startButton.isPressed())//start the timer
    {      
      timerState = 1;//changing this value to 1 will start the while loop below      
      startButton.setColor(RED);//change UI for start button
      startButton.setTitle("Stop");
      startButton.update(); 
    }
    
    previousMillis = millis();//store the current time in millis 

    while(timerState == 1)//while timerstate = 1, this loop will continue running until timerstate = 0;
    {    
      currentMillis = millis();   //measure the current time
      if(currentMillis - previousMillis >= interval)//if the previous time stored and the current time has 1000 milliseconds in difference
      {
        if(mode == CountDOWN)// if the timer is in count down mode
        {
          secCount--;        //decrease the seconds value
          if(secCount == -1)//clocks don't display neg numbers so change it
          {
            secCount = 59;
            minCount--;
          }
          printTime();
          if(minCount == 0 && secCount == 0)//if the time reached 00:00, then
          {
            timerState = 0;               //exit the timer while loop
            startButton.setColor(GREEN);  //change the start button UI
            startButton.setTitle("Start");
            startButton.update();
          }        
          previousMillis = currentMillis;//now the previousMillis will store the current time as reference
        }
        else if(mode == CountUP)// if the timer is in count up mode
        {      
          sprintf(buffer2,"%02d:%02d",minUP,secUP); //since the data is used to display the time to count up to,
          timeDisplay.setUnit(buffer2);             //display the value in the units of the display
          timeDisplay.update();

          secUP++;        //increase the seconds value by 1
          if(secUP == 60)//again, clocks don't display 60
          {
            secUP = 0;  //when seconds reach 60, reset value to 0
            minUP++;    // and increase minutes by 1
            sprintf(buffer2,"%02d:%02d",minUP,secUP);//display the time in units again
            timeDisplay.setUnit(buffer2);
            timeDisplay.update();
          }
          if(minUP == minCount && secUP == secCount)//if the count up has reached the user set value,
          {
            timerState = 0;               //exit the timer while loop
            sprintf(buffer2,"%02d:%02d",minUP,secUP);//display the time in units again
            timeDisplay.setUnit(buffer2);
            timeDisplay.update();
            startButton.setColor(GREEN);  //change the start button UI
            startButton.setTitle("Start");
            startButton.update();
          }
          
          previousMillis = currentMillis; //now the previousMillis will store the current time as reference   
        }
      }

      if(startButton.isPressed())//if the start button is pressed while in the while loop
      {
        //this means that the timer is on 
        timerState = 0;             //so the only logical thing to do is off the timer
        startButton.setColor(GREEN);//change the start button UI 
        startButton.setTitle("Start");
        startButton.update();
      }
    }      
  }
}

void printTime()// this is just a simple function that prints out the time and send it to the data buffer and to the andee
{
  sprintf(buffer,"%02d:%02d",minCount,secCount);
  timeDisplay.setData(buffer);
  timeDisplay.update();
}
