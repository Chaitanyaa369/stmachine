ARM MCBSTM32C Finite State Machine with timers and interrupts

This project illustrates the behavior of a finite state machine using timers and interrupts. We use the case of a Mealy machine. The Mealy machine is a finite state machine whose output values are determined both by its current state and the current inputs.

Finite state machine is created to control a toy car’s servomotor and the 08 LEDs using the ARM board. The toy car has 4 gears: Neutral – Gear 1 -­‐ Gear 2 -­‐ Gear 3 – Gear 4

The car starts in neutral state
a-­‐ The car waits for the user to press the ‘WAKEUP’ push button. (interrupt)
b-­‐ The car switches to Gear 1 and drives for 3 seconds. (timer)
c-­‐ The car switches to Gear 2 and drives for 5 seconds. (timer)
d-­‐ The car switches to Gear 3 and holds the same gear until the driver makes a choice. (interrupt)
-­‐ if the driver presses the ‘TAMPER’ push button, the car downshift to Gear 2.
-­‐ if the driver presses the ‘WAKEUP’ push button, the car should cycle through the 8 LEDS on the
board switches then switch to Gear 4.
e-­‐ In Gear 4, the car drives for 6 seconds and switches back to Neutral, the initial state. (timer)

The state machine uses a combination of timers loops and interrupts. You will need 5 case statements to implement the state machine. Steps a and d use interrupts. Steps b, c and e use timers.
The LCD is ued to display the current states and operations of the program.
