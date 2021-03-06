/*
 * GccApplication1.c
 *
 * Created: 10/03/2020 17:16:22
 * Author : Raiza Guipajós
 */

// O nosso lema é ousadia e alegria a nossa cara é pagode todo dia, cheia estilo na canção eu vou com tudo só espalhando ousadia pelo (que isso?)
#define F_CPU 16000000UL //define a frequencia do microcontrolador 16MHz (necessário para utilizar as rotinas de atraso
#include <avr/io.h> // definições do componente expecificado
#include <util/delay.h> //biblioteca para as rotinas de _delay_ms() e delay_us()
// definições de macros - para o trabalho com os bits de uma variável
#define set_bit(Y, bit_x) (Y|= (1<<bit_x)) // ativa o bit x da variavel y
#define clr_bit(Y, bit_x) (Y&=~ (1<<bit_x)) // limpa o bit x da variavel y
#define cpl_bit(Y, bit_x) (Y^= (1<<bit_x)) // troca o estado do bit x da variavel y
#define tst_bit(Y, bit_x) (Y& (1<<bit_x)) // testa o bit x da variavel y
#define LED PB3 // LED é o substituto de PB3 na programção
#define BOTAO PC2// BOTAO é o substituto de A2 na programação
//---------------------------------------------------------------------------------------------------------------------------------------------------------
int main()
{
DDRB = 0b00001000; //configura o PORTB, PB3 (arduino 11) saída
DDRC = 0b00000000; // configura PORTC, PC2 (arduino A2) todos os pinos são entrada
PORTC = 0b00000100; // habilita o pull-up para o botão PC2

while(1) // laço infinito,
{

if(!tst_bit(PINC, BOTAO)) // SE O BOTAO FOR PRECIONADO EXECUTA O IF
{
while(!tst_bit(PINC, BOTAO))
; //fica preso até soltar o botao
_delay_ms(10); //atraso de 10ms para eliminar o ruido do botao
// if (tst_bit(PORTB, LED)) //se o LED estiver apagado liga o LED
// clr_bit(PORTB, LED);
// else // se não apaga o LED
// set_bit(PORTB, LED);
cpl_bit(PORTB, LED);// pode substituir este laço if-else
}// if do botao pressionado
}//laço infinito
}
