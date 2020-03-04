# MG_ZS_Wakeup
EV charger addition to wake up the MG ZS EV   when its time to charge solves problem of delayed Charge
This is a simple board that can be made on a proto board to mount on a aurdino UNO   or the  Nano 
the relay is used to break the Pilot signal that is conected to the lead that runs to The EV 
This will Remove the 12volt signal for a few seconds and then reconect it . 
This wakes the car up to charge as it is the same as plugging the car In to charge .
the princble of opration is as follows 
with no car conected the pilot signal is at 12 volts dc   the board will take no action relay is not powerd and the pilot signal is conected . 
when a car is conected the pilot signal is pulled down to 9 volts the board will see this and will then wait to see a 
1khz PWM signal +9 volt -12volt DC  being trasmited by the EV charger to the Car . when it sees this condition it will expect to see the + 9volt signal drop to +6volt as the car requsts the charger to conect 
240 volt AC   it will wait 15 seconds for this to take place if it does then the board will do nothing 
as the car is charging .. 
if the signal does not drop to 6 volts the after 15 seconds it will power the relay for 5 seconds and break 
the pilot line to the car causing the car to reset and wake up . when the pilot line signal is restored 
the car will now compleat the above procedure and begin to charge. 
once a charge has been started the board will set a flag and if the car terminates the charge by letting the pilot signal return to 9 volts it will note that the car has teminated the charge and do nothing . 
if the car requsts additional charge by making the pilot 6 volts again the board will do nothing and let the charge start the board is reset by the voltage going back to 9 volts and the pwm signal being removed 
