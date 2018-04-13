#include "C:\Keil_v5\ARM\INC\TI\TM4C123\TM4C123GH6PM.h"
#include "functions.h"
//Variables
volatile unsigned int* PRIRegister = (unsigned int *) 0xE000E000; //Datasheet page 154
volatile unsigned int* NVIC_Enable = (unsigned int *) 0xE000E000; //Datasheet page 142

void timer0Config(void){
    SYSCTL->RCGCTIMER |= (1<<0); //Enable Timer 0 Clock
    TIMER0->CTL &= ~(1<<0);      //Disable the
    Timer
    TIMER0->CFG = 0x0;
    //Configure 16/32-bit timer to 32-bit timer mode
    TIMER0->TAMR |= 0x12;       //Periodic Timer Mode and TACDIR(Timer Direction - Counts up)
    TIMER0->TAILR = 0xFFFFFFFF; //Upper bound for the timeout event (Maximum value timer can reach)
    //TIMER0->IMR |= (1<<0);    //Enable Mask for interrupts on timer A
    //NVIC_EnableIRQ(TIMER0A_IRQn); //Enable Interrupts for Timer0 A
    //TIMER0->CTL |= (1<<0);        //Start the Timer
}

// Call after all GPIO setup has been done;
void enableAllInterrupts(void){
//NVIC_Enable[0x100/4] |= 0x1;
NVIC_Enable[0x100/4] |= 0x2;
NVIC_Enable[0x100/4] |= 0x8;
NVIC_Enable[0x100/4] |= 0x10; // PORT E
}

void startAllInterrupts(void){
//enableInterruptA();
enableInterruptB();
enableInterruptD();
enableInterruptE();
}

void clearAllICR(void){
//GPIOA->ICR = 0x20;
GPIOB->ICR = 0x20;
GPIOD->ICR = 0x80;
GPIOE->ICR = 0x20;
}

void timer0Enable(void){
TIMER0->TAV = 0x00000000; //Zero
TIMER0->CTL |= (1<<0);    //Start the Timer
}

void timer0Disable(void){
TIMER0->CTL &= ~(1<<0);   //Stop the Timer
}

void systemClock(void){
    SYSCTL->RCC=0x00000800;
    SYSCTL->RCC|=0x540;
    //SYSCTL->RCC|=0x02400000;//configure to 40 MHz
    //SYSCTL->RCC|=0x01C00000;//configure to 50 MHz
    SYSCTL->RCC|=0x01400000;  //configure to 66.67 MHz

    //if we use pwm, we may need to configure clock
    while(end==0){
        char pllBit=SYSCTL->RIS; //poll bit and wait for pll to
        lock
        pllBit&=0x40;
        if(pllBit==0x40)end=1;
    }

    SYSCTL->RCC&=0xFFFFF7FF;   //clear bypass bit

    /*
    //PLL
    //Step 1
    SYSCTL->RCC &= 0xFFBFF7FF; //Set USESYSDIV to 0, kept everything
    else the same
    SYSCTL->RCC |= 0x00400800;
    //Step 2
    SYSCTL->RCC &= 0xFFFFD80F;
    SYSCTL->RCC |= (0x15<<6);
    //step 3
    SYSCTL->RCC &= 0xF83FFFFF;
    SYSCTL->RCC |= 0x01400000; //Sets system divider to 0x2 (66.67 MHz)
    and enables the USESYS bit
    //step 4 (5 in manual)
    SYSCTL->RCC &= 0xFFFFF7FF;
    */
}

//Sensor 1
void enableGPIOPortA(void){
    SYSCTL->RCGCGPIO |= 0x1; //Enable Clock for Port A
    GPIOA->CR = 0xFF;
    //Write 1111 1111 to Commit Regsiter
    //GPIOA->DIR |= 0x20; //Enable Port PA5 to Input
    GPIOA->AFSEL = 0x0;   //Disable Alternate Functionality
    GPIOA->DEN |= 0x20;   //Digital Enable PA5
    //Interrupt Settings
    GPIOA->IS = 0x00;
    //PA5 is set to 0, edge-sensitive
    GPIOA->IBE = 0x00;  //PA5 is set to 0, falling edge or low level input
    GPIOA->IEV = 0x00;  //PA5 is set to 0, falling edge sensative
    //GPIOA->ICR = 0x20;//Clear Flag for PA5
    GPIOA->IM = 0x20;   //Arm interrupt on PA5 (Interrupt Mask)
    //PRIRegister[0x400/4] &= 0x1F; 
    //Priority Register 0, offset 0x400, set to GPIOA Handler to Priority 0
    disableInterruptA();
}

