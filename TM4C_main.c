#include "TM4C123.h"
#include "functions.h"
#include "stdint.h"

volatile int sensor1Time = 0;
volatile int sensor2Time = 0;
volatile int sensor3Time = 0;
volatile int sensor1Flag = 0;
volatile int sensor2Flag = 0;
volatile int sensor3Flag = 0;
volatile int syncFlashTime = 0;
volatile int xSweep = 0;
volatile int firstSweep = 0;
volatile int times[100][3];
volatile uint8_t uBuf[300][33];
volatile uint16_t uIndex = 0;
volatile int syncFlashTimes[100];
char * tempWord;

//PA5
void GPIOA_Handler(void){
    clearIntAFlag();
    GPIOF->DATA = 0xFF;
    GPIOF->DATA = 0x0;
    sensor1Time = TIMER0->TAV;
    if (sensor1Time < 1000)
        return;
    disableInterruptA();
}

void GPIOE_Handler(void){
    //clearIntEFlag();
    GPIOE->ICR = 0x20;
    GPIOF->DATA = 0xFF;
    GPIOF->DATA = 0x0;
    sensor1Time = TIMER0->TAV;
    if (sensor1Time < 50000)
        return;
    disableInterruptE();
}

//PB5
void GPIOB_Handler(void){
    clearIntBFlag();
    GPIOF->DATA = 0xFF;
    GPIOF->DATA = 0x0;
    sensor2Time = TIMER0->TAV;
    if (sensor2Time < 50000)
        return;
    disableInterruptB();
}

//PD7
void GPIOD_Handler(void){
    clearIntDFlag();
    GPIOF->DATA = 0xF0;
    GPIOF->DATA = 0x0;
    sensor3Time = TIMER0->TAV;
    if (sensor3Time < 50000)
        return;
    disableInterruptD();
}

void writeToUart(char *string){
    int character = 0;
    for(character = 0; character < 33; character++){
        while((UART0->FR & 0x20) == 0x20);
        UART0->DR = string[character];
    }
}

int main(void){
    int data = 0;
    int delay = 0;
    int temp = 0;
    int syncFlash = 0;
    int firstTime = 0;
    systemClock(); //Change system clock
    initializeUART();
    //enableGPIOPortA();  //Sensor 1 Interrupt PA5
    enableGPIOPortB();    //Sensor 2 Interrupt PB5
    enableGPIOPortD();    //Sensor 3 Interrupt PD7
    enableGPIOPortE();
    enableGPIOPortF();    //Testing Purposes
    enableAllInterrupts();
    timer0Config();
 
    while(1){
        syncFlash = 0;
        while (GPIOE->DATA != 0x0 || GPIOB->DATA != 0x0 || GPIOD>DATA != 0x0){}
        while (GPIOE->DATA == 0x0 && GPIOB->DATA == 0x0 && GPIOD>DATA == 0x0){
            if(syncFlash == 0){
                syncFlash++;
                timer0Disable();
                timer0Enable();
                // Don't try and store anything on the first run through...
                if (firstTime == 0){
                    firstTime++;
                }
                else {
                    times[temp][0] = sensor1Time;
                    times[temp][1] = sensor2Time;
                    times[temp][2] = sensor3Time;
                    if (xSweep == 0){
                        uBuf[temp][0] = 'x';
                        uBuf[temp][1] = ',';
                        xSweep++;
                    }
                    else {
                        uBuf[temp][0] = 'y';
                        uBuf[temp][1] = ',';
                        xSweep = 0;
                    }
                data = sensor1Time;
                iToA(data,(char*)&uBuf[temp][2]); // 9 digit number
                uBuf[temp][11] = ',';

                data = sensor2Time;
                iToA(data,(char*)&uBuf[temp][12]); // 9 digit number
                uBuf[temp][21] = ',';

                data = sensor3Time;
                iToA(data,(char*)&uBuf[temp][22]); // 9 digit number
                uBuf[temp][31] = '~';
                }
                if (temp > 300)
                    temp = 0;

                while (GPIOE->DATA == 0x0 || GPIOB->DATA == 0x0 || GPIOD->DATA == 0x0){}

                clearAllICR();
                startAllInterrupts();

                if(firstTime == 1)
                    firstTime++;
                else{
                    writeToUart((char *)&uBuf[temp][0]);
                }
            }
        }
    }
}


