   /* 100 --- 1
   160 --- 2
   200 --- 3
   240 --- 4 */
#include <AFMotor.h>
#include <Servo.h> 
#include <SoftwareSerial.h>
short int a=0,b=0,flag=0, currtezi,limittezi=160,turndelay=450,mode=1,key=1,dir[10];
String voice="",command1="",command2="",command3="",command4="";
int d;
SoftwareSerial RiseRSerial(50, 51); // RX, TX
AF_DCMotor motor1 (1, MOTOR12_2KHZ); 
AF_DCMotor motor2 (2, MOTOR12_2KHZ);
AF_DCMotor motor3 (3, MOTOR12_2KHZ);
AF_DCMotor motor4 (4, MOTOR12_2KHZ);
Servo RiseRServo;
#define trigPin 46
#define echoPin 47

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  RiseRSerial.begin(9600);
  RiseRServo.attach(10); //define our servo pin (the motor shield servo1 input = digital pin 10)
  RiseRServo.write(90);   
  settezi(limittezi);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  Serial.flush();
  RiseRSerial.flush();

}

void loop() {Serial.println("****************************************************************************************************");
  Serial.print("Loop No. : ");
  Serial.println(a);
  a=a+1;
  // put your main code here, to run repeatedly: 

  
  check();delay(300);
  
  while (RiseRSerial.available()){  
  delay(10);
  char c = RiseRSerial.read(); 
  if (c == '#') {break;}
  voice += c; 
  if(flag==0) command1 += c;
  if(flag==1) command2 += c; 
  if(flag==2) command3 += c; 
  if(flag==3) command4 += c;
  if(c == ' ') flag=flag+1;
  }
  command1.trim();
  command2.trim();
  command3.trim(); 
  flag=0;
  voice.toLowerCase();command1.toLowerCase();command2.toLowerCase();command3.toLowerCase();
  if (voice.length()>1){
    Serial.println("#####################################################################");
    Serial.print("Here comes voice command : ");
    Serial.println(voice);
    Serial.print("Command1 : *");
    Serial.print(command1);
    Serial.println("#"); 
    Serial.print("Command2 : *");
    Serial.print(command2);
    Serial.println("#");
    Serial.print("Command3 : *");
    Serial.print(command3);
    Serial.println("#");
      
    if(command1=="command"&& command2=="mode"){mode=1;}
    if(command1=="auto"&& command2=="mode"){mode=2;}
    if(command1=="path"&& command2=="mode"){mode=3;b=0;}
    if(command1=="set") 
    {
      if(command2=="speed"||command2=="tezi"||command2=="thrust")// set speed set tezi set thrust
      {
       if(command3=="one"||command3=="1") limittezi=100;
       if(command3=="two"||command3=="2") limittezi=160;
       if(command3=="three"||command3=="3") limittezi=200;
       if(command3=="four"||command3=="4") limittezi=240;
       settezi(0);settezi(limittezi);
      }
    }
  }
    if(mode==1){
    if (voice=="straight"||voice=="ahead"||voice=="forward"||voice=="front"||voice=="go")
    {
     settezi(0);
     suddenstop();
     if(dist()>20){forward();}
    }
    
    else if(voice=="back"||voice=="backward"||voice=="rear")
    {
      settezi(0);
      suddenstop();
      backward(700);
    }
    
    else if(voice=="left"||voice=="turn left"||voice=="anticlockwise")
    {
      settezi(0);
      suddenstop();
      turn(180);
      d=dist();
      turn(90);
      if(d>20){left(turndelay);}
    }
    
    else if(voice=="right"||voice=="turn right"||voice=="clockwise")
    {
      settezi(0);
      suddenstop();
      turn(0);
      d=dist();
      turn(90);
      if(d>20){right(turndelay);}
     }
     
    else if(voice=="stop"||voice=="ruko"||voice=="bus")
    {
      settezi(0);
      suddenstop();
    }
    
    
    else if(command1=="turn"||command1=="right"||command1=="left") 
    {
      if(command2=="right"||command2=="left")  {turndelay=50*((setnumber(command3)));}
      if(command3=="unit")                     {turndelay=50*((setnumber(command2)));}// left 2 unit
      if(command1=="right"||command2=="right")    
     {
      settezi(0);
      suddenstop();
      turn(0);
      d=dist();
      turn(90);
      if(d>20){right(turndelay);}
     }
      if(command1=="left"||command2=="left")    
      {
      settezi(0);
      suddenstop();
      turn(180);
      d=dist();
      turn(90);
      if(d>20){left(turndelay);}
      }
    
    }
   else if(command1=="straight"||command1=="ahead"||command1=="forward"||command1=="front"||command1=="go")
   {
    if(command3=="m"||command3=="metre"||command3=="cm"||command3=="centimetre"){
    short int temp;
    temp=setnumber(command2);
    if(temp>0&&temp<100)
      {
     if(command3=="m"||command3=="metre"){temp=temp*100;}
    else if(command3=="cm"||command3=="centimetre"){temp=temp;}   
    else temp=0;
     }
     forward(temp);  
   }
   }
}//mode 1 end

if(mode==2)
{
      if(command1=="stop"||command2=="stop"||voice=="stop"||voice=="ruko"||voice=="bus")
    { settezi(0);
      suddenstop();mode=0;}
      
  Serial.println("..........Auto Mode.......................");
  if(key==1){limittezi=100;settezi(0);settezi(limittezi);key=0;}

long n;
if(dist()<40)
{ n=random(1,20);Serial.print("n : ");Serial.println(n);
  settezi(0);
  suddenstop();
  if(n>10){
           turn(180);d=dist();turn(90);
               if(d>30){left(turndelay);}
           
                else  { turn(0);d=dist(); turn(90);
                        if(d>30){right(turndelay);}
                        else{left(150);}  
                      }
          }
      
      else{
            turn(0);d=dist();turn(90);
                if(d>30) {right(turndelay);}
           
                    else{ turn(180);d=dist(); turn(90);
                          if(d>30){left(turndelay);}
                          else{right(150);}
                        }
          }
}//dist<30

check();
forward();
delay(400);}//mode 2 end

if(mode==3)
{
  if(voice.length()>0){
if(command1=="straight"||command1=="ahead"||command1=="forward"||command1=="front"||command1=="go"){dir[b]=1;b=b+1;}   
else if(command1=="back"||command1=="backward"||command1=="rear"){dir[b]=2;b=b+1;}
else if(command1=="left"||command1=="turn left"||command1=="anticlockwise"){dir[b]=3;b=b+1;}
else if(command1=="right"||command1=="turn right"||command1=="clockwise"){dir[b]=4;b=b+1;}
else if(command1=="stop"||command1=="ruko"||command1=="bus"){dir[b]=5;b=b+1;}}

  if(command1=="start"||command2=="start"){settezi(0);suddenstop();
    for(int c=0;c<b;c++)
    {if(dir[c]>0){
      if(dir[c]==1){while(1){if(dist()>20){forward();}
                             else{settezi(0);suddenstop();break;}delay(700);}}
      else if (dir[c]==2){backward(700);settezi(0);suddenstop();}    
      else if (dir[c]==3){turn(180);d=dist();turn(90);if(d>30){left(turndelay);}}
      else if (dir[c]==4){turn(0);d=dist();turn(90);if(d>30){right(turndelay);}}
    }}}
  if(command1=="end"||command2=="end"){
    for(int c=0;c<b;c++){dir[c]=0;}
    }
    }//mode 3 end
voice="",command1="",command2="",command3="";
}
void slow (short int n)
{
  Serial.println("Here comes slow function");
  short int b=currtezi;
  while(b>n)
  {
   b=b-20;
   motor1.setSpeed(b);
   motor2.setSpeed(b);
   motor3.setSpeed(b);
   motor4.setSpeed(b);
   Serial.print("Motor speed : ");
   Serial.println(b);
   delay(10);
  }
}
void tez (short int n)
{
  Serial.println("Here comes tez function");
  short int b =currtezi;
  while(b<n)
  {
   b=b+20;
   motor1.setSpeed(b);
   motor2.setSpeed(b);
   motor3.setSpeed(b);
   motor4.setSpeed(b);
   Serial.print("Motor speed : ");
   Serial.println(b);
   delay(10);                                                                                                                
  }
}

