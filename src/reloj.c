#include "reloj.h"
#include "stdio.h"
#include <string.h>


#define UNIDAD_SEG  5
#define DECENA_SEG  4
#define UNIDAD_MIN  3
#define DECENA_MIN  2
#define UNIDAD_HOR  1
#define DECENA_HOR  0

#define TIEMPO_DECENA 6
#define TIEMPO_UNIDAD 10



struct alarma_s{
	uint8_t alarma_actual[6];
	bool alarma_valida;
    evento_alarma_t evento_alarm;
    uint16_t tempo;
    

};

struct clock_GJ_s {
    uint8_t hora_actual[6];
	uint32_t tics_per_second;
    uint32_t tics;
	bool valida;
    
	struct alarma_s alarma[1];
    
    /*
    uint8_t alarma_actual[6];
	bool alarma_valida;

    uint8_t posponer_alarma[6];
    bool alarma_pospuesta;

    evento_alarma_t evento_alar;
    */

};



void Alarma_valida(clock_GJ_t reloj);

bool Compara_Alarma_Hora(clock_GJ_t reloj);

void Alarma_disparo(clock_GJ_t);

void Temporizador(clock_GJ_t reloj);


/*

static struct clock_t solicitud;

void verificar_alarma(clock_t reloj);


void verificar_alarma(clock_t reloj){
    if(reloj -> valida!=0 && reloj -> alarma_valida!=0){

    if(memcmp(reloj -> hora_actual, reloj -> alarma_actual, 6)==0){
        reloj -> evento_alar(reloj);
    
    if(reloj -> alarma_pospuesta == 1){
        reloj -> alarma_pospuesta = 0;
        memcpy(reloj -> alarma_actual, reloj -> posponer_alarma, 6);
    }

    }    
    }
}

*/


void Alarma_valida(clock_GJ_t reloj){
    static uint32_t i;

    if (reloj -> alarma -> alarma_valida & Compara_Alarma_Hora(reloj)) {        //&&?
        
        if (!i) { Alarma_disparo(reloj);

            i = reloj -> tics_per_second;
        }

        i--;
    }
}


bool Compara_Alarma_Hora(clock_GJ_t reloj){
    if (memcmp(reloj -> alarma -> alarma_actual, reloj -> hora_actual, sizeof(reloj -> hora_actual))== 0){
        return true;
    }   return false;
}

void Alarma_disparo(clock_GJ_t reloj){
    reloj -> alarma -> evento_alarm(reloj);
}
        

void Temporizador(clock_GJ_t reloj){
    if (reloj -> alarma -> tempo){
        reloj -> alarma -> tempo--;

    if (reloj -> alarma -> tempo == 0){
        Alarma_disparo(reloj);
    }
    }
}



clock_GJ_t ClockCreate(int tics_por_segundo, evento_alarma_t evento_alar){
	static struct clock_GJ_s self[1];
	memset(self, 0, sizeof(self));

    self -> tics_per_second = tics_por_segundo;
    self -> alarma -> evento_alarm = evento_alar;

	
	return self;
    //return NULL;
}

bool ClockGetTime(clock_GJ_t reloj, uint8_t * hora, int size){
	memcpy(hora, reloj->hora_actual, size);
	return reloj->valida;
    //return true;
}

bool ClockSetTime(clock_GJ_t reloj,const uint8_t * hora, int size){

	memcpy(reloj->hora_actual, hora, size);
	reloj->valida = true;
	
	return true;
}

void ClockTick(clock_GJ_t reloj){
	
	reloj -> tics++;

	if (reloj -> tics == reloj -> tics_per_second) {
		reloj -> hora_actual[UNIDAD_SEG]++;
		reloj -> tics = 0;
        Temporizador(reloj);
	}

	if (reloj -> hora_actual[UNIDAD_SEG] == TIEMPO_UNIDAD) {
        reloj -> hora_actual[UNIDAD_SEG] = 0;
        reloj -> hora_actual[DECENA_SEG]++;
    }

    if (reloj -> hora_actual[DECENA_SEG] == TIEMPO_DECENA) {
        reloj -> hora_actual[DECENA_SEG] = 0;
        reloj -> hora_actual[UNIDAD_MIN]++;
    }

    if (reloj -> hora_actual[UNIDAD_MIN] == TIEMPO_UNIDAD) {
        reloj -> hora_actual[UNIDAD_MIN] = 0;
        reloj -> hora_actual[DECENA_MIN]++;
    }

    if (reloj -> hora_actual[DECENA_MIN] == TIEMPO_DECENA) {
        reloj -> hora_actual[DECENA_MIN] = 0;
        reloj -> hora_actual[UNIDAD_HOR]++;
    }

    if (reloj -> hora_actual[UNIDAD_HOR] == TIEMPO_UNIDAD) {
        reloj -> hora_actual[UNIDAD_HOR] = 0;
        reloj -> hora_actual[DECENA_HOR]++;
    }

    if ((reloj -> hora_actual[DECENA_HOR] == 2) && (reloj -> hora_actual[UNIDAD_HOR] == 4)) {
         reloj -> hora_actual[UNIDAD_HOR] = 0;
         reloj -> hora_actual[DECENA_HOR] = 0;
    }

    Alarma_valida(reloj);
}



bool ClockGetAlarma(clock_GJ_t reloj, uint8_t * hora, int size){
	memcpy(hora, reloj -> alarma -> alarma_actual, size);
	return reloj -> alarma -> alarma_valida;
}

bool ClockSetAlarma(clock_GJ_t reloj, const uint8_t * hora, int size){
	memcpy(reloj -> alarma -> alarma_actual, hora, size);
	reloj -> alarma -> alarma_valida = true;

	return reloj -> alarma -> alarma_valida; 
}

bool ClockAlarmaRetardo(clock_GJ_t reloj, int retardo){
	//return (reloj -> alarma -> alarma_valida);
    return (reloj -> alarma -> tempo = (60 * retardo));
}

bool ClockAlarmaToggle(clock_GJ_t reloj){
	//reloj -> alarma -> alarma_valida ^= true;		// consultar
    if (reloj -> alarma -> alarma_valida){
        reloj -> alarma -> alarma_valida = false;

    }else{  reloj -> alarma -> alarma_valida = true;    }

    return reloj -> alarma -> alarma_valida;
}