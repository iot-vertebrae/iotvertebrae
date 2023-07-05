/*
Testing:

Sending 0 volts DAC V04:
iot@iot-vertebrae:~ $ i2cset -y 1 0x10 0x72 0x0000 w

Reading ADC A4
iot@iot-vertebrae:~ $ i2cget -y 1 0x10 0x61 w
0x2121
iot@iot-vertebrae:~ $ i2cget -y 1 0x10 0x61 w
0x33f0

Sending 10 volts DAC V04 (0x0FFF or 4095):
iot@iot-vertebrae:~ $ i2cset -y 1 0x10 0x72 0xff0f w

Reading ADC A4
iot@iot-vertebrae:~ $ i2cget -y 1 0x10 0x61 w
0x2121
iot@iot-vertebrae:~ $ i2cget -y 1 0x10 0x61 w
0x67a0
iot@iot-vertebrae:~ $
----
Sending 10 volts DAC V03:
iot@iot-vertebrae:~ $ i2cset -y 1 0x10 0x52 0xff0f w

Reading ADC A3:
iot@iot-vertebrae:~ $ i2cget -y 1 0x10 0x41 w
0x2121
iot@iot-vertebrae:~ $ i2cget -y 1 0x10 0x41 w
0x6720

Sending 10 volts DAC V03:
iot@iot-vertebrae:~ $ i2cset -y 1 0x10 0x52 0x0000 w

Reading ADC A3:
iot@iot-vertebrae:~ $ i2cget -y 1 0x10 0x41 w
0x2121
iot@iot-vertebrae:~ $ i2cget -y 1 0x10 0x41 w
0x33a0
iot@iot-vertebrae:~ $ 
----
Sending 10 volts DAC V02:
iot@iot-vertebrae:~ $ i2cset -y 1 0x10 0x32 0xff0f w

Reading ADC A2:
iot@iot-vertebrae:~ $ i2cget -y 1 0x10 0x21 w
0x2121
iot@iot-vertebrae:~ $ i2cget -y 1 0x10 0x21 w
0x6770

Sending 0 volts DAC V02:
iot@iot-vertebrae:~ $ i2cset -y 1 0x10 0x32 0x0000 w

Reading ADC A2:
iot@iot-vertebrae:~ $ i2cget -y 1 0x10 0x21 w
0x2121
iot@iot-vertebrae:~ $ i2cget -y 1 0x10 0x21 w
0x33e0
iot@iot-vertebrae:~ $
----
Sending 10 volts DAC V01:
iot@iot-vertebrae:~ $ i2cset -y 1 0x10 0x12 0xff0f w

Reading ADC A1:
iot@iot-vertebrae:~ $ i2cget -y 1 0x10 0x01 w
0x2121
iot@iot-vertebrae:~ $ i2cget -y 1 0x10 0x01 w
0x6780

Sending 0 volts DAC V01:
iot@iot-vertebrae:~ $ i2cset -y 1 0x10 0x12 0x0000 w

Reading ADC A1:
iot@iot-vertebrae:~ $ i2cget -y 1 0x10 0x01 w
0x2121
iot@iot-vertebrae:~ $ i2cget -y 1 0x10 0x01 w
0x33e0
iot@iot-vertebrae:~ $
----

* Register:
b7 b6 b5 b4 b3 b2 b1 b0
0  x  x  x  0  0  0  1     REG_RD
0  x  x  x  0  0  1  0     REG_WR
0  0  x  x  0  1  0  1     RD_CFG
0  0  0  0  0  1  1  0     VERT_VERS
0  0  0  0  0  1  1  1     CHECK

b4: 0 <-- A rib
b4: 1 <-- B rib

b6 b5 : port address (0..3)

** Register REG_RD:
b7 b6 b5 b4 b3 b2 b1 b0
0  x  x  x  0  0  0  1     REG_RD

Reading ADC's channel 0, A rib
i2cget -y 1 0x10 0x01 w

Reading ADC's channel 1, A rib
i2cget -y 1 0x10 0x21 w

Reading ADC's channel 2, A rib
i2cget -y 1 0x10 0x41 w

Reading ADC's channel 3, A rib
i2cget -y 1 0x10 0x61 w

** Register REG_WR:
* 
b7 b6 b5 b4 b3 b2 b1 b0
0  x  x  x  0  0  1  0     REG_WR

Sending 1 (0..4095) to 0 channel A rib
i2cset -y 1 0x10 0x02 0x0100 w

Sending 256 (0..4095) to 0 channel B rib
i2cset -y 1 0x10 0x12 0x0001 w

Sending 4095 (0..4095) to 3 channel A rib
i2cset -y 1 0x10 0x62 0xff0f w

** Register RD_CFG:
b7 b6 b5 b4 b3 b2 b1 b0
0  0  x  x  0  1  0  1     RD_CFG

b5: 0 <-- Read ribs configuration (2 bytes)
b5: 1 <-- Read A or B rib configuration (1 byte)

b4: 0 <-- A rib
b4: 1 <-- B rib

Raspberry Pi needs two I2C sendings to read properly.

A rib configuration (b5: 1, b4: 0):
i2cget -y 1 0x10 0x25
i2cget -y 1 0x10 0x25
0x01

B rib configuration (b5: 1, b4: 1):
i2cget -y 1 0x10 0x35
i2cget -y 1 0x10 0x35
0x02

0  0  0  0  0  0  0  0  (0x00) Rib is not connected or rib type is unknown.
0  0  0  0  0  0  0  1  (0x01) Rib is ADC
0  0  0  0  0  0  1  0  (0x02) Rib is DAC

Both ribs configuration (b5: 0, b4: x):
i2cget -y 1 0x10 0x05 w
i2cget -y 1 0x10 0x05 w
0x0201

Answer
x  x  x  x  x  x  x  x  0  0  0  0  0  0  0  0  A rib is not connected or rib type is unknown.
0  0  0  0  0  0  0  0  x  x  x  x  x  x  x  x  B rib is not connected or rib type is unknown.
x  x  x  x  x  x  x  x  0  0  0  0  0  0  0  1  A rib is ADC
0  0  0  0  0  0  0  1  x  x  x  x  x  x  x  x  B rib is ADC
x  x  x  x  x  x  x  x  0  0  0  0  0  0  1  0  A rib is DAC
0  0  0  0  0  0  1  0  x  x  x  x  x  x  x  x  B rib is DAC

0x0201: B rib is DAC (0x02) and A rib is ADC (0x01)

** Register VERSION:
b7 b6 b5 b4 b3 b2 b1 b0
x  x  x  x  x  1  1  0     VERT_VERS

Raspberry Pi needs two I2C sendings to read properly.
i2cget -y 1 0x10 0x06 w
i2cget -y 1 0x10 0x06 w
0x0100

Answer 0x0100:
01: VERSION
00: SUBVERSION

** Register CHECK:
b7 b6 b5 b4 b3 b2 b1 b0
x  x  x  x  x  1  1  1     CHECK

i2cset -y 1 0x10 0x07

Jordi Binefa - 20230605
iotvertebrae.com
*/
#include "driver/twai.h"
#include <Wire.h>

