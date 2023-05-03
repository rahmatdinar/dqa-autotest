#include <Arduino.h>
#include "PinDefinitionsAndMore.h" //Define macros for input and output pin etc.
#include <IRremote.hpp>
#include <EasyButton.h>

#define STOP_FROM_BEGINING 1
// #define TESTING_MODE

#define CUSTOMCODE_FACT_L 0xEE // 0x11 // 0xFF // 0x11 //0xFF// 0xFB
#define CUSTOMCODE_FACT_H 0xFF // 0x00 // 0xEE // 0x00 //0xEE// 0x04

#define CUSTOMCODE_L 0x7F // 0xDF // 0x20 // 0x04 (V)
#define CUSTOMCODE_H 0x80 // 0x20 // 0xDF // 0xFB (V)

const unsigned char IrCommandTypeNECSmartTV[] =
    {
        0x5A, //  IRKEY_POWER
        0x1B, //  IRKEY_NUM_0
        0x00, //  IRKEY_NUM_1
        0x10, //  IRKEY_NUM_2
        0x11, //  IRKEY_NUM_3
        0x13, //  IRKEY_NUM_4
        0x14, //  IRKEY_NUM_5
        0x15, //  IRKEY_NUM_6
        0x17, //  IRKEY_NUM_7
        0x18, //  IRKEY_NUM_8
        0x19, //  IRKEY_NUM_9
        0x4A, //  IRKEY_SELECT
        0x49, //  IRKEY_LEFT
        0x4B, //  IRKEY_RIGHT
        0x47, //  IRKEY_UP
        0x4D, //  IRKEY_DOWN
        0x4E, //  IRKEY_MENU
        0x06, //  IRKEY_BACK
        0X0F, //  IRKEY_HOME
        0X1E, //  IRKEY_SETTING
        0X30, //  IRKEY_NETFLIX
        0X34, //  IRKEY_GOOGLE_PLAY
        0X31, //  IRKEY_YOUTUBE
        0X37, //  IRKEY_MOLA
        0X03, //  IRKEY_VOLUME_UP
        0X41, //  IRKEY_VOLUME_DOWN
        0X02, //  IRKEY_CHANNEL_UP
        0X09, //  IRKEY_CHANNEL_DOWN
        0X53, //  IRKEY_MUTE
        0X40, //  IRKEY_SOURCE
        0X5C, //  IRKEY_RED
        0X5D, //  IRKEY_GREEN
        0X5E, //  IRKEY_YELLOW
        0X5F, //  IRKEY_BLUE
        0X45, //  IRKEY_PLAY
        0X08, //  IRKEY_PAUSE
        0X5B, //  IRKEY_STOP
        0X12, //  IRKEY_PREVIOUS
        0X51, //  IRKEY_NEXT
        0X43, //  IRKEY_TEXT
        0X42, //  IRKEY_SUBTITLE
        0X0B, //  IRKEY_AUDIO
        0X07, //  IRKEY_INFO
        0X48, //  IRKEY_GUIDE
        0X21, //  IRKEY_GOOLE_VOICE
        0x45, //  IRKEY_FAC_DIRECT_FACTORY_MENU
        0x0F, //  KEY_FACT_DIRECT_FACTORY_SETTING
        0x5C, //  KEY_FACT_DIRECT_VOL_80
        0x5D, //  KEY_FACT_DIRECT_VOL_INC10
        0x5E, //  KEY_FACT_DIRECT_VOL_DEC10

};

typedef enum
{
    KEY_POWER = 0,
    KEY_NUM_0,
    KEY_NUM_1,
    KEY_NUM_2,
    KEY_NUM_3,
    KEY_NUM_4,
    KEY_NUM_5,
    KEY_NUM_6,
    KEY_NUM_7,
    KEY_NUM_8,
    KEY_NUM_9,
    KEY_SELECT,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_MENU,
    KEY_BACK,
    KEY_HOME,
    KEY_SETTING,
    KEY_NETFLIX,
    KEY_GOOGLE_PLAY,
    KEY_YOUTUBE,
    KEY_MOLA,
    KEY_VOLUME_UP,
    KEY_VOLUME_DOWN,
    KEY_CHANNEL_UP,
    KEY_CHANNEL_DOWN,
    KEY_MUTE,
    KEY_SOURCE,
    KEY_RED,
    KEY_GREEN,
    KEY_YELLOW,
    KEY_BLUE,
    KEY_PLAY,
    KEY_PAUSE,
    KEY_STOP,
    KEY_PREVIOUS,
    KEY_NEXT,
    KEY_TEXT,
    KEY_SUBTITLE,
    KEY_AUDIO,
    KEY_INFO,
    KEY_GUIDE,
    KEY_GOOLE_VOICE,
    KEY_START_FACTORY,
    KEY_FAC_DIRECT_FACTORY_MENU = KEY_START_FACTORY,
    KEY_FACT_DIRECT_FACTORY_SETTING,
    KEY_FACT_DIRECT_VOL_80,
    KEY_FACT_DIRECT_VOL_INC10,
    KEY_FACT_DIRECT_VOL_DEC10,
    KEY_NULL = 0xFF,
} EN_KEY;

