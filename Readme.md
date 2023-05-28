 We have three tasks represented by task1(), task2(), and task3(). Each task simulates a spinning motor with a different speed by toggling the corresponding pins connected to the motors. The context switching is performed in the contextSwitch() function, which saves the context of the current task and loads the context of the next task.

The initializePorts() function sets the corresponding pins as outputs for the motor control. The initializeTimers() function configures Timer0 for a 1ms interrupt using the CTC mode. The ISR(TIMER0_COMPA_vect) interrupt service routine is triggered every 1ms, and it calls the current task's function.

Note that this example uses the avr/io.h and avr/interrupt.h headers for AVR-specific registers and interrupts. Make sure you have the necessary AVR toolchain and libraries set up to compile and upload the code to the ATmega328P microcontroller