void enableGPIOPortE(void){
    SYSCTL->RCGCGPIO |= 0x10; //Enable Clock for Port E
    //GPIOA->CR = 0xFF;
    //Write 1111 1111 to Commit Regsiter
    //GPIOA->DIR |= 0x20; //Enable Port PA5 to Input
    GPIOE->AFSEL = 0x0;   //Disable Alternate Functionality
    GPIOE->DEN |= 0x20;   //Digital Enable PA5
    //Interrupt Settings
    GPIOE->IS = 0x00;
    //PA5 is set to 0, edge-sensitive
    GPIOE->IBE = 0x00;   //PA5 is set to 0, falling edge or low level input
    GPIOE->IEV = 0x00;   //PA5 is set to 0, falling edge sensative
    //GPIOA->ICR = 0x20; //Clear Flag for PA5
    GPIOE->IM = 0x20;    //Arm interrupt on PA5 (Interrupt Mask)
    //PRIRegister[0x400/4] &= 0x1F; 
    //Priority Register 0, offset 0x400, set to GPIOA Handler to Priority 0
    disableInterruptE();
}

//Sensor 2
void enableGPIOPortB(void){
    SYSCTL->RCGCGPIO |= 0x2; //Enable Clock for Port B
    GPIOB->CR = 0xFF;        //Write 1111 1111 to Commit Regsiter
    //GPIOB->DIR |= 0x20;    //Enable Port PB5 to Input
    GPIOB->AFSEL = 0x0;      //Disable Alternate Functionality
    GPIOB->DEN |= 0x20;      //Digital Enable PB5
    //Interrupt Settings
    GPIOB->IS = 0x0;  //PB5 is set to 0, edge-sensitive
    GPIOB->IBE = 0x0; //PB5 is set to 0, falling edge or low level input
    GPIOB->IEV = 0x0; //PB5 is set to 0, falling edge sensative
    //GPIOB->ICR = 0x20; //Clear Flag for PB5
    GPIOB->IM = 0x20;    //Arm interrupt on PB5 (Interrupt Mask)
    //PRIRegister[0x400/4] |= 0x2000; 
    //Priority Register 0, offset 0x400, set to GPIOB Handler to Priority 1
    disableInterruptB();
}

//Sensor 3
void enableGPIOPortD(void){
    SYSCTL->RCGCGPIO |= 0x8;  //Enable Clock for Port D
    GPIOD->LOCK = 0x4C4F434B; //Unlock Port D
    GPIOD->CR = 0xFF;     //Write 1111 1111 to Commit Regsiter
    //GPIOD->DIR |= 0x80; //Enable Port PD7 to Input
    GPIOD->AFSEL = 0x0;   //Disable Alternate Functionality
    GPIOD->DEN |= 0x80;   //Digital Enable PD7
    //Interrupt Settings
    GPIOD->IS = 0x0;   //PB5 is set to 0, edge-sensitive
    GPIOD->IBE = 0x0;  //PB5 is set to 0, falling edge or low level input
    GPIOD->IEV = 0x0;  //PB5 is set to 0, falling edge sensative
    //GPIOD->ICR = 0x80; //Clear Flag for PB5
    GPIOD->IM = 0x80;    //Arm interrupt on PB5 (Interrupt Mask)
    //PRIRegister[0x400/4] |= 0x40000000; 
    //Priority Register 0, offset 0x400, set to GPIOD Handler to Priority 2
    disableInterruptD();
}

//Testing Purposes
void enableGPIOPortF(void){
    SYSCTL->RCGCGPIO |= 0x20; //Enable Clock for Port F
    GPIOF->LOCK = 0x4C4F434B; //Unlock Port F
    GPIOF->CR = 0x1F;  //Write 0011 1111 to Commit Register
    GPIOF->PUR = 0xFF; //Enable Pull Up Configuration on Port PF4 and PF0
    GPIOF->DIR = 0xFF; //Enable Port PF4 and PF0 to Input
    GPIOF->DEN = 0xFF; //Digital Enable all ports on F
}

void enableInterruptA(void){
    GPIOA->IM = 0x20;
}

void enableInterruptB(void){
    GPIOB->IM = 0x20;
}

void enableInterruptD(void){
    GPIOD->IM = 0x80;
}

void enableInterruptE(void){
    GPIOE->IM = 0x20;
}

void disableInterruptA(void){
    GPIOA->IM = 0x0;
}

void disableInterruptB(void){
    GPIOB->IM = 0x0;
}

void disableInterruptD(void){
    GPIOD->IM = 0x0;
}

void disableInterruptE(void){
    GPIOE->IM = 0x0;
}

void clearIntAFlag(void){
    GPIOA->ICR = 0x20; //Clear Flag for PA5
}

void clearIntBFlag(void){
    GPIOB->ICR = 0x20; //Clear Flag for PB5
}

void clearIntDFlag(void){
    GPIOD->ICR = 0x80; //Clear Flag for PB5
}

// WARNING: THIS MAY NOT BE RESETTING THE CURRENT VALUE OF SYSTICK
// (TRY WRITING TO SYSTICK->VAL)
void setSysTick(void){
    SysTick->CTRL = 0x0;
    __nop();
    __nop();
    SysTick->LOAD = 0xFFFFFF;
    __nop();
    __nop(); 
    SysTick->VAL = 0x0;
    __nop();
    __nop();
    SysTick->CTRL = 0x5;
    __nop();
    __nop(); // use system clock, enable sysTick;
}