typedef struct
{
    unsigned int bit_0 : 1;
    unsigned int bit_1 : 1;
    unsigned int bit_2 : 1;
    unsigned int bit_3 : 1;
    unsigned int bit_4 : 1;
    unsigned int bit_5 : 1;
    unsigned int bit_6 : 1;
    unsigned int bit_7 : 1;
} sBIT_BYTE;

typedef struct
{
    unsigned int bit_0 : 1;
    unsigned int bit_1 : 1;
    unsigned int bit_2 : 1;
    unsigned int bit_3 : 1;
    unsigned int bit_4 : 1;
    unsigned int bit_5 : 1;
    unsigned int bit_6 : 1;
    unsigned int bit_7 : 1;
    unsigned int bit_8 : 1;
    unsigned int bit_9 : 1;
    unsigned int bit_10 : 1;
    unsigned int bit_11 : 1;
    unsigned int bit_12 : 1;
    unsigned int bit_13 : 1;
    unsigned int bit_14 : 1;
    unsigned int bit_15 : 1;
} sBIT_WORD;

typedef struct
{
    unsigned int byte_0 : 8; // LSB
    unsigned int byte_1 : 8; // MSB
} sBYTE_WORD; 

typedef union
{
    sBIT_BYTE bit_access;
    unsigned char byte_access;
} sBIT_BYTE_ACCESS;

typedef union
{
    sBIT_WORD bit_access;
    unsigned int word_access;
} sBIT_WORD_ACCESS;

typedef union
{
    sBYTE_WORD byte_access;
    unsigned int word_access;
} sBYTE_WORD_ACCESS;

typedef union
{
    sBIT_WORD bit_access;
    sBYTE_WORD byte_access;
    unsigned int word_access;
} sBIT_BYTE_WORD_ACCESS;
struct bit_def
{
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
    unsigned char b6 : 1;
    unsigned char b7 : 1;
};

union byte_def
{
    struct bit_def bitke;
    char byte;
};

typedef struct
{
    unsigned char RemoteStd;
    sBIT_BYTE_WORD_ACCESS Custom;
    sBIT_BYTE_WORD_ACCESS Command;
} stDataIR;


IRsend irsend; // An IR LED must be connected to Arduino PWM pin 3.
unsigned long irdata;
unsigned long ocrRemotePressedMillis = 0;
unsigned long lastCommandMillis = 0;
unsigned long nextCommandMillis = 50;
char sendDataCommand = '\0';
int sequence = 0;
int stage = 0;
int lastStage = 0;
int repeat = 0;
int queue = 0;
int stillLooping = 1;
int startDelay = 13000;
int fastDelay = 500;
int normalDelay = 1500;
int longDelay = 3000;
int qComms = 0;
int remoteSteps = 0;
bool brepeat = false;
bool bneedrepeat = false;
bool bStart = false;
bool bStop = false;
bool bSkip = false;

char usbMenu[][13][6] = {
    {{'0', '1'}, {'3'}, {'0', '2', '1'}, {'0', '2', '1'},{'3'}, {'0', '2', '1'}, {'0', '2', '1'}},
    {{'0', '2', '1', '2', '1'}, {'4'}, {'5'}, {'2'}, {'4'}, {'5'}, {'2'}, {'4'}, {'5'}, {'2'}, {'4'}, {'5'}, {'2'}}
};

char dtvMenu[][10][3] = {
    {{'0', '1'}, {'3'}, {'0', '2', '1'}, {'0', '2', '1'}, {'3'}, {'0', '2', '1'}, {'3'}, {'0', '2', '1'}, {'0', '2', '1'}},
    {{}}
};

char atvMenu[][10][3] = {
    {{'0', '1'}, {'3'}, {'0', '2', '1'}, {'0', '2', '1'}, {'3'}, {'0', '2', '1'}, {'3'}, {'0', '2', '1'}, {'0', '2', '1'}},
    {{}}
};