#define I2C_ADDR_BASE 0x10

#define DVERT_VERSION    0x01
#define DVERT_SUBVERSION 0x00

#define ADDR0 20
#define ADDR1 19
#define ADDR2 16
#define ADDR3 15
#define LED_MQTT 48
#define BT_IO0 0

#define SCL0 14
#define SDA0 21
#define SCL1  1
#define SDA1  2
#define SCL2  7
#define SDA2  6
#define SCL3  5
#define SDA3  4

#define RX_PIN 18
#define CRX RX_PIN
#define TX_PIN 17
#define CTX TX_PIN

#define CAN_REQST_ID  0x7DF
#define CANPID_RPM    0x0C

#define REG_CFG   0x00
#define REG_RD    0x01
#define REG_WR    0x02
#define REG_PWM   0x03
#define CFG_PWM   0x04
#define RD_CFG    0x05
#define VERT_VERS 0x06

#define TYPE_NONE 0x00
#define TYPE_ADC  0x01
#define TYPE_DAC  0x02

// #define REG_CFG   0x00
#define REG_RD    0x01
#define REG_WR    0x02
// #define REG_PWM   0x03
// #define CFG_PWM   0x04
#define RD_CFG    0x05
#define VERT_VERS 0x06
#define CHECK 0x07

#define MINIMUM_DELAY_ALLOWING_MULTITASKING 50

#define FUNC_vReadAdc             1
#define FUNC_vWriteDac            2
#define FUNC_vCheckI2C            3
#define FUNC_vManageReadRibTypes  4

TaskHandle_t TaskSlave;
TaskHandle_t TaskMaster;

uint32_t i = 0;
  
TwoWire I2C_M = TwoWire(0); // Main I2C. ESP32-S3 acting as slave
TwoWire I2C_S = TwoWire(1); // Slaves I2C. ESP32-S3 acting as master

const byte cbSDA_S[4] = {SDA0,SDA1,SDA2,SDA3};
const byte cbSCL_S[4] = {SCL0,SCL1,SCL2,SCL3};

byte byLastReadNumber,byRibA,byRibB;

static SemaphoreHandle_t mutex;
static const uint8_t nMsgQueueLen = 255;
static const uint8_t nTicksTryingAddItemToQueue = 10;
static QueueHandle_t qMsgQueue;

struct stI2C{
  int nFunc;
  byte byReg;
  short int siValue;
};

struct stAdcRibs{
  word wArib[4];
  word wBrib[4];
}stADC;

bool bIO0pressed(){
  return !digitalRead(0);
}

void vSetupAddr(){
  pinMode(LED_MQTT, OUTPUT);
  pinMode(ADDR0, INPUT_PULLUP);
  pinMode(ADDR1, INPUT_PULLUP);
  pinMode(ADDR2, INPUT_PULLUP);
  pinMode(ADDR3, INPUT_PULLUP); 
}

byte byReadI2cAddress(){
  byte byAddr = (digitalRead(ADDR3))?0x00:0x1;
  
  byAddr |= (digitalRead(ADDR2))?0x00:0x2;
  byAddr |= (digitalRead(ADDR1))?0x00:0x4;
  byAddr |= (digitalRead(ADDR0))?0x00:0x8;
  
  return I2C_ADDR_BASE | byAddr;
}

