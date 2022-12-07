
#include <msp430.h>

void delay(){

    volatile unsigned long int i;

//   i = 900000;                          // SW Delay
    i = 5000;

    while (i != 0)
    {
        i--;
    }
}

void clean()
{
    P1OUT &= ~BIT5;      //1
    P1OUT &= ~BIT6;      //2
    P1OUT &= ~BIT7;      //3
}

void timerReset(){
    TB0CTL |= TBCLR;             // Reset timer
    TB0CTL |= TBSSEL__ACLK;      // Clock = ACKL
    TB0CTL |= MC__CONTINOUS;     // Mode=Continuous

    TB0CTL |= TBIE;
}

char code[] = {'\0'};
unsigned int x = 0;
volatile unsigned int signal = 0;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//INTERRUPTS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma vector = PORT1_VECTOR
__interrupt void ISR_PORT1_BIT2(void) {

    code[x] = 'a';
    x++;
    P3OUT ^= BIT6;
    timerReset();

   P1IFG &= ~BIT2;         // Clear P1.2 IRQ Flag

}

#pragma vector = PORT2_VECTOR
__interrupt void ISR_PORT2_BIT1(void) {

    if (P2IFG & BIT1){
        code[x] = 'b';
        x++;
        P3OUT ^= BIT6;
        timerReset();
    }
    if (P2IFG & BIT2){
        if(code[0] == 'c' && code[1] == 'a' && code[2] == 'b' && code[3] == '\0'){
            signal = 1;
            code[0] = '\0';
            code[1] = '\0';
            code[2] = '\0';
            code[3] = '\0';
            x = 0;
        }
        else{
            code[0] = '\0';
            code[1] = '\0';
            code[2] = '\0';
            code[3] = '\0';
            x = 0;
        }
    }



   P2IFG &= ~BIT2;         // Clear P2.2 IRQ Flag

   P2IFG &= ~BIT1;         // Clear P2.1 IRQ Flag

}

#pragma vector = PORT3_VECTOR
__interrupt void ISR_PORT3_BIT0(void) {

    code[x] = 'c';
    x++;
    P3OUT ^= BIT6;
    timerReset();

   P3IFG &= ~BIT0;         // Clear P3.0 IRQ Flag
}

#pragma vector = PORT4_VECTOR
__interrupt void ISR_PORT4_BIT4(void) {

    code[x] = 'd';
    x++;
    P3OUT ^= BIT6;
    timerReset();

   P4IFG &= ~BIT4;         // Clear P4.4 IRQ Flag
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TIMER INTERRUPT
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma vector = TIMER0_B1_VECTOR
__interrupt void ISR_TB0_Overflow(void){

   //P1OUT ^= BIT7;           // Toggle

   code[0] = '\0';
   code[1] = '\0';
   code[2] = '\0';
   code[3] = '\0';
   x = 0;

   TB0CTL &= ~TBIFG;             // Clear IRQ Flag
}





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MAIN CODE
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;// Disable the GPIO power-on default high-impedance mode

    P1OUT ^= BIT0;      // Toggle P1.0 using exclusive-OR

    int lightCondition = 0;

    //setting up signal output pins
    P1DIR |= BIT5;//1
    P1DIR |= BIT6;//2
    P1DIR |= BIT7;//3
    P3DIR |= BIT6;//4
    P5DIR |= BIT2;//5
    P4DIR |= BIT5;//6

    P4DIR &= ~BIT0;                 // Clear Bit P4.1. 0 = Input, 1 = Output
    P4REN |= BIT0;                  // Set 1 to Bit 1
    P4OUT &= ~BIT0;                  // Set 0 to Bit 1

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // INTERRUPT PORTS SETUP
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    P2DIR &= ~BIT1;
    P2IES |=  BIT1;

    P2DIR &= ~BIT2;
    P2IES |=  BIT2;

    P3DIR &= ~BIT0;
    P3IES |=  BIT0;

    P4DIR &= ~BIT4;
    P4IES |=  BIT4;

    P1DIR &= ~BIT2;
    P1IES |=  BIT2;




    P2IFG &= ~BIT1;
    P2IE |= BIT1;

    P2IFG &= ~BIT2;
    P2IE |=  BIT2;

    P3IFG &= ~BIT0;
    P3IE |= BIT0;

    P4IFG &= ~BIT4;
    P4IE |= BIT4;

    P1IFG &= ~BIT2;
    P1IE |= BIT2;


    timerReset();


    clean();

    P1OUT ^= BIT5;

    __enable_interrupt();       // Enable Global Interrupt
    TB0CTL &= ~TBIFG;        // Clear IRQ Flag


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // MAIN CODE LOOP SECTION
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    while(1)
    {
        int SW1;
        SW1 = P4IN;                 // Store a copy
        SW1 &= BIT0;

        if(SW1 == 1 && lightCondition == 0){
            P1OUT ^= BIT6;
            P1OUT ^= BIT5;
            lightCondition = 1;
        }

        if (SW1 == 0 && lightCondition == 1){
            P1OUT ^= BIT6;
            P1OUT ^= BIT5;
            lightCondition = 0;
        }


        if(signal == 1){
            P1OUT ^= BIT7;

            delay();

            signal = 0;
            P1OUT ^= BIT7;
        }



    }

    return 0;
}
