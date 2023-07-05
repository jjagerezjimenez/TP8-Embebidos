#include "unity.h"
#include "reloj.h"
#include <string.h>


#define TICS_POR_SEGUNDO 5

#define SIMULADOR_SEGUNDOS(VALOR, VALOR_2)\
	for (int i = 0; i < VALOR; i++) {\
		for (int index = 0; index < TICS_POR_SEGUNDO; index++) {\
			VALOR_2;\
		}\
    }

void Detecto_alarma(clock_t reloj);

static clock_t	reloj;
static uint8_t hora[6];
static bool bandera_alarma;



void Detecto_alarma(clock_t reloj){
	bandera_alarma = true;
}




void setUp(void) {
	bandera_alarma = false;
   	static const uint8_t INICIAL[] = {1, 2, 3, 4, 0, 0};
	reloj = ClockCreate(TICS_POR_SEGUNDO, Detecto_alarma);

	ClockSetTime(reloj, INICIAL, 4);

	//TEST_ASSERT_TRUE(ClockSetTime(reloj, INICIAL, sizeof(INICIAL)));

	
   
}




void test_reloj_arranca_con_hora_invalida(void) {
	static const uint8_t ESPERANDO[] = {0, 0, 0, 0, 0, 0};
	uint8_t hora[6] = {0xFF};

	clock_t reloj = ClockCreate(TICS_POR_SEGUNDO, Detecto_alarma);

	TEST_ASSERT_FALSE(ClockGetTime(reloj, hora, 6));
	TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERANDO, hora, 6);
}



void test_ajustar_hora(void){
	static const uint8_t ESPERANDO[] = {1, 2, 3, 4, 0, 0};
	uint8_t hora[6];                  //no hace falta dejarlo en FF
	
	clock_t reloj = ClockCreate(TICS_POR_SEGUNDO, Detecto_alarma);
	//CloclSetTime(reloj, ESPERANDO, 4);
	TEST_ASSERT_TRUE(ClockSetTime(reloj, ESPERANDO, 4));           //ojo, corregir, uso get y no deberia, se cuelag porque pide escritura
	TEST_ASSERT_TRUE(ClockGetTime(reloj, hora, 6));
	TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERANDO, hora, 6);

}



void test_unidad_segundos(void){
	static const uint8_t INICIAL[] = {1, 2, 3, 4, 0, 0};
	static const uint8_t ESPERANDO[] = {1, 2, 3, 4, 0, 1};
	uint8_t hora[6];                  //no hace falta dejarlo en FF

	clock_t reloj = ClockCreate(TICS_POR_SEGUNDO, Detecto_alarma);
	ClockSetTime(reloj, INICIAL, 4);

	SIMULADOR_SEGUNDOS(1, ClockTick(reloj));
	ClockGetTime(reloj, hora, 6);

	TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERANDO, hora, 6);

}

void test_decena_segundos(void){
	static const uint8_t INICIAL[] = {1, 2, 3, 4, 0, 0};
	static const uint8_t ESPERANDO[] = {1, 2, 3, 4, 1, 0};
	uint8_t hora[6];                  //no hace falta dejarlo en FF

	clock_t reloj = ClockCreate(TICS_POR_SEGUNDO, Detecto_alarma);
	ClockSetTime(reloj, INICIAL, 4);

	SIMULADOR_SEGUNDOS(10, ClockTick(reloj));
	ClockGetTime(reloj, hora, 6);

	TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERANDO, hora, 6);

}

void test_unidad_minutos(void){
	static const uint8_t INICIAL[] = {1, 2, 3, 4, 0, 0};
	static const uint8_t ESPERANDO[] = {1, 2, 3, 5, 0, 0};
	uint8_t hora[6];                  //no hace falta dejarlo en FF

	clock_t reloj = ClockCreate(TICS_POR_SEGUNDO, Detecto_alarma);
	ClockSetTime(reloj, INICIAL, 4);

	SIMULADOR_SEGUNDOS(60, ClockTick(reloj));
	ClockGetTime(reloj, hora, 6);

	TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERANDO, hora, 6);

}

void test_decena_minutos(void){
	static const uint8_t INICIAL[] = {1, 2, 3, 4, 0, 0};
	static const uint8_t ESPERANDO[] = {1, 2, 4, 4, 0, 0};
	uint8_t hora[6];                  //no hace falta dejarlo en FF

	clock_t reloj = ClockCreate(TICS_POR_SEGUNDO, Detecto_alarma);
	ClockSetTime(reloj, INICIAL, 4);

	SIMULADOR_SEGUNDOS(600, ClockTick(reloj));
	ClockGetTime(reloj, hora, 6);

	TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERANDO, hora, 6);

}

