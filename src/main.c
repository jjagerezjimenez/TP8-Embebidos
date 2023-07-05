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

/* === Macros definitions ====================================================================== */


/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================= */

int main(void) {

    //int divisor = 0;
    board_t board = BoardCreate();



    while (true) {


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



    }
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
