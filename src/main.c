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
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "event_groups.h"


/* === Macros definitions ====================================================================== */
#ifndef TICKS_POR_SEGUNDO
#define TICKS_POR_SEGUNDO 600
#endif

#ifndef PARPADEO
#define PARPADEO 600
#endif

#ifndef ALARM_POSP
#define ALARM_POSP 5
#endif

#define EVENT_KEY_ACEPTAR_ON        (1 << 0)
#define EVENT_KEY_CANCELAR_ON       (1 << 1)
#define EVENT_KEY_INCREMENTAR_ON    (1 << 2)
#define EVENT_KEY_DECREMENTAR_ON    (1 << 3)
#define EVENT_KEY_SET_TIME_ON       (1 << 4)
#define EVENT_KEY_SET_ALARM_ON      (1 << 5)

#define EVENT_KEY_ACEPTAR_OFF       (1 << 6)
#define EVENT_KEY_CANCELAR_OFF      (1 << 7)
#define EVENT_KEY_INCREMENTAR_OFF   (1 << 8)
#define EVENT_KEY_DECREMENTAR_OFF   (1 << 9)
#define EVENT_KEY_SET_TIME_OFF      (1 << 10)
#define EVENT_KEY_SET_ALARM_OFF     (1 << 11)
/* === Private data type declarations ========================================================== */
typedef enum{
    ESTADO_INICIAL,
    HORA_RUN,
    MINUTOS_CONFIG,
    HORA_CONFIG,
    ALARMA_MIN_CONFIG,
    ALARMA_HORA_CONFIG
} maq_esta_t;
/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */
void Alarma_disparo_main(clock_GJ_t reloj);
void Maquina_de_estado(maq_esta_t modo);

void funcion_incrementar(uint8_t valor[2], const uint8_t max[2]);      //consultar
void funcion_decrementar(uint8_t valor[2], const uint8_t max[2]);

void Dot_On(int posicion);
void Dot_Off(int posicion);

void Alarma_On(clock_GJ_t reloj);
void Alarma_Off(clock_GJ_t reloj);


static void Aceptar_ProgTask(void * val);
static void Cancelar_ProgTask(void * val);
static void Incrementar_ProgTask(void * val);
static void Decrementar_ProgTask(void * val);
static void SetTime_ProgTask(void * val);
static void SetAlarm_ProgTask(void * val);

static void Tic_ProgTask(void * val);
static void Display_Refresh_ProgTask(void * val);
static void Key_Task(void * val);
/* === Public variable definitions ============================================================= */
static board_t board;
static clock_GJ_t reloj;
static maq_esta_t estado;
static bool posp_alarm;

static uint8_t carga[4];
EventGroupHandle_t app_eventos;

static int val_act_tick;
static int contador_seg_dec = 0;
/* === Private variable definitions ============================================================ */
static const uint8_t MINUTOS_MAX[] = {5, 9};
static const uint8_t HORA_MAX[] = {2, 3};
/* === Private function implementation ========================================================= */
void Alarma_disparo_main(clock_GJ_t reloj){

    DigitalOutput_Activate(board -> Buzzer);
    posp_alarm = true;

}

void Maquina_de_estado(maq_esta_t modo){

    estado = modo;

    switch (estado)
    {
    case ESTADO_INICIAL:
        
        Display_Parpadeo(board -> display, 0, 3, PARPADEO);     //para que parpadee todo al prender
        //if(!Display_TogglePuntos(board -> display, 1))
         //       Display_TogglePuntos(board -> display, 1);
        
        Dot_Off(0);
        Dot_On(1);
        Dot_Off(2);
        Dot_Off(3);
        break;
    
    case HORA_RUN:
        
        Display_Parpadeo(board -> display, 0, 3, 0);

        Dot_Off(0);
        Dot_On(1);
        Dot_Off(2);
        Dot_Off(3);

        break;

    case MINUTOS_CONFIG:
        
        Display_Parpadeo(board -> display, 2, 3, PARPADEO);

        Dot_Off(0);
        Dot_On(1);
        Dot_Off(2);
        Dot_Off(3);

        break;

    case HORA_CONFIG:
        
        Display_Parpadeo(board -> display, 0, 1, PARPADEO);

        Dot_Off(0);
        Dot_On(1);
        Dot_Off(2);
        Dot_Off(3);

        break;

    case ALARMA_MIN_CONFIG:
        
        Display_Parpadeo(board -> display, 2, 3, PARPADEO);
        /*
        Display_TogglePuntos(board -> display, 0);
        Display_TogglePuntos(board -> display, 1);
        Display_TogglePuntos(board -> display, 2);
        Display_TogglePuntos(board -> display, 3);
        */
       Dot_On(0);
       Dot_On(1);
       Dot_On(2);
       Dot_On(3);

        break;

    case ALARMA_HORA_CONFIG:
        
        Display_Parpadeo(board -> display, 0, 1, PARPADEO);
        /*
        Display_TogglePuntos(board -> display, 0);
        Display_TogglePuntos(board -> display, 1);
        Display_TogglePuntos(board -> display, 2);
        Display_TogglePuntos(board -> display, 3);
        */
        Dot_On(0);
        Dot_On(1);
        Dot_On(2);
        Dot_On(3);

        break;
    
    default:
        break;
    }

}