void initializeUART(void){
    // We might have to adjust this for our current system clock;
    SYSCTL->RCGCUART |= (1<<0);
    SYSCTL->RCGCGPIO |= (1<<0);
    GPIOA->AFSEL = (1<<1)|(1>>0);
    GPIOA->PCTL = (1<<0) | (1<<4);
    GPIOA->DEN = (1<<0) | (1<<1);
    UART0->CTL &= ~(1<<0);
    //Baud Rate (256000 @ 66.67 mhz sys clock)
    UART0->IBRD = 0x10; // 16
    UART0->FBRD = 0x12; // 18
    //256000 Baudrate @ 16 mhz
    //UART0->IBRD = 0x3;
    //UART0->FBRD = 0x3A;
    UART0->LCRH = (0x3<<5);
    UART0->CTL = (1<<0)|(1<<8)|(1<<9);
}

void iToA (int32_t const num, char *string){
    int32_t n;
    int32_t temp = 0;
    // Clear the string
    for(temp = 0; temp < 10; temp++){
        string[temp] = '0';
        if(num<0){
            n = -num;
        } 
        else{
            n = num;
        }
        if(n>9999999){ // 10000000 to 99999999
            if(num<0){
                string[0] = '-';
            } 
            else {
                string[0] = ' ';
            }
            string[1] = '0'+n/10000000;
            n = n%10000000;
            string[2] = '0'+n/1000000;
            n = n%1000000;
            string[3] = '0'+n/100000;
            n = n%100000;
            string[4] = '0'+n/10000;
            n = n%10000;
            string[5] = '0'+n/1000;
            n = n%1000;
            string[6] = '0'+n/100;
            n = n%100;
            string[7] = '0'+n/10;
            n = n%10;
            string[8] = '0'+n;
            return;
        }

       if(n>999999){ // 1000000 to 9999999
            string[0] = ' ';
            if(num<0){
                string[1] = '-';
            }
            else {
                string[1] = ' ';
            }
            string[2] = '0'+n/1000000;
            n = n%1000000;
            string[3] = '0'+n/100000;
            n = n%100000;
            string[4] = '0'+n/10000;
            n = n%10000;
            string[5] = '0'+n/1000;
            n = n%1000;
            string[6] = '0'+n/100;
            n = n%100;
            string[7] = '0'+n/10;
            n = n%10;
            string[8] = '0'+n;
            return;
        }

        if(n>99999){ // 100000 to 999999
            string[0] = ' ';
            string[1] = ' ';
            if(num<0){
                string[2] = '-';
            }
            else {
                string[2] = ' ';
            }
            string[3] = '0'+n/100000;
            n = n%100000;
            string[4] = '0'+n/10000;
            n = n%10000;
            string[5] = '0'+n/1000;
            n = n%1000;
            string[6] = '0'+n/100;
            n = n%100;
            string[7] = '0'+n/10;
            n = n%10;
            string[8] = '0'+n;
            return;
        }

        if(n>9999){ // 10000 to 99999
            string[0] = ' ';
            string[1] = ' ';
            string[2] = ' ';
            if(num<0){
                string[3] = '-';
            }
            else {
                string[3] = ' ';
            }
            string[4] = '0'+n/10000;
            n = n%10000;
            string[5] = '0'+n/1000;
            n = n%1000;
            string[6] = '0'+n/100;
            n = n%100;
            string[7] = '0'+n/10;
            n = n%10;
            string[8] = '0'+n;
            return;
        }
        
        if(n>999){ // 1000 to 9999
            string[0] = ' ';
            string[1] = ' ';
            string[2] = ' ';
            string[3] = ' ';
            string[4] = ' ';
            if(num<0){
                string[5] = '-';
            }
            else {
                string[5] = ' ';
            }
            string[6] = '0'+n/1000;
            n = n%1000;
            string[7] = '0'+n/100;
            n = n%100;
            string[8] = '0'+n/10;
            n = n%10;
            string[9] = '0'+n;
            return;
        }

        if(n>99){ // 100 to 999
            string[0] = ' ';
            string[1] = ' ';
            string[2] = ' ';
            string[3] = ' ';
            string[4] = ' ';
            string[5] = ' ';
            if(num<0){
                string[6] = '-';
            }
            else {
                string[6] = ' ';
            }
            string[7] = '0'+n/100;
            n = n%100;
            string[8] = '0'+n/10;
            n = n%10;
            string[9] = '0'+n;
            return;
        }
        
        if(n>9){ // 10 to 99
            string[0] = ' ';
            string[1] = ' ';
            string[2] = ' ';
            string[4] = ' ';
            string[5] = ' ';
            string[6] = ' ';
            if(num<0){
                string[7] = '-';
            }
            else {
                string[7] = ' ';
            }
            string[8] = '0'+n/10;
            n = n%10;
            string[9] = '0'+n;
            return;
        }
        // 0 to 9
        string[0] = ' ';
        string[1] = ' ';
        string[2] = ' ';
        string[4] = ' ';
        string[5] = ' ';
        string[6] = ' ';
        string[7] = ' ';
        if(num<0){
            string[8] = '-';
        }
        else {
            string[8] = ' ';
        }
        string[9] = '0'+n;
    }
}      