char*** testTypeCommand_;
unsigned long previousMillis = 0;
const long interval = 4000;
const String takeImageCommand = "takeshot";
const String beginCommand = "begin";
const String finishedCommand = "finish";
const String noData = "no data";
const String check = "check";
const String firstLayerFinished = "firstLayerFinished";
const String secondLayerFinished = "secondLayerFinished";
String camIsReady;
String data = "usb";
String tempData;
String testType;
bool trialTest = true, takeShot;
bool firstLayer = true, secondLayer = false;
int a=0;
int numLayer, numRows, numCols,  loopLayer, loopRows, loopCols;;


stDataIR remoteData;
EN_KEY keyTemp = KEY_NULL;

#define START_BUTTON_PIN 6
#define STOP_BUTTON_PIN 2

EasyButton button1(START_BUTTON_PIN);
EasyButton button2(STOP_BUTTON_PIN);

#ifdef USE_OLED_DISPLAY
#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);
#endif

#define STAGE_NUM STAGE4

byte flipByte(byte c)
{
    char r = 0;
    for (byte i = 0; i < 8; i++)
    {
        r <<= 1;
        r |= c & 1;
        c >>= 1;
    }
    return r;
}

#define min(a, b) ((a) < (b) ? (a) : (b))
uint64_t reverseBits(uint64_t input, uint16_t nbits)
{
    if (nbits <= 1)
        return input; // Reversing <= 1 bits makes no change at all.
    // Cap the nr. of bits to rotate to the max nr. of bits in the input.
    nbits = min(nbits, (uint16_t)(sizeof(input) * 8));
    uint64_t output = 0;
    for (uint16_t i = 0; i < nbits; i++)
    {
        output <<= 1;
        output |= (input & 1);
        input >>= 1;
    }
    // Merge any remaining unreversed bits back to the top of the reversed bits.
    return (input << nbits) | output;
}

uint32_t encodeData(uint16_t address, uint16_t command)
{
    command &= 0xFF; // We only want the least significant byte of command.
    // sendNEC() sends MSB first, but protocol says this is LSB first.
    command = reverseBits(command, 8);
    command = (command << 8) + (command ^ 0xFF); // Calculate the new command.
    if (address > 0xFF)
    { // Is it Extended NEC?
        address = reverseBits(address, 16);
        return (((uint32_t)address << 16) + command); // Extended.
    }
    else
    {
        address = reverseBits(address, 8);
        return (((uint32_t)address << 24)) + ((uint32_t)(address ^ 0xFF) << 16) + command; // Normal.
    }
}

void(*resetFunc) (void) = 0;

void setup(){
    Serial.begin(115200);
    keyTemp = KEY_NULL;

#if defined(IR_SEND_PIN)
    irsend.begin(); // Start with IR_SEND_PIN as send pin and enable feedback LED at default feedback LED pin
#else
    irsend.begin(3, ENABLE_LED_FEEDBACK); // Specify send pin and enable feedback LED at default feedback LED pin
#endif
#if !defined(SEND_PWM_BY_TIMER)
    irsend.enableIROut(38); // Call it with 38 kHz to initialize the values printed below
#endif
}