void funcion_incrementar(uint8_t valor[2], const uint8_t max[2]){

    valor[1]++;
    
    if ((valor[0] >= max[0]) && valor[1] > max[1]) {

        valor[0] = 0;
        valor[1] = 0;
    }

    if(valor[1] > 9){

        valor[1] = 0;
        valor[0]++;
    }
        
}

void funcion_decrementar(uint8_t valor[2], const uint8_t max[2]){       //modificaR, no funciona copmo deberia

    valor[1]--;

    if (valor[1] > 9) {
        
        valor[1] = 9;
        valor[0]--;

    }

    if ((valor[0] >= max[0]) && valor[1] >= max[1]) {                //modificaR, no funciona copmo deberia

        valor[0] = max[0];
        valor[1] = max[1];

    }

}

void Dot_On(int posicion){

    if (!Display_TogglePuntos(board -> display, posicion)){
        Display_TogglePuntos(board -> display, posicion);
    }

}

void Dot_Off(int posicion){
    
    if (Display_TogglePuntos(board -> display, posicion)){
        Display_TogglePuntos(board -> display, posicion);
    }

}

void Alarma_On(clock_GJ_t reloj){
    if (!ClockAlarmaToggle(reloj)){

        ClockAlarmaToggle(reloj);
    }
        Dot_On(3);
}
 
void Alarma_Off(clock_GJ_t reloj){
    if (!ClockAlarmaToggle(reloj)){

        ClockAlarmaToggle(reloj);
    }
        Dot_Off(3);
}




