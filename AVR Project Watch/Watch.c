#include <mega16.h>
#include <stdio.h>
#include <alcd.h>

// Declare your global variables here
int hour = 10;
int minute = 10;
int second = 0;
int milisecond = 0;
int which_one = 1; //1 for hour     //2 for minute      //3 for second 
char lcd_str[20];


void display_LCD()
{
    lcd_gotoxy(0,0);
    sprintf(lcd_str,"%2d:%2d:%2d:%02d", hour, minute, second, milisecond);
    lcd_puts(lcd_str);
}

// External Interrupt 0 service routine
interrupt [EXT_INT0] void ext_int0_isr(void)
{
    if(which_one == 1)    
    {
        hour--; 
        if(hour == -1)
            hour = 23;     
    }
    else if(which_one == 2)
    {
        minute--;  
        if(minute == -1)
            minute = 59;  
    }
    else if(which_one == 3) 
    {
        second--;
        if(second == -1)
            second = 59;  
    }
    
    display_LCD();     
}

// External Interrupt 1 service routine
interrupt [EXT_INT1] void ext_int1_isr(void)
{
    if(which_one == 1)    
    {
        hour++; 
        if(hour == 24)
            hour = 0;     
    }
    else if(which_one == 2)
    {
        minute++;  
        if(minute == 60)
            minute = 0;  
    }
    else if(which_one == 3) 
    {
        second++;
        if(second == 60)
            second = 0;  
    }
    
    display_LCD();    
}

// External Interrupt 2 service routine
interrupt [EXT_INT2] void ext_int2_isr(void)
{
    which_one++;
    if(which_one == 4)
        which_one = 1;    
}

// Timer 0 overflow interrupt service routine
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
    // Reinitialize Timer 0 value
    TCNT0=0x06;
    // Place your code here
    milisecond++;
    if(milisecond == 125)
    {
        milisecond = 0;
        second++;
        if(second == 60)
        {
            second = 0;
            minute++;
            if(minute == 60)
            {
                minute = 0;
                hour++;
                if(hour == 24)
                {
                    hour = 0;
                }    
            }    
        }
    }
    display_LCD();
}

void main(void)
    {
    // Declare your local variables here

    // Input/Output Ports initialization
    // Port A initialization
    // Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
    DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
    // State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
    PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);

    // Port B initialization
    // Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
    DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
    // State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
    PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

    // Port C initialization
    // Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
    DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
    // State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
    PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

    // Port D initialization
    // Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
    DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
    // State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
    PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

    // Timer/Counter 0 initialization
    // Clock source: System Clock
    // Clock value: 31.250 kHz
    // Mode: Normal top=0xFF
    // OC0 output: Disconnected
    // Timer Period: 4 ms
    TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (0<<CS00);
    TCNT0=0x83;
    OCR0=0x00;

    // Timer(s)/Counter(s) Interrupt(s) initialization
    TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);

    // External Interrupt(s) initialization
    // INT0: On
    // INT0 Mode: Any change
    // INT1: On
    // INT1 Mode: Any change
    // INT2: On
    // INT2 Mode: Falling Edge
    GICR|=(1<<INT1) | (1<<INT0) | (1<<INT2);
    MCUCR=(0<<ISC11) | (1<<ISC10) | (0<<ISC01) | (1<<ISC00);
    MCUCSR=(0<<ISC2);
    GIFR=(1<<INTF1) | (1<<INTF0) | (1<<INTF2);

    // Alphanumeric LCD initialization
    // Connections are specified in the
    // Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
    // RS - PORTA Bit 0
    // RD - PORTA Bit 1
    // EN - PORTA Bit 2
    // D4 - PORTA Bit 4
    // D5 - PORTA Bit 5
    // D6 - PORTA Bit 6
    // D7 - PORTA Bit 7
    // Characters/line: 16
    lcd_init(16);

    // Global enable interrupts
    #asm("sei")
                                                  
    lcd_gotoxy(0, 1);
    lcd_puts("Alireza"); 
    
    while (1)
    {
        // Place your code here

    }
}
