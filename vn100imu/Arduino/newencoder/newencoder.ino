#define lf 4 
#define lb 5
#define rf 6
#define rb 7
#define lpwm 9
#define rpwm 10

#define l_encoder 0
#define r_enconder 1

#define wheel_diameter 5.6 // cm
#define pi 3.1415
#define ticksper_rev 32

float toothgap;
//float toothgapcm;

int dir = 1;

int count = 0;
long timenow = 0,timelast = 0,timediff = 0;
long lastprinttime = 0,printtimediff = 0;
int printval=0;
bool tick = 0;
bool alreadyprinted = 1;

float vel = 0.0;
float lastvel = 0.0;
float accel  = 0.0;
float dist = 0.0;

int val = 0;

void setup() {
  toothgap = pi*wheel_diameter/ticksper_rev; // in cm
 // toothgapcm = toothgap*100;
  pinMode(lf,OUTPUT);
  pinMode(rf,OUTPUT);
  pinMode(lb,OUTPUT);
  pinMode(rb,OUTPUT);
  pinMode(lpwm,OUTPUT);
  pinMode(rpwm,OUTPUT);

  pinMode(2,INPUT); 
  pinMode(3,INPUT);

  analogWrite(lpwm,150);
  analogWrite(rpwm,100);

   attachInterrupt(1, doEncoder, RISING); 
   
  Serial.begin(115200);
  stop_bot();
  timelast = millis();
  lastprinttime = timelast;
  delay(50);
  
}

void loop() {
   if(Serial.available())
   {
      String s = Serial.readString();
      char c = s[0];
      if(c == 's')
        stop_bot();
      else
      {  
        String ss = s.substring(1,s.length());
        val = ss.toInt();
        count = 0;
    //   Serial.println(s);
        if(c == 'f')
          forward();
        else if(c == 'b')
          backward();
        else if(c == 'p')
        {
          analogWrite(rpwm,val-50);
          analogWrite(lpwm,val);
          val = 0;
        }
        else
          stop_bot();
      }
     //Serial.println(c);
   }
   //Serial.println("l");
    if((micros()-timelast)>50000 && !alreadyprinted)
    {
      alreadyprinted = 1;
      vel = 0.0;
      timediff = 0;
    //  Serial.println(dist);
      lastprinttime = micros();
    }
    printtimediff = (micros()-lastprinttime);
    if(printtimediff>=20000)
    {
       // if(printtimediff >100000)
       //  printtimediff = 10000;
        // accel = (vel - lastvel)*100.0; // 100 Hz is accurate so timescale can be multiplied on pc 
        //lastvel = vel;
        lastprinttime = micros();
        //printval = timediff/100.0;
        Serial.println(timediff); 
    }
   if(tick)
   {
    if(timediff>0) //  due to bouncing signal above max possible
    {  
      tick = 0;
      count += dir;
     // if(count%4 == 0) 
    //  {
        dist = toothgap*count;
        if(dist >= val)
        {
          stop_bot();
         // Serial.println("Reached");
        //  Serial.println(dist);
        }
      //  vel =  toothgap/timediff*1000.0;  
     //   Serial.println(dist);
        alreadyprinted = 0;
     // }
    }
   }
}
void doEncoder()
{
    tick = 1;
    timenow = micros();
    timediff = timenow - timelast;
    timelast = timenow;
   // Serial.println(timenow);
}
void forward()
{
  PORTD &= 0x0F;
  PORTD |= 0b01010000;
  /*digitalWrite(lf,HIGH);
  digitalWrite(rf,HIGH);
  digitalWrite(lb,LOW);
  digitalWrite(rb,LOW);*/
}
void backward()
{
  PORTD &= 0x0F;
  PORTD |= 0b10100000;
  /*digitalWrite(lf,LOW);
  digitalWrite(rf,LOW);
  digitalWrite(lb,HIGH);
  digitalWrite(rb,HIGH);*/
}
void stop_bot()
{
  PORTD |= 0xF0;
/*  digitalWrite(lf,HIGH);
  digitalWrite(rf,HIGH);
  digitalWrite(lb,HIGH);
  digitalWrite(rb,HIGH); */
}