/* === Public function implementation ========================================================= */
static void Display_Refresh_ProgTask(void * val) {
    while (true) {
        Display_Refresh(board -> display);
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}


//int main(void) {

    

    //int divisor = 0;
    //board_t board = BoardCreate();
    //uint8_t carga[4];
    static void Tic_ProgTask(void * val) {
    TickType_t ultimo_Val = xTaskGetTickCount();
    static const int medio_seg = TICKS_POR_SEGUNDO/2;
    uint8_t hora[6];
    //reloj = ClockCreate(TICKS_POR_SEGUNDO, Alarma_disparo_main);
    //board = BoardCreate();
    while (true) {

        val_act_tick = ClockTick(reloj);

        if (val_act_tick == medio_seg || val_act_tick == 0) {

            if (estado <= HORA_RUN) {

                ClockGetTime(reloj, hora, sizeof(hora));
                Display_WriteBCD(board -> display, hora, sizeof(hora));

                if (estado == HORA_RUN)
                    Display_TogglePuntos(board -> display, 1);
            }
            if ((val_act_tick == 0) && contador_seg_dec) {
                contador_seg_dec--;
            }
        }

        vTaskDelayUntil(&ultimo_Val, pdMS_TO_TICKS(1));
    }
}

static void Key_Task(void * val) {
    uint16_t est_sig, est_act, cambio, eventos;


    //SisTick_Init(TICKS_POR_SEGUNDO);      //test
    //Maquina_de_estado(ESTADO_INICIAL);
    while (true) {

        /*
        if (DigitalInput_HasActivate(board -> Aceptar)){
            Display_WriteBCD(board -> display, (uint8_t[]){0,1,2,3},4);
        }

        if (DigitalInput_HasActivate(board -> Cancelar)){
            Display_WriteBCD(board -> display, NULL, 0);
            DigitalOutput_Desactivate(board -> Buzzer);           //para testear Buzzer
        }

        //test 

        if (DigitalInput_HasActivate(board -> Set_time)){
            Display_WriteBCD(board -> display, (uint8_t[]){0,0,0,1},4);
        }
        
        if (DigitalInput_HasActivate(board -> Set_alarm)){
            Display_WriteBCD(board -> display, (uint8_t[]){0,0,2,0},4);
            DigitalOutput_Activate(board -> Buzzer);               //para testear Buzzer
        }

        if (DigitalInput_HasActivate(board -> decrementar)){
            Display_WriteBCD(board -> display, (uint8_t[]){0,3,0,0},4);
        }

        if (DigitalInput_HasActivate(board -> incrementar)){
            Display_WriteBCD(board -> display, (uint8_t[]){4,0,0,0},4);
        }


        //Intento testear Buzzer, pero no se muy bien como hacerlo con la placa que tengo
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

       /*if (DigitalInput_HasActivate(board -> Aceptar)){
            //Display_Parpadeo(board -> display,0,3,PARPADEO);        //parpadea todo

            if(!posp_alarm){

                if (estado == HORA_RUN) {
                Alarma_On(reloj);
            } else
                    if(estado == MINUTOS_CONFIG){

                        Maquina_de_estado(HORA_CONFIG);
                    } else 
                            if (estado == HORA_CONFIG) {

                            ClockSetTime(reloj, carga, sizeof(carga));
                            Maquina_de_estado(HORA_RUN);
            } else

                if (estado == ALARMA_MIN_CONFIG) {
                    Maquina_de_estado(ALARMA_HORA_CONFIG);
                } else 
                        if (estado == ALARMA_HORA_CONFIG) {

                        ClockSetAlarma(reloj, carga, sizeof(carga));
                        Maquina_de_estado(HORA_RUN);
                        Alarma_On(reloj);

                        
                } else {

                    DigitalOutput_Desactivate(board -> Buzzer);
                    ClockAlarmaRetardo(reloj, ALARM_POSP);

                    posp_alarm = false;
                }

            }


       }*/

       vTaskDelay(pdMS_TO_TICKS(150));

        est_act = 0;
        if (DigitalInput_GetState(board -> Aceptar)) {
            est_act |= EVENT_KEY_ACEPTAR_ON;
        };

        if (DigitalInput_GetState(board -> Cancelar)) {
            est_act |= EVENT_KEY_CANCELAR_ON;
        };

        if (DigitalInput_GetState(board -> Set_time)) {
            est_act |= EVENT_KEY_SET_TIME_ON;
        };

        if (DigitalInput_GetState(board -> Set_alarm)) {
            est_act |= EVENT_KEY_SET_ALARM_ON;
        };

        if (DigitalInput_GetState(board -> incrementar)) {
            est_act |= EVENT_KEY_INCREMENTAR_ON;
        };

        if (DigitalInput_GetState(board -> decrementar)) {
            est_act |= EVENT_KEY_DECREMENTAR_ON;
        };
        

        cambio  = est_act ^ est_sig;
        est_sig = est_act;
        eventos = ((cambio & !est_act << 6) | (cambio & est_act));

        xEventGroupSetBits(app_eventos, eventos);
    }
}

static void Aceptar_ProgTask(void * val) {
    while (true) {
        xEventGroupWaitBits(app_eventos, EVENT_KEY_ACEPTAR_ON, true, false, portMAX_DELAY);

        if(!posp_alarm){

                if (estado == HORA_RUN) {
                Alarma_On(reloj);
            } else
                    if(estado == MINUTOS_CONFIG){

                        Maquina_de_estado(HORA_CONFIG);
                    } else 
                            if (estado == HORA_CONFIG) {

                            ClockSetTime(reloj, carga, sizeof(carga));
                            Maquina_de_estado(HORA_RUN);
            } else

                if (estado == ALARMA_MIN_CONFIG) {
                    Maquina_de_estado(ALARMA_HORA_CONFIG);
                } else 
                        if (estado == ALARMA_HORA_CONFIG) {

                        ClockSetAlarma(reloj, carga, sizeof(carga));
                        Maquina_de_estado(HORA_RUN);
                        Alarma_On(reloj);

                        
                } else {

                    DigitalOutput_Desactivate(board -> Buzzer);
                    ClockAlarmaRetardo(reloj, ALARM_POSP);

                    posp_alarm = false;
                }

            }
    }
}


       /*if (DigitalInput_HasActivate(board -> Cancelar)){

            if(!posp_alarm){ 

                if(estado == HORA_RUN){
                    Alarma_Off(reloj);
                }else

                        if (ClockGetTime(reloj, carga, sizeof(carga)) && (estado != HORA_RUN)) {

                            Maquina_de_estado(HORA_RUN);

                        } else 
                                if (estado == HORA_RUN){
                                    Alarma_Off(reloj);
                                }

                        else {

                            Maquina_de_estado(ESTADO_INICIAL);
                        }

                } else {
                            DigitalOutput_Desactivate (board -> Buzzer);
                            posp_alarm = false;

                        }

            }*/
            static void Cancelar_ProgTask(void * val) {
    while (true) {
        xEventGroupWaitBits(app_eventos, EVENT_KEY_CANCELAR_ON, true, false, portMAX_DELAY);

        if(!posp_alarm){ 

                if(estado == HORA_RUN){
                    Alarma_Off(reloj);
                }else

                        if (ClockGetTime(reloj, carga, sizeof(carga)) && (estado != HORA_RUN)) {

                            Maquina_de_estado(HORA_RUN);

                        } else 
                                if (estado == HORA_RUN){
                                    Alarma_Off(reloj);
                                }

                        else {

                            Maquina_de_estado(ESTADO_INICIAL);
                        }

                } else {
                            DigitalOutput_Desactivate (board -> Buzzer);
                            posp_alarm = false;

                        }
    }
}



        /*if (DigitalInput_HasActivate(board -> Set_time)){

            Maquina_de_estado(MINUTOS_CONFIG);
            ClockGetTime(reloj, carga, sizeof(carga));
            Display_WriteBCD(board->display, carga, sizeof(carga));

        }*/

        static void SetTime_ProgTask(void * val) {
    while (true) {
        xEventGroupWaitBits(app_eventos, EVENT_KEY_SET_TIME_ON, true, false, portMAX_DELAY);
        
            Maquina_de_estado(MINUTOS_CONFIG);
            ClockGetTime(reloj, carga, sizeof(carga));
            Display_WriteBCD(board->display, carga, sizeof(carga));
    }
}


        /*if (DigitalInput_HasActivate(board -> Set_alarm)){

            Maquina_de_estado(ALARMA_MIN_CONFIG);
            ClockGetTime(reloj, carga, sizeof(carga));
            Display_WriteBCD(board->display, carga, sizeof(carga));

        }*/

        static void SetAlarm_ProgTask(void * object) {
    while (true) {
        xEventGroupWaitBits(app_eventos, EVENT_KEY_SET_ALARM_ON, true, false, portMAX_DELAY);
        
            Maquina_de_estado(ALARMA_MIN_CONFIG);
            ClockGetTime(reloj, carga, sizeof(carga));
            Display_WriteBCD(board->display, carga, sizeof(carga));
    }
}


        /*if (DigitalInput_HasActivate(board -> incrementar)){

            if (estado == MINUTOS_CONFIG) {
                funcion_incrementar(&carga[2], MINUTOS_MAX);

                Display_WriteBCD(board -> display, carga, sizeof(carga));
            } else 
                    if (estado == HORA_CONFIG) {

                    funcion_incrementar(carga, HORA_MAX);

                    Display_WriteBCD(board -> display, carga, sizeof(carga));
            } else  //agrego modificador de alarma
                    if (estado == ALARMA_MIN_CONFIG){
                        funcion_incrementar(&carga[2], MINUTOS_MAX);

                        Display_WriteBCD(board -> display, carga, sizeof(carga));
                    } else  
                            if (estado == ALARMA_HORA_CONFIG){
                                funcion_incrementar(carga, HORA_MAX);

                                Display_WriteBCD(board -> display, carga, sizeof(carga));
                            }

                //Display_WriteBCD(board -> display, carga, sizeof(carga));

        }*/

        static void Incrementar_ProgTask(void * object) {
    while (true) {
        xEventGroupWaitBits(app_eventos, EVENT_KEY_INCREMENTAR_ON, true, false, portMAX_DELAY);

        if (estado == MINUTOS_CONFIG) {
                funcion_incrementar(&carga[2], MINUTOS_MAX);

                Display_WriteBCD(board -> display, carga, sizeof(carga));
            } else 
                    if (estado == HORA_CONFIG) {

                    funcion_incrementar(carga, HORA_MAX);

                    Display_WriteBCD(board -> display, carga, sizeof(carga));
            } else  //agrego modificador de alarma
                    if (estado == ALARMA_MIN_CONFIG){
                        funcion_incrementar(&carga[2], MINUTOS_MAX);

                        Display_WriteBCD(board -> display, carga, sizeof(carga));
                    } else  
                            if (estado == ALARMA_HORA_CONFIG){
                                funcion_incrementar(carga, HORA_MAX);

                                Display_WriteBCD(board -> display, carga, sizeof(carga));
                            }

                //Display_WriteBCD(board -> display, carga, sizeof(carga));
    }
}


       /* if (DigitalInput_HasActivate(board -> decrementar)){

            if (estado == MINUTOS_CONFIG) {
                funcion_decrementar(&carga[2], MINUTOS_MAX);

                Display_WriteBCD(board -> display, carga, sizeof(carga));
            } else 
                    if (estado == HORA_CONFIG) {

                    funcion_decrementar(carga, HORA_MAX);

                    Display_WriteBCD(board -> display, carga, sizeof(carga));
            } else  //agrego modificador de alarma
                    if (estado == ALARMA_MIN_CONFIG){
                        funcion_decrementar(&carga[2], MINUTOS_MAX);

                        Display_WriteBCD(board -> display, carga, sizeof(carga));
                    } else  
                            if (estado == ALARMA_HORA_CONFIG){
                                funcion_decrementar(carga, HORA_MAX);

                                Display_WriteBCD(board -> display, carga, sizeof(carga));
                            }

                //Display_WriteBCD(board -> display, carga, sizeof(carga));

        }*/

        static void Decrementar_ProgTask(void * object) {
    while (true) {
        xEventGroupWaitBits(app_eventos, EVENT_KEY_DECREMENTAR_ON, true, false, portMAX_DELAY);
        
        if (estado == MINUTOS_CONFIG) {
                funcion_decrementar(&carga[2], MINUTOS_MAX);

                Display_WriteBCD(board -> display, carga, sizeof(carga));
            } else 
                    if (estado == HORA_CONFIG) {

                    funcion_decrementar(carga, HORA_MAX);

                    Display_WriteBCD(board -> display, carga, sizeof(carga));
            } else  //agrego modificador de alarma
                    if (estado == ALARMA_MIN_CONFIG){
                        funcion_decrementar(&carga[2], MINUTOS_MAX);

                        Display_WriteBCD(board -> display, carga, sizeof(carga));
                    } else  
                            if (estado == ALARMA_HORA_CONFIG){
                                funcion_decrementar(carga, HORA_MAX);

                                Display_WriteBCD(board -> display, carga, sizeof(carga));
                            }

                //Display_WriteBCD(board -> display, carga, sizeof(carga));
    }
}



        /*for (int i=0; i<20; i++){
            for(int delay = 0; delay <25000; delay++){
                __asm("NOP");
            }

            }*/


int main(void) {

    reloj = ClockCreate(TICKS_POR_SEGUNDO, Alarma_disparo_main);
    board = BoardCreate();

    SisTick_Init(TICKS_POR_SEGUNDO);

    app_eventos = xEventGroupCreate();

    Maquina_de_estado(ESTADO_INICIAL);

    xTaskCreate(Tic_ProgTask, "Tic_ProgTask", 256, NULL, tskIDLE_PRIORITY + 3, NULL);

    xTaskCreate(Aceptar_ProgTask,       "AcceptProgram", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(Cancelar_ProgTask,      "CancelProgram", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(Incrementar_ProgTask,   "IncrementProgram", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(Decrementar_ProgTask,   "DecrementProgram", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(SetTime_ProgTask,       "SetTimeProgram", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(SetAlarm_ProgTask,      "SetAlarmProgram", 256, NULL, tskIDLE_PRIORITY + 1, NULL);

    xTaskCreate(Key_Task, "Key_Task", 256, NULL, tskIDLE_PRIORITY + 1, NULL);

    xTaskCreate(Display_Refresh_ProgTask, "RefreshTask", 256, NULL, tskIDLE_PRIORITY + 2, NULL);

    vTaskStartScheduler();

    while (true) {
    }

    return 0;
}

/*void SysTick_Handler(void){

    static const int medio_seg = TICKS_POR_SEGUNDO/2;
    int valor_actual;
    uint8_t hora[6];

    Display_Refresh(board -> display);

    valor_actual = ClockTick(reloj);
    

    if (valor_actual == medio_seg || valor_actual == 0) {
        if (estado <= HORA_RUN) {
            ClockGetTime(reloj, hora, sizeof(hora));
            Display_WriteBCD(board->display, hora, sizeof(hora));
            if (estado == HORA_RUN)
                Display_TogglePuntos(board->display, 1);
        }
    }
}*/








/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
