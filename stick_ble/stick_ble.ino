#include <SPI.h>

const int LED=13;
int rsvData = 0;
int loadCell = 0;

const byte R_BIT = 0x80;
const byte R_MASK = 0xBF;
const byte W_MASK = 0x3F;
const byte CS = 10;
const byte OE = 9;
int accelX = 0;
int accelY = 0;
int accelZ = 0;

unsigned int readRegister(byte reg){
  unsigned int result;
  reg |= R_BIT;
  reg &= R_MASK;
  digitalWrite(CS, LOW);
  SPI.transfer(reg);
  result = SPI.transfer(0x00);
  digitalWrite(CS, HIGH);
  return result;
}

void writeRegister(byte reg, byte data) {
  reg &= W_MASK;
  digitalWrite(CS, LOW);
  SPI.transfer(reg);
  SPI.transfer(data);
  digitalWrite(CS, HIGH);
}
//SerialはMega系のみ

void setup() {

  unsigned int res = 0;

  pinMode(CS, OUTPUT);
  pinMode(OE, OUTPUT);
  digitalWrite(OE, LOW);
  
  pinMode(LED, OUTPUT);
  Serial.begin(9600);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(SPI_CLOCK_DIV2);

  digitalWrite(OE, HIGH);

  writeRegister(0x20, 0x27);
  res = readRegister(0x0F);

  Serial.print(res);
}

void loop() {
  int x, y, z;
  int h, l;


  writeRegister(0x20, 0x27);

  l = readRegister(0x28);
  h = readRegister(0x29);
  x = h << 8 | l;

  l = readRegister(0x2A);
  h = readRegister(0x2B);
  y = h << 8 | l;

  l = readRegister(0x2C);
  h = readRegister(0x2D);
  z = h << 8 | l;

  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.print(z);
  // 受信処理
  if(Serial.available() > 0){
    rsvData = Serial.read();
    if(rsvData == 1){ 
      digitalWrite(LED, HIGH);
    }else if(rsvData == 0){
      digitalWrite(LED, LOW);
    } 
  } 

  //センサー読み取り～送信
  loadCell = analogRead(A0); 
  accelX = analogRead(A1);
  accelY = analogRead(A2);
  accelZ = analogRead(A3);
   
  //  sensorVal = sensorVal>>2; //10bit->8bit値
  Serial.write(loadCell);
  Serial.write(accelX);
  Serial.write(accelY);
  Serial.write(accelZ);
  
  //ウェイト
  delay(100);
}