void settezi(short int n)
{
  Serial.println("Here comes settezi function");  
  Serial.print("Current speed : ");
  Serial.println(currtezi);
  if(currtezi>n) slow(n);
  else if(currtezi<n) tez(n);
  motor1.setSpeed(n);
  motor2.setSpeed(n);
  motor3.setSpeed(n);
  motor4.setSpeed(n);
  Serial.print("Final speed : ");
  Serial.println(n);
  currtezi=n;
}

void forward()
{
  Serial.println("Here comes forward function");
  settezi(100);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  settezi(limittezi);
}


void backward(short int n)
{
  Serial.println("Here comes backward function");  
  Serial.print("Turn delay : ");
  Serial.println(n);
  settezi(100);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);  
  settezi(limittezi);
  delay(n);
  settezi(0);
  suddenstop(); turndelay=500;
}

void left(short int n)
{
  Serial.println("Here comes left function");
  Serial.print("Turn Delay : ");
  Serial.println(n);
  settezi(200);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(n);
  settezi(0);
  suddenstop(); turndelay=450;
}

void right(short int n)
{
  Serial.println("Here comes right function");
  Serial.print("Turn Delay : ");
  Serial.println(n);
  settezi(200);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD); 
  delay(n);
  settezi(0);
  suddenstop(); turndelay=450;
}

