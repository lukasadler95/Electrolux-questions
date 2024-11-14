#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h> // For sleep()

#define PROTECTED_TIME 10

typedef enum {
    OFF,
    ON,
    PROTECTED
} State;

State currentState = OFF;
bool buttonPressed = false;
time_t protectedStartTime;

void updateButtonState(bool pressed) {
    buttonPressed = pressed;
}

void stateMachine() {
    switch (currentState) {
        case OFF:
            if (buttonPressed) {
                currentState = ON;
                printf("State changed to ON\n");
            }
            break;
        case ON:
            if (!buttonPressed) {
                currentState = PROTECTED;
                protectedStartTime = time(NULL);
                printf("State changed to PROTECTED\n");
            }
            break;
        case PROTECTED:
            if (difftime(time(NULL), protectedStartTime) >= PROTECTED_TIME) {
                currentState = OFF;
                printf("State changed to OFF\n");
            }
            break;
    }
}

int main() {
    updateButtonState(true);
    stateMachine();
    updateButtonState(false);
    stateMachine();

    for (int i = 0; i < PROTECTED_TIME; i++) {
        printf("Waiting: %d seconds\n", i + 1);
        stateMachine();
        sleep(1);
    }
    
    stateMachine();
    
    return 0;
}
