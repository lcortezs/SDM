/*
Lab 3 
SDM
Luiza C S T Arruda

AQUI TEM DOIS CODIGOS 

*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h> //incluir biblioteca de interrupção
#include <util/delay.h>
#define set_bit(REG, BIT) (REG |= (1<<BIT))
#define clear_bit(REG, BIT) (REG &= ~(1<<BIT))
#define test_bit(REG, BIT) (REG & (1<<BIT))
#define LED1 PB2
#define SW1 PC1

ISR(PCINT1_vect); // Protótipo da Função de Interrupção
char off=0;

int main(void);
{
DDRC = 0x00; //Botao como entrada
PORTC = 0xFF; //Habilita Pull-up no botão
DDRB = 0x04; // Seleciona o LED como saída
PORTB = 0xff; // Apaga o LED

set_bit(PCICR,PCIE1); // Habilita as interrupções do PORTC Página 163
set_bit(PCMSK1,PCINT9); // Conecta o pino C1 à interupção. Página 30/31

sei(); // Liga a chave geral de interrupções.

	while (1){
		if(off) // off diferente de 0
			set_bit(PORTB, LED1); // apaga o led
		
		 else {
			clear_bit(PORTB, LED1); //pisca o led
			_delay_ms(300);
			set_bit(PORTB, LED1);
			_delay_ms(300);
 			}
		 }
}


ISR(PCINT1_vect)
{
	if(!test_bit(PINC,SW1))
		off = ~off;
}

-----------------------------------------------------------------------------
// Teste de interrupção INT0 e INT1

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define set_bit(REG,BIT) (REG |= (1<<BIT))
#define cpl_bit(REG,BIT) (REG ^= (1<<BIT))
#define LED1 PB5

ISR(INT0_vect); // Protótipo da Interrupção INT0.
ISR(INT1_vect); // Protótipo da Interrupção INT1.

int main(void){
DDRD = 0x00; // Seleciona os botões como entrada. PD2 (INT0) e PD3 (INT1)
PORTD = 0x0C; // Habilita pull-up nos botões.
DDRB = 0x20; // Seleciona o LED como saída.
PORTB = 0xFF; // Apaga o LED. (led apaga com um)
UCSR0B = 0x00; // necessário desabilitar Rx e Tx para trabalhar com pinos do
PORTD

// Borda de descida em INT0 e nível 0 em INT1.
set_bit(EICRA,ISC01); //pg. 163/164
EIMSK = 0x03; // Ativa INT0 e INT1. pg. 164
sei(); // Liga a chave geral de interrupções.

while (1){
// NADA.
}
}

ISR(INT0_vect){
cpl_bit(PORTB,LED1); // trocar de estado
 }

ISR(INT1_vect){
cpl_bit(PORTB,LED1);
_delay_ms(200);// enquanto pressionado, pisca led
 }

