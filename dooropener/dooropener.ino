/*
MIT Media Lab door opener with charlie card v1
Valentin Heun 
heun at mit.edu

Build up on Aaron Weiss "RFID Eval 13.56MHz Shield example sketch v10" 
OSHW license: http://freedomdefined.org/OSHW

Ported to Arduino 1.0 by scottgwald
*/

#include <SoftwareSerial.h>
#include <Servo.h> 

 
Servo myservo;  
SoftwareSerial rfid(7, 8);
int swit = 0;

//Hold your Tag to the reader and look up the ID send via Serial.
//write the ID in the following Array. You are ready to go.                       

int IDS[] = {12345,12345};


int inPin1 = 4; 
int inPin2 = 5; 

//Prototypes
void check_for_notag(void);
void halt(void);
void parse(void);
void print_serial(void);
void read_serial(void);
void seek(void);
void set_flag(void);

//Global var
int flag = 0;
int Str1[11];
int waitsome = 0;
int i;
int fail;

//INIT
void setup()  
{
  Serial.begin(9600);
  Serial.println("Start");
  
   pinMode(inPin1, INPUT);
   pinMode(inPin2, INPUT);
  
  rfid.begin(19200);
  delay(10);
  halt();

}


void loop()                 
{
 if (swit==0) { read_serial();}
  
  if (digitalRead(inPin1) == HIGH)
  { swit = 1;
   myservo.attach(9);
      myservo.write(170); 
         delay(1000); 
         myservo.detach() ;
  }
  
  
   if (digitalRead(inPin2) == HIGH)
  { swit = 0;
   myservo.attach(9);
      myservo.write(70); 
         delay(1000); 
         myservo.detach() ;
  }
  
  
  
}

void check_for_notag()
{
  seek();
  delay(10);
  parse();
  set_flag();
  
  if(flag = 1){
    seek();
    delay(10);
    parse();
  }
}

void halt()
{
 //Halt tag
  rfid.print((char)255);
  rfid.print((char)0);
  rfid.print((char)1);
  rfid.print((char)147);
  rfid.print((char)148);
}

void parse()
{
  while(rfid.available()){
    if(rfid.read() == 255){
      for(int i=1;i<11;i++){
        Str1[i]= rfid.read();
      }
    }
  }
}

void servo()
{
  if(flag == 1){
    int key_ = 0;
    key_ = Str1[8] * Str1[7] * Str1[6] * Str1[5];
    
 fail=1;
 for (i = 0; i < (sizeof(IDS)/sizeof(int)) - 1; i++) {
 
   if(key_ == IDS[i])
   {
      myservo.attach(9);
      myservo.write(170); 
    delay(5000); 
     myservo.write(70);
         delay(1000); 
    myservo.detach() ;
    fail =0;
   }
   
   
   
}
if (fail = 1)
{
 delay(3000);  
}

Serial.print(key_);
    Serial.println();
    delay(100);
  }
}

void read_serial()
{
  seek();
  delay(10);
  parse();
  set_flag();
  servo();
  delay(100);
}

void seek()
{
  //search for RFID tag
  
  rfid.print((char)255);
  rfid.print((char)0);
  rfid.print((char)1);
  rfid.print((char)130);
  rfid.print((char)131);
  delay(10);
}

void set_flag()
{
  if(Str1[2] == 6){
    flag++;
  }
  if(Str1[2] == 2){
    flag = 0;
  }
}