void test_unidad_horas(void){
	static const uint8_t INICIAL[] = {1, 2, 3, 4, 0, 0};
	static const uint8_t ESPERANDO[] = {1, 3, 3, 4, 0, 0};
	uint8_t hora[6];                  //no hace falta dejarlo en FF

	clock_t reloj = ClockCreate(TICS_POR_SEGUNDO, Detecto_alarma);
	ClockSetTime(reloj, INICIAL, 4);

	SIMULADOR_SEGUNDOS(3600, ClockTick(reloj));		//60*10*6
	ClockGetTime(reloj, hora, 6);

	TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERANDO, hora, 6);

}

void test_decena_horas(void){
	static const uint8_t INICIAL[] = {1, 2, 3, 4, 0, 0};
	static const uint8_t ESPERANDO[] = {2, 2, 3, 4, 0, 0};
	uint8_t hora[6];                  //no hace falta dejarlo en FF

	clock_t reloj = ClockCreate(TICS_POR_SEGUNDO,Detecto_alarma);
	ClockSetTime(reloj, INICIAL, 4);

	SIMULADOR_SEGUNDOS(36000, ClockTick(reloj));
	ClockGetTime(reloj, hora, 6);

	TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERANDO, hora, 6);

}

void test_incrementar_dia(void){
    static const uint8_t ESPERADO[]={0, 0, 0, 0, 0, 0}; 
    static const uint8_t INICIAL[]={2, 3, 5, 9, 5, 9};
	
    TEST_ASSERT_TRUE(ClockSetTime(reloj, INICIAL, sizeof(INICIAL)));

    SIMULADOR_SEGUNDOS(1, ClockTick(reloj));

    TEST_ASSERT_TRUE(ClockGetTime(reloj, hora, sizeof(hora)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
}


void test_ajustar_alarma(void){
	static const uint8_t ESPERANDO[] = {1, 2, 3, 4, 0, 0};
	uint8_t hora[6];                  //no hace falta dejarlo en FF
	
	TEST_ASSERT_FALSE(ClockGetAlarma(reloj, hora, 6));
	//clock_t reloj = ClockCreate(TICS_POR_SEGUNDO);
	ClockSetAlarma(reloj, ESPERANDO, sizeof(ESPERANDO));
	ClockGetAlarma(reloj, hora, 6);

	TEST_ASSERT_TRUE(ClockSetAlarma(reloj, ESPERANDO, 4));           //ojo, corregir, uso get y no deberia, se cuelag porque pide escritura
	TEST_ASSERT_TRUE(ClockGetAlarma(reloj, hora, 6));
	TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERANDO, hora, 6);
	// y este? -> TEST_ASSERT_FALSE(ClockGetAlarma(reloj, hora, 6)); consultar

}

/*
void test_alarma_activada(void) {
    static const uint8_t ESPERADO[] = {1, 3, 3, 4, 0, 0};
    ClockSetAlarma(reloj, ESPERADO, 6);
    SIMULADOR_SEGUNDOS(60 * 60, ClockTick(reloj));
    TEST_ASSERT_TRUE(bandera_alarma);
}
 */


void test_disparar_alarma(void) {
    static const uint8_t ALARMA_INICIAL[] = {1, 3, 3, 4, 0, 0};

    ClockSetAlarma(reloj, ALARMA_INICIAL, 4);

    TEST_ASSERT_FALSE(bandera_alarma);

    SIMULADOR_SEGUNDOS(1800, ClockTick(reloj));

    TEST_ASSERT_FALSE(bandera_alarma);

    SIMULADOR_SEGUNDOS(1801, ClockTick(reloj));		//30*60+1

    TEST_ASSERT_TRUE(bandera_alarma);
}


void test_alarma_retardo(void){

	static const uint8_t ALARMA_INICIAL[] = {1, 3, 3, 4, 0, 0};

    ClockSetAlarma(reloj, ALARMA_INICIAL, 4);
	SIMULADOR_SEGUNDOS(3601, ClockTick(reloj))			//1*60*60+1

    //TEST_ASSERT_FALSE(bandera_alarma);	//consultar
	TEST_ASSERT_TRUE(bandera_alarma);
	
	ClockAlarmaRetardo(reloj, 5);
	bandera_alarma = false;
    SIMULADOR_SEGUNDOS(240, ClockTick(reloj));			//4*60

    TEST_ASSERT_FALSE(bandera_alarma);

    SIMULADOR_SEGUNDOS(60, ClockTick(reloj));		

    TEST_ASSERT_TRUE(bandera_alarma);

}













