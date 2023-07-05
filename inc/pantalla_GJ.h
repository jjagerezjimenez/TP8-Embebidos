#ifndef PANTALLA_GJ_H
#define PANTALLA_GJ_H

#include <stdint.h>
#include <stdbool.h>



#define SEGMENT_A (1 << 0)
#define SEGMENT_B (1 << 1)
#define SEGMENT_C (1 << 2)
#define SEGMENT_D (1 << 3)
#define SEGMENT_E (1 << 4)
#define SEGMENT_F (1 << 5)
#define SEGMENT_G (1 << 6)
#define SEGMENT_P (1 << 7)


typedef struct display_s * display_t;

typedef void (*display_screen_off_t)(void);

typedef void(*display_segments_on_t)(uint8_t segments);

typedef void(*display_digit_on_t)(uint8_t digit);


typedef struct display_driver_s{
    display_screen_off_t    ScreenTurnOff;
    display_segments_on_t   SegmentsTurnOn;
    display_digit_on_t      DigitTurnOn;
} const * const display_driver_t;



display_t Display_Create(uint8_t digits, display_driver_t driver);

void Display_WriteBCD(display_t display, uint8_t * number, uint8_t size);

void Display_Refresh(display_t display);


#endif