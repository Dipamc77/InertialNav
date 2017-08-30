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
long timenow_right = 0,timelast_right = 0,timediff_right = 0;
long timenow_left = 0,timelast_left = 0,timediff_left = 0;
long lastprinttime = 0,printtimediff = 0;
int printval=0;
bool tick_right = 0,tick_left = 0;
bool alreadyprinted = 1;

float vel = 0.0;
float lastvel = 0.0;
float accel  = 0.0;
float dist = 0.0;

int val = 0;

bool movementflag = 0,lrbflag = 0;
int currentpwm = 200;

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

  analogWrite(lpwm,currentpwm);
  analogWrite(rpwm,currentpwm-10);

   attachInterrupt(1, rightEncoder, RISING); 
   attachInterrupt(0, leftEncoder, RISING); 
  Serial.begin(115200);
  stop_bot();
  timelast_right = millis();
  timelast_left = timelast_right;
  lastprinttime = timelast_right;
  delay(50);
}

void loop() {
   if(Serial.available())
   {
      String s = Serial.readString();
      char c = s[0];
      if(c == 's')
        stop_bot();
      else if(c == 'f')
        forward();
      else if(c == 'b')
        backward();
      else if(c == 'r')
        turnright();
      else if(c == 'l')
        turnleft();
      else
      {  
        String ss = s.substring(1,s.length());
        val = ss.toInt();
        count = 0;
    //   Serial.println(s);
        if(c == 'p')
        {
          analogWrite(lpwm,val);
          analogWrite(rpwm,val-10);
          currentpwm = val;
          val = 0;
        }
        else
          stop_bot();
      }
   }
    if((micros()-timelast_left)>50000 || lrbflag == 1)
    {
      vel = 0.0;
      timediff_right = 0;
      timediff_left = 0;
    }
    printtimediff = (micros()-lastprinttime);
    if(printtimediff>=20000)
    {
        lastprinttime = micros();
        Serial.println(String(timediff_left) + "," + String(timediff_right) + "," + String(movementflag)); 
    }
   if(tick_right)
   {
    timediff_right = timenow_right - timelast_right;
    timelast_right = timenow_right;
    if(timediff_right>0) //  due to bouncing signal above max possible
    {  
      tick_right = 0;
     // count += dir;
     // dist = toothgap*count;
     // if(dist >= val)
     //   stop_bot();
    }
  }
  if(tick_left)
  {
    timediff_left = timenow_left - timelast_left;
    timelast_left = timenow_left;
    if(timediff_left>0) //  due to bouncing signal above max possible
      tick_left = 0;
  }
}
void leftEncoder()
{
  tick_left = 1;
  timenow_left = micros();
    // Serial.println(timenow);
}
void rightEncoder()
{
  tick_right = 1;
  timenow_right = micros();
   // Serial.println(timenow);
}
void forward()
{
  PORTD &= 0x0F;
  PORTD |= 0b01010000;
  movementflag = 1;
  lrbflag = 0;
  /*digitalWrite(lf,HIGH);
  digitalWrite(rf,HIGH);
  digitalWrite(lb,LOW);
  digitalWrite(rb,LOW);*/
}
void turnright()
{
  analogWrite(lpwm,50);
  analogWrite(rpwm,50);
  PORTD &= 0x0F;
  PORTD |= 0b01100000;
  lrbflag = 1;
  movementflag = 0;
  
  //analogWrite(lpwm,currentpwm);
  //analogWrite(rpwm,currentpwm);
}
void turnleft()
{
  analogWrite(lpwm,50);
  analogWrite(rpwm,50);
  PORTD &= 0x0F;
  PORTD |= 0b10010000;
  lrbflag = 1;
  movementflag = 0;
  
  //analogWrite(lpwm,currentpwm);
  //analogWrite(rpwm,currentpwm);
}
void backward()
{
  PORTD &= 0x0F;
  PORTD |= 0b10100000;
  lrbflag = 1;
  movementflag = 0;
  /*digitalWrite(lf,LOW);
  digitalWrite(rf,LOW);
  digitalWrite(lb,HIGH);
  digitalWrite(rb,HIGH);*/
}
void stop_bot()
{
  PORTD |= 0xF0;
  movementflag = 0;
  lrbflag = 0;
  analogWrite(lpwm,currentpwm);
  analogWrite(rpwm,currentpwm-10);
/*  digitalWrite(lf,HIGH);
  digitalWrite(rf,HIGH);
  digitalWrite(lb,HIGH);
  digitalWrite(rb,HIGH); */
}