uint16_t zero(){
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x00;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
uint16_t one(){
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x01;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
uint16_t four(){
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x04;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
uint16_t menu(){
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            
    remoteData.Command.byte_access.byte_0 = 0x0B;                                   
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; 
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
uint16_t enter(){
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x14;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
uint16_t atas(){
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x0C;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
uint16_t bawah(){
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x0D;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
uint16_t kanan(){
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x0E;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
uint16_t kiri(){
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x0F;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
uint16_t back(){
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x4B;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
uint16_t power(){
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x17;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
uint16_t source(){
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x5C;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
uint16_t mute(){
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x15;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}


char*** testCommand(String data) {
    if (data == "dtv"){
        numLayer = sizeof(dtvMenu) / sizeof(dtvMenu[0]);
        numRows = sizeof(dtvMenu[0]) / sizeof(dtvMenu[0][0]);
        numCols = sizeof(dtvMenu[0][0]) / sizeof(dtvMenu[0][0][0]);
    }
    else if (data == "usb"){
        numLayer = sizeof(usbMenu) / sizeof(usbMenu[0]);
        numRows = sizeof(usbMenu[0]) / sizeof(usbMenu[0][0]);
        numCols = sizeof(usbMenu[0][0]) / sizeof(usbMenu[0][0][0]);
    }
    else if (data == "atv"){
        numLayer = sizeof(atvMenu) / sizeof(atvMenu[0]);
        numRows = sizeof(atvMenu[0]) / sizeof(atvMenu[0][0]);
        numCols = sizeof(atvMenu[0][0]) / sizeof(atvMenu[0][0][0]);
    }
    char*** testTypeCommand_ = new char**[numLayer];
    for(int i=0; i<numLayer; i++){ 
        testTypeCommand_[i] = new char*[numRows];
        for(int j=0; j<numRows; j++){
            testTypeCommand_[i][j] = new char[numCols];
        }
    }
    for(int i=0; i<numLayer; i++){
        for(int j=0; j<numRows; j++){
            for(int k=0; k<numCols; k++){
                if(data=="dtv"){ testTypeCommand_[i][j][k] = dtvMenu[i][j][k]; }
                else if(data=="usb"){ testTypeCommand_[i][j][k] = usbMenu[i][j][k]; }
                else if(data=="atv"){ testTypeCommand_[i][j][k] = atvMenu[i][j][k]; }
                Serial.println(testTypeCommand_[i][j][k]);
                delay(100);
            }
        }
    }
    return testTypeCommand_;
}

void loop(){
    unsigned long currentMillis = millis();
    if(Serial.available()>0){
        currentMillis = millis();
        data = Serial.readString();
        // Serial.write(data.c_str());
        if(data == "usb"){ testTypeCommand_ = testCommand(data); }
        if(data == "dtv"){ testTypeCommand_ = testCommand(data); }
        if(data == "atv"){ testTypeCommand_ = testCommand(data); }
        if(data == "camIsReady"){
            for(loopLayer=0; loopLayer<numLayer; loopLayer++){
                for(loopRows=0; loopRows<numRows; loopRows++){
                    takeShot = false;
                    for(loopCols=0; loopCols<numCols; loopCols++){
                        if(testTypeCommand_[loopLayer][loopRows][loopCols] != NULL){
                            sendDataCommand = testTypeCommand_[loopLayer][loopRows][loopCols];
                            // Serial.write(sendDataCommand);
                            if(firstLayer == true){
                                takeShot = true;
                                switch(sendDataCommand){
                                    case '0': menu(); break;
                                    case '1': enter(); break;
                                    case '2': bawah(); break;
                                    case '3': atas(); break;
                                    case '4': kiri(); break;
                                    case '5': kanan(); break;
                                    case '6': back(); break;
                                }
                                delay(fastDelay);
                            }
                            if(secondLayer == true){
                                if(sendDataCommand == '4' || sendDataCommand == '5'){
                                    Serial.write(takeImageCommand.c_str());
                                    String repetitionData = Serial.readString();
                                    int repetition = 100-repetitionData.toInt();
                                    if(sendDataCommand == '4'){
                                        for(int singleButtonRepeated=0; singleButtonRepeated<repetition; singleButtonRepeated++){
                                            // Serial.write(sendDataCommand);
                                            switch (sendDataCommand){
                                                case '4': kiri(); break;
                                                case '5': kanan(); break;
                                            }
                                            delay(85);
                                        }
                                        takeShot = true;
                                    }
                                    else{
                                        for(int singleButtonRepeated=0; singleButtonRepeated<100; singleButtonRepeated++){
                                            // Serial.write(sendDataCommand);
                                            switch (sendDataCommand){
                                                case '4': kiri(); break;
                                                case '5': kanan(); break;
                                            }
                                            delay(85);
                                        }
                                        takeShot = true;
                                    }
                                    
                                }
                                else{
                                    switch(sendDataCommand){
                                        case '0': menu(); break;
                                        case '1': enter(); break;
                                        case '2': bawah(); break;
                                        case '3': atas(); break;
                                        case '4': kiri(); break;
                                        case '5': kanan(); break;
                                        case '6': back(); break;
                                    }
                                    delay(fastDelay);
                                    takeShot = false;
                                }
                            }        
                        }
                    }

                    if(takeShot == true){
                        Serial.write(takeImageCommand.c_str());
                        while(Serial.readString()!="ocrFinished"){delay(100);}
                    }
                    
                }
                if(loopLayer==0){
                    Serial.write(firstLayerFinished.c_str());
                    firstLayer = false;
                    secondLayer = true;
                    delay(500);
                }
                else if(loopLayer==1){
                    Serial.write(secondLayerFinished.c_str());
                    firstLayer = false;
                    secondLayer = false;
                    delay(500);
                }
            }
            Serial.write(finishedCommand.c_str());
        }
        if(data == "abort"){ resetFunc(); } 
    }
}
