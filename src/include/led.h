#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <ch32v003fun.h>

// LED pin definitions
#define PIN_HAND_LED PC4
#define PIN_THUMB_LED PD4
#define PIN_LITTLE_LED PD3
#define PIN_RING_LED PD2
#define PIN_MIDDLE_LED PC2
#define PIN_INDEX_LED PC1

// LED state bit positions
#define LED_STATE_LITTLE 0
#define LED_STATE_RING   1
#define LED_STATE_MIDDLE 2
#define LED_STATE_INDEX  3
#define LED_STATE_THUMB  4
#define LED_STATE_HAND   5

// LED control functions
void led_init(void);
void turn_on_led(int pin);
void turn_off_led(int pin);
void toggle_led(int pin, int led_bit);
void blink_finger(int pin, int n);
void blink_finger_custom_delay(int pin, int n, int delay);
void blink_fingers_in_sequence(void);
void turn_off_all_leds(void);
void boot_sequence(int _delay);
void blink_all_leds(int n);
int are_all_leds_on(void);
void handle_led_challenge(uint8_t value);
void challenge_success(int pin);
void challenge_failure(int pin);
void all_challenges_success(void);
#endif // LED_H

