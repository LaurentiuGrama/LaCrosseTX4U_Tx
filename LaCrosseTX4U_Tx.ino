
#include "DHT.h"

byte               TData[11]={0,10, 0, 4,4, 0,0,0,0,0,0};
byte               HData[11]={0,10,14, 4,5, 0,0,0,0,0,0};

unsigned char      sendPin=5;
//unsigned char      VccPin=4;
float       CorrectionT= -0.6;


#define receicePin 4
#define DHTTYPE DHT22
DHT dht(receicePin, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("Started;LACrosseTX4Y_Tx.;;");
  pinMode(sendPin, OUTPUT);
  dht.begin();

}

void getData(){
  float h = dht.readHumidity() ;
  FillValue(h,"Humy");
  DebugF("Humidity",h);
  
  float t = dht.readTemperature()+ CorrectionT;
  FillValue(t,"Temp");
  DebugF("Temperature",t);
  
 
}

void FillValue (float V, String type){
  int z=V/10 ;
  int u=V-z*10;
  int d=V*10 -z*100 -u*10;
    
  if (type=="Temp") { 
    TData[5]=z+5;
    TData[6]=u;
    TData[7]=d;
    TData[8]=z+5;
    TData[9]=u;
  }
  else {
    HData[5]=z;
    HData[6]=u;
    HData[8]=z;
    HData[9]=u;
  }
 
}



void Validate (byte *Data){
  byte Sum=0;
  for (int i = 0; i<10; i=i+1) Sum +=Data[i];

  Data[10]=Sum % 16;  
  
}



void Send_1(){
  digitalWrite(sendPin, HIGH);   
  delayMicroseconds(550);             
  digitalWrite(sendPin, LOW);    
  delayMicroseconds(1050);   
}

void Send_0(){
  digitalWrite(sendPin, HIGH);   
  delayMicroseconds(1300);             
  digitalWrite(sendPin, LOW);    
  delayMicroseconds(1050);   
}


void  SendData(byte *d){
  for (int k = 0; k< 2; k++ ) {
    for (int i = 0; i< 11; i++ ) {
      for (int j = 3; j>= 0; j-- ) {
        //Serial.print(bitRead(d[i],j));
        if (bitRead(d[i],j)==1 ) Send_1();
        else Send_0(); 
      }
    }
    delay(30);
  }
}


void loop() {
  
  String a;
  getData();
  Validate(HData);
  Validate(TData);
  DebugData(HData);
  DebugData(TData);
  SendData(TData);
  delay(250);
  SendData(HData);
  delay(59000);

/*
  if (Validate(Data)) {
    //DebugData(Data);
    if (Data[2]==0x00) {
      lcTemp=millis();
      ArrayAddT();
    }
    else if (Data[2]==0x0E) {
      lcHumy=millis();
      ArrayAddH();
    }
    
    //DebugS("",DecodeSenzor(Data));
    if (lcTemp!=0 && lcHumy!=0){
      delay(500);
      lcTemp=lcHumy=0;
      SendData(TData);
      delay(700);
      SendData(HData);
      DebugS("",DecodeSenzor(TData));
      //delay(300);
      DebugS("",DecodeSenzor(HData));
      //DebugData(TData);
      //DebugData(HData);
    }
      
    
  }
*/
}


/*
void ArrayAddT(){
  for (int i = 0; i<= 10; i=i+1) {
    TData[i]=Data[i];

  }
}


void ArrayAddH(){
  for (int i = 0; i<= 10; i=i+1) {
    HData[i]=Data[i];
  }
}

*/

void DebugF(String te,float t){
  Serial.print(millis());
  Serial.print(";");
  Serial.print(te);
  Serial.print("");
  Serial.print(t,2);
  Serial.println("");
}


void DebugS(String te,String t){
  Serial.print(millis());
  Serial.print(";");
  Serial.print(te);
  Serial.print("");
  Serial.print(t);
  Serial.println("");
}

void DebugB(String te,byte t){
  Serial.print(millis());
  Serial.print("  ");
  Serial.print(te);
  Serial.print("=");
  Serial.print(t,DEC);
  Serial.println("");
}

void DebugL(String te,long t){
  Serial.print(millis());
  Serial.print("  ");
  Serial.print(te);
  Serial.print("=");
  Serial.print(t);
  Serial.println("");
}

void DebugData(byte *Data){
  Serial.print(millis());
  Serial.print("  ");
  for (int i = 0; i<= 10; i=i+1) {
    Serial.print(Data[i],DEC);
    Serial.print(" ");
  }
  Serial.println("");
}
