#include <Arduino.h>
#include <stdlib.h>

#include <ArduinoBLE.h>

#include <Andee_for_Nano.h>

char Andee_NanoVersion[5] = {'0','.','0','.','1'};
char versionBuff[18];

int nameFlag = 0;
int buttonNumber = 24;
char buttonBuffer[50];

char sensorsBuffer[64];

char bleBuffer[256];
char readBuffer[READBUFFERMAX];
char readPartBuffer[READPARTBUFFERMAX];
int bufferCount = 0;
uint16_t current_len;
char phoneBuffer[64];
char sliderBuffer[MAXSLIDER][20];

char JoystickBufferX [4];
char JoystickBufferY [4];

bool AndeeConnected = false;
bool versionAndClear = false;
bool resetBLEFlag = true;
bool AndeeAlive = false;

bool dataLog = false;

AndeeClass Andee;

////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                    //
//                                          BLE Functions                                             //
//                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////

// create service
BLEService andeeService("516e7d03-c4ea-4103-9bd2-c560221a0c16");
// create counter characteristic
BLECharacteristic AndeeTx("516e7d04-c4ea-4103-9bd2-c560221a0c16", BLEWrite | BLEWriteWithoutResponse | BLENotify, PACKET_LEN);
BLECharacteristic AndeeRx("516e7d05-c4ea-4103-9bd2-c560221a0c16", BLEWrite | BLERead, PACKET_LEN);

//////////////////////BT Callbacks///////////////////
void blePeripheralConnectHandler(BLEDevice central)
{  
  Serial.println("Connected!");
  AndeeRx.setValue(0);
  AndeeTx.setValue(0);
}

void blePeripheralDisconnectHandler(BLEDevice central)
{
  Serial.println("Disconnected");
  Serial.println("Advertising!");
  
  AndeeRx.setValue(0);
  AndeeTx.setValue(0);
}

void characteristicSubscribed(BLEDevice central, BLECharacteristic characteristic) {
  // characteristic subscribed event handler
  AndeeConnected = true;
  versionAndClear = false;
  
  Serial.println(F("Characteristic event, subscribed"));
}

void characteristicUnsubscribed(BLEDevice central, BLECharacteristic characteristic) {
  // characteristic unsubscribed event handler
  AndeeConnected = false;
  resetBLEFlag = false;
  Serial.println(F("Characteristic event, unsubscribed"));
}

void read_callback(BLEDevice central, BLECharacteristic characteristic)
{
  unsigned int mLen;
  unsigned int rLen;
  char buffer[20];
  
  if(AndeeRx.written())
  {
	memset(buffer,0x00,20);

	mLen = strlen( (const char*) AndeeRx.value() );
	memcpy(buffer, (char*) AndeeRx.value() , mLen);
	buffer[mLen] = '\0';

	if(readPartBuffer[0] != 0x00)
	{
		memcpy(readPartBuffer+(strlen(readPartBuffer)),buffer,mLen);
	}
	else
	{
		memset(readPartBuffer,0x00,READPARTBUFFERMAX);				
		memcpy(readPartBuffer,buffer,mLen);			
	}
	
	// printHEX("readPartBuffer",readPartBuffer);
	rLen = strlen(readPartBuffer);
	
	for(unsigned int b = 0; b < rLen ;b++)
	{
		
		if(readPartBuffer[b] == END_TAG_REPLY)
		{
			
			readPartBuffer[b] = '\0';
			memset(readBuffer,0x00,READBUFFERMAX);
			memcpy(readBuffer,readPartBuffer,rLen);
			memset(readPartBuffer,0x00,READPARTBUFFERMAX);
		}
	}
  }  
}
/////////////////////////////////////////////////////

