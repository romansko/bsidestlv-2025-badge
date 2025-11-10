#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

// Button pin definitions
#define PIN_BTN_0 PD0
#define PIN_BTN_1 PC0

// Button timing constants
#define BUTTON_DEBOUNCE_MS 50
#define BUTTON_PRESS_TIMEOUT_MS 5000

// Button control functions
void button_init(void);
int wait_for_button_press(void);
int capture_4_bits(void);
void button_challenge(void);

#endif // BUTTON_H

