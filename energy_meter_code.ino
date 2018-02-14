Embedded Code

#include <Energy.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
EnergyMonitor emon1;             
int a,b=0,i=0,num=0,n;
long int ti;
int threshold[12] = {222, 162, 88, 396, 356, 313, 512, 488, 455, 575, 7, 275};
char keys[12] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'A', 'B'};
float energy = 0, pd, pm, pvd=0, pvm=0, ac,nac,Penergy = 0,tenergy,count;
unsigned long start,t;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
SoftwareSerial mySerial(6, 7);
void setup()
{  
  Serial.begin(9600);
  mySerial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Real Time Power");
  lcd.setCursor(3,1);
  lcd.print("Monitoring");
  start = millis();
  t = millis();
  }



void loop()
{
  keypadcall();
  
  emon1.calcVI(20,2000);         
  emon1.serialprint();           
  float realPower       = emon1.realPower;        
  float apparentPower   = emon1.apparentPower;    
  float powerFActor     = emon1.powerFactor;      
  float supplyVoltage   = emon1.Vrms;             
  float Irms            = emon1.Irms;             
  float s1 = ((millis() - t)/(60000.0));
  if(realPower<10.0)realPower = abs(realPower - 4.0);
  tenergy = 1.2* (realPower/1000.0) * s1;
  if((millis() - t) >= 10000)
  {
  Penergy = Penergy + tenergy;
  t = millis();
  }
  lcd.clear();
  lcd.print("Power(W) :");
  lcd.setCursor(10,0);
  lcd.print(realPower);
  lcd.setCursor(0,1);
  lcd.print("Units    :");
  lcd.setCursor(10,1);
  lcd.print(Penergy);
  pd = Penergy - pvd;
  pm = Penergy - pvm;
  unsigned long s = millis() - start;
  if((s/3600000.0) >= 24*30)
  {
    mySerial.println("AT+CMGF=1");   
     delay(1000); 
   mySerial.println("AT+CMGS=\"+919500531432\"\r"); 
   delay(1000); 
    mySerial.println("Your last month consumption is :");
     delay(100); 
    mySerial.println(pm);
     delay(100); 
    mySerial.println((char)26);
     delay(1000);
    pm = 0;
    pvm = Penergy;
    start =millis();
  }
    if((s/3600000.0) >= 0.009)
  {
    if(count == 0)count = 30;
    count -= 1;
    if(pd>nac)
    {
  mySerial.println("AT+CMGF=1");    
     delay(1000); 
  mySerial.println("AT+CMGS=\"+919500531432\"\r"); 
   delay(1000);  
    mySerial.println("Your per minute consumption is :");
     delay(100); 
    mySerial.println(pd);
      delay(100); 
    mySerial.println("Your consumption is ");
     delay(100); 
    float b = ((pd/nac)*100.0)-100.0;
    mySerial.println(b);
       delay(100); 
      mySerial.println("% more than avg level");
     delay(100); 
    nac = ((num - pd)/count);
    mySerial.println("Your new avg consumption level is");
    delay(100);
    mySerial.println(nac);
     delay(100);
    mySerial.println((char)26);// ASCII code of CTRL+Z
     delay(1000); 
    }
    else
    {
      
      mySerial.println("AT+CMGF=1");    
     delay(1000); 
  mySerial.println("AT+CMGS=\"+919500531432\"\r"); 
   delay(1000); 
    mySerial.println("Your per day consumption is :");
     delay(100); 
    mySerial.println(pd);
   
     delay(100); 
     nac = ((num - pd)/count);
     mySerial.println("Your new avg consumption level is");
        delay(100);
     mySerial.println(nac);
     delay(100);
     
    mySerial.println((char)26);
     delay(1000); 
    }
    pd = 0;
    pvd = Penergy;
    start =millis();
  }
  
  
}

void keypadcall()
{
    a = analogRead(0);
   
 

 if ( abs(a - threshold[10])<15)
    {
     num = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Enter units");
    delay(1000);
      while(abs(a - threshold[11]) > 15)
            {
              a = analogRead(0);
              for(int i = 0; i < 12; i++)
                 {
                   if(abs(a - threshold[i]) < 15)
                     {
                     if(keys[i]!='B')
                     num = (num * 10) + int(keys[i]-48);
                     while(analogRead(0)<1000)
                     delay(100);
                     lcd.setCursor(0,1);
                     lcd.print(num);
                     //Serial.println(num);
                     
                     }
                 }
             }
             lcd.clear();
             lcd.print("Received Units");
             lcd.setCursor(0,1);
             lcd.print(num);
             count = 30.0;
             ac = num/count;
             nac = ac;
            mySerial.println("AT+CMGF=1");    
    delay(1000); 
  mySerial.println("AT+CMGS=\"+919500531432\"\r"); 
   delay(1000);  
    mySerial.println("Received units for the month is :");
     delay(100); 
      mySerial.println(num);// The SMS text you want to send
     delay(100); 
     mySerial.println("Average consumption for the month should be,  UNITS :");
     delay(100); 
     mySerial.println(ac);
     delay(100); 
     mySerial.println((char)26);
     delay(1000); 
 }
    }
