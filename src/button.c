#include <stdio.h>
#include <ch32v003fun.h>
#include "button.h"
#include "led.h"



void button_init(void)
{
    // Initialize button pins as input with pull-up (assuming buttons pull to ground when pressed)
    // If buttons are active-high, change to GPIO_CNF_IN_FLOATING
    funPinMode(PIN_BTN_0, GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD);
    funPinMode(PIN_BTN_1, GPIO_Speed_10MHz | GPIO_CNF_IN_PUPD);
}

// Wait for button press with debouncing
// Returns: 0 for BTN_0, 1 for BTN_1, -1 for timeout
int wait_for_button_press(void)
{
    int timeout_count = 0;
    int btn0_pressed = 0;
    int btn1_pressed = 0;
    int btn0_debounce_count = 0;
    int btn1_debounce_count = 0;
    
    while (timeout_count < (BUTTON_PRESS_TIMEOUT_MS / 10))
    {
        // Read button states (assuming active-low: pressed = 0, released = 1)
        // If your buttons are active-high, remove the ! operator
        int btn0_state = !funDigitalRead(PIN_BTN_0); // Inverted: 1 when pressed
        int btn1_state = !funDigitalRead(PIN_BTN_1); // Inverted: 1 when pressed
        
        // Detect button 0 press
        if (btn0_state)
        {
            if (!btn0_pressed)
            {
                btn0_pressed = 1;
                btn0_debounce_count = 0;
            }
            else
            {
                btn0_debounce_count++;
                if (btn0_debounce_count >= (BUTTON_DEBOUNCE_MS / 10))
                {
                    // Wait for release
                    while (btn0_state)
                    {
                        Delay_Ms(10);
                        btn0_state = !funDigitalRead(PIN_BTN_0);
                        timeout_count++;
                        if (timeout_count >= (BUTTON_PRESS_TIMEOUT_MS / 10))
                        {
                            return -1;
                        }
                    }
                    return 0; // BTN_0 was pressed and released
                }
            }
        }
        else
        {
            btn0_pressed = 0;
            btn0_debounce_count = 0;
        }
        
        // Detect button 1 press
        if (btn1_state)
        {
            if (!btn1_pressed)
            {
                btn1_pressed = 1;
                btn1_debounce_count = 0;
            }
            else
            {
                btn1_debounce_count++;
                if (btn1_debounce_count >= (BUTTON_DEBOUNCE_MS / 10))
                {
                    // Wait for release
                    while (btn1_state)
                    {
                        Delay_Ms(10);
                        btn1_state = !funDigitalRead(PIN_BTN_1);
                        timeout_count++;
                        if (timeout_count >= (BUTTON_PRESS_TIMEOUT_MS / 10))
                        {
                            return -1;
                        }
                    }
                    return 1; // BTN_1 was pressed and released
                }
            }
        }
        else
        {
            btn1_pressed = 0;
            btn1_debounce_count = 0;
        }
        
        Delay_Ms(10);
        timeout_count++;
    }
    
    return -1; // Timeout
}

// Capture 4 bits from button presses
// Returns the 4-bit value (0-15), or -1 on timeout
int capture_4_bits(void)
{
    uint8_t value = 0;
    int bit;
    
    printf("Enter 4-bit value (BTN_0=0, BTN_1=1):\r\n");
    
    for (bit = 0; bit < 4; bit++)
    {
        printf("Bit %d: ", bit);
        
        int button = wait_for_button_press();
        
        if (button == -1)
        {
            printf("Timeout\r\n");
            return -1;
        }
        
        value |= (button << bit);
        printf("%d\r\n", button);
        
        // Small delay between bits
        Delay_Ms(200);
    }
    
    printf("Captured: %d (0x%02x)\r\n", value, value);
    return value;
}

// Main button challenge function
void button_challenge(void)
{
    printf("=== Button Challenge ===\r\n");
    printf("Press BTN_0 for 0, BTN_1 for 1\r\n");
    printf("Enter 4 bits to control LEDs\r\n");
    printf("Exit: Turn on all LEDs, then enter 1111\r\n\r\n");
    
    while (1)
    {
        // Check if all LEDs are on before capturing next sequence
        if (are_all_leds_on())
        {
            printf("All LEDs are ON! Enter 1111 to exit, or any other value to continue...\r\n");
            int value = capture_4_bits();
            
            if (value == -1)
            {
                continue;
            }
            
            // If all LEDs are on and user entered 1111 (15), exit
            if (value == 15)
            {
                printf("Challenge completed! Exiting...\r\n");
                blink_all_leds(3);
                challenge_success(PIN_LITTLE_LED);
                break;
            }
            
            // Otherwise, handle the LED challenge normally
            handle_led_challenge(value);
            printf("\r\n");
        }
        else
        {
            // Normal operation: capture and handle LED challenge
            int value = capture_4_bits();
            
            if (value == -1)
            {
                continue;
            }
            
            handle_led_challenge(value);
            printf("\r\n");
        }
    }
}

