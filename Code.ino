#include <call.h>
#include <gps.h>
#include <GSM.h>
#include <inetGSM.h>
#include <LOG.h>
#include <SIM900.h>
#include <sms.h>
#include <Streaming.h>
#include <WideTextFinder.h>

#include "SIM900.h"
#include <SoftwareSerial.h>
#include "sms.h"
SMSGSM sms;

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 

#define analogSensor A0
int speaker=8;
int gas_value,Gas_alert_val, Gas_shut_val;
int Gas_Leak_Status;
int sms_count=0;
int numdata;
char smsbuffer[160];
char n[20];

void setup()
{
    pinMode(speaker,OUTPUT);
    Serial.begin(9600);
    lcd.begin(16,2);
    if(gsm.begin(2400)){
        Serial.println("System is initializing.....");
    }  
    delay(500);
};
void loop()
{
    CheckGas();
    CheckShutDown();
}
void CheckGas()
{
    lcd.setCursor(0,0);
    lcd.print("Gas Scan - ON");
    Gas_alert_val=ScanGasLevel();
    if(Gas_alert_val==LOW){
        SetAlert(); 
    }
}
int ScanGasLevel()
{
    gas_value=analogRead(analogSensor); 
    if(gas_value>=400){
        return LOW;
    }
    else{
        return HIGH;
    }
}
void SetAlert()
{
    digitalWrite(speaker,HIGH);
    Gas_Leak_Status=1;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Gas Alert!");
    lcd.setCursor(0,1); 
    lcd.print("SMS Sent!");
    while(sms_count<2){  
        sendMessage();
    }
}
void CheckShutDown()
{
    if(Gas_Leak_Status==1){
      Gas_shut_val=ScanGasLevel();
      if(Gas_shut_val==HIGH){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("No Gas Leaking");
        digitalWrite(speaker,LOW);
        delay(2000);
        lcd.clear();        
        sms_count=0;
        Gas_Leak_Status=0;   
      }
    }
}
void sendMessage()
{
    if(Gas_Leak_Status==1){
      if ((sms.SendSMS("94704314612", "Gas Leaking!!!")) && (sms.SendSMS("94715875632", "Gas Leaking!!!")) && (sms.SendSMS("94710857357", "Gas Leaking!!!")) && (sms.SendSMS(1, "Gas Leaking!!!")) && (sms.SendSMS(2, "Gas Leaking!!!")) && (sms.SendSMS(3, "Gas Leaking!!!")))
          Serial.println("SMS sent OK");
          CheckGas();
          sms_count++;     
    }
}
