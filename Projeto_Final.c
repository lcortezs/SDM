/*
Projeto Final Sistemas Digitais Microprocessados
Alunos: Luiza Cortez da Silva Tapajoz de Arruda
		Rafael Guimarães Vieira da Silva

 Sensor de Umidade de Solo

- PD2, o botão de iniciar, inicia o processo;
- O LED PB4 irá indicar o funcionamento, ficará ligado durante o processo;
- O sensor A0, conectado em PC0, mede a umidade do solo, esse valor é convertido.
O sensor funciona como um potenciômetro, ou seja, o valor de 0 à 5V corresponde
de 0 à 1023 em bits;
- Se o valor da umidade for menor que 50\% (o valor de referência para o ADC é de
511), o motor PB2 é acionado e o buzzer PB3 também. Ambos ficam acionados por 15
segundos;
- Caso a umidade seja maior que 85\% (o valor de referência para o ADC é de 153),
aciona-se o LED PB1, que sinaliza um alerta. O buzzer PB3 é ativado juntamente.
O LED PB4 e o Motor são desativados;
- O botão PD3 é o botão de emergência, que sinaliza uma interrupção externa. Ao
ser acionado o motor irá parar, o LED de alerta irá acender, o LED PB4 irá
apagar e o buzzer irá tocar.

*/
#define F_CPU 16000000UL //Definição da frequência do Microcontrolador
#include <avr/io.h> //Definições do componente especificado
#include <avr/interrupt.h> //Incluindo biblioteca de interrupção
#include <util/delay.h> //bibliot. para as rotinas de _delay_ms() e delay_us()
#define set_bit(Y,bit_x) (Y|=(1<<bit_x)) /*ativa o bit */
#define clr_bit(Y,bit_x) (Y&=~(1<<bit_x)) /*limpa o bit */
#define tst_bit(Y,bit_x) (Y&(1<<bit_x)) /*testa o bit */

// Tabela de vetores
ISR(INT1_vect); // Protótipo da Interrupção externa INT1.
ISR(TIMER1_OVF_vect); // Protótipo da Interrupção TIMER1. (16 bits)

//Entradas e Saídas
#define alerta PB1
#define motor PD5
#define buzzer PB3
#define indicador PB4
#define sensor_umidade PC0
#define botao_inicia PD2
#define botao_emergencia PD3
#define botao_desliga PD4

//Variáveis
char processo = 0; // variavel do processo
char flag_tempo = 0; // permite contar tempo
char segundo = 0;
char status = 0; // estado das saidas da maquina
char status_motor = 0; // estado do motor (duty cycle)
char tempo=0;

int main(void)
{
//Configuração de entradas e saídas digitais
DDRB = 0x1A; // pinos PB1, PB3 e PB4: saidas
DDRD = 0x20; // pino PD2 e PD3: entradas e PD5: saida
PORTD = 0x0C; // pull up: PD3 e PD2

//Configuração do ADC
DIDR0 = 0x01;// entrada analógica no PC0
ADCSRA = 0x87; // ADC habilitado, prescaler = 128
ADMUX = 0x40; //Tensão AVCC, alinhada a direita, ADC0

//Configuração da interrupção externa INT1 do botão de emergência
UCSR0B = 0x00; // desabilitar Rx e Tx usar PORTD
EICRA = 0x08; // nivel baixo em INT1
EIMSK = 0x02; // Ativa INT1

//Configuração da interrupção de timer no TC1(16 bits)
TCCR1A = 0; // timer para oper.normal OC1A e OC1B desconect.
TCCR1B = 0; //limpa registrador
TCCR1B |= (1<<CS10)|(1 << CS12); // configura prescaler 1024
TCNT1 = 0xC2F7; //valor para que estouro ocorra em 1 segundo
 // 65536-(16MHz/1024/1Hz) = 65536 - 15.625 = 49911(0xC2F7)
TIMSK1 |= (1 << TOIE1);

//Configuração do PWM no PD5 => TC0 => OCR0B
TCCR0A = 0x31; //PWM com fase corrigida, saida OC0B não invertida, modo 1 e TOP =
0xff pg.196
TCCR0B = 0x04; //PWM fase corrigida e prescaler = 256
// desliga motor
OCR0B = 0; // duty cycle = 0

sei(); // Liga a chave geral de interrupções.

while(1) //laço infinito
{
	if(tst_bit(PIND, botao_inicia)==0) //se o botão for pressionado, inicia o processo
	processo = 1;

	if(tst_bit(PIND, botao_desliga)==0) //se o botão for pressionado o processo para
	processo = 0;

	_delay_ms(500);

	while(processo==1)
	{
		set_bit(PORTB,indicador); //LED para indicar inicio do processo
		set_bit(ADCSRA,ADSC); //inicia a conversão
		while(tst_bit(ADCSRA,ADSC)==1); //aguarda a conversão
/***************************
 Variáveis que o usuário de adaptar ao seu uso
 O usuário deve fazer a seguinte conta para encontrar o valor ADC ideal para seu cultivo
 [(%min de água * 5 * 1023)/5] = valor ADC para a menor % de água aceitável
 [(%max de água * 5 * 1023)/5] = valor ADC para a maior % de água aceitável
****************************/
		if(ADC>511)//Umidade menor do que 50%, ou seja, maior que [(2,5*1023)/5]=511
		{
			flag_tempo=1;
			TCNT1=0xC2F7;
			while(segundo<=15) //define quanto tempo o usuário quer que o sistema de irrigação fica ligado
			{
				OCR0B = 256; //duty cycle = 100%, motor ligado em potência máxima o usuário poderá modificar a intensidade do sistema de irrigação dependendo da demanda
				set_bit(PORTB,PB3); //liga o buzzer
			}
			flag_tempo=0; //Para de contar
		}

		if(ADC<153)//Umidade maior do que 85%, ou seja, menor que [(0,75*1023)/5] = 153
		{
			OCR0B = 0; //desliga motor
			PORTB = 0x00; // desliga tudo
			set_bit(PORTB,alerta);
			set_bit(PORTB,buzzer);
		}
	}
}
}
/***************************
 Rotina de tratamento de Interrupção
**************************/
ISR(INT1_vect)
{
		status = PORTB; //salva conteudo das saidas na PORTB
		status_motor = OCR0B;
		OCR0B = 0; //desliga motor
		PORTB = 0x00; // desliga tudo
		clr_bit(PORTB,alerta);
		clr_bit(PORTB,buzzer);

		// verifica se o botão foi pressionado novamente, caso ele tenha sido, ele voltará
		ao processo
		while((!tst_bit(PIND,botao_emergencia))==0)
		{
			PORTB = status; // retorna condição das saídas
			OCR0B = status_motor; // …inclusive do motor	
		}
}
/***************************
 Rotina de tratamento de Timer (1s)- clock interno
****************************/
ISR(TIMER1_OVF_vect)
{
// verifica flag_tempo, se igual a zero, sai da rotina
if(flag_tempo !=0)
	{
		TCNT1 = 0xC2F7; //reseta o contador
		segundo++;
	}
}
