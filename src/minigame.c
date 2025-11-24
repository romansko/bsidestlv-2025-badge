#include <stdio.h>
#include <ch32v003fun.h>
#include "include/minigame.h"
#include "include/button.h"
#include "include/led.h"

// Minigame: Hold button and release when all 5 LEDs are lit
// Loops until success
// Returns: 1 on success (always returns 1 since it loops until success)
int led_fill_minigame(int fill_rate_ms)
{
    const int LED_DELAY_MS = fill_rate_ms; // Delay between each LED lighting up
    const int DEBOUNCE_MS = 50;
    const int CHECK_INTERVAL_MS = 10;
    
    // Array of 6 LEDs
    int leds[6] = {
        PIN_LITTLE_LED,
        PIN_RING_LED,
        PIN_MIDDLE_LED,
        PIN_INDEX_LED,
        PIN_THUMB_LED,
        PIN_HAND_LED
    };
    
    printf("=== LED Fill Minigame ===\r\n");
    printf("Hold BTN_0 and release when all 6 LEDs are lit!\r\n");
    
    // Loop until success
    while (1)
    {
        printf("Waiting for button press...\r\n");
        
        // Wait for button press with debouncing
        int btn_pressed = 0;
        int debounce_count = 0;
        
        while (1)
        {
            int btn_state = !funDigitalRead(PIN_BTN_0); // 1 when pressed
            
            if (btn_state)
            {
                if (!btn_pressed)
                {
                    debounce_count++;
                    if (debounce_count >= (DEBOUNCE_MS / CHECK_INTERVAL_MS))
                    {
                        btn_pressed = 1;
                        break; // Button is pressed and debounced
                    }
                }
            }
            else
            {
                debounce_count = 0;
            }
            
            Delay_Ms(CHECK_INTERVAL_MS);
        }
        
        printf("Button pressed! Filling LEDs...\r\n");
        
        // Turn off all LEDs first
        for (int i = 0; i < 6; i++)
        {
            turn_off_led(leds[i]);
        }
        
        // Fill LEDs one by one while checking if button is still held
        int current_led = 0;
        int button_released = 0;
        
        while (current_led < 6)
        {
            // Check if button is still held
            int btn_state = !funDigitalRead(PIN_BTN_0);
            
            if (!btn_state)
            {
                // Button was released too early
                button_released = 1;
                printf("Button released too early! (LED %d/6)\r\n", current_led);
                break;
            }
            
            // Turn on current LED
            turn_on_led(leds[current_led]);
            current_led++;
            
            // Small delay before checking next LED (except for the last one)
            if (current_led < 6)
            {
                Delay_Ms(LED_DELAY_MS);
            }
        }
        
        // If we reached here and button wasn't released early, check if it's released now
        if (!button_released)
        {
            // Give a small window to check if button is released (grace period)
            Delay_Ms(LED_DELAY_MS);
            
            int btn_state = !funDigitalRead(PIN_BTN_0);
            
            if (!btn_state)
            {
                // Success! Button was released when all LEDs are lit
                printf("SUCCESS! Perfect timing!\r\n");
                
                // Success animation
                for (int i = 0; i < 6; i++)
                {
                    blink_all_leds(1);
                    Delay_Ms(50);
                }
                
                // Turn off all LEDs to show success
                for (int i = 0; i < 6; i++)
                {
                    turn_off_led(leds[i]);
                }
                
                return 1; // Success - exit the function
            }
            else
            {
                // Button still held - failure
                printf("FAILED! Button held too long. Release when all LEDs are lit!\r\n");
                button_released = 0; // Mark as failure (button still held)
                // turn off all LEDs from 5 to 0
                for (int i = 5; i >= 0; i--)
                {
                    turn_off_led(leds[i]);
                    Delay_Ms(LED_DELAY_MS);
                }
            }
        }
        
        // Failure case
        if (button_released)
        {
            // Button was released too early
            printf("FAILED! Try again.\r\n");
            
            // Failure animation - blink the LEDs that were lit
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < current_led; j++)
                {
                    turn_off_led(leds[j]);
                }
                Delay_Ms(100);
                for (int j = 0; j < current_led; j++)
                {
                    turn_on_led(leds[j]);
                }
                Delay_Ms(100);
            }
        }
        
        // Turn off all LEDs on failure
        for (int i = 0; i < 6; i++)
        {
            turn_off_led(leds[i]);
        }
        
        // Wait a bit before restarting
        Delay_Ms(500);
        printf("\r\n");
    }
}