void vDiscoverI2cDevices(TwoWire *i2c,int whichOne){
  byte error, address;
  int nDevices;
  
  Serial.print("Scanning I2C");
  Serial.print(whichOne,DEC);
  Serial.println(":");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    i2c->beginTransmission(address);
    error = i2c->endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  Serial.println("---------------------------"); 
}

byte byDiscoverI2cDevices(TwoWire *i2c,int whichOne){
  byte error, address;
  int nDevices;
  
  // Serial.print("Scanning I2C");
  // Serial.print(whichOne,DEC);
  // Serial.println(":");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    i2c->beginTransmission(address);
    error = i2c->endTransmission();
    if (error == 0) {
      // Serial.print("I2C device found at address 0x");
      if (address<16) {
        // Serial.print("0");
      }
      // Serial.println(address,HEX);
      if(address == 0x48){
        return TYPE_ADC;
      }
      if((address == 0x60)||(address == 0x61)){
        return TYPE_DAC;
      }
      nDevices++;
    }
    else if (error==4) {
      // Serial.print("Unknow error at address 0x");
      if (address<16) {
        // Serial.print("0");
      }
      // Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    // Serial.println("No I2C devices found\n");
    return TYPE_NONE;
  }else {
    // Serial.println("done\n");
  }
  // Serial.println("---------------------------");
  
  return TYPE_NONE;
}

void vManageReadRibTypes(byte byRegister){
  byte byA = byRibA, byB = byRibB; 
  byte arr[2] = {byA,byB}; 
  
  vReadRibTypes();
  arr[0] = byRibA;
  arr[1] = byRibB;

// b7 b6 b5 b4 b3 b2 b1 b0
// 0  0  x  x  0  1  0  1     RD_CFG
// 
// b5: 0 <-- Read ribs configuration (2 bytes)
// b5: 1 <-- Read A or B rib configuration (1 byte)
// 
// b4: 0 <-- A rib
// b4: 1 <-- B rib

  if((byRegister & 0x20) == 0){
    I2C_M.slaveWrite(arr,2);
    /*Serial.print("I2C slave answers 0x");
    Serial.print(arr[0],HEX);
    Serial.print(" 0x");
    Serial.print(arr[1],HEX);
    Serial.print((arr[1]<0x10)?" (0x0":" (0x");
    Serial.print(arr[1],HEX);
    Serial.print((arr[0]<0x10)?"0":"");
    Serial.print(arr[0],HEX);
    Serial.println(")");*/
  }else{
    // Serial.print("I2C slave answers 0x");
    if(byRegister & 0x10){ // B rib
      I2C_M.write(byRibB);
      // Serial.println(byRibB,HEX); 
    }else{ // A rib
      I2C_M.write(byRibA);
      // Serial.println(byRibA,HEX); 
    }
  }    
}

void onRequest(){ 
  byte byA = byRibA, byB = byRibB,byWhichDev = 0x00; 
  byte arr[2] = {byA,byB},byRg;
  word wAnswer;

  Serial.println("onRequest");
  Serial.print("byLastReadNumber: 0x");
  Serial.println(byLastReadNumber,HEX);

  if((byLastReadNumber & 0x0F) == REG_RD){
/*    
b4: 0 <-- A rib
b4: 1 <-- B rib

b6 b5 : port address (0..3)

** Register REG_RD:
b7 b6 b5 b4 b3 b2 b1 b0
0  x  x  x  0  0  0  1     REG_RD
*/
    byRg = byLastReadNumber;
    byWhichDev |= (byRg & 0x20)?0x01:0x00;
    byWhichDev |= (byRg & 0x40)?0x02:0x00;
    
    // vAddQueue("readADC",byLastReadNumber,0x00);
    if( mutex != NULL ){
      if( xSemaphoreTake( mutex, ( TickType_t ) 10 ) == pdTRUE ){
        wAnswer = (byRg & 0x10)?stADC.wBrib[byWhichDev]:stADC.wArib[byWhichDev];
        xSemaphoreGive( mutex );    
      }else{
        Serial.println("We could not obtain the mutex semaphore and can therefore not access the shared resource safely.");
      }
    }
    I2C_M.write((byte)(wAnswer & 0xFF));
    I2C_M.write((byte)(wAnswer >> 8));
    Serial.print("readADC answers 0x");
    Serial.println(wAnswer,HEX);
  }

  if((byLastReadNumber & 0x0F) == REG_WR){
/*
b4: 0 <-- A rib
b4: 1 <-- B rib

b6 b5 : port address (0..3)

** Register REG_WR:
b7 b6 b5 b4 b3 b2 b1 b0
0  x  x  x  0  0  1  0     REG_WR
*/
    byWhichDev |= (byLastReadNumber & 0x20)?0x01:0x00;
    byWhichDev |= (byLastReadNumber & 0x40)?0x02:0x00;
    Serial.print(byWhichDev,HEX);
    if(byLastReadNumber & 0x10){
      Serial.println(" channel. B rib");
    }else{
      Serial.println(" channel. A rib");
    }
  }
  if((byLastReadNumber & 0x0F) == RD_CFG){
    vManageReadRibTypes(byLastReadNumber);
    // Funciona a la tercera crida si es demanen les dues costelles. Individualitzades no funciona:
    // vAddQueue("manageReadRibTypes",byLastReadNumber,0x00);
  }
  if((byLastReadNumber & 0x0F) == VERT_VERS){
    I2C_M.write(DVERT_SUBVERSION);
    I2C_M.write(DVERT_VERSION);
    Serial.print("Esclau respon 0x");
    Serial.print(DVERT_SUBVERSION,HEX);
    Serial.print(" 0x");
    Serial.println(DVERT_VERSION,HEX);
  }
}

