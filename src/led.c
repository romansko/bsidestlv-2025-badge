#include <stdio.h>
#include <ch32v003fun.h>
#include "led.h"

// LED state tracking for toggling
static uint8_t led_states = 0; // Bit 0-5 for each LED

void turn_on_led(int pin)
{
    funDigitalWrite(pin, FUN_HIGH);
}

void turn_off_led(int pin)
{
    funDigitalWrite(pin, FUN_LOW);
}

// Toggle LED state
void toggle_led(int pin, int led_bit)
{
    if (led_states & (1 << led_bit))
    {
        turn_off_led(pin);
        led_states &= ~(1 << led_bit);
    }
    else
    {
        turn_on_led(pin);
        led_states |= (1 << led_bit);
    }
}

void blink_finger_custom_delay(int pin, int n, int delay)
{
    int i;
    for (i = 0; i < n; ++i)
    {
        turn_on_led(pin);
        Delay_Ms(delay);
        turn_off_led(pin);
        Delay_Ms(delay);
    }
}


void blink_finger(int pin, int n)
{
    blink_finger_custom_delay(pin, n, 100);
}



void blink_fingers_in_sequence(void)
{
    blink_finger(PIN_LITTLE_LED, 3);
    blink_finger(PIN_RING_LED, 3);
    blink_finger(PIN_MIDDLE_LED, 3);
    blink_finger(PIN_INDEX_LED, 3);
    blink_finger(PIN_THUMB_LED, 3);
    blink_finger(PIN_HAND_LED, 3);
}

void turn_off_all_leds(void)
{
    turn_off_led(PIN_LITTLE_LED);
    turn_off_led(PIN_RING_LED);
    turn_off_led(PIN_MIDDLE_LED);
    turn_off_led(PIN_INDEX_LED);
    turn_off_led(PIN_THUMB_LED);
    turn_off_led(PIN_HAND_LED);
}

void boot_sequence(void)
{
    int delay = 75;
    for (int i = 0; i < 3; i++)
    {
        turn_on_led(PIN_LITTLE_LED);
        Delay_Ms(delay);
        turn_on_led(PIN_RING_LED);
        Delay_Ms(delay);
        turn_on_led(PIN_MIDDLE_LED);
        Delay_Ms(delay);
        turn_on_led(PIN_INDEX_LED);
        Delay_Ms(delay);
        turn_on_led(PIN_THUMB_LED);
        Delay_Ms(delay);
        turn_on_led(PIN_HAND_LED);
        Delay_Ms(delay);
        turn_off_led(PIN_LITTLE_LED);
        Delay_Ms(delay);
        turn_off_led(PIN_RING_LED);
        Delay_Ms(delay);
        turn_off_led(PIN_MIDDLE_LED);
        Delay_Ms(delay);
        turn_off_led(PIN_INDEX_LED);
        Delay_Ms(delay);
        turn_off_led(PIN_THUMB_LED);
        Delay_Ms(delay);
        turn_off_led(PIN_HAND_LED);
        Delay_Ms(delay);
    }
    turn_off_all_leds();
}

void blink_all_leds(int n)
{
    int i;

    for (i = 0; i < n; ++i)
    {
        turn_on_led(PIN_LITTLE_LED);
        turn_on_led(PIN_RING_LED);
        turn_on_led(PIN_MIDDLE_LED);
        turn_on_led(PIN_INDEX_LED);
        turn_on_led(PIN_THUMB_LED);
        turn_on_led(PIN_HAND_LED);
        Delay_Ms(100);
        turn_off_led(PIN_LITTLE_LED);
        turn_off_led(PIN_RING_LED);
        turn_off_led(PIN_MIDDLE_LED);
        turn_off_led(PIN_INDEX_LED);
        turn_off_led(PIN_THUMB_LED);
        turn_off_led(PIN_HAND_LED);
        Delay_Ms(100);
    }
}

// Check if all LEDs are on
int are_all_leds_on(void)
{
    // Check if all LED state bits are set (bits 0-5 for the 6 LEDs)
    uint8_t all_on_mask = (1 << LED_STATE_LITTLE) | (1 << LED_STATE_RING) | 
                          (1 << LED_STATE_MIDDLE) | (1 << LED_STATE_INDEX) | 
                          (1 << LED_STATE_THUMB) | (1 << LED_STATE_HAND);
    return (led_states & all_on_mask) == all_on_mask;
}

