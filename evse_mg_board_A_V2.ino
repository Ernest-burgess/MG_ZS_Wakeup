
 /*    MG ZS EV  Delayed Charge Problem 
 This software is for a board has been built up to sit on the Pilot output 
 from a EV charger That is able to do delayed timed charging ie eco 7  
 or simalar . The software monitors the signals on the pilot line and if 
 after reciving the PWM signal from the charge controler > it does not see 
 the car pulling the pilot down to 6 volts to accept The Charge it will oprate the NC Relay
 This will remove the pilot signal from the car for 5 seconds and then return the signal 
 this should wake up the car and get it to accept the Charge .
 the calibration pot should be ajusted to give the following sensor reading   
              422 = 12 volt steady dc input 
              360 = 10.3 volt 
              240 = 7 volt   ..... this used as calbration point 
              235 = 6.8 volt 
              
  v2  add wtd  timer 
  Code written by Bill Burgess  8/2/20  note this code uses segments of other code found in the public domain 
   I thank all who have posted there code so that we can all learn from there efforts and trust that this may 
   help others 
   
*/

#include <avr/wdt.h>  // wtd code 

byte PWM_PIN=3;  // pwm input Pin 3 
int pwm_value;    // pwm value 
 
int sensorPin = A0;    //  input pin to read pilot line voltage 
const int RelPin = 8;      // output pin for the relay 
const int setpointvalue= 240;  // digital input value coresponds to 7 volts  actual voltage. 
const int ledPin =  LED_BUILTIN;// the number of the onboard  LED pin
const int setupledpin = 7;  // output pin for the status led 
// Variables will change:

int incharge = 0 ;  // flag to show that the car has allrady been charging 
int car_terminated = 0;  // flag to show that the car was charging and has terminated the charge by lifting voltage back to 9 volts 
int ledState = LOW;             // ledState used to set the LED
int sensorValue = 0;  // variable to store the voltage digital value 


void setup() {
  // declare the RelPin as an OUTPUT:
      pinMode(RelPin, OUTPUT);
    // set the digital pin as output:
       pinMode(ledPin, OUTPUT);
         pinMode(setupledpin, OUTPUT);
           pinMode(PWM_PIN,INPUT);  // pwm
  
   
   // initialize the serial communication:
      Serial.begin(9600);
       Serial.println("we have started ");
       watchdogSetup();  // start watchdog
       wdt_reset();
   }

 void loop() {
   Get_sensor();  // read pwm and volt
   Serial.println ("Pilot value set for 422 at 12 vdc  "); 
   Serial.print (sensorValue);  // pilot voltage 
   Serial.println ("PWM value   "); 
   Serial.print(pwm_value);
   
   Serial.print ("");

   // if the LED is off turn it on and vice-versa this is to flash led on board heart beat 
    if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
     }

    // set the LED with the ledState of the variable:
       digitalWrite(ledPin, ledState);

   
   
  // if voltage at charge voltage light status led  
  if (sensorValue < setpointvalue){   // indicater light on in charge mode or fault voltage below 7 volts 
     digitalWrite(setupledpin, HIGH);
  } 
    else
   {
   digitalWrite(setupledpin, LOW);
  }

 
  
// ok now we make our decisions on the values above 
// pwm value  60 is about 5% pw car should have min of 10%
if (pwm_value< 60 && sensorValue>240 ){   // car  pluged in pilot voltage 9 volt no pwm signal 
     incharge=0;
     car_terminated=0;
     
       Serial.println ("pilot > 9 volt car pluged In waiting for PWM !  ");  
             Serial.print("sensor value " );
              Serial.println(sensorValue);  
               Serial.print ("pwm_value  ");
                Serial.println (pwm_value); 
                 Serial.print ("In charge mode status  ");
                   Serial.println (incharge );
                    Serial.print ("Car teminated status  ");
                     Serial.println (car_terminated);
                     Serial.println ("");
                      Serial.print ("");

                    
  }



if (pwm_value< 60 && sensorValue>360 ){   // car not pluged in pilot voltage 12 volt 
     incharge=0;
     car_terminated=0;
     
       Serial.println ("pilot > 9 volt car not pluged In !  ");  
             Serial.print("sensor value " );
              Serial.println(sensorValue);  
               Serial.print ("pwm_value  ");
                Serial.println (pwm_value); 
                 Serial.print ("In charge mode status  ");
                   Serial.println (incharge );
                    Serial.print ("Car teminated status  ");
                     Serial.println (car_terminated);
                     Serial.println ("");
                       Serial.print ("");

                      
}


 if (pwm_value> 60 && sensorValue>240 && incharge==0){  // we are with pwm and at 9 volt  
  sec_delay(5);  // lets give car time to respond might have just changed 5 seconds
    Get_sensor();  // read pwm and volt  have things changed ?

        if (pwm_value> 60 && sensorValue>240 && incharge==0){  // we are with pwm and at 9 volt lets do a reset 
  
           Serial.println ("stuck at 9 volt Doing a reset   ");  
             Serial.print("sensor value " );
              Serial.println(sensorValue);  
               Serial.print ("pwm_value  ");
                Serial.println (pwm_value); 
                 Serial.print ("In charge mode status  ");
                   Serial.println (incharge );
                    Serial.print ("Car teminated status  ");
                     Serial.println (car_terminated);
                     Serial.println ("");
                       Serial.print ("");
                   
             digitalWrite(RelPin, HIGH);   // turn on relay this removes the pilot signal from car  
               Serial.println("relay on " );
                 sec_delay(5);                   // wait 5 sec 
    
             // turn the relPin off:        //  turn off relay car conected, same as pluging it in again 
               digitalWrite(RelPin, LOW);
                 Serial.println("relay off " );

                 sec_delay(5); // wait 5 sec befor we check values again 

                    Serial.println("read values again are we good now  " );

                    Get_sensor();  // read pwm and volt  have things changed ?
                                   // we can fall out and set flag next if statment 

                                   Serial.print("sensor value " );
                                    Serial.println(sensorValue);  
                                      Serial.print ("pwm_value  ");
                                       Serial.println (pwm_value); 
                                        Serial.print ("In charge mode status  ");
                                         Serial.println (incharge );
                                          Serial.print ("Car teminated status  ");
                                           Serial.println (car_terminated);
                                            Serial.println ("");
                                             Serial.print ("");
        }
 }

