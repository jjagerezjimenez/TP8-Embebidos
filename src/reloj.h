#include <stdbool.h>
#include <stdint.h>

typedef struct clock_s * clock_t;

typedef void (*evento_alarma_t)(clock_t reloj);



clock_t ClockCreate(int tics_por_segundo, evento_alarma_t evento_alar);

bool ClockGetTime(clock_t reloj, uint8_t * hora, int size);

bool ClockSetTime(clock_t reloj, const uint8_t * hora, int size);

void ClockTick(clock_t reloj);



bool ClockGetAlarma(clock_t reloj, uint8_t * hora, int size);

bool ClockSetAlarma(clock_t reloj, const uint8_t * hora, int size);

bool ClockAlarmaRetardo(clock_t reloj, int retardo);

bool ClockAlarmaToggle(clock_t reloj);



