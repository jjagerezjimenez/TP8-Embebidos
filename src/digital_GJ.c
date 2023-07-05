#include <stdbool.h>
#include <stdio.h>			// controlar
#include <stdint.h>
#include "digital_GJ.h"
#include "chip.h"           //consultar

//#define INSTANCIAS_GPIO 4		//ver 4

typedef struct DigitalInput_s {
    uint8_t port;
    uint8_t pin;
    uint8_t inverted;       //Bool?
    bool    last_state;
    bool    ocupado;
} *DigitalInput_t;

typedef struct DigitalOutput_s {
    uint8_t port;
    uint8_t pin;
    bool    ocupado;
} *DigitalOutput_t;

//static struct DigitalInput  Input[INSTANCES]  = {0};
//static struct DigitalOutput Output[INSTANCES] = {0};


DigitalInput_t DigitalInput_Ocupado(void);

DigitalInput_t DigitalInput_Ocupado(void){
    DigitalInput_t Input = NULL;
    static struct DigitalInput_s instances[INSTANCIAS_GPIO]={0};
    for(int i=0;i<INSTANCIAS_GPIO;i++){
        if(!instances[i].ocupado){
            instances[i].ocupado=true;
            Input=&instances[i];
            break;
        }
    }
    return Input;
}


DigitalInput_t DigitalInput_Create(uint8_t port, uint8_t inverted, uint8_t pin) {

    DigitalInput_t Input = DigitalInput_Ocupado();
    if(Input){
        Input->port           = port;
        Input->pin            = pin;
        Input->inverted       = inverted;

        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, Input->port, Input->pin, false);
    }
    

    return Input;
}


bool DigitalInput_GetState(DigitalInput_t Input) {
    return  Input->inverted ^ Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, Input->port, Input->pin);
    }


bool DigitalInput_HasChange(DigitalInput_t Input) {

    bool current_state = DigitalInput_GetState(Input);
    bool resultado = current_state != Input->last_state;
    Input->last_state = current_state;
    return resultado;
}


bool DigitalInput_HasActivate(DigitalInput_t Input) {

    bool current_state = DigitalInput_GetState(Input);
    bool resultado = current_state && !Input->last_state;
    Input->last_state = current_state;
    return resultado;
}


bool DigitalInput_HasDesactivate(DigitalInput_t Input) {

    bool current_state = DigitalInput_GetState(Input);
    bool resultado = !current_state && !Input->last_state;
    Input->last_state = current_state;
    return resultado;
}



//Salidas // Ver

DigitalOutput_t DigitalOutput_Ocupado(void);

DigitalOutput_t DigitalOutput_Ocupado(void){
    DigitalOutput_t Output = NULL;
    static struct DigitalOutput_s instances[INSTANCIAS_GPIO]={0};
    for(int i=0;i<INSTANCIAS_GPIO;i++){
        if(!instances[i].ocupado){
            instances[i].ocupado=true;
            Output=&instances[i];
            break;
        }
    }
    return Output;
}


DigitalOutput_t DigitalOutput_Create(uint8_t port, uint8_t pin) {

    DigitalOutput_t Output = DigitalOutput_Ocupado();
    if(Output){
        Output->port           = port;
        Output->pin            = pin;

        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, Output->port, Output->pin, true);
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, Output->port, Output->pin, false);
    }
    

    return Output;
}


void DigitalOutput_Activate(DigitalOutput_t Output) {		//output?
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, Output->port, Output->pin, true);
}


void DigitalOutput_Desactivate(DigitalOutput_t Output) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, Output->port, Output->pin, false);
}


void DigitalOutput_Toggle(DigitalOutput_t Output) {
    Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, Output->port, Output->pin);
}