// Handle LED control based on 4-bit value
void handle_led_challenge(uint8_t value)
{
    switch (value)
    {
        case 0: // 0000 - Power off all LEDs
            turn_off_led(PIN_INDEX_LED);
            turn_off_led(PIN_RING_LED);
            turn_off_led(PIN_MIDDLE_LED);
            turn_off_led(PIN_LITTLE_LED);
            turn_off_led(PIN_THUMB_LED);
            turn_off_led(PIN_HAND_LED);
            led_states = 0;
            printf("All LEDs OFF\r\n");
            break;
            
        case 1: // 0001 - Toggle little finger
            toggle_led(PIN_LITTLE_LED, LED_STATE_LITTLE);
            printf("Little finger LED toggled\r\n");
            break;
            
        case 2: // 0010 - Toggle ring finger
            toggle_led(PIN_RING_LED, LED_STATE_RING);
            printf("Ring finger LED toggled\r\n");
            break;
            
        case 3: // 0011 - Toggle middle finger
            toggle_led(PIN_MIDDLE_LED, LED_STATE_MIDDLE);
            printf("Middle finger LED toggled\r\n");
            break;
            
        case 4: // 0100 - Toggle index finger
            toggle_led(PIN_INDEX_LED, LED_STATE_INDEX);
            printf("Index finger LED toggled\r\n");
            break;
            
        case 5: // 0101 - Toggle thumb
            toggle_led(PIN_THUMB_LED, LED_STATE_THUMB);
            printf("Thumb LED toggled\r\n");
            break;
            
        case 6: // 0110 - Toggle hand
            toggle_led(PIN_HAND_LED, LED_STATE_HAND);
            printf("Hand LED toggled\r\n");
            break;
            
        default:
            printf("Value %d: No action\r\n", value);
            break;
    }
}

void led_init(void)
{
    // Initialize all LED pins as output push-pull
    funPinMode(PIN_HAND_LED, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);
    funPinMode(PIN_THUMB_LED, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);
    funPinMode(PIN_LITTLE_LED, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);
    funPinMode(PIN_RING_LED, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);
    funPinMode(PIN_MIDDLE_LED, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);
    funPinMode(PIN_INDEX_LED, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);

    // Initialize all LEDs to LOW (off)
    turn_off_led(PIN_HAND_LED);
    turn_off_led(PIN_THUMB_LED);
    turn_off_led(PIN_LITTLE_LED);
    turn_off_led(PIN_RING_LED);
    turn_off_led(PIN_MIDDLE_LED);
    turn_off_led(PIN_INDEX_LED);
    
    // Reset LED states
    led_states = 0;
}

void challenge_success(int pin)
{
    blink_finger_custom_delay(pin, 10, 10);
    blink_finger_custom_delay(pin, 10, 20);
    blink_finger_custom_delay(pin, 10, 30);
    blink_finger_custom_delay(pin, 10, 40);
    turn_on_led(pin);
    Delay_Ms(50);
    // printf("Challenge success for pin %d\r\n", pin);
}

void challenge_failure(int pin)
{
    blink_finger_custom_delay(pin, 10, 10);
    blink_finger_custom_delay(pin, 10, 20);
    blink_finger_custom_delay(pin, 10, 30);
    blink_finger_custom_delay(pin, 10, 40);
    turn_off_led(pin);
    Delay_Ms(10);
    // printf("Challenge failure for pin %d\r\n", pin);
}

void all_challenges_success(void)
{
    turn_off_all_leds();
    
    while (1)
    {   
    turn_on_led(PIN_HAND_LED);
    Delay_Ms(1000);
    turn_on_led(PIN_MIDDLE_LED);
    Delay_Ms(1000);
    turn_on_led(PIN_RING_LED);
    turn_on_led(PIN_INDEX_LED);
    Delay_Ms(1000);
    turn_on_led(PIN_LITTLE_LED);
    turn_on_led(PIN_THUMB_LED);
    Delay_Ms(1000);
    turn_off_all_leds();
    // Delay_Ms(500);
    }
}