void vAddQueue(String sFunc,byte byReg,short int siValue){
  struct stI2C st;

  st.byReg = byReg;
  st.siValue = siValue;
  if(sFunc == "writeDAC"){
    st.nFunc = FUNC_vWriteDac;
    if (xQueueSend(qMsgQueue, (void *)&st, nTicksTryingAddItemToQueue) != pdTRUE) { Serial.println("Queue full. Not adding "); Serial.println(sFunc);}
  }else{
    if(sFunc == "readADC"){
      st.nFunc = FUNC_vReadAdc;
      if (xQueueSend(qMsgQueue, (void *)&st, nTicksTryingAddItemToQueue) != pdTRUE) { Serial.println("Queue full. Not adding "); Serial.println(sFunc);}
    }else{
      if(sFunc == "checkI2C"){
        st.nFunc = FUNC_vCheckI2C;
        if (xQueueSend(qMsgQueue, (void *)&st, nTicksTryingAddItemToQueue) != pdTRUE) { Serial.println("Queue full. Not adding "); Serial.println(sFunc);}
      }else{
        if(sFunc == "manageReadRibTypes"){
          st.nFunc = FUNC_vManageReadRibTypes;
          if (xQueueSend(qMsgQueue, (void *)&st, nTicksTryingAddItemToQueue) != pdTRUE) { Serial.println("Queue full. Not adding "); Serial.println(sFunc);}
        }    
      }    
    }    
  }
}

void onReceive(int len){
  enum {rd,wr,rdCfg,versio,checkI2C,no} enReg = no;
  byte byA = 0x01, byB = 0x10,byReg,byWhichDev = 0x00;
  byte arr[2] = {byA,byB};
  int n = 0;
  short int siN;
  
  Serial.printf("onReceive[%d]: ", len);
  // while(Wire.available()){
  //  Serial.write(Wire.read());
  // }
  // Serial.println();

  digitalWrite(LED_MQTT,HIGH);
  
  while (1 < I2C_M.available()) { // loop through all but the last
    byLastReadNumber = I2C_M.read();   // receive byte as a character
    Serial.print(n,DEC);
    Serial.print(") El màster m'ha fet arribar un 0x");
    Serial.println(byLastReadNumber,HEX);
    switch(n){
      case 0:
        Serial.print("byLastReadNumber & 0x07: 0x");
        Serial.println(byLastReadNumber & 0x07,HEX);
        switch(byLastReadNumber & 0x07){
          case REG_RD:  enReg = rd; Serial.println("enReg = rd");break;
          case REG_WR:  enReg = wr; Serial.println("enReg = wr");    break;
          case RD_CFG: enReg = rdCfg; Serial.println("enReg = rdCfg");break;
          case VERT_VERS: enReg = versio; Serial.println("enReg = versio");break;
          case CHECK: enReg = checkI2C; Serial.println("enReg = checkI2C");break;
          default: enReg = no; Serial.println("enReg = no");
        }
        byReg = byLastReadNumber;
        break;
      case 1:
        switch(enReg){
          case wr:
            siN = ((word)byLastReadNumber) << 8;
            Serial.print("siN previ: 0x");
            Serial.println(siN,HEX);
            break;
        }
        break;
    }
    n++;
  }
  byLastReadNumber = I2C_M.read();
  Serial.print(n,DEC);
  Serial.print("> El màster m'ha fet arribar un 0x");
  Serial.println(byLastReadNumber,HEX);
  switch(n){
    case 0:
        switch(byLastReadNumber & 0x07){
          case REG_RD:  enReg = rd; Serial.println("> enReg = rd");break;
          case REG_WR:  enReg = wr; Serial.println("> enReg = wr");    break;
          case RD_CFG: enReg = rdCfg; Serial.println("> enReg = rdCfg");break;
          case VERT_VERS: enReg = versio; Serial.println("> enReg = versio");break;
          case CHECK: enReg = checkI2C; Serial.println("> enReg = checkI2C");break;
          default: enReg = no; Serial.println("> enReg = no");
        }
        byReg = byLastReadNumber;
        switch(enReg){
          case checkI2C:
            vAddQueue("checkI2C",byReg,0x00);
            break;
      }
    case 2:
      switch(enReg){
          case wr:
            siN |= byLastReadNumber;
            Serial.print("siN: ");
            Serial.print(siN,DEC);
            Serial.print(" - 0x");
            Serial.println(siN,HEX);
            vAddQueue("writeDAC",byReg,siN);
            break;
          
      }
      break;
  }

  // xSemaphoreGive(mutex);
  digitalWrite(LED_MQTT,LOW);
  // arr[1] = byLastReadNumber;
  // I2C_M.slaveWrite(arr,2);
}

