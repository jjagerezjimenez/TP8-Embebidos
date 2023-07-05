#include <stdbool.h>
#include <stdint.h>

typedef struct clock_GJ_s * clock_GJ_t;

typedef void (*evento_alarma_t)(clock_GJ_t reloj);




clock_GJ_t ClockCreate(int tics_por_segundo, evento_alarma_t evento_alar);

bool ClockGetTime(clock_GJ_t reloj, uint8_t * hora, int size);

bool ClockSetTime(clock_GJ_t reloj, const uint8_t * hora, int size);

void ClockTick(clock_GJ_t reloj);



bool ClockGetAlarma(clock_GJ_t reloj, uint8_t * hora, int size);

bool ClockSetAlarma(clock_GJ_t reloj, const uint8_t * hora, int size);

bool ClockAlarmaRetardo(clock_GJ_t reloj, int retardo);

bool ClockAlarmaToggle(clock_GJ_t reloj);