void suddenstop()
{  
  Serial.println("Here comes suddenstop function");
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);   
}

void turn(short int n)
{
Serial.println("Here comes turn function");
Serial.print("Angle : ");
Serial.println(n);
RiseRServo.write(n);
delay(1000);
}

short int setnumber(String s)
{
  Serial.println("Here comes setnumber function");
  Serial.print("String s : ");
  Serial.println(s);
  short int n;
  
if(s=="one") n=1;
else if(s=="two") n=2;
else if(s=="three") n=3;
else if(s=="four") n=4;
else if(s=="five") n=5;
else if(s=="six") n=6;
else if(s=="seven") n=7;
else if(s=="eight") n=8;
else if(s=="nine") n=9;
else if(s=="ten") n=10;
else if(s=="eleven") n=11;
else if(s=="tweleve") n=12;
else if(s=="thirteen") n=13;
else if(s=="fourteen") n==14;
else if(s=="fifteen") n=15;
else if(s=="sixteen") n=16;
else if(s=="seventien") n=17;
else if(s=="eightein") n=18;
else if(s=="nintein") n=19;
else if(s=="twenty") n=20;
else{ n=s.toInt();}

Serial.print("Returning value : ");
Serial.println(n);
if(n>10) n=10;
return n;
}

short int setnumber(String s1,String s2)
{
  s2=" ";
  Serial.println("Here comes setnumber function");
  Serial.print("String s1 : ");
  Serial.println(s1);
  Serial.print("String s2 : ");
  Serial.println(s2);
  short int n1,n2;
if(s1=="twenty") n1=20;
else if(s1=="thirty") n1=30;
else if(s1=="fourty") n1=40;
else if(s1=="fifty") n1=50;
else if(s1=="sixty") n1=60;
else if(s1=="seventy") n1=70;
else if(s1=="eighty") n1=80;
else if(s1=="ninty") n1=90;
else if(s1==" ") n1=0;
else{ n1=s1.toInt();}

if(s2=="one") n2=1;
else if(s2=="two") n2=2;
else if(s2=="three") n2=3;
else if(s2=="four") n2=4;
else if(s2=="five") n2=5;
else if(s2=="six") n2=6;
else if(s2=="seven") n2=7;
else if(s2=="eight") n2=8;
else if(s2=="nine") n2=9;

else if(s2==" ") n2=0;
else{ n2=s2.toInt();}

Serial.print("Returning value : ");
Serial.println(n1+n2);
return (n1+n2);
}

int dist()
{
  long duration;
  int cm;   
digitalWrite(trigPin, LOW);
delayMicroseconds(5);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
pinMode(echoPin, INPUT);
duration = pulseIn(echoPin, HIGH);
cm = (duration/2) / 29.1;
Serial.print("Distance : ");
Serial.println(cm);
return cm;
}

void check()
{
  Serial.println("Check Function");
  d=dist();
  if (d<65 && d>=35){if(currtezi>100){settezi(100);} delay(50); d=dist();}
  if (d<35){if(currtezi>0){settezi(0); suddenstop();}}
  if (d<20){if(currtezi>0){suddenstop();}}
}

void forward(short int n)
{
 int d1,d2;
     d1=dist();
     d2=  min(n,d1);
     forward();
     do
     {
      delay(250);
      check();
      }
     while(dist()>(d1-d2)); 
     settezi(0);
     suddenstop();   
}