///////////////Send BT function/////////////////
void btSend(char* UI)
{
  char partialUI[PACKET_LEN + 1];
  int msgLen = 0;  
	
  msgLen = strlen(UI);
  if(AndeeConnected == true)
  {
    if(msgLen > PACKET_LEN)
    {
      for (int i = 0; i <= msgLen;)
      {
        memset(partialUI,0x00,PACKET_LEN + 1);
        memcpy(partialUI, UI + i, PACKET_LEN);
		// printHEX("BTMsg",partialUI);
        
		AndeeTx.writeValue((const char*)partialUI);
		delay(5);
        i = i + PACKET_LEN;
      }
    }
    else
    {
	  // printHEX("BTMsg",UI);
      AndeeTx.writeValue((const char*)UI);
	  delay(5);
    }	
	AndeeTx.setValue(0);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                    //
//                                          General Functions                                         //
//                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////

void printHEX(const char* title,char* buffer)
{
	int len = strlen(buffer);
	Serial.print(title);Serial.print(":");
	for(int i = 0; i< len; i++)
	{
		Serial.print(" ");
		Serial.print((unsigned char)buffer[i],HEX );
	}
	Serial.println("");
}
void printHEX(const char* title,char buffer)
{
	Serial.print(title);Serial.print(":");	
	Serial.print(" ");
	Serial.print((unsigned char)buffer,HEX );	
	Serial.println("");
}


void convertColor (const char* colorBuffer, char* outputColor)//converting color from 0 to 255 to 32 to 200
{
    int x = 0;
    char tmp[3];
    int colorByte;
    char newColor[5];

    for(x = 0; x < 4; x++ )
    {
        strncpy(tmp,colorBuffer + x*2,2);
		tmp[2] = '\0';
		
        sscanf(tmp,"%x",&colorByte);		
		
        colorByte = ((colorByte * 200) / 255) + 32;
        newColor[x] = (char)colorByte;
    }
	
    newColor[4] = '\0';
	strcpy(outputColor,newColor);   
}

void systemTime(void)
{
	char msgToPhone[18] = {START_TAG_COMMAND,TIMEEPOCH,END_TAG_COMMAND,0x00,0x00,0x00,
						   0x00,0x00,0x00,0x00,0x00,0x00,
						   0x00,0x00,0x00,0x00,0x00,0x00};
	btSend(msgToPhone);
	delay(5);
	memset(msgToPhone,0x00,18);
}

void floattostrf(char* dest, int decPlace, float source)
{
	snprintf(dest, 10,"%.*f",decPlace,source);
}
void floattostrf(char* dest, int decPlace, double source)
{
	snprintf(dest, 10,"%.*f",decPlace,source);
}

void versionNumber(void)
{
	memset(versionBuff,0x00,18);
	versionBuff[0] = START_TAG_VERSION;
	versionBuff[1] = '1';
	versionBuff[2] = SEPARATOR;
	versionBuff[3] = Andee_NanoVersion[0];
	versionBuff[4] = Andee_NanoVersion[1];
	versionBuff[5] = Andee_NanoVersion[2];
	versionBuff[6] = Andee_NanoVersion[3];
	versionBuff[7] = Andee_NanoVersion[4];
	versionBuff[8] = END_TAG_VERSION;
	versionBuff[9] = '\0';		
}



///////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                           //
//            				   Processing Replies from Smart Device      					 //
//                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////

void processReply()
{
	unsigned char pressBuffer = 0;
	if(readBuffer[0] != 0x00)
	{
		printHEX("readBuffer" , readBuffer);
	}	
	else
	{
		//Serial.println("no reply");
	}
	
	if (	readBuffer[0] == BUTTON_IN || readBuffer[0] == KEYBOARD_IN|| 
			readBuffer[0] == TIME_IN || readBuffer[0] == DATE_IN || readBuffer[0] == CIRCLE_BUTTON)		
	{
		buttonBuffer[0] = 'B';
		buttonBuffer[1] = 'U';
		 for(int i = 0;i< (buttonNumber+1);i++)
		{				
		
				
			if(buttonBuffer[((i*2)+2)] == readBuffer[2])
			{				
				sprintf(buttonBuffer + ((i*2)+3),"%c",(readBuffer[4]+16));
				break;
			}
			else if(buttonBuffer[((i*2)+2)] == 0x00)//
			{
				sprintf(buttonBuffer + ((i*2)+2), "%c%c",readBuffer[2],(readBuffer[4]+16));				
				break;
			}
		}
		
		if(readBuffer[0] == KEYBOARD_IN || readBuffer[0] == TIME_IN || readBuffer[0] == DATE_IN)
		{
			memset(phoneBuffer,0x00,64);
			int buffLen = strlen(readBuffer);			
			memcpy(phoneBuffer,readBuffer + 4,(buffLen - 4));
			// Serial.print("phoneBuffer:"); Serial.println(phoneBuffer);
		}
		memset(readBuffer,0x00,READBUFFERMAX);
		return;
	}
	else if(readBuffer[0] == SLIDER_IN)
	{
		char value[18];
		char iid;
		unsigned int i = 0;
		
		iid = readBuffer[2];
		
		for(unsigned int i = 0; i < strlen(readBuffer);i++)
		{
			if(readBuffer[i+4] != SEPARATOR)
			{
				value[i] = readBuffer[i+4];
			}
			else 
			{
				value[i]='\0';
				break;
			}				
		}
		
		for(i = 0;i<MAXSLIDER;i++)
		{
			if(sliderBuffer[i][0] == 0x00)
			{
				sliderBuffer[i][0] = iid;
				strcpy(sliderBuffer[i]+1,value);
				break;
			}
			else if(sliderBuffer[i][0] == iid)
			{
				memset(sliderBuffer[i],0x00,20);
				sliderBuffer[i][0] = iid;
				strcpy(sliderBuffer[i]+1,value);
				break;
			}
		}
    
		memset(readBuffer,0x00,READBUFFERMAX);
	}
	else if(readBuffer[0] == TIMEEPOCH)
	{
		char buffer[11];
		char gmtBuffer[6];		
		unsigned long sysTime;
		long gmt,dst;
		char *ptr,*dls;
		int pLen,dLen;
		
		ptr = strchr(readBuffer+2,SEPARATOR);
		ptr++;
				
		dls = strchr(ptr,SEPARATOR);
		dls++;
		if(ptr != NULL)
		{
			pLen = strlen(ptr);
			dLen = strlen(dls);
			strncpy(gmtBuffer,ptr,pLen-dLen);
			gmtBuffer[pLen - 1] = 0x00;
			gmt = atol(gmtBuffer);
		}
		else
		{
			gmt = 0.00;
		}		
		if(dls !=NULL)
		{
			dst = atol(dls);
		}
		else
		{
			dst = 0.00;
		}
		memcpy(buffer,readBuffer+2,10);
		sysTime = atol(buffer) + (gmt*3600) + (dst*3600) ;
		sprintf(phoneBuffer,"%ld",sysTime);
		//setTime(sysTime);
		memset(readBuffer,0x00,READBUFFERMAX);
	}
	else if(readBuffer[0] == JOYSTICK)
	{
		memset(phoneBuffer,0x00,64);
		memcpy(phoneBuffer,readBuffer,10);
		phoneBuffer[9] = '\0';
		memset(readBuffer,0x00,READBUFFERMAX);		
	}
	else if(readBuffer[0] == GYRO)
	{
		memset(sensorsBuffer,0x00,64);		
		memcpy(sensorsBuffer,readBuffer+2,strlen(readBuffer)-2);
		memset(readBuffer,0x00,READBUFFERMAX);
	}
	else if(readBuffer[0] == LAC)
	{
		memset(sensorsBuffer,0x00,64);		
		memcpy(sensorsBuffer,readBuffer+2,strlen(readBuffer)-2);
		memset(readBuffer,0x00,READBUFFERMAX);
	}
	else if(readBuffer[0] == GRAV)
	{
		memset(sensorsBuffer,0x00,64);		
		memcpy(sensorsBuffer,readBuffer+2,strlen(readBuffer)-2);
		memset(readBuffer,0x00,READBUFFERMAX);
	}
	else if(readBuffer[0] == GPS)
	{
		memset(sensorsBuffer,0x00,64);		
		memcpy(sensorsBuffer,readBuffer+2,strlen(readBuffer)-2);
		memset(readBuffer,0x00,READBUFFERMAX);
	}
	
	else if(readBuffer[0] == WATCH)
	{
		if (readBuffer[4] == 'T')
		{
			buttonBuffer[0] = 'B';
			buttonBuffer[1] = 'U';
			for(int i = 0; i < (buttonNumber+1); i++)
			{
					
				if(buttonBuffer[((i*2)+2)] == (readBuffer[2] + 32))
				{
					pressBuffer = buttonBuffer[((i*2)+3)];				
					pressBuffer++;
					sprintf(buttonBuffer + ((i*2)+3),"%c",pressBuffer);
					break;
				}		
				else if(buttonBuffer[((i*2)+2)] == 0x00)
				{
					sprintf(buttonBuffer + ((i*2)+2), "%c%c",(readBuffer[2] + 32),'1');
                    
					break;
				}				
			}
			if (readBuffer[2] == 76)
			{
				memset(phoneBuffer,0x00,64);
				int buffLen = 0;
				
				buffLen = strlen(readBuffer);
				readBuffer[buffLen-3] = '\0';			
				memcpy(phoneBuffer,readBuffer + 4,(buffLen - 1));			
				
			}
		}
		memset(readBuffer,0x00,READBUFFERMAX);
	}
	 else if(readBuffer[0] == 0x88)
	{
		AndeeAlive= true;
		memset(readBuffer,0x00,READBUFFERMAX);
		Serial.println("testing receivers");	
		return;
	}
	else if(readBuffer[0] == 0x00)
	{
		//ignore this state
	}
	else
	{
		memset(readBuffer,0x00,READBUFFERMAX);
		Serial.println("Command not recognised");
	} 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                    //
//                                        Annikken AndeeClass                                         //
//                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////

//

void AndeeClass::versionClear()
{
	if(versionAndClear == false)
	{
		delay(1000);
		Andee.sendVersion();	
		versionAndClear = true;
	}
}

bool AndeeClass::isConnected()
{
	/* if(AndeeAlive == true)
	{
		AndeeNRF52.sendVersion();
		AndeeAlive = false;
	}
	return AndeeConnected; */
	
	
	return AndeeConnected;    
}

void AndeeClass::broadcast()
{
	//BLECentral central = AndeeNRF52Peripheral.central();
}

void AndeeClass::resetBLE()
{
	if(resetBLEFlag == false)
	{		
		memset(buttonBuffer,0x00,50);
		memset(phoneBuffer,0x00,64);
		memset(readBuffer,0x00,READBUFFERMAX);
		memset(readPartBuffer,0x00,READPARTBUFFERMAX);
		memset(sliderBuffer,0x00,64);
		resetBLEFlag = true;
	}
}

void AndeeClass::begin()
{
	Serial.begin(9600);
	
	// begin initialization
    if (!BLE.begin()) {
	  Serial.println("starting BLE failed!");

	  while (1);
    }
	
	// set device name
	if (nameFlag == 0)
	{
		BLE.setLocalName("Andee Nano");
	}	
	// set the UUID for the service this peripheral advertises
	BLE.setAdvertisedService(andeeService);

	// add the characteristic to the service
	andeeService.addCharacteristic(AndeeTx);
	andeeService.addCharacteristic(AndeeRx);
	// add service
	BLE.addService(andeeService);

	// assign event handlers for connected, disconnected to peripheral
	BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
	BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

	// assign event handlers for characteristic
	AndeeRx.setEventHandler(BLEWritten, read_callback);
	AndeeTx.setEventHandler(BLESubscribed, characteristicSubscribed);
	AndeeTx.setEventHandler(BLEUnsubscribed, characteristicUnsubscribed);

	// start advertising
	BLE.advertise();
	
	memset(buttonBuffer,0x00,50);
	memset(phoneBuffer,0x00,64);
	memset(readBuffer,0x00,READBUFFERMAX);
	memset(readPartBuffer,0x00,READPARTBUFFERMAX);
	memset(sliderBuffer,0x00,64);
	
	versionNumber();
	
	Serial.println("Andee Nano Ready!");
}

void AndeeClass::poll()
{
	BLE.poll();
	processReply();
}

void AndeeClass::setName(const char* name)
{
	BLE.setLocalName(name);
	nameFlag = 1;
	delay(1);
}

void AndeeClass::clear()
{
	char msgToPhone[18] = {START_TAG_COMMAND, CLEAR, END_TAG_COMMAND, '\0', 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 	
	btSend(msgToPhone);
	delay(5);
	memset(msgToPhone,0x00,18);
}

void AndeeClass::getDeviceTime(int* h,int* m, int* s)
{
	systemTime();
	delay(50);
	
	/* *h = hour();
	*m = minute();
	*s = second(); */
}

void AndeeClass::getDeviceDate(int* d, int* m, int* y )
{
	systemTime();
	delay(50);
	
	/* *d = day();
	*m = month();
	*y = year(); */
}

long AndeeClass::getDeviceTimeStamp(void)
{
	long buffer;
	systemTime();
	delay(50);
	buffer = atol(phoneBuffer);
	return buffer;
}

void AndeeClass::sendVersion(void)
{	
	btSend(versionBuff);
	delay(5);	
}

void AndeeClass::disconnect(void){
    char buffer[4];
    memset(buffer,0x00,4);
    buffer[0] = START_TAG_COMMAND;
    buffer[1] = DC;
    buffer[2] = END_TAG_COMMAND;
    buffer[3] = '\0';
    
    btSend(buffer);
    delay(5);
}
/////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                         //
//                                      Text To Speech                                     //
//                                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////

void AndeeClass::textToSpeech(const char* speech, float speed, float pitch, char accent)
{
	char buffer[128];
	sprintf(buffer, "%c%c%c%s%c%.01f%c%.01f%c%c%c", START_TAG_COMMAND, TTS, SEPARATOR, speech, SEPARATOR, speed, SEPARATOR, pitch, SEPARATOR, accent, END_TAG_COMMAND);
	btSend(buffer);	
}

//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                               Control for Smartphone Camera                              //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

void AndeeClass::takePhoto(char cameraType, char autoFocus, char flash)
{
	char buffer[11];
	
	sprintf(buffer, "%c%c%c%c%c%c%c%c%c", START_TAG_COMMAND, CAMERA, SEPARATOR, cameraType, SEPARATOR,	autoFocus, SEPARATOR, flash, END_TAG_COMMAND);
	btSend(buffer);
	delay(2);
}
///////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                           //
//            Gyroscope, Linear Accelerometer, Gravity and GPS data from Smartphone          //
//                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////

void AndeeClass::gyroInit(int interval, int iteration)
{
	char buffer[64];	
	sprintf(buffer,"%c%c%c%c%d%c%d%c",START_TAG_COMMAND,GYRO,'0',SEPARATOR,interval,SEPARATOR,iteration,END_TAG_COMMAND);
	btSend(buffer);
	delay(50);
}

void AndeeClass::gyroStop()
{
	char buffer[6];	
	sprintf(buffer,"%c%c%c%c",START_TAG_COMMAND,GYRO,'1',END_TAG_COMMAND);
	btSend(buffer);
	delay(2);
}

void AndeeClass::getGyroReading(float* x,float* y,float* z)
{
	char* pEnd;	
	
	
	*x = strtod (sensorsBuffer, &pEnd);
	*y = strtod (pEnd, &pEnd);
	*z = strtod (pEnd, NULL);
	
	return;	
}

void AndeeClass::lacInit(int interval, int iteration)
{
	char buffer[64];	
	sprintf(buffer,"%c%c%c%c%d%c%d%c",START_TAG_COMMAND,LAC,'0',SEPARATOR,interval,SEPARATOR,iteration,END_TAG_COMMAND);
	btSend(buffer);
	delay(50);
}

void AndeeClass::lacStop()
{
	char buffer[6];	
	sprintf(buffer,"%c%c%c%c",START_TAG_COMMAND,LAC,'1',END_TAG_COMMAND);
	btSend(buffer);
	delay(2);
}

void AndeeClass::getLacReading(float* x,float* y,float* z)
{
	char* pEnd;	
	
	
	*x = strtod (sensorsBuffer, &pEnd);
	*y = strtod (pEnd, &pEnd);
	*z = strtod (pEnd, NULL);
	
	return;	
}

void AndeeClass::gravInit(int interval, int iteration)
{
	char buffer[64];	
	sprintf(buffer,"%c%c%c%c%d%c%d%c",START_TAG_COMMAND,GRAV,'0',SEPARATOR,interval,SEPARATOR,iteration,END_TAG_COMMAND);
	btSend(buffer);
	delay(50);
}

void AndeeClass::gravStop()
{
	char buffer[6];	
	sprintf(buffer,"%c%c%c%c",START_TAG_COMMAND,GRAV,'1',END_TAG_COMMAND);
	btSend(buffer);
	delay(2);
}

void AndeeClass::getGravReading(float* x,float* y,float* z)
{
	char* pEnd;	
	
	
	*x = strtod (sensorsBuffer, &pEnd);
	*y = strtod (pEnd, &pEnd);
	*z = strtod (pEnd, NULL);
	
	return;	
}

void AndeeClass::gpsInit(int interval, int iteration)
{
	char buffer[64];
	sprintf(buffer,"%c%c%c%c%d%c%d%c",START_TAG_COMMAND,GPS,'0',SEPARATOR,interval,SEPARATOR,iteration,END_TAG_COMMAND);
	btSend(buffer);
	delay(50);
}

void AndeeClass::gpsStop()
{
	char buffer[6];	
	sprintf(buffer,"%c%c%c%c",START_TAG_COMMAND,GPS,'1',END_TAG_COMMAND);	
	btSend(buffer);
	delay(2);
}

void AndeeClass::getGpsReading(float* x,float* y,float* z)
{
	char* pEnd;	
	
	
	*x = strtod (sensorsBuffer, &pEnd);
	*y = strtod (pEnd, &pEnd);
	*z = strtod (pEnd, NULL);
	
	return;	
}

void AndeeClass::changeScreen(int screen)
{	
	char buffer[8];
	sprintf(buffer,"%c%c%c%c%c%i%c",START_TAG_UIXYWH,WATCH,SEPARATOR,'C',SEPARATOR,screen,END_TAG_UIXYWH);	
	buffer[7] = '\0';
	btSend(buffer);
	delay(2);
}

void AndeeClass::showScreen()
{
	char buffer[6];
	sprintf(buffer,"%c%c%c%c%c",START_TAG_UIXYWH,WATCH,SEPARATOR,'S',END_TAG_UIXYWH);
	buffer[5] = '\0';
	btSend(buffer);
	delay(2);
}

void AndeeClass::hideScreen()
{
	char buffer[6] = {START_TAG_UIXYWH,WATCH,SEPARATOR,'H',END_TAG_UIXYWH};
	buffer[5] = '\0';
	btSend(buffer);
	delay(2);
}

void AndeeClass::textInput()
{
	char buffer[6] = {START_TAG_UIXYWH,WATCH,SEPARATOR,'X',END_TAG_UIXYWH};
	buffer[5] = '\0';
	btSend(buffer);
	delay(2);
}

void AndeeClass::sendSMS(const char* number,const char* message)
{
	char buffer[64];
	sprintf(buffer,"%c%c%c%s%c%s%c",START_TAG_COMMAND,SMS,SEPARATOR,number,SEPARATOR,message,END_TAG_COMMAND);
	btSend(buffer);
	delay(2);
}

void AndeeClass::vibrate()
{
	char buffer[5];
	sprintf(buffer,"%c%c%c",START_TAG_COMMAND,VIBRATE,END_TAG_COMMAND);
	btSend(buffer);
	delay(2);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                           //
//                              Annikken AndeeHelper Functions                              //
//                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////


void AndeeHelper::setId(int value)
{
	id = value + 32;
	
	sprintf(dataBuffer,"%c%c%c%c",0x20,0x20,0x20,0x20);
	sprintf(titleBuffer,"%c%c%c%c",0x20,0x20,0x20,0x20);
	sprintf(unitBuffer,"%c%c%c%c",0x20,0x20,0x20,0x20);
	
	convertColor("FF00FF00",titleBGBuffer);
	convertColor("FFFF0000",titleFontBuffer);
	convertColor("FF0000FF",bodyFontBuffer);
	convertColor("FF83A4C8",bodyBGBuffer);	
	
	sprintf(maxBuffer,"%s","   ");
	sprintf(minBuffer,"%s","   ");
	inputTypeBuffer = '0';
	subBuffer = '0';
	flashBuffer = '0';		
	
	if (value == WATCH_TITLE)
	{
		watchBuffer = '0';
	}	
	else if(value == WATCH_BUTTON1)
	{
		watchBuffer = '1';
	}
	else if(value == WATCH_BUTTON2)
	{
		watchBuffer = '2';
	}
	else if(value == WATCH_BUTTON3)
	{
		watchBuffer = '3';
	}
	else if(value == WATCH_BUTTON4)
	{
		watchBuffer = '4';
	}
}

void AndeeHelper::setType(char type)
{
	typeBuffer = type;
}

void AndeeHelper::setLocation(char row, char order, char span){	
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int w = 0;
	unsigned int h = 20;	
	
	if(row == 0)
	{
		y = 4;
	}
	else if(row == 1)
	{
		y = 28;
	}
	else if(row == 2)
	{
		y = 52;
	}
	else if(row == 3)
	{
		y = 76;
	}
	
	switch(span){
		case ONE_THIRD:
			if(order == 0)
			{
				x = 4;
				w = 30;
			}
			else if(order == 1)
			{
				x = 35;
				w = 30;
			}
			else if(order == 2)
			{
				x = 66;
				w = 30;
			}
			break;
		case TWO_THIRD:
			if(order == 0)
			{
				x = 4;
			}
			else if(order == 1)
			{
				x = 35;
			}
			else if(order == 2)
			{
				x = 66;
			}
			w = 61;
			break;
		case FULL:
			x = 4;
			w = 92;
			break;
		case ONE_QUART:
			if(order == 0)
			{
				x = 4;
			}
			else if(order == 1)
			{
				x = 28;
			}
			else if(order == 2)
			{
				x = 52;
			}
			else if(order == 3)
			{
				x = 76;
			}
			w = 20;
			break;
		case THREE_QUART:
			if(order == 0)
			{
				x = 4;
			}
			else if(order == 1)
			{
				x = 28;
			}
			else if(order == 2)
			{
				x = 52;
			}
			w = 68;
			break;
		case HALF:
			if(order == 0)
			{
				x = 4;
			}
			else if(order == 1)
			{
				x = 52;
			}
			w =44;
			break;
		default:
			//Serial.println("Andee UI Span ERROR");
			break;
	}
	memset(xywhBuffer,0x00,5);
	xywhBuffer[0] = x+32;
	xywhBuffer[1] = y+32;
	xywhBuffer[2] = w+32;
	xywhBuffer[3] = h+32;
	xywhBuffer[4] = '\0';
}

void AndeeHelper::setCoord(int x, int y, int w, int h)
{
	if(x > C_HLIMIT)
	{
		x = C_HLIMIT;
	}        
    else if(x < C_LLIMIT)
	{
		 x = C_LLIMIT;
	}
	
	
	if(y > C_HLIMIT)
	{
		y = C_HLIMIT;
	}        
    else if(y < C_LLIMIT)
	{
		 y = C_LLIMIT;
	}
	
	
	if(w > C_HLIMIT)
	{
		w = C_HLIMIT;
	}        
    else if(w < C_LLIMIT)
	{
		 w = C_LLIMIT;
	}
	
	
	if(h > C_HLIMIT)
	{
		h = C_HLIMIT;
	}        
    else if(h < C_LLIMIT)
	{
		 h = C_LLIMIT;
	}
		
	memset(xywhBuffer,0x00,5);
	xywhBuffer[0] = x+32;
	xywhBuffer[1] = y+32;
	xywhBuffer[2] = w+32;
	xywhBuffer[3] = h+32;
	//sprintf(xywhBuffer, "%02u%02u%02u%03u", x, y, w, h);
}

////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                               Input Mode and Sub Type                              //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////

void AndeeHelper::setInputMode(char type)
{
	if(type < 32)
		inputTypeBuffer = type + 48;
	
	else
		inputTypeBuffer = type;
	
}

void AndeeHelper::setSubType(char type)
{
	if(type < 32)
		subBuffer = type + 48;
	
	else
		subBuffer = type;
	
	
}

/////////////////////////////////////////////////////////////////////
//                                                                 //
//                           Set Colours                           //
//                                                                 //
/////////////////////////////////////////////////////////////////////



void AndeeHelper::setTitleColor(const char* color)
{
	memset(titleBGBuffer,0x00,5);
	convertColor(color,titleBGBuffer);	
}
void AndeeHelper::setTitleColor(char* color)
{
	memset(titleBGBuffer,0x00,5);
	convertColor(color,titleBGBuffer);	
}



void AndeeHelper::setTitleTextColor(const char* color)
{
	memset(titleFontBuffer,0x00,5);
	convertColor(color,titleFontBuffer);
}
void AndeeHelper::setTitleTextColor(char* color)
{
	memset(titleFontBuffer,0x00,5);
	convertColor(color,titleFontBuffer);
}


void AndeeHelper::setColor(const char* color)
{
	memset(bodyBGBuffer,0x00,5);
	convertColor(color,bodyBGBuffer);
} 
void AndeeHelper::setColor(char* color)
{
	memset(bodyBGBuffer,0x00,5);
	convertColor(color,bodyBGBuffer);
} 


void AndeeHelper::setTextColor(const char* color)
{
	memset(bodyFontBuffer,0x00,5);
	convertColor(color,bodyFontBuffer);
} 
void AndeeHelper::setTextColor(char* color)
{
	memset(bodyFontBuffer,0x00,5);
	convertColor(color,bodyFontBuffer);
} 

//////////////////////////////////////////////////////////////////////
//                                                                  //
//                  SetData, Unit, Title & MinMax                   //
//                                                                  //
//////////////////////////////////////////////////////////////////////

void AndeeHelper::setData(const char* data)
{
	memset(dataBuffer,0x00,64);	
	int l = strlen(data);
		
	if(l <= 0)
	{
		data = "     ";
		l = 5;
	}
	else if (l >= 62)
	{
		l = 62;
	}	
	snprintf(dataBuffer, l + 1, "%s", data);	
}

void AndeeHelper::setData(int data)
{		
	memset(dataBuffer,0x00,64);			
	sprintf(dataBuffer, "%d", data);
}

void AndeeHelper::setData(float data,char decPlace)
{		
	memset(dataBuffer,0x00,64);		
	floattostrf(dataBuffer,decPlace,data);
}

void AndeeHelper::setData(double data,char decPlace)
{		
	memset(dataBuffer,0x00,64);	
	floattostrf(dataBuffer,decPlace,data);
}



void AndeeHelper::setTitle(const char* title)
{
	memset(titleBuffer,0x00,32);	
	int l = strlen(title);
		
	if(l <= 0)
	{
		title = "     ";
		l = 5;
	}
	else if (l >= 30)
	{
		l = 30;
	}	
	snprintf(titleBuffer, l+1, "%s", title);
	Serial.print("titleBuffer");Serial.println(titleBuffer);
}

void AndeeHelper::setTitle(int title)
{
	memset(titleBuffer,0x00,32);	
	sprintf(titleBuffer, "%d", title);
}

void AndeeHelper::setTitle(float title, char decPlace)
{
	memset(titleBuffer,0x00,32);	
	floattostrf(titleBuffer,decPlace,title);
}

void AndeeHelper::setTitle(double title, char decPlace)
{
	memset(titleBuffer,0x00,32);	
	floattostrf(titleBuffer,decPlace,title);
}



void AndeeHelper::setUnit(const char* unit)
{
	memset(unitBuffer,0x00,32);    
	int l = strlen(unit);
		
	if(l <= 0)
	{
		unit = "     ";
		l = 5;
	}
	else if (l >= 30)
	{
		l = 30;
	}	
	snprintf(unitBuffer, l+1, "%s", unit);
}

void AndeeHelper::setUnit(int unit)
{
	memset(unitBuffer,0x00,32);	
	sprintf(unitBuffer, "%d", unit);
}

void AndeeHelper::setUnit(float unit, char decPlace)
{
	memset(unitBuffer,0x00,32);	
	floattostrf(unitBuffer,decPlace,unit);
}

void AndeeHelper::setUnit(double unit, char decPlace)
{
	memset(unitBuffer,0x00,32);	
	floattostrf(unitBuffer,decPlace,unit);
}

//////////////////////////////////////////////////////////////////////////////////////
//                                                                                  //
//                         Miscellaneous Data Functions                             //
//                                                                                  //
//////////////////////////////////////////////////////////////////////////////////////

void AndeeHelper::setMinMax(int min, int max)
{
	memset(minBuffer,0x00,10);	
	memset(maxBuffer,0x00,10);	
	sprintf(minBuffer,"%d", min);
	sprintf(maxBuffer,"%d", max);
}

void AndeeHelper::setMinMax(float min,float max,char decPlace)
{
	memset(minBuffer,0x00,10);	
	memset(maxBuffer,0x00,10);
	floattostrf(minBuffer,decPlace,min);
	floattostrf(maxBuffer,decPlace,max);
}

void AndeeHelper::setMinMax(double min,double max,char decPlace)
{
	memset(minBuffer,0x00,10);	
	memset(maxBuffer,0x00,10);
	floattostrf(minBuffer,decPlace,min);
	floattostrf(maxBuffer,decPlace,max);
}

/* void AndeeHelper::setKeyboardType(char type)
{
	inputTypeBuffer = type;
} */

void AndeeHelper::getKeyboardMessage(char* message)
{	
	int l = strlen(phoneBuffer);
	if(l >= 32)
	{
		snprintf(message, 32, "%s", phoneBuffer);
	}
	else
	{
		snprintf(message, l + 1, "%s", phoneBuffer);
	}	
	return;	
}

void AndeeHelper::setDefaultDate(int date, int month, int year)
{
	sprintf(dataBuffer,"%02i%02i%04i", date%31, month%12, year);
}

void AndeeHelper::getDateInput(int* d,int* m, int* y)
{
	*y = atoi(phoneBuffer+4);
	phoneBuffer[4] = 0x00;
	*m = atoi(phoneBuffer+2);
	phoneBuffer[2] = 0x00;
	*d = atoi(phoneBuffer);
	return;
}

void AndeeHelper::setDefaultTime(int hour,int min, int sec)
{
	sprintf(dataBuffer,"%02i%02i%02i", hour%24, min%60, sec%60);
}

void AndeeHelper::getTimeInput(int* h,int* m,int* s)
{
	*s = atoi(phoneBuffer+4);
	phoneBuffer[4] = 0x00;
	*m = atoi(phoneBuffer+2);
	phoneBuffer[2] = 0x00;
	*h = atoi(phoneBuffer);	
	return;
}


/////////////////////////////////////////////////////////////////////
//                                                                 //
//                            BUTTONS                              //
//                                                                 //
/////////////////////////////////////////////////////////////////////

int AndeeHelper::isPressed(void)
{
	// printHEX("buttonBuffer",buttonBuffer);
	for(int i = 0; i<buttonNumber;i++)
	{
		int pos = (i*2)+2;
		if(id == buttonBuffer[pos])
		{
			if(buttonBuffer[pos+1] > '0')
			{
				flashBuffer = buttonBuffer[pos+1];
				buttonBuffer[pos+1] = 0x30;				
				
				if(inputTypeBuffer == 48)
				{
					return '1';
				}
				else
				{
					return flashBuffer - '0';
				}				
			}
		}
	}
	flashBuffer = 0x00;
	return 0;
}
 void AndeeHelper::ack(void)
{	 
	if(bleBuffer[1] == BUTTON_IN)
	{
		if(inputTypeBuffer == '0')
		{
			char* buffer = new char[18];
			// buffer[0] = END_TAG_UIXYWH;
			buffer[0] = START_TAG_UIXYWH;
			buffer[1] = ACKN;
			buffer[2] = SEPARATOR;
			buffer[3] = id;
			buffer[4] = END_TAG_UIXYWH;
			buffer[5] = '\0';
			for(int l = 6; l < 18; l++)
			{
				buffer[l] = 0x00;
			}
			btSend(buffer);
		}

	}
	else
	{
		char* buffer = new char[18];
			// buffer[0] = END_TAG_UIXYWH;
			buffer[0] = START_TAG_UIXYWH;
			buffer[1] = ACKN;
			buffer[2] = SEPARATOR;
			buffer[3] = id;
			buffer[4] = END_TAG_UIXYWH;
			buffer[5] = '\0';
			for(int l = 6; l < 18; l++)
			{
				buffer[l] = 0x00;
			}		 
			btSend(buffer);
	}
}

int AndeeHelper::pressCounter()
{		
	return(int)(flashBuffer-48);
}

////////////////////////////////////////////////////////////////////////////////////
//                                                                                //
//                                   SLIDERS                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void AndeeHelper::setSliderInitialValue(int value)
{
	sprintf(dataBuffer,"%d",value);
	// sprintf(tempBuffer,"%d",value);
	flashBuffer = '1';
}

void AndeeHelper::setSliderInitialValue(float value,char decPlace)
{
	floattostrf(dataBuffer,decPlace,value);
	// floattostrf(tempBuffer,decPlace,value);
	flashBuffer = '1';
}
void AndeeHelper::setSliderInitialValue(double value,char decPlace)
{
	floattostrf(dataBuffer,decPlace,value);
	// floattostrf(tempBuffer,decPlace,value);
	flashBuffer = '1';
}

void AndeeHelper::setSliderNumIntervals(int numInterval)
{	
	if(numInterval <= 223 && numInterval >= 0)
	{
		subBuffer = numInterval + 32;
	}
	else if(numInterval > 223)
	{
		subBuffer = 255;
	}
	else if (numInterval <= 0)
	{
		subBuffer = 0;
	}
}

bool AndeeHelper::getSliderValue(int* x)
{
	char buffer[20];
	unsigned int i = 0;
	memset(buffer,0x00,20);
	
	for(i = 0; i < MAXSLIDER; i++)
	{
		if(sliderBuffer[i][0] == id)
		{
			strcpy(buffer,sliderBuffer[i] + 1);			
		}
	}
	
	if(buffer[0] == 0x00)
	{
		return false;
		// *x = atoi(tempBuffer);
	}		
	else
	{
		*x = atoi(buffer);
		setSliderInitialValue(*x);	
		return true;
	}		
}

bool AndeeHelper::getSliderValue(float* f, int decPlace)
{
	char buffer[20];
	unsigned int i = 0;
	memset(buffer,0x00,20);
	for(i = 0; i < MAXSLIDER; i++)
	{
		if(sliderBuffer[i][0] == id)
		{
			strcpy(buffer,sliderBuffer[i] + 1);
		}
	}
	if(buffer[0] == 0x00)
	{
		return false;
		// *f = atof(tempBuffer);
	}		
	else
	{		
		*f = atof(buffer);
		setSliderInitialValue(*f, decPlace);
		return true;
	}		
}

bool AndeeHelper::getSliderValue(double* d, int decPlace)
{
	char buffer[20];
	unsigned int i = 0;
	memset(buffer,0x00,20);
	for(i = 0; i < MAXSLIDER; i++)
	{
		if(sliderBuffer[i][0] == id)
		{
			strcpy(buffer,sliderBuffer[i] + 1);
		}
	}
	if(buffer[0] == 0x00)
	{
		return false;
		// *d = strtod(tempBuffer,NULL);
	}		
	else
	{
		*d = strtod(buffer,NULL);
		setSliderInitialValue(*d, decPlace);
		return true;
	}		
}

//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//                                    Joystick Control                                  //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

void AndeeHelper::getJoystick(int* x,int* y)
{
	char bufferX[4];
	char bufferY[4];
	
	for(int i = 0;i<3;i++)
	{
		bufferX[i] = phoneBuffer[i+3];
	}
	bufferX[3] = '\0';
	
	for(int j = 0;j<3;j++)
	{
		bufferY[j] = phoneBuffer[j+6];
	}
	bufferY[3] = '\0';
	 
	
	if(id == phoneBuffer[1])
	{
		*x = atoi(bufferX); //- (int)100;
		*y = atoi(bufferY); //- (int)100;		
	}	
}


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void AndeeHelper::update(unsigned int loop)
{
	Andee.poll();	
	Andee.versionClear();
	
	if(loop == 0 || loop == updateLoop)
	{
		memset(bleBuffer,0x00,256);
		if(Andee.isConnected())
		{
			switch(typeBuffer)
			{
				case DATA_OUT:		
					sprintf(bleBuffer , "%c%c%c%s%c%c%s%s%s%s%c%s%c%s%c%s%c", (START_TAG_UIXYWH),(DATA_OUT),(id), xywhBuffer,'0',SEPARATOR,titleBGBuffer,titleFontBuffer,bodyBGBuffer,bodyFontBuffer,SEPARATOR,	titleBuffer,SEPARATOR,unitBuffer,SEPARATOR,dataBuffer,(END_TAG_UIXYWH));
				break;
				
				case DATA_OUT_CIRCLE:
					sprintf(bleBuffer , "%c%c%c%s%c%c%s%s%s%c%s%c%s%c%s%c", (START_TAG_UIXYWH),(DATA_OUT_CIRCLE),(id), xywhBuffer,'0',SEPARATOR,titleFontBuffer,bodyBGBuffer,bodyFontBuffer,SEPARATOR,titleBuffer,SEPARATOR,unitBuffer,SEPARATOR,dataBuffer,(END_TAG_UIXYWH));
				break;
				
				case DATA_OUT_HEADER:
					sprintf(bleBuffer , "%c%c%c%s%c%c%s%s%c%s%c", (START_TAG_UIXYWH),(DATA_OUT_HEADER),(id), xywhBuffer,'0',SEPARATOR,bodyBGBuffer,bodyFontBuffer,SEPARATOR,titleBuffer,(END_TAG_UIXYWH));
				break;
				
				case BUTTON_IN:
					memcpy(titleBGBuffer,bodyBGBuffer,5);
					memcpy(titleFontBuffer,bodyFontBuffer,5);
					sprintf(bleBuffer, "%c%c%c%s%c%c%s%s%c%s%c",START_TAG_UIXYWH,BUTTON_IN, id,xywhBuffer,inputTypeBuffer,SEPARATOR,titleBGBuffer,titleFontBuffer,SEPARATOR,titleBuffer,END_TAG_UIXYWH);
				break;
				
				case CIRCLE_BUTTON:
					sprintf(bleBuffer, "%c%c%c%s%c%c%s%s%c%s%c",START_TAG_UIXYWH,CIRCLE_BUTTON, id,xywhBuffer,inputTypeBuffer,SEPARATOR,titleBGBuffer,titleFontBuffer,SEPARATOR,titleBuffer,END_TAG_UIXYWH);
				break;
				
				case ANALOG_DIAL_OUT:
					if (strcmp (maxBuffer," ") == 0)
					{
						sprintf(maxBuffer,"%s","255");
					}
					if (strcmp (minBuffer," ") == 0)
					{
						sprintf(minBuffer,"%c",'0');
					}
					sprintf(bleBuffer,"%c%c%c%s%c%c%s%s%c%s%c%s%c%s%c%s%c%s%c", START_TAG_UIXYWH, ANALOG_DIAL_OUT, id, xywhBuffer,'0', SEPARATOR, titleBGBuffer,bodyBGBuffer, SEPARATOR, titleBuffer, SEPARATOR, unitBuffer,SEPARATOR, dataBuffer, SEPARATOR, maxBuffer, SEPARATOR, minBuffer, END_TAG_UIXYWH);
				break;
				
				case KEYBOARD_IN:
					sprintf(bleBuffer, "%c%c%c%s%c%c%s%c%s%c%s%c", START_TAG_UIXYWH, KEYBOARD_IN, id, xywhBuffer, inputTypeBuffer,SEPARATOR, titleFontBuffer, SEPARATOR, titleBuffer, SEPARATOR, dataBuffer, END_TAG_UIXYWH);
				break;
				
				case DATE_IN:
					sprintf(bleBuffer, "%c%c%c%s%c%c%s%c%s%c%s%c", START_TAG_UIXYWH, DATE_IN,  id,xywhBuffer, inputTypeBuffer,SEPARATOR, titleFontBuffer, SEPARATOR, titleBuffer, SEPARATOR, dataBuffer, END_TAG_UIXYWH);
				break;
				
				case TIME_IN:
					sprintf(bleBuffer,"%c%c%c%s%c%c%s%c%s%c%s%c", START_TAG_UIXYWH, TIME_IN,  id,xywhBuffer, inputTypeBuffer,SEPARATOR, titleFontBuffer, SEPARATOR, titleBuffer, SEPARATOR, dataBuffer, END_TAG_UIXYWH);
				break;
				
				case SLIDER_IN:
					sprintf(bleBuffer, "%c%c%c%s%c%c%s%s%c%s%c%s%c%s%c%s%c%c%c%c%c", START_TAG_UIXYWH,SLIDER_IN, id,xywhBuffer,inputTypeBuffer,SEPARATOR,titleBGBuffer,bodyBGBuffer,SEPARATOR,titleBuffer,SEPARATOR,dataBuffer,SEPARATOR,maxBuffer,SEPARATOR,minBuffer,SEPARATOR,subBuffer,SEPARATOR,flashBuffer,END_TAG_UIXYWH);
				break;
				
				case JOYSTICK:
					sprintf(bleBuffer,"%c%c%c%s%c%c%s%s%s%c%s%c", START_TAG_UIXYWH,JOYSTICK, id,xywhBuffer,inputTypeBuffer,SEPARATOR,	titleBGBuffer,titleFontBuffer,bodyBGBuffer,SEPARATOR,titleBuffer,END_TAG_UIXYWH);
				break;
				
				case WATCH:
					sprintf(bleBuffer,"%c%c%c%c%c%s%s%c%s%c", START_TAG_UIXYWH,WATCH,SEPARATOR,watchBuffer,SEPARATOR,titleBGBuffer,titleFontBuffer,SEPARATOR,titleBuffer,END_TAG_UIXYWH);
				break;
				
				default:
					Serial.println("\nUnknown widget type!");
				break;
			}
			btSend(bleBuffer);
			// delay(5);
		}
		updateLoop = 1;
	}
	else
	{
		updateLoop++;
		delayMicroseconds(1);
	}
}

void AndeeHelper::update(){
	update(0);
}


void AndeeHelper::remove()
{
	char removeUI[6] = {START_TAG_UIXYWH, REMOVE, SEPARATOR, (char)id, END_TAG_UIXYWH, 0x00};
	
	btSend(removeUI);
	delay(2);
}



