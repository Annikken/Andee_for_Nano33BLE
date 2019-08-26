/************************************************************
 [Apple Watch Lesson 1]
 ================
 Create a Toggle Button on your Apple Watch
 Objective:
 1) Learn how to create a UI on your Apple Watch
 2) Learn the Diff from creating a UI on your Phone VS on your Watch
 
 Note:
 Because Apple Watch do not allow dynamically creation of UI, 
 we need to use an Existing UI template and modified it

 
 Check out our Resources section for more information and 
 ideas on what you can do with the Annikken Andee!
 https://annikken.gitbooks.io/annikken-andee/content/
  
 Contact us at andee@annikken.com if there are 
 bugs in this sketch or if you need help with the 
 Annikken Andee
************************************************************/
#include <Andee_for_Nano.h>

// Apple Watch UI
AndeeHelper WatchTitle; // This will be used to set the Title on the Watch UI
AndeeHelper WatchButton1; // This will be used to set the attribute and behavior the Button on the Watch UI

// iPhone UI
AndeeHelper EnableWatch; // Press this button on iPhone to create the Apple Watch UI

bool WatchButtonState; // Watch Button State (on or off)

int state; // Program State
// state == 0  -> Wait for User Button Press on Phone
// state == 1  -> Create UI on Apple Watch

bool spamupdate; // Will only update UI upon Connection

void setup()
{
  state = 0;
  spamupdate = true;
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.clear();  // Clear the screen of any previous displays
  setup_AndeeWatch(); // Setup the UI on the Watch
  setup_AndeePhone(); // Setup the UI on the iPhone
  pinMode(13, OUTPUT); // control the LED
}

void loop()
{

  if (state == 0) {
    // Program State == 0
    // Create a Button on the PHONE
    // UI on the Apple watch will only be create when this button is pressed
    if (EnableWatch.isPressed()) {
      EnableWatch.ack();
      state = 1; // Change Program State to 1
      spamupdate = true;
    }
    EnableWatch.update();
  }

  else if (state == 1) {
    // Program State == 0
    // Apple Watch UI
    if (spamupdate == true) {
      // Create the Watch UI
      // This segment of codes will only be called once
      
      spamupdate = false;// Set to false so that this segment of codes will not be called again
      setup_AndeeWatch();
      
      Andee.hideScreen();// Hide the previous Watch UI Screen if Any
      delay(100);
      
      // Select the Screen you want on the UI
      // Refer to the lesson page for the graphical representation of the UI
      // 1 Button UI -> "WATCH_1BUTTON_A", "WATCH_1BUTTON_B"
      // 2 Button UI -> "WATCH_2BUTTON_A", "WATCH_2BUTTON_B"
      // 3 Button UI -> "WATCH_3BUTTON_A", "WATCH_3BUTTON_B"
      // 4 Button UI -> "WATCH_4BUTTON_A", "WATCH_4BUTTON_B"
      // Game Pad UI -> "WATCH_GAMEPAD"
      
      Andee.changeScreen(WATCH_1BUTTON_A); 
      delay(100);
      WatchTitle.update(); //Set the Title Text
      WatchButton1.update(); //Set the Button 1 Text and Color
      delay(100);
      Andee.showScreen(); // Show the UI on your Apple Watch

    }

    if (WatchButton1.isPressed()) {// Button on Watch is Pressed
      WatchButton1.ack();
      WatchButtonState = !WatchButtonState; // Toggle State

      if (WatchButtonState == true) {
        WatchButton1.setWatchText("Turn OFF");
        WatchButton1.setWatchColor("FFFF0000");
        digitalWrite(13, HIGH);// Turn On LED
      }
      else if (WatchButtonState == false) {
        WatchButton1.setWatchText("Turn ON");
        WatchButton1.setWatchColor("FF00FF00");
        digitalWrite(13, LOW);//Turn OFf LED
      }
      WatchButton1.update(); // Button1 Text and Color Change

    }
  }
  delay(250);
}

void setup_AndeeWatch()
{
  WatchButtonState = false;
  
  
  // This Andee Helper is for the Watch Title
  // do not set your own arbitrary Id value 
  WatchTitle.setId(WATCH_TITLE); 
  WatchTitle.setType(WATCH); // For Watch UI
  WatchTitle.setWatchText("Single Button"); // Change the Title text to "Single Button"
  
  // This Andee Helper is for the Button
  // do not set your own arbitrary Id value
  WatchButton1.setId(WATCH_BUTTON1);
  WatchButton1.setType(WATCH);

  // Set the Button Text as "Turn On" and Color to AARRGGBB "FF7959FF"
  WatchButton1.setWatchText("Turn ON");
  WatchButton1.setWatchColor("FF00FF00");
}

void setup_AndeePhone()
{
  // Setup the UI On the iOS device
  // Do not setId from (44 - 49)
  // use 0 - 43
  EnableWatch.setId(10);
  EnableWatch.setType(BUTTON_IN);
  EnableWatch.setLocation(0, 0, FULL);
  EnableWatch.setTitle("Enable Watch");
}
