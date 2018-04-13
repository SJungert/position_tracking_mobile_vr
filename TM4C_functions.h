#include "TM4C123.h"
void timer0Config(void);
void timer0Enable(void);
void timer0Disable(void);
void setSysTick(void);
void systemClock(void); //Sets system clock to 66.67 MHz
void enableInterruptA(void);
void enableInterruptB(void);
void enableInterruptD(void);
void enableInterruptE(void);
void disableInterruptA(void);
void disableInterruptB(void);
void disableInterruptD(void);
void disableInterruptE(void);
void clearIntAFlag(void);
void clearIntBFlag(void);
void clearIntDFlag(void);
void enableGPIOPortA(void); //Sensor 1 Interrupt PA5
void enableGPIOPortB(void); //Sensor 2 Interrupt PB5
void enableGPIOPortD(void); //Sensor 3 Interrupt PD7
void enableGPIOPortE(void);
void enableGPIOPortF(void); //Testing Purposes
void initializeUART(void);
void iToA(int32_t const num, char *string);
void enableAllInterrupts(void);
void startAllInterrupts(void);
void clearAllICR(void);