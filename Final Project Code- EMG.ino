// emg-26, led(RED)-32, led(BLUE)-27, led(GREEN)-25, pushbutton-13, servo1-16 , servo2-18

#define SENSOR_PIN 26      //EMG Pin
#define LED_PIN 32         //LED 32 (RED)       Indicating EMG Mode
#define LED2_PIN 27        //LED 2(YELLOW) 27   Indicating Control Mode
#define LED3_PIN 25        //LED 3(GREEN)  25   Controlling LED
#define BUTTON_PIN 13      //pushSwitch pin

int sensorValue;
int lastState = HIGH; 
int currentState;     
int count = 0;
int Max=0;
int mid;
int flag=0;
int threshold;


#include <LiquidCrystal_I2C.h>
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

//servo
#include <Servo.h>

static const int servoPin = 4;
static const int servoPin2 = 18;

Servo servo1;
Servo servo2;

void setup() 
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  servo1.attach(servoPin);
  servo2.attach(servoPin2);
    
  lcd.init();              // initialize LCD
  lcd.setBacklight(HIGH);
  delay(10);               //to stabilize everything
  Serial.begin(115200);
  
  while(analogRead(SENSOR_PIN)<200)
  {
    lcd.setCursor(0, 0);
    lcd.print("Insert the Lead");
  }
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Calibrating...");
  for (int j = 0; j < 2500; j++)
  {
    sensorValue = analogRead(SENSOR_PIN);      
    if (sensorValue > Max)
    {
    Max = sensorValue;   
    }  
    Serial.println(sensorValue);
    delay(2);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibration done");
  Serial.println(0);
  delay(2000);
  lcd.clear();
  
  mid = Max/2;
  threshold = (mid-(mid/10)) ;
  
  lcd.setCursor(0, 0);
  lcd.print("Threshold");
  lcd.setCursor(0, 1);
  lcd.print(threshold);
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("EMG: ON-1  OFF-3");
  lcd.setCursor(0, 1);
  lcd.print("CONTROL: 2");
}


void loop() 
{
  currentState = digitalRead(BUTTON_PIN);
  if(lastState == LOW && currentState == HIGH)
  {
    count++;
    lcd.clear();
  }
  
  if(count == 1)        //EMG START
  {
    digitalWrite(LED_PIN, HIGH);      //RED LED ON
    EMG();
  }

  if(count == 2)        //CONTROL
  {
    digitalWrite(LED2_PIN, HIGH);      //YELLOW LED ON
    CONTROL_WITH_EMG();
    
  }
  
  if(count == 3)         //EMG STOP
  { 
    digitalWrite(LED_PIN, LOW);        //RED LED OFF
    digitalWrite(LED2_PIN, LOW);      //YELLOW LED OFF
    digitalWrite(LED3_PIN,LOW);

    NOEMG();
  }
  
  if(count == 4)         //RESTART
  {
    LCDRESTART();
    BLINKLED();                        //RED LED BLINKING 3 TIMES BEFORE RESTART
    ESP.restart();
  }
  
lastState = currentState;
delay(4);   
}


        //..............FUNCTION DEFINITIONS...........

void EMG()
{
  LCDEMG();     //Print in LCD
  sensorValue = analogRead(SENSOR_PIN);
  Serial.println(sensorValue);
}

void NOEMG()
{
  LCDNOEMG();   //Print in LCD  
  Serial.println(0);
}

void CONTROL_WITH_EMG()
{
  LCDCONTROL();
  sensorValue = analogRead(SENSOR_PIN);
  Serial.println(sensorValue); 
  if(sensorValue > threshold)   
    {
      flag++;
      if(flag%2!=0 && flag!=0)
      {
        digitalWrite(LED3_PIN,HIGH);
        delay(10);
        servo1.write(180);
        servo2.write(180);
        delay(10);
        lcd.setCursor(0, 1);
        lcd.print("flag=");
        lcd.setCursor(6, 1);
        lcd.print(flag);
        lcd.setCursor(9, 1);
        lcd.print("LED ON");
      }
      if(flag%2==0 && flag!=0)
      {
        digitalWrite(LED3_PIN,LOW);
        delay(10);
        servo1.write(0);
        servo2.write(0);
        delay(10);
        lcd.setCursor(0, 1);
        lcd.print("flag=");
        lcd.setCursor(6, 1);
        lcd.print(flag);
        lcd.setCursor(9, 1);
        lcd.print("LED OFF");
      }
      delay(600); 
    }
}

void LCDEMG()
{
  lcd.setCursor(0, 0);
  lcd.print("EMG in progress");
  lcd.setCursor(0, 1);
  lcd.print("Press to Control");
}

void LCDNOEMG()
{
  lcd.setCursor(0, 0);
  lcd.print("EMG Stopped!");
  lcd.setCursor(0, 1);
  lcd.print("Press to Restart");
} 

void LCDCONTROL()
{
  lcd.setCursor(0, 0);
  lcd.print("CONTROL MODE");
}

void LCDRESTART()
{
  lcd.setCursor(0, 0);
  lcd.print("Restarting...");
}

void BLINKLED()
{
for(int i=1 ; i<=3; i++)
  {
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
  }
}
