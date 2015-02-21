#include "msp430.h"

#define LED0 BIT3
#define LED1 BIT2
#define Mf BIT6
#define Mb BIT7
#define IRrec BIT4
#define IRrec1 BIT5
#define Switch1 BIT1
#define Battery BIT0


int bit =0;
int bitt =0;
int count =0;
int resetcount =0;
int loopcount=0;
int loopcountMAX=0;
int one =0;
int two=0;
int three =0;
int four=0;
int five=0;
int six=0;
int seven=0;
int eight =0;
int ledcount =0;
int odd=0;
unsigned long realdata =0;
unsigned long rxdata =0;
unsigned long remoteButton =0;
unsigned long writeFirst = 0;
unsigned long writeSecond= 0;
int on =0;
int i =0;
int program =0;
unsigned int temp1=0;
unsigned long temp2=0;
unsigned long motorcount=0;

int *addr = (int *)0x1040 ; // Address of the flash memory segment starting
int *addr2 = (int *)0x1082 ; // Address of the flash memory segment starting


void flash_erase(int *addrR);
void flash_write(int *addrR , int value);

void main(void){

WDTCTL = WDTPW + WDTHOLD;                            // Stop watchdog timer

  P1DIR |= Mf + Mb + LED1 + LED0;
  P1REN |= Switch1;
  P1OUT |= Switch1;

  P1IE |= IRrec; // P1.3 interrupt enabled
  P1IES &= ~IRrec; // P1.3 interrupt enabled
  P1IFG &= ~IRrec; // P1.3 IFG cleared
  P1IE |= IRrec1; // P1.3 interrupt enabled
  P1IES |= IRrec1; // P1.3 interrupt enabled
  P1IFG &= ~IRrec1; // P1.3 IFG cleared

  TA0CCTL0 |= CCIE;                             // CCR0 interrupt enabled
  TA0CTL = TASSEL_2 + MC_1 + ID_2;           // SMCLK/8, upmode
  TA0CCR0 =  20;

  temp1 = *addr;
  temp2 = *addr2;
temp2= (temp2 << 16);
  remoteButton = (temp2 + temp1);
  __enable_interrupt(); // enable all interrupts                                   // Enable CPU interrupts



  while(1)
  {
    _delay_cycles(100);

    if(resetcount == 200)
    {
      loopcountMAX=loopcount;
      loopcount =0;
      realdata = rxdata >> 1;

    }
    if(realdata != 0) //gun 1 code
    {
      P1IE &= ~(IRrec + IRrec1);
      P1OUT |=LED1;
      P1OUT &=~LED0;
      program=0;
      for(i=4;i>=0;i--)
      {
    	    if((P1IN & Switch1) != Switch1)
    	    {
    	    	program++;
    	    }
    	    _delay_cycles(300);
      }
      if(program>4)
      {
    	  writeFirst= (realdata & 0x0000FFFF);
    	  writeSecond = (realdata & 0xFFFF0000)>>16;
    	    flash_erase(addr);
    	    flash_erase(addr2);
    	    flash_write(addr, (int)writeFirst);
    	    flash_write(addr2, (int)writeSecond);
    	    temp1 = *addr;
    	    temp2 = *addr2;
    	    temp2= (temp2 << 16);
    	    remoteButton = (temp2 + temp1);


          P1OUT &= ~LED1;
          _delay_cycles(100000);
          P1OUT |= LED1;
          _delay_cycles(100000);
          P1OUT &= ~LED1;
          _delay_cycles(100000);
          P1OUT |= LED1;
          _delay_cycles(100000);
          P1OUT &= ~LED1;
          _delay_cycles(100000);
          P1OUT |= LED1;
          _delay_cycles(100000);
          P1OUT &= ~LED1;
          _delay_cycles(100000);
          P1OUT |= LED1;
      }
      ledcount=0;

      if(realdata == remoteButton) //gun 1 code
      {
        if(on ==1 )
        {
          P1OUT |= Mf;
          P1OUT &= ~Mb;
          __delay_cycles(200000);
          P1OUT |= Mb;
          motorcount=0;
          on =0;
        }
        else
        {
          P1OUT |= Mb;
          P1OUT &= ~Mf;
          __delay_cycles(200000);
          P1OUT |= Mf;
          motorcount=0;
          on =1;
        }
      }
      realdata =0;
      P1IE |= (IRrec + IRrec1);
    }
    if(ledcount ==2000)
    {
      P1OUT |=LED0;
      P1OUT &=~LED1;
    }
    if(ledcount == 4000)
    {
      P1OUT &=~LED0;
      P1OUT &=~LED1;
    }
    if(motorcount == 9000)
    {
    	P1OUT &= ~(Mf + Mb);
    }
    ledcount++;
    if(ledcount >5000)
    {
      ledcount =5000;
    }
    resetcount++;
    if(resetcount >1000)
    {
      resetcount =999;
    }
    if(count >1000)
    {
      count =999;
    }
    motorcount++;
    if(motorcount >10000)
    {
    	motorcount =10000;
    }

  }
}




#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  TA0CTL |= TACLR;

  if(loopcount == 0)
  {
    rxdata =0;
  }
  if (loopcount == 50)
  {
    one = count;
  }
  else if (loopcount ==51)
  {
    two = count;
  }
  else if(loopcount ==52)
  {
    three = count;
  }
  else if(loopcount ==53)
  {
    four = count;
  }
  else if(loopcount == 54)
  {
    five = count;

  }
  else if(loopcount == 55)
  {
    six = count;

  }
  else if(loopcount == 56)
  {
    seven = count;

  }
  else if(loopcount == 57)
  {
    eight = count;

  }

  odd = (loopcount & 0x1);
  if(odd == 0)
  {
    if(count >13 && count < 24)
    {
      bit = 1;
    }
    else
    {
      bit = 0;
    }

    rxdata += bit;
    rxdata<<=1;
  }

  loopcount++;
  count=0;
  resetcount=0;
  P1IFG &= ~IRrec; // P1.3 IFG cleared
  P1IFG &= ~IRrec1; // P1.3 IFG cleared
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
  count++;
}


void flash_erase(int *addrR)
{
  __disable_interrupt();               // Disable interrupts. This is important, otherwise,
                                       // a flash operation in progress while interrupt may
                                       // crash the system.
  while(BUSY & FCTL3);                 // Check if Flash being used
  FCTL2 = FWKEY + FSSEL_1 + FN3;       // Clk = SMCLK/4
  FCTL1 = FWKEY + ERASE;               // Set Erase bit
  FCTL3 = FWKEY;                       // Clear Lock bit
  *addr = 0;                           // Dummy write to erase Flash segment
  while(BUSY & FCTL3);                 // Check if Flash being used
  FCTL1 = FWKEY;                       // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                // Set LOCK bit
  __enable_interrupt();
}

void flash_write(int *addrR , int value)
{
  __disable_interrupt();
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  FCTL3 = FWKEY;                            // Clear Lock bit
  *addrR = 0;                           // Dummy write to erase Flash segment

  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

  *addrR = value;         // copy value to flash

  FCTL1 = FWKEY;                        // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                 // Set LOCK bit
  while(BUSY & FCTL3);
  __enable_interrupt();
}


