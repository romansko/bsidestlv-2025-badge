#ifndef MINIGAME_H
#define MINIGAME_H

#include <stdint.h>

// Minigame function
// Hold button and release when all LEDs are lit
// Loops until success
// Returns: 1 on success (always returns 1 since it loops until success)
int led_fill_minigame(void);

#endif // MINIGAME_H

