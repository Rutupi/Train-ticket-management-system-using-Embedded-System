#include <at89c5131.h>

bit tx_complete = 0, rx_complete = 0; // Bit flags for interrupts

/**********************************************************
uart_init(): 
	Initialization function to be completed
	Initializes UART peripheral for 8-bit transfer, 
	1 start and 1 stop bit. 
	
	Please write TH1 value for required baud rate
***********************************************************/	

// Connect P3.0 of the kit to transmit data line (TxD) of the adapter.
// Connect P3.1 of the kit to receive data line (RxD) of the adapter.
void uart_init(void)
{
	TMOD = 0x20;			          // Configure Timer 1 in Mode 2
	TH1 = 204;					      // Load TH1 to obtain require Baudrate (Refer Serial.pdf for calculations)
	SCON = 0x50;			          // Configure UART peripheral for 8-bit data transfer 
	TR1 = 1;						  // Start Timer 1
	ES = 1;							  // Enable Serial Interrupt
	EA = 1;							  // Enable Global Interrupt
}

/**********************************************************
transmit_char(<unsigned char ch>): 
	Transmits a character using UART
***********************************************************/	
void transmit_char(unsigned char ch)
{
	SBUF = ch;				        // Load data in SBUF
	while(!tx_complete); 			// Wait for tx_complete flag (interrupt to complete)
	tx_complete = 0;		      // Clear tx_complete flag 
}

/**********************************************************
transmit_string(<String pointer>): 
	Transmit a string using UART
***********************************************************/	
void transmit_string(unsigned char *s)
{
	while(*s != 0)
	{
			transmit_char(*s++);
	}
}


void transmit_int(int value)
{
    char buffer[6];   // enough for -32768 to 32767
    int i = 0, j;
    char temp;

    // Handle negative numbers
    if(value < 0)
    {
        transmit_char('-');
        value = -value;
    }

    // Convert integer to string (reverse order)
    do
    {
        buffer[i++] = (value % 10) + '0';
        value = value / 10;
    } while(value != 0);

    // Reverse the string
    for(j = 0; j < i/2; j++)
    {
        temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }

    // Transmit characters
    for(j = 0; j < i; j++)
    {
        transmit_char(buffer[j]);
    }
}

/**********************************************************
receive_char(): 
	Receives a character through UART. Returns a 
	character.
***********************************************************/		
unsigned char receive_char(void)
{
	unsigned char ch = 0;
	while(!rx_complete);				// Wait for rx_complete(interrupt to complete)
	rx_complete = 0;
	ch = SBUF;					        // Read data from SBUF
	return ch;					        // Return read character
}

/**********************************************************
Serial_ISR(): 
	Interrupt service routine for UART interrupt.
	Determines whether it is a transmit or receive
	interrupt and raise corresponding flag.
	Transmit or receive functions (defined above) monitor
	for these flags to check if data transfer is done.
***********************************************************/	
void serial_ISR(void) interrupt 4
{
		if(TI==1)			// Check whether TI is set
		{
			TI = 0;			// Clear TI flag
			tx_complete = 1;	// Set tx_complete flag indicating interrupt completion
		}
		else if(RI==1)			// Check whether RI is set
		{
			RI = 0;			// Clear RI flag
			rx_complete = 1;	// Set rx_complete flag indicating interrupt completion
		}
}