int nCanSendAnswer(String sType,byte byI2cAddr){
  bool bSend = false;
  twai_message_t msgAnswer;

  msgAnswer.identifier = 0x0000 | (word)I2C_ADDR_BASE | (word)byI2cAddr;
  Serial.printf("msgAnswer.identifier: 0x%X\n",msgAnswer.identifier);
  msgAnswer.extd = 0;
  msgAnswer.rtr = 0;
  if(sType == "CFG_VERS"){
    msgAnswer.data_length_code = 4;
    msgAnswer.data[0] = byRibA;
    msgAnswer.data[1] = DVERT_VERSION;
    msgAnswer.data[2] = DVERT_SUBVERSION;
    msgAnswer.data[3] = byRibB;    
    bSend = true;  
  }
  if(sType == "A_rib_4ADC"){
    msgAnswer.data_length_code = 8;
    msgAnswer.data[0] = (byte)(stADC.wArib[0] >> 8);
    msgAnswer.data[1] = (byte)(stADC.wArib[0] & 0xFF);
    msgAnswer.data[2] = (byte)(stADC.wArib[1] >> 8);
    msgAnswer.data[3] = (byte)(stADC.wArib[1] & 0xFF);
    msgAnswer.data[4] = (byte)(stADC.wArib[2] >> 8);
    msgAnswer.data[5] = (byte)(stADC.wArib[2] & 0xFF);
    msgAnswer.data[6] = (byte)(stADC.wArib[3] >> 8);
    msgAnswer.data[7] = (byte)(stADC.wArib[3] & 0xFF);
    bSend = true;  
  }
  if(sType == "B_rib_4ADC"){
    msgAnswer.data_length_code = 8;
    msgAnswer.data[0] = (byte)(stADC.wBrib[0] >> 8);
    msgAnswer.data[1] = (byte)(stADC.wBrib[0] & 0xFF);
    msgAnswer.data[2] = (byte)(stADC.wBrib[1] >> 8);
    msgAnswer.data[3] = (byte)(stADC.wBrib[1] & 0xFF);
    msgAnswer.data[4] = (byte)(stADC.wBrib[2] >> 8);
    msgAnswer.data[5] = (byte)(stADC.wBrib[2] & 0xFF);
    msgAnswer.data[6] = (byte)(stADC.wBrib[3] >> 8);
    msgAnswer.data[7] = (byte)(stADC.wBrib[3] & 0xFF);
    bSend = true;  
  }
  if(bSend){
    if (twai_transmit(&msgAnswer, pdMS_TO_TICKS(1000)) == ESP_OK) {
      Serial.println("Answer message queued for transmission");
      
      if (msgAnswer.extd) {
          Serial.println("Answer message is in Extended Format\n");
      } else {
          Serial.println("Answer message is in Standard Format\n");
      }
      Serial.printf("ID is 0x%X\n", msgAnswer.identifier);
      if (!(msgAnswer.rtr)) {
          for (int i = 0; i < msgAnswer.data_length_code; i++) {
              Serial.printf("Data byte %d = 0x%X\n", i, msgAnswer.data[i]);
          }
      }
      return 1;
    } else {
      Serial.println("Failed to queue answer message for transmission");
      return 0;
    }
  }
  
  return 0;
}

void vReadingCan(byte byI2cAddr){
  int nPwmChannel,nBit,i;
  byte byWhichDev = 0x00;
  word wV;
  float fV;
  
  twai_message_t message;

  if (twai_receive(&message, pdMS_TO_TICKS(10000)) == ESP_OK) {
    Serial.println("Message received\n");
  } else {
      Serial.println("Failed to receive CAN message\n");
      return;
  }
  
  //Process received message
  if (message.extd) {
      Serial.println("Message is in Extended Format");
  } else {
      Serial.println("Message is in Standard Format");
  }
  Serial.printf("ID is 0x%X, I2C address: 0x%X\n\r", message.identifier,byI2cAddr);
  Serial.printf("message.identifier & 0x00FF = 0x%X\n\r",message.identifier & 0x00FF);
  if((message.identifier & 0x00FF) == (word)byI2cAddr){
    Serial.println("CAN message is for this vertebra.");
    if (!(message.rtr)) {
      Serial.println("message.rtr = 0");
      if((message.data[0] & 0x0F) == REG_WR){
        Serial.println("REG_WR");
        byWhichDev |= (message.data[0] & 0x20)?0x01:0x00;
        byWhichDev |= (message.data[0] & 0x40)?0x02:0x00;
        Serial.print(byWhichDev,HEX);
        if(message.data[0] & 0x10){ // b4:0 <-- A rib, b4:1 <-- B rib
          Serial.println(" channel. B rib");
        }else{
          Serial.println(" channel. A rib");
        }
        wV = (word)message.data[1]<<8 | (word)message.data[2];
        fV = (float)(10*(int)wV) / 4095;
        Serial.printf("0x%X --> %.2fV\n",wV,fV);
        vAddQueue("writeDAC",message.data[0],(short int)wV);
      }
      if((message.data[0] & 0x0F) == CHECK){
        vAddQueue("checkI2C",CHECK,0x00);
      }
      for (int i = 0; i < message.data_length_code; i++) {
        Serial.printf("Data byte %d = 0x%X\n\r", i, message.data[i]);
      }
    }else{
      Serial.println("message.rtr = 1");
      if((message.identifier & 0xFF00) == 0x0000){
        Serial.println("Asking for A rib 4 ADC values");
        nCanSendAnswer("A_rib_4ADC",byI2cAddr);
      }
      if((message.identifier & 0xFF00) == 0x0100){
        Serial.println("Asking for B rib 4 ADC values");
        nCanSendAnswer("B_rib_4ADC",byI2cAddr);
      }
      if((message.identifier & 0xFF00) == 0x0200){
        Serial.println("Asking for configuration and version");
        nCanSendAnswer("CFG_VERS",byI2cAddr);
      }
      if((message.identifier & 0xFF00) == 0x0300){
        Serial.println("Asking for a check");
        vAddQueue("checkI2C",CHECK,0x00);
      }
    }
  }else{
    Serial.println("CAN message is not for this vertebra.");
  }
}