if (pwm_value> 60 && sensorValue<235&& incharge==0 ){  // we are Charging yes it worked 
  incharge = 1;   // status  flag incharge  means car has been in charge mode
  car_terminated= 0 ; // reset car teminated as we are charging again 
  Serial.println("we should be charging now. YES !  " );
    Serial.print("sensor value " );
      Serial.println(sensorValue);  
       Serial.print ("pwm_value  ");
         Serial.println (pwm_value); 
          Serial.print ("In charge mode status  ");
            Serial.println (incharge );
            Serial.print ("Car teminated status  ");
            Serial.println (car_terminated);
             Serial.println ("");
            Serial.print ("");
} 
if (pwm_value> 60 && sensorValue<235&& incharge==1&& car_terminated==1){  // we are Charging for a second time this could be car with preheat 
  incharge = 1;   // status  flag incharge  means car has been in charge mode
  car_terminated= 0 ; // reset car teminated as we are charging again 
  Serial.println("Car has Requsted additional charging now  " );
     Serial.print("sensor value " );
      Serial.println(sensorValue);  
       Serial.print ("pwm_value  ");
         Serial.println (pwm_value); 
          Serial.print ("In charge mode status  ");
            Serial.println (incharge );
             Serial.print ("Car teminated status  ");
              Serial.println (car_terminated);
               Serial.println ("");
                Serial.print ("");
} 




if (pwm_value< 60 && sensorValue>240&&incharge==1){   // we can now reset incharge flag 
     incharge = 0;  // reset so we can give it a kick again or start a following  timed charge 
      car_terminated=0;
    Serial.println(" no pwm  ,we are at 12 volt or 9 volt reset ready for next charge  " ); 
     Serial.print("sensor value " );
      Serial.println(sensorValue);  
       Serial.print ("pwm_value  ");
         Serial.println (pwm_value);
           Serial.print ("In charge mode status  ");
            Serial.println (incharge );
              Serial.print ("Car teminated status  ");
                Serial.println (car_terminated);
                 Serial.println ("");
                  Serial.print ("");
}
 if (pwm_value> 60 && sensorValue>240&&incharge==1 &&car_terminated==0){   // the car has terminated the charge  we do nothing as car can start charge again if it wants 
   car_terminated = 1;
 Serial.println("we have  pwm and  9 volt car has terminated the charge   " ); 
  Serial.print("sensor value " );
      Serial.println(sensorValue);  
       Serial.print ("pwm_value  ");
         Serial.println (pwm_value);
            Serial.print ("In charge mode status  ");
             Serial.println (incharge );
              Serial.print ("Car teminated status  ");
               Serial.println (car_terminated);
                Serial.println ("");
                 Serial.print ("");
           
}


 if (pwm_value< 60 && sensorValue<238&&incharge==1){   // we have lost pwm signal but still pilot <9volt this is a fault 
 Serial.println("we have  lost pwm and  pilot < 9 volt car in fault mode ?    " ); 
    Serial.print("sensor value " );
      Serial.println(sensorValue);  
       Serial.print ("pwm_value  ");
         Serial.println (pwm_value);
            Serial.print ("In charge mode status  ");
            Serial.println (incharge );
             Serial.print ("Car teminated status  ");
              Serial.println (car_terminated);
               Serial.println ("");
                Serial.print ("");
 }    
  //  Serial.print ("  force wtd   ");
  //  while (true){
   //    }
    
    sec_delay(3);  //we loop every 3 seconds why rush save Energy 
}

   void Get_sensor(){
    // read the values 
       sensorValue = analogRead(sensorPin);  // pilot voltage 
          pwm_value = pulseIn(PWM_PIN,HIGH);  // pilot pwm duty msec 
 
   }

   void sec_delay(unsigned int x){  // this gives delay while resetind wdt
    while ( x<0 );
    {
       
      delay(1000);
      wdt_reset();
      x=x-1;
    }

   }
   
void watchdogSetup(void)
{
cli();  // disable all interrupts
wdt_reset(); // reset the WDT timer
/*
 WDTCSR configuration:
 WDIE = 1: Interrupt Enable
 WDE = 1 :Reset Enable
 WDP3 = 0 :For 2000ms Time-out
 WDP2 = 1 :For 2000ms Time-out
 WDP1 = 1 :For 2000ms Time-out
 WDP0 = 1 :For 2000ms Time-out
*/
// Enter Watchdog Configuration mode:
WDTCSR |= (1<<WDCE) | (1<<WDE);
// Set Watchdog settings:
//         1           1          0           1           1           1
 WDTCSR = (0<<WDIE) | (1<<WDE) | (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);
sei();
}

ISR(WDT_vect) // Watchdog timer interrupt.  (not used in this code) 
{
// Include your code here - be careful not to use functions they may cause the interrupt to hang and
// prevent a reset.
}
