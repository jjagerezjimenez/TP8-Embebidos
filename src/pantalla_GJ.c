#include "pantalla_GJ.h"
#include <string.h>
#include "chip.h"

#ifndef DISPLAY_MAX_DIGITS
#define DISPLAY_MAX_DIGITS 8 //consultar
#endif

struct display_s {
    uint8_t     digits;
    uint8_t     active_digit;

    uint8_t parpadeo_desde;
    uint8_t parpadeo_hasta;
    uint8_t parpadeo_frecuencia;
    uint8_t parpadeo_contador;

    uint8_t     memory[DISPLAY_MAX_DIGITS];
    struct display_driver_s driver[1];      // el uno viene de una "travesura", puntero a una estructura y usar flecha simepre
};


static const uint8_t IMAGES[] = {
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F,             //! < 0
    SEGMENT_B | SEGMENT_C,                                                             //! < 1
    SEGMENT_A | SEGMENT_B | SEGMENT_D | SEGMENT_E | SEGMENT_G,                         //! < 2
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_G,                         //! < 3
    SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G,                                     //! < 4
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G,                         //! < 5
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,             //! < 6
    SEGMENT_A | SEGMENT_B | SEGMENT_C,                                                 //! < 7
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G, //! < 8
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G,                         //! < 9
};



static display_t Display_Ocupado(void);

display_t Display_Ocupado(void) {
    static struct display_s instances[1] = {0};
    return &instances[0];
}


display_t Display_Create(uint8_t digits, display_driver_t driver){
    display_t display = Display_Ocupado();

    if(display) {
        display         ->  digits = digits;
        display         ->  active_digit = digits - 1;

        display         ->  parpadeo_desde = 0;
        display         ->  parpadeo_hasta = 0;
        display         ->  parpadeo_frecuencia = 0;
        display         ->  parpadeo_contador = 0;

        memcpy(display  ->  driver, driver, sizeof(display->driver));
        memset(display  ->  memory, 0, sizeof(display -> memory));
        display         ->driver -> ScreenTurnOff();

    }
    return display;
}

void Display_WriteBCD(display_t display, uint8_t * number, uint8_t size) {
    //memset(display -> memory, 0, sizeof(display -> memory));
    for(int i = 0; i < size; i++){
        if(i >= display -> digits) break;
        display -> memory[i] &= SEGMENT_P;
        display -> memory[i] |= IMAGES[number[i]];
       

    }

}

void Display_Refresh(display_t display){
    uint8_t segmentos;

    display -> driver -> ScreenTurnOff();
    display -> active_digit = (display -> active_digit + 1) % display -> digits;
    //display -> driver -> SegmentsTurnOn(display -> memory[display -> active_digit]);

    segmentos = display -> memory[display -> active_digit];

    if (display -> parpadeo_frecuencia){
        if (display -> active_digit == 0){
            display -> parpadeo_contador = (display -> parpadeo_contador + 1) % display -> parpadeo_frecuencia;
        }

        if (display -> active_digit >= display -> parpadeo_desde && display -> active_digit <= display -> parpadeo_hasta){
            if (display -> parpadeo_contador > (display -> parpadeo_frecuencia/2)) {
                segmentos = 0;
            }
        }
    }
    display -> driver -> SegmentsTurnOn(segmentos);

    display -> driver -> DigitTurnOn(display -> active_digit);

}

void Display_Parpadeo(display_t display, uint8_t desde, uint8_t hasta, uint16_t frecuencia){
        display         ->  parpadeo_desde = desde;
        display         ->  parpadeo_hasta = hasta;
        display         ->  parpadeo_frecuencia = frecuencia;
        display         ->  parpadeo_contador = 0;
}


bool Display_TogglePuntos(display_t display, uint8_t digit){
    display -> memory[digit] ^= SEGMENT_P;
    return (display -> memory[digit] & SEGMENT_P);

}