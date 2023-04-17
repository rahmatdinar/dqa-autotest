// #define USE_OLED_DISPLAY
//  https://github.com/Arduino-IRremote/Arduino-IRremote
#include <Arduino.h>

// #define EXCLUDE_EXOTIC_PROTOCOLS // saves around 240 bytes program memory if IrSender.write is used
// #define SEND_PWM_BY_TIMER
// #define USE_NO_SEND_PWM
// #define NO_LED_FEEDBACK_CODE // saves 566 bytes program memory

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

#define STAGE1 0
#define STAGE2 1
#define STAGE3 2
#define STAGE4 3
#define STAGEMAX 0xff

typedef enum
{
    SEQUENCE0_STAGE1 = 0,
    SEQUENCE1_STAGE1,
    SEQUENCE2_STAGE1,
    SEQUENCE3_STAGE1,
    SEQUENCEMAX_STAGE1,
} STATE_SEQUENCE_STAGE1;

typedef enum
{
    SEQUENCE0_STAGE2 = 0,
    SEQUENCE1_STAGE2,
    SEQUENCE2_STAGE2,
    SEQUENCE3_STAGE2,
    SEQUENCE4_STAGE2,
    SEQUENCE5_STAGE2,
    SEQUENCE6_STAGE2,
    SEQUENCE7_STAGE2,
    SEQUENCE8_STAGE2,
    SEQUENCE9_STAGE2,
    SEQUENCE10_STAGE2,
    SEQUENCE11_STAGE2,
    SEQUENCE12_STAGE2,
    SEQUENCE13_STAGE2,
    SEQUENCE14_STAGE2,
    SEQUENCE15_STAGE2,
    SEQUENCE16_STAGE2,
    SEQUENCE17_STAGE2,
    SEQUENCE18_STAGE2,
    SEQUENCE19_STAGE2,
    SEQUENCE20_STAGE2,
    SEQUENCE21_STAGE2,
    SEQUENCEMAX_STAGE2,
} STATE_SEQUENCE_STAGE2;

typedef enum
{
    SEQUENCE0_STAGE3 = 0,
    SEQUENCE1_STAGE3,
    SEQUENCE2_STAGE3,
    SEQUENCE3_STAGE3,
    SEQUENCE4_STAGE3,
    SEQUENCE5_STAGE3,
    SEQUENCE6_STAGE3,
    SEQUENCE7_STAGE3,
    SEQUENCE8_STAGE3,
    SEQUENCE9_STAGE3,
    SEQUENCE10_STAGE3,
    SEQUENCE11_STAGE3,
    SEQUENCE12_STAGE3,
    SEQUENCE13_STAGE3,
    SEQUENCE14_STAGE3,
    SEQUENCE15_STAGE3,
    SEQUENCEMAX_STAGE3,
} STATE_SEQUENCE_STAGE3;

typedef enum
{
    SEQUENCE0_STAGE4 = 0,
    SEQUENCE1_STAGE4,
    SEQUENCE2_STAGE4,
    SEQUENCE3_STAGE4,
    SEQUENCE4_STAGE4,
    SEQUENCE5_STAGE4,
    SEQUENCE6_STAGE4,
    SEQUENCE7_STAGE4,
    SEQUENCE8_STAGE4,
    SEQUENCE9_STAGE4,
    SEQUENCE10_STAGE4,
    SEQUENCE11_STAGE4,
    SEQUENCE12_STAGE4,
    SEQUENCE13_STAGE4,
    SEQUENCE14_STAGE4,
    SEQUENCE15_STAGE4,
    SEQUENCEMAX_STAGE4,
} STATE_SEQUENCE_STAGE4;
// +++++++++++++++++++++++++++
// +  union structure
// +++++++++++++++++++++++++++
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

// #define NULL 0

IRsend irsend; // An IR LED must be connected to Arduino PWM pin 3.
unsigned long irdata;
unsigned long ocrRemotePressedMillis = 0;
unsigned long lastCommandMillis = 0;
unsigned long nextCommandMillis = 50;
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

void setup(){
    Serial.begin(115200);
    // Serial.println("Sequence IR");
    keyTemp = KEY_NULL;
#if defined(IR_SEND_PIN)
    irsend.begin(); // Start with IR_SEND_PIN as send pin and enable feedback LED at default feedback LED pin
#else
    irsend.begin(3, ENABLE_LED_FEEDBACK); // Specify send pin and enable feedback LED at default feedback LED pin
#endif
#if !defined(SEND_PWM_BY_TIMER)
    /*
       Print internal signal generation info
    */
    irsend.enableIROut(38); // Call it with 38 kHz to initialize the values printed below
#endif
}

