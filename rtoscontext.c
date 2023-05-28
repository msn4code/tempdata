#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define MAX_TASKS 3
#define STACK_SIZE 128

typedef struct {
    uint8_t stack[STACK_SIZE];
    void (*taskFn)(void);
} TaskControlBlock;

TaskControlBlock tcb[MAX_TASKS];
volatile uint8_t currentTask = 0;

void task1(void) {
    while (1) {
        // Task 1: Slow spinning motor
        PORTB |= (1 << PB0);
        _delay_ms(100);
        PORTB &= ~(1 << PB0);
        _delay_ms(100);
    }
}

void task2(void) {
    while (1) {
        // Task 2: Medium spinning motor
        PORTB |= (1 << PB1);
        _delay_ms(50);
        PORTB &= ~(1 << PB1);
        _delay_ms(50);
    }
}

void task3(void) {
    while (1) {
        // Task 3: Fast spinning motor
        PORTB |= (1 << PB2);
        _delay_ms(25);
        PORTB &= ~(1 << PB2);
        _delay_ms(25);
    }
}

void initializePorts(void) {
    // Set PB0, PB1, PB2 as outputs
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);
}

void initializeTimers(void) {
    // Configure Timer0 for CTC mode
    TCCR0A = (1 << WGM01);
    
    // Set prescaler to 64
    TCCR0B = (1 << CS01) | (1 << CS00);
    
    // Set compare value for 1ms interrupt
    OCR0A = 249;
    
    // Enable compare match interrupt
    TIMSK0 = (1 << OCIE0A);
}

void contextSwitch(void) {
    // Save the context of the current task
    asm volatile("push r0\n\t"
                 "push r1\n\t"
                 "push r2\n\t"
                 "push r3\n\t"
                 "push r4\n\t"
                 "push r5\n\t"
                 "push r6\n\t"
                 "push r7\n\t"
                 "push r8\n\t"
                 "push r9\n\t"
                 "push r10\n\t"
                 "push r11\n\t"
                 "push r12\n\t"
                 "push r13\n\t"
                 "push r14\n\t"
                 "push r15\n\t"
                 "push r16\n\t"
                 "push r17\n\t"
                 "push r18\n\t"
                 "push r19\n\t"
                 "push r20\n\t"
                 "push r21\n\t"
                 "push r22\n\t"
                 "push r23\n\t"
                 "push r24\n\t"
                 "push r25\n\t"
                 "push r26\n\t"
                 "push r27\n\t"
                 "push r28\n\t"
                 "push r29\n\t"
                 "push r30\n\t"
                 "push r31\n\t"
                 "in r0, __SREG__\n\t"
                 "cli\n\t"
                 "push r0\n\t");
    
    // Update the current task index
    currentTask = (currentTask + 1) % MAX_TASKS;
    
    // Load the context of the next task
    asm volatile("pop r0\n\t"
                 "out __SREG__, r0\n\t"
                 "pop r31\n\t"
                 "pop r30\n\t"
                 "pop r29\n\t"
                 "pop r28\n\t"
                 "pop r27\n\t"
                 "pop r26\n\t"
                 "pop r25\n\t"
                 "pop r24\n\t"
                 "pop r23\n\t"
                 "pop r22\n\t"
                 "pop r21\n\t"
                 "pop r20\n\t"
                 "pop r19\n\t"
                 "pop r18\n\t"
                 "pop r17\n\t"
                 "pop r16\n\t"
                 "pop r15\n\t"
                 "pop r14\n\t"
                 "pop r13\n\t"
                 "pop r12\n\t"
                 "pop r11\n\t"
                 "pop r10\n\t"
                 "pop r9\n\t"
                 "pop r8\n\t"
                 "pop r7\n\t"
                 "pop r6\n\t"
                 "pop r5\n\t"
                 "pop r4\n\t"
                 "pop r3\n\t"
                 "pop r2\n\t"
                 "pop r1\n\t"
                 "pop r0\n\t"
                 "reti\n\t");
}

int main(void) {
    // Initialize ports and timers
    initializePorts();
    initializeTimers();
    
    // Set the task functions in TCB
    tcb[0].taskFn = task1;
    tcb[1].taskFn = task2;
    tcb[2].taskFn = task3;
    
    // Enable global interrupts
    sei();
    
    while (1) {
        // Context switch between tasks
        contextSwitch();
    }

    return 0;
}

// Timer0 compare match ISR
ISR(TIMER0_COMPA_vect) {
    // Call the task function
    tcb[currentTask].taskFn();
}