void vTaskMainSlave( void * pvParameters ){
  byte byI2cAddr;
  
  Serial.print("vTaskSlave running on core ");
  Serial.println(xPortGetCoreID());
  vSetupAddr();
  byI2cAddr = byReadI2cAddress();
  Serial.print("I2C address: 0x");
  Serial.println(byI2cAddr,HEX);
  I2C_M.onReceive(onReceive);
  I2C_M.onRequest(onRequest);
  I2C_M.begin((uint8_t)byI2cAddr);
  for(;;){
    vReadingCan(byI2cAddr);
    vTaskDelay(MINIMUM_DELAY_ALLOWING_MULTITASKING);
  }
}

void vReadRibTypes(){
  int n = 0; // A rib
  I2C_S.begin(cbSDA_S[n],cbSCL_S[n],400000);
  byRibA = byDiscoverI2cDevices(&I2C_S,n);
  I2C_S.end();
  n = 2; // B rib
  I2C_S.begin(cbSDA_S[n],cbSCL_S[n],400000);
  byRibB = byDiscoverI2cDevices(&I2C_S,n);
  I2C_S.end();  
}

void vReadAdc(byte byReg,short int siValue){
  byte byWhichDev = 0x00,byAdcAddr = 0x48;
  int n;
  byte cbSDA_Adc,cbSCL_Adc;
  // word wAdcCfg[4] = {0x83C1,0x83D1,0x83E1,0x83F1},wAnswer;
  word wAdcCfg[4] = {0x83D1,0x83E1,0x83F1,0x83C1},wAnswer;

  byWhichDev |= (byReg & 0x20)?0x01:0x00;
  byWhichDev |= (byReg & 0x40)?0x02:0x00;
  // Serial.print("Reading ");
  // Serial.print(byWhichDev,HEX);
  // Serial.println((byReg & 0x10)?" channel. B rib":" channel. A rib");
  if(byReg & 0x10){
    // Serial.println(" channel. B rib"); // n = 2
    cbSDA_Adc = cbSDA_S[2];
    cbSCL_Adc = cbSCL_S[2];
  }else{
    // Serial.println(" channel. A rib"); // n = 0
    cbSDA_Adc = cbSDA_S[0];
    cbSCL_Adc = cbSCL_S[0];
  }
  I2C_S.begin(cbSDA_Adc,cbSCL_Adc,400000);
  I2C_S.beginTransmission(byAdcAddr);
  I2C_S.write(0x01);
  I2C_S.write((byte)(wAdcCfg[byWhichDev] & 0xFF));
  I2C_S.write((byte)(wAdcCfg[byWhichDev]>>8));
  I2C_S.endTransmission();
  I2C_S.beginTransmission(byAdcAddr);
  I2C_S.write(0x00);
  I2C_S.endTransmission();
  I2C_S.requestFrom(byAdcAddr,(byte)2);
  wAnswer = ((I2C_S.read() << 8) | I2C_S.read());
  I2C_S.beginTransmission(byAdcAddr);
  I2C_S.write(0x00);
  I2C_S.endTransmission();
  I2C_S.requestFrom(byAdcAddr,(byte)2);
  wAnswer = ((I2C_S.read() << 8) | I2C_S.read());
  I2C_S.end();
  if(bIO0pressed()){
    Serial.print("Reading ");
    Serial.print(byWhichDev,HEX);
    Serial.println((byReg & 0x10)?" channel. B rib":" channel. A rib");
    Serial.print("wAnswer: 0x");
    Serial.println(wAnswer,HEX);      
  }
  // byWhichDev = (byWhichDev == 0)?3:byWhichDev-1;
  if( mutex != NULL ){
    if( xSemaphoreTake( mutex, ( TickType_t ) 10 ) == pdTRUE ){
      if(byReg & 0x10){
        stADC.wBrib[byWhichDev] = wAnswer;
      }else{
        stADC.wArib[byWhichDev] = wAnswer;    
      }
      xSemaphoreGive( mutex );    
    }else{
      Serial.println("We could not obtain the mutex semaphore and can therefore not access the shared resource safely.");
    }
  }
}

