#include <at89c5131.h>
#include "lcd.h"		// Header file with LCD interfacing functions
#include "serial.h"	// Header file with UART interfacing functions
#include <string.h>

// Main function
unsigned char code string_1[]  = "Txn failed: Insufficient Balance\r\n";
unsigned char code string_2[]  = "Max. 4 passengers allowed\r\n";
unsigned char code train_1[]  = "1 Rajdhani Express- Rs. 1500\r\n";
unsigned char code train_2[]  = "2 Chennai Express- Rs. 1000\r\n";
unsigned char code train_3[]  = "3 Hapa Duronto- Rs. 1200\r\n";
void main(void)
{
	int amount = 0;
	int balance = 5000;
	unsigned char balance_buffer[5];
	unsigned char amount_buffer[5];
	unsigned char ch=0;
	unsigned char ID[7];
	unsigned char password[9];
	unsigned char capcha_gen[4];
	unsigned char capcha_user[4];
	int i,temp;
	int seed = 3982;
	// Initialize port P1 for output from P1.7-P1.4
	P1 = 0x0F;
	
	// Call initialization functions
	lcd_init();
	uart_init();
//User Authentication
while(1){

	// 
	while(1)
	{
			// Receive a character
				transmit_string("Login ID:\r\n");
				
				// Room Selection State
				for(i=0;i<6;i++)
        ID[i] = receive_char();
				
				ID[6] = '\0';
				transmit_string("Password:\r\n");
		
				for(i=0;i<8;i++)
        password[i] = receive_char();
		
				password[8] = '\0';
				if(strcmp(ID,"rutupa")==0 && strcmp(password,"rutu3982")==0)
        break;
				else 
					transmit_string("Invalid\r\n");
	}
	

	// Capcha stage 
	while(1)
	{
			// Receive a character
				seed = ((seed * 1024) + 2025) % 1000;
				temp = seed;
				for (i=0;i<3;i++){
						capcha_gen[2-i] = temp%10 + '0';
						temp = temp/10;
				}
				capcha_gen[3] ='\0';
				transmit_string(capcha_gen);		
				transmit_string("\r\n");
				for (i=0;i<3;i++){
						ch = receive_char();
						capcha_user[i] = ch;
				} 
				capcha_user[3] = '\0';
				if(strcmp(capcha_user,capcha_gen)==0)
        break;
				else 
					transmit_string("Invalid\r\n");
			}
			temp = balance;
			for(i=0;i<4;i++){
				balance_buffer[3-i] = temp%10 + '0';
				temp = temp/10;
			}
			balance_buffer[4] = '\0';
			transmit_string("Wallet Balance:");
			for(i=0;i<4;i++){
				transmit_char(balance_buffer[i]);
			}
			transmit_string("\r\n");
					
// ticket booking procedure.
while(1){
	// Train Booking State
	while(1){
	transmit_string(train_1);
	transmit_string(train_2);
	transmit_string(train_3);
	ch = receive_char();
	if (ch == '1'){
		amount = 1500;
		break;
	}
	else if (ch == '2'){
		amount = 1000;
		break;
	}
	else if (ch == '3'){
		amount = 1200;
		break;
	}
	else{
	transmit_string("Invalid Train No.\r\n");
	}
}
	//Passenger booking
	while(1){
		transmit_string("No. of passenger \r\n");
	ch = receive_char();
	if (ch == '1'){
		amount = amount;
		break;
	}
	else if (ch == '2'){
		amount = amount*2;
		break;
	}
	else if (ch == '3'){
		amount = amount*3;
		break;
	}
	else if (ch == '4'){
		amount = amount*4;
		break;
	}
	else{
	transmit_string(string_2);
	}
	}
	temp = amount;
	for(i=0;i<4;i++){
				amount_buffer[3-i] = temp%10 + '0';
				temp = temp/10;
			}
			amount_buffer[4] = '\0';
		
			for(i=0;i<4;i++){
				transmit_char(amount_buffer[i]);
			}
	transmit_string("\r\n");
	
	// Payment and Booking Confirmation state
	transmit_string("Confirm Txn (y/n):\r\n");
		ch = receive_char();
	if (ch =='n') {
		transmit_string("Txn failed\r\n");
	}
	else if (ch =='y'){
		if (amount<=balance){
			transmit_string("Booking successful\r\n");
			balance = balance - amount;
			temp = balance;
			for(i=0;i<4;i++){
				balance_buffer[3-i] = temp%10 + '0';
				temp = temp/10;
			}
			balance_buffer[4] = '\0';
			for(i=0;i<4;i++){
				transmit_char(balance_buffer[i]);
			}
			break;
		}
		else{
			transmit_string(string_1);
		}
	}
	
}
}
}
		