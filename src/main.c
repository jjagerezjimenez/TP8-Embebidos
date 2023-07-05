/* Copyright 2022, Laboratorio de Microprocesadores
 * Facultad de Ciencias Exactas y Tecnología
 * Universidad Nacional de Tucuman
 * http://www.microprocesadores.unt.edu.ar/
 * Copyright 2022, Esteban Volentini <evolentini@herrera.unt.edu.ar>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** \brief Simple sample of use LPC HAL gpio functions
 **
 ** \addtogroup samples Sample projects
 ** \brief Sample projects to use as a starting point
 ** @{ */

/* === Headers files inclusions =============================================================== */

#include "chip.h"
#include <stdbool.h>
#include "bsp_GJ.h"
#include "digital_GJ.h"
#include "pantalla_GJ.h"
#include <stddef.h>
#include "reloj.h"


/* === Macros definitions ====================================================================== */
#define TICKS_POR_SEGUNDO 600
#define PARPADEO 600

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */
void Alarma_disparo_main(clock_GJ_t reloj);
/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */
static board_t board;
static clock_GJ_t reloj;
/* === Private function implementation ========================================================= */
void Alarma_disparo_main(clock_GJ_t reloj){

}
/* === Public function implementation ========================================================= */

int main(void) {

    //int divisor = 0;
    //board_t board = BoardCreate();
    uint8_t hora[6];
    reloj = ClockCreate(10, Alarma_disparo_main);
    board = BoardCreate();


    SisTick_Init(TICKS_POR_SEGUNDO);      //test
    while (true) {

        /*
        if (DigitalInput_HasActivate(board -> Aceptar)){
            Display_WriteBCD(board -> display, (uint8_t[]){0,1,2,3},4);
        }

        if (DigitalInput_HasActivate(board -> Cancelar)){
            Display_WriteBCD(board -> display, NULL, 0);
            DigitalOutput_Desactivate(board -> Buzzer);           //para testear buzzer
        }

        //test 

        if (DigitalInput_HasActivate(board -> Set_time)){
            Display_WriteBCD(board -> display, (uint8_t[]){0,0,0,1},4);
        }
        
        if (DigitalInput_HasActivate(board -> Set_alarm)){
            Display_WriteBCD(board -> display, (uint8_t[]){0,0,2,0},4);
            DigitalOutput_Activate(board -> Buzzer);               //para testear buzzer
        }

        if (DigitalInput_HasActivate(board -> decrementar)){
            Display_WriteBCD(board -> display, (uint8_t[]){0,3,0,0},4);
        }

        if (DigitalInput_HasActivate(board -> incrementar)){
            Display_WriteBCD(board -> display, (uint8_t[]){4,0,0,0},4);
        }


        //Intento testear buzzer, pero no se muy bien como hacerlo con la placa que tengo
        //divisor++;
        //if (divisor == 5) {
        //    divisor = 0;
        //    //Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, LED_3_GPIO, LED_3_BIT);
        //    DigitalOutput_Toggle(board->Buzzer);
        //}
        

        Display_Refresh(board -> display);

        for (int i=0; i<100; i++){
            for(int delay = 0; delay <50; delay++){
                __asm("NOP");
            }
        }
        */

       if (DigitalInput_HasActivate(board -> Aceptar)){
            Display_Parpadeo(board -> display,0,3,PARPADEO);        //parpadea todo
       }

       if (DigitalInput_HasActivate(board -> Cancelar)){
            Display_Parpadeo(board -> display,0,0,0);
       }

       

       if (DigitalInput_HasActivate(board -> incrementar)){
           Display_TogglePuntos(board -> display, 0);
         }

        if (DigitalInput_HasActivate(board->decrementar)){

        }

        


       ClockGetTime(reloj, hora, sizeof(hora));
       __asm volatile("cpsid i");

       Display_WriteBCD(board -> display,hora, sizeof(hora));       //       Display_WriteBCD(board -> display,(uint8_t[]){hora[0],hora[1],hora[2],hora[3]},4);
       __asm volatile("cpsie i");

       

    }
    
}

void SysTick_Handler(void){
    Display_Refresh(board -> display);
    ClockTick(reloj);
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