int zero(){
    qComms++;
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x00;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
int one(){
    qComms++;
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x01;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
int four(){
    qComms++;
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x04;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
int menu(){
    qComms++;
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            
    remoteData.Command.byte_access.byte_0 = 0x0B;                                   
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; 
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
int enter(){
    qComms++;
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x14;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
int atas(){
    qComms++;
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x0C;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
int bawah(){
    qComms++;
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x0D;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
int kanan(){
    qComms++;
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x0E;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
int kiri(){
    qComms++;
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x0F;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
int back(){
    qComms++;
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x4B;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
int power(){
    qComms++;
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x17;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
int source(){
    qComms++;
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x5C;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
int mute(){
    qComms++;
    remoteData.Custom.byte_access.byte_0 = CUSTOMCODE_H;                            // 0x80;//0x10;//CUSTOMCODE_H;
    remoteData.Custom.byte_access.byte_1 = CUSTOMCODE_L;                            // 0x7f;//0xef;//CUSTOMCODE_L;
    remoteData.Command.byte_access.byte_0 = 0x15;                                   // 0x0d;//IrCommandTypeNECSmartTV[0];
    remoteData.Command.byte_access.byte_1 = ~remoteData.Command.byte_access.byte_0; // 0xf4;//0xF2;//IrCommandTypeNECSmartTV[0];
    irsend.sendNEC(remoteData.Custom.word_access, remoteData.Command.word_access, 0);
}
char usbMenu[][3] = {
    {'0', '1'}, //pict 1
    {'3'}, //pict 2
    {'0', '2', '1'}, //sound
    {'0', '2', '1'}, //settings
    {'3'}, //settings 2
    {'0', '2', '1'}, //about
    {'0', '2', '1'} //input source
};

char dtvMenu[][3] = {
    {'0', '1'},
    {'3'},
    {'0', '2', '1'},
    {'0', '2', '1'},
    {'3'},
    {'0', '2', '1'},
    {'3'},
    {'0', '2', '1'},
    {'0', '2', '1'},
};

char atvMenu[][3] = {
    {'0', '1'},
    {'3'},
    {'0', '2', '1'},
    {'0', '2', '1'},
    {'3'},
    {'0', '2', '1'},
    {'3'},
    {'0', '2', '1'},
    {'0', '2', '1'},
};

char** testTypeCommand_;
unsigned long previousMillis = 0;
const long interval = 4000;
const String takeImageCommand = "takeshot";
const String beginCommand = "begin";
const String finishedCommand = "finish";
const String noData = "no data";
const String check = "check";
String camIsReady;
String data;
String tempData;
String testType;
int a=0;
int numRows;
int numCols;

void loop(){
    unsigned long currentMillis = millis();
    if(Serial.available()>0){
        currentMillis = millis();
        data = Serial.readString();
        Serial.write(data.c_str());
        if(data == "usb"){ 
            numRows = sizeof(usbMenu)/sizeof(usbMenu[0]);
            numCols = sizeof(usbMenu[0])/sizeof(usbMenu[0][0]);
            testTypeCommand_ = new char*[numRows];
            for(int i=0; i<numRows; i++){ testTypeCommand_[i] = new char[numCols+1];}
            for(int i=0; i<numRows; i++){
                for(int j=0; j<numCols; j++){ testTypeCommand_[i][j] = usbMenu[i][j];}
            }
        }
        if(data == "dtv"){ 
            numRows = sizeof(dtvMenu)/sizeof(dtvMenu[0]);
            numCols = sizeof(dtvMenu[0])/sizeof(dtvMenu[0][0]);
            testTypeCommand_ = new char*[numRows];
            for(int i=0; i<numRows; i++){ testTypeCommand_[i] = new char[numCols+1];}
            for(int i=0; i<numRows; i++){
                for(int j=0; j<numCols; j++){ testTypeCommand_[i][j] = dtvMenu[i][j];}
            }
        }
        if(data == "atv"){ 
            numRows = sizeof(atvMenu)/sizeof(atvMenu[0]);
            numCols = sizeof(atvMenu[0])/sizeof(atvMenu[0][0]);
            testTypeCommand_ = new char*[numRows];
            for(int i=0; i<numRows; i++){ testTypeCommand_[i] = new char[numCols+1];}
            for(int i=0; i<numRows; i++){
                for(int j=0; j<numCols; j++){ testTypeCommand_[i][j] = atvMenu[i][j];}
            }
        }
        if(data == "camIsReady"){
            for(int a=0; a<numRows; a++){
                for(int b=0; b<numCols; b++){
                    switch(testTypeCommand_[a][b]){
                        case '0':
                        menu();
                        break;
                        case '1':
                        enter();
                        break;
                        case '2':
                        bawah();
                        break;
                        case '3':
                        atas();
                        break;
                        case '4':
                        kiri();
                        break;
                        case '5':
                        kanan();
                        break;
                        case '6':
                        back();
                        break;
                    }
                    delay(500);
                }
                Serial.write(takeImageCommand.c_str());
                delay(4500);
            }
            Serial.write(finishedCommand.c_str());
        }
        if(data == "abort"){
            exit(0);
            
            // Serial.write(takeImageCommand.c_str());
            // delay(20);
            // Serial.write(finishedCommand.c_str());
        }
    }
}