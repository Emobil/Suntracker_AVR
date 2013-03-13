#include "twislave.h"   //Biblotek aus dem Internet welche die twi. h verwendet! 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#define SLAVE_ADRESSE 0x13   // I2C Adresse !! Für Raspberry 

uint32_t hall_counter_1, hall_counter_2;  
uint32_t hall_counter_1_buf, hall_counter_2_buf;   

int Check; 
ISR(INT0_vect){ hall_counter_1++; }		
ISR(INT1_vect){ hall_counter_2++; }

void init_hall_counters()
{
	hall_counter_1 = 0x00;				// Zähler auf Null Setzen!! 
	hall_counter_2 = 0x00;
	hall_counter_1_buf = 0x00;
	hall_counter_2_buf = 0x00;

	DDRD &= ~( (1<<PD2) | (1<<PD3) );
	PORTD |= (1<<PD2)|(1<<PD3);
	EICRA |= (1<<ISC11)|(1<<ISC01); // External Interrupt Control Register AInterrupt PIN --> Datenblatt Seite 81 // Steigende Flanke bei INT1 generiert eine Interrupt. 
	EIMSK |= (1<<INT1)|(1<<INT0);   // External Interrupt Mask Register Dataenblatt Seite 82
}
int main( void )
{

	TCCR0A= (0<<WGM01) | (1<<WGM00)|(1<<COM0B1) | (0<<COM0B0);//(1<<COM0A1) | (0<<COM0A0)
	TCCR0B=(0<<FOC0A) | (0<<FOC0B) | (0<<WGM02) | (0<<CS02) | (1<<CS01) | (1<<CS00); // --> Siehe Datenblatt s 101 ff

	// Achtung PWM am MODUS 3 Setzen 

	// Meine Frage dazu im Netz // http://www.mikrocontroller.net/topic/281776#2977371
	/*Du setzt den Modus 7.
	Das ist Fast-PWM mit OCR0A als Top.
	
	Damit fällt OCR0A schon mal als PWM weg, und da OCR0A den gleichen Wert 
	hat wie OCR0B, wirst du von der PWM auf dem B Kanal nicht viel sehen.
	*/
	OCR0A = 0x7f; // zum einstellen des Tastverhältnisses 
	OCR0B = 0x7f; // zum einstellen des Tastverhältnisses 

	DDRD = (1<<PD5)|(1<<PD7)|(1<<PD6);//PD6 und PD5 wo OC0A und OC0B anliegen (Ausgabe der PWM)  // PD7 für M1 dir
	DDRB = (1<<PB0)|(1<<PB2)|(1<<PB7); //PB 1 ist Ausgang M1D1 --> D1 // PB2 ist für D2
	DDRC |= (1<<PC0); 
	// Laut Wahheitstabele und Schaltung muss EN -> H / D1-> L / D2 -> H / IN1-> H / IN2 -> L 

	// I2C Initialisieren
	init_twi_slave(SLAVE_ADRESSE);   //TWI als Slave mit Adresse slaveadr starten
	init_hall_counters();
	sei();

	i2cdata[0] = 0x13;
	i2cdata[1] = 0x22;
	i2cdata[2] = 0xFE;
	Check = 0; 
	
	while(1){
		
		if((hall_counter_1 > 400) | (hall_counter_2 > 400))
		{
			TCCR0A = 0x00;
			TCCR0B=0x00;
			Check = 0; 
			}
			else{}
		PORTD = (1<<PD7);
		PORTB = (1<<PB2)|(1<<PB7);   // D2 - PIN 
		//PORTB &= ~(1<<PB0);
		PORTC = (1<<PC0);  // EN - PIN } 
		}	
return 0; 
}