void vWriteDac(byte byReg,short int siValue){
  byte byWhichDev = 0x00,byDacAddr;
  int n;
  /*
  b4: 0 <-- A rib
  b4: 1 <-- B rib
            
  b6 b5 : port address (0..3)
            
  ** Register REG_WR:
  b7 b6 b5 b4 b3 b2 b1 b0
  0  x  x  x  0  0  1  0     REG_WR
  */          
  // byWhichDev |= (byReg & 0x20)?0x01:0x00;
  // byWhichDev |= (byReg & 0x40)?0x02:0x00;
  byWhichDev = byReg >> 5;
  Serial.print("Writing "); Serial.print(siValue,DEC); Serial.print(" to ");
  Serial.print(byWhichDev,HEX);
  if(byReg & 0x10){
    Serial.println(" channel. B rib"); // n = 2
    switch(byWhichDev){
      case 0: n = 2; byDacAddr = 0x61;
        break;
      case 1: n = 2; byDacAddr = 0x60;
        break;
      case 2: n = 3; byDacAddr = 0x61;
        break;
      case 3: n = 3; byDacAddr = 0x60;
        break;
    }
  }else{
    Serial.println(" channel. A rib"); // n = 0
    switch(byWhichDev){
      case 0: n = 0; byDacAddr = 0x61;
        break;
      case 1: n = 0; byDacAddr = 0x60;
        break;
      case 2: n = 1; byDacAddr = 0x61;
        break;
      case 3: n = 1; byDacAddr = 0x60;
        break;
    }
  }

  I2C_S.begin(cbSDA_S[n],cbSCL_S[n],400000);
  I2C_S.beginTransmission(byDacAddr);
  I2C_S.write(0x40);
  I2C_S.write(siValue / 16);
  I2C_S.write((siValue%16) << 4);
  I2C_S.endTransmission();
  I2C_S.end();  

  Serial.print("n: "); Serial.println(n,DEC);
  Serial.print("cbSDA_S[n]"); Serial.println(cbSDA_S[n]);
  Serial.print("cbSCL_S[n]"); Serial.println(cbSCL_S[n]);
  pinMode(cbSDA_S[n],OUTPUT);  
  pinMode(cbSCL_S[n],OUTPUT);
  digitalWrite(cbSDA_S[n],HIGH);  
  digitalWrite(cbSCL_S[n],HIGH);  
}

void vCheckI2C(byte byReg){
  int n;
  
  if((byReg & 0x0F) == CHECK){
    for(n=0;n < 4;n++){
      I2C_S.begin(cbSDA_S[n],cbSCL_S[n],400000);
      vDiscoverI2cDevices(&I2C_S,n);
      I2C_S.end();
      // n = (n==3)?0:n+1;    
      vTaskDelay(MINIMUM_DELAY_ALLOWING_MULTITASKING);
    }
  }
}

void vManageI2cMessage(struct stI2C stM){
  switch(stM.nFunc){
    case FUNC_vReadAdc:
      Serial.print("FUNC_vReadAdc(byReg: 0x"); Serial.print(stM.byReg,HEX);
      Serial.print(",siValue: "); Serial.print(stM.siValue,DEC);Serial.println(")");
      vReadAdc(stM.byReg,stM.siValue);
      break;
    case FUNC_vWriteDac:
      Serial.print("FUNC_vWriteDac(byReg: 0x"); Serial.print(stM.byReg,HEX);
      Serial.print(",siValue: "); Serial.print(stM.siValue,DEC);Serial.println(")");
      vWriteDac(stM.byReg,stM.siValue);
      break;
    case FUNC_vCheckI2C:
      Serial.print("FUNC_vCheckI2C(byReg: 0x"); Serial.print(stM.byReg,HEX);Serial.println(")");
      vCheckI2C(stM.byReg);
      break;
    case FUNC_vManageReadRibTypes:
      Serial.print("FUNC_vManageReadRibTypes(byReg: 0x"); Serial.print(stM.byReg,HEX);Serial.println(")");
      vManageReadRibTypes(stM.byReg);
      break;
  }
}

void vTaskMaster( void * pvParameters ){ 
  struct stI2C stR;
  int nRib = 0; //0: A, 1: B
  int nAdcChannel = 0;
  byte byR = 0x00;
  
  Serial.print("vTaskMaster running on core ");
  Serial.println(xPortGetCoreID());
  digitalWrite(LED_MQTT,HIGH);
  /*
  // for(n=0;n < 4;n++){
  for(n=0;n < 4;n+=2){ // Only checking I2C0 and I2C2. It is faster at the beginning.
    I2C_S.begin(cbSDA_S[n],cbSCL_S[n],400000);
    vDiscoverI2cDevices(&I2C_S,n);
    I2C_S.end();
    // n = (n==3)?0:n+1;    
    vTaskDelay(MINIMUM_DELAY_ALLOWING_MULTITASKING);
  }*/
  vReadRibTypes();
  
  digitalWrite(LED_MQTT,LOW);
  for(;;){
    // See if there's a message in the queue (do not block)
    if (xQueueReceive(qMsgQueue, (void *)&stR, 0) == pdTRUE) {
      vManageI2cMessage(stR);
    }else{ // If there is empty queue read next ADC channel
      if(bIO0pressed()){
        Serial.print("Rib "); Serial.print((nRib)?"B":"A");
        Serial.print(", channel "); Serial.println(nAdcChannel);
      }
      if(nRib){ // B rib
        if(byRibB == TYPE_ADC){
          byR = 0x11;
          byR |= nAdcChannel << 5;
          // b4: 1 (B rib)
          // b6 b5: adc channel
          if(bIO0pressed()){
            Serial.print("Read ADC. B rib, channel "); Serial.print(nAdcChannel);
            Serial.print(" - byte 0x"); Serial.println(byR,HEX);
          }
          vReadAdc(byR,0x00);
          nAdcChannel++;
        }else{
          nRib = 0; // A rib
        }
      }else{ // A rib
        if(byRibA == TYPE_ADC){
          byR = 0x01;
          byR |= nAdcChannel << 5;
          // b4: 0 (A rib)
          // b6 b5: adc channel
          if(bIO0pressed()){
            Serial.print("Read ADC. A rib, channel "); Serial.print(nAdcChannel);
            Serial.print(" - byte 0x"); Serial.println(byR,HEX);
          }
          vReadAdc(byR,0x00);
          nAdcChannel++;
        }else{
          nRib = 1; // B rib
        }        
      }
      if(nAdcChannel > 3){
          nRib = (nRib)?0:1;
          nAdcChannel = 0;
      }
    }
    vTaskDelay(MINIMUM_DELAY_ALLOWING_MULTITASKING);
  }
}

void vSetupCan(){
  // Serial.setTxTimeoutMs(0);   // prevent slow timeouts
  // pinMode(CAN_RS, OUTPUT);    // INPUT (high impedance) = slope control mode, OUTPUT = see next line
  // digitalWrite(CAN_RS, LOW);  // LOW = high speed mode, HIGH = low power mode (listen only)
  // twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_PIN, (gpio_num_t)RX_PIN, TWAI_MODE_LISTEN_ONLY);  // TWAI_MODE_NORMAL, TWAI_MODE_NO_ACK or TWAI_MODE_LISTEN_ONLY
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_PIN, (gpio_num_t)RX_PIN, TWAI_MODE_NORMAL);  // TWAI_MODE_NORMAL, TWAI_MODE_NO_ACK or TWAI_MODE_LISTEN_ONLY
  twai_timing_config_t t_config  = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t f_config  = TWAI_FILTER_CONFIG_ACCEPT_ALL();
  twai_driver_install(&g_config, &t_config, &f_config);
  twai_start(); 
}

void setup() {
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.setDebugOutput(true);
  pinMode(BT_IO0,INPUT);

/*
#if CONFIG_IDF_TARGET_ESP32
  char message[64];
  snprintf(message, 64, "%u Packets.", i++);
  I2C_M.slaveWrite((uint8_t *)message, strlen(message));
  Serial.println("CONFIG_IDF_TARGET_ESP32");
#endif
*/
  mutex = xSemaphoreCreateMutex();

  // Create queue
  qMsgQueue = xQueueCreate(nMsgQueueLen, sizeof(stI2C));

  xTaskCreatePinnedToCore(
                    vTaskMainSlave,   /* Task function. */
                    "vTaskMainSlave",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &TaskSlave,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */ 

  xTaskCreatePinnedToCore(
                    vTaskMaster,   /* Task function. */
                    "vTaskMaster",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &TaskMaster,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
  vSetupCan();                 
}

boolean bPressedButton(int nWhichOne) {
  if (digitalRead(nWhichOne))
    return false;
  return true;
}

int CANsend (void) {
  static unsigned int n = 250;
  static byte byOutA=0xFF,byOutB=0x00,byMask=0x01;

  n++;

  byOutA = ~byMask;
  byOutB = byMask;
  byMask = (byMask==0x80)?0x01:byMask<<1;
  
  twai_message_t message;
  message.identifier = CAN_REQST_ID;
  message.extd = 0;
  message.rtr = 0;
  message.data_length_code = 8;
  message.data[0] = 2;
  message.data[1] = 1;
  message.data[2] = CANPID_RPM;
  message.data[3] = byOutA;
  message.data[4] = byOutB;
  message.data[5] = 0;
  message.data[6] = (byte)(n & 0xFF);
  message.data[7] = (byte)(n >> 8); 
  //return twai_transmit(&message, 0);

  if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
    Serial.println("Message queued for transmission");
    
    if (message.extd) {
        Serial.println("Message is in Extended Format\n");
    } else {
        Serial.println("Message is in Standard Format\n");
    }
    Serial.printf("ID is 0x%X\n", message.identifier);
    if (!(message.rtr)) {
        for (int i = 0; i < message.data_length_code; i++) {
            Serial.printf("Data byte %d = 0x%X\n", i, message.data[i]);
        }
    }
    
    return 1;
  } else {
    Serial.println("Failed to queue message for transmission");
    return 0;
  }
}

void loop() {
  static boolean bIO0wasPressed = false;
  boolean bIO0currentState = bPressedButton(BT_IO0); 
  static unsigned int n = 1;
   
  if (bIO0wasPressed != bIO0currentState) {
    delay(2);
    if (bIO0currentState) {
      Serial.print(n++);
      Serial.println(") Button 0 pressed (BOOT)");
      CANsend();
    }
    bIO0wasPressed = bIO0currentState;
  } 
  vTaskDelay(MINIMUM_DELAY_ALLOWING_MULTITASKING);
}
