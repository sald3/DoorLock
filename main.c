#include <msp430.h> 


int main(void){

   WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

   // Settings for P1.0 (LED1)
   P1DIR |= BIT0;
   P1DIR |= BIT1;
   P1DIR |= BIT2;
   P1DIR |= BIT3;

   P1DIR &= ~BIT4;             // Set PxDIR <- 0
   P1REN &= ~BIT4;             // Set PxREN <- 0


   P4DIR &= ~BIT1;              // Clear P4.1 as turned off
   P1REN &= ~BIT1;             // Clear P4.1 as turned off
   PM5CTL0 &= ~LOCKLPM5;

   TB0CTL |= TBCLR;             // Reset timer
   TB0CTL |= TBSSEL__ACLK;      // Clock = ACKL
   TB0CTL |= MC__CONTINOUS;     // Mode=Continuous

   // Set up interrupts

      P4IFG &= ~BIT1;             // Clear P4.1 IRQ Flag
      P4IE |= BIT1;               // Enable P4.1 IRQ


   //Setup TB0 overflow IRQ
   TB0CTL |= TBIE;          // Local enable for TB0 Overflows
   __enable_interrupt();       // Enable Global Interrupt
   TB0CTL &= ~TBIFG;        // Clear IRQ Flag

   while (1){

   }

   return 0;

}

#pragma vector = PORT4_VECTOR
__interrupt void ISR_PORT4_BIT1(void) {

   int b1 = P4IFG & BIT1;
   int b2 = P4IFG & BIT5;

   if (b1 == 0){
       P1OUT ^= BIT0;
   }
   if (b2 == 0){
       P6OUT ^= BIT6;
   }

   P4IFG &= ~BIT1;             // Clear P4.1 IRQ Flag
   P4IFG &= ~BIT5;             // Clear P4.1 IRQ Flag
}


#pragma vector = TIMER0_B1_VECTOR
__interrupt void ISR_TB0_Overflow(void){

   P1OUT ^= BIT2;           // Toggle
   P1OUT ^= BIT3;

   TB0CTL &= ~TBIFG;             // Clear IRQ Flag
}

