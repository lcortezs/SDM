/*
Projeto Final Sistemas Digitais Microprocessados
Alunos: Luiza Cortez da Silva Tapajoz de Arruda
        Rafael Guimarães Vieira da Silva

Sensor de Umidade de Solo
Utilizando o que já temos do projeto + as ideias daqui (me diz o que tu acha daí)

a. PD2 inicia o processo;
b. Sensor mede a umidade do solo, esse valor é convertido;
c. Se o valor da umidade for menor que um valor x, o motor PB2 é acionado e o buzzer PB3 também. Ficam acionados por determinado período de tempo (acho que aqui iria daí a 
utilização do timer
d. Caso a umidade seja maior que z valor, aciona-se o led PB1 de alerta e o buzzer PB3. Não sei se isso seria necessário, pq se colocar água enquanto o sensor mede daria ruim
mas caso a gente não use assim, pode ser um led que indique que está sendo medido. 
e. Botão PD3 é o botão de emergência. Ele cria uma interrupção externa e caso ele seja acionado o motor irá parar, o led de alerta irá acender e o buzzer irá tocar.

Link pro overleaf: https://www.overleaf.com/2389755347wgsncfpfrcpj

*/

#define F_CPU 16000000UL //Definição da frequência do Microcontrolador
#include <avr/io.h> //Definições do componente especificado
#include <avr/interrupt.h> //Incluindo biblioteca de interrupção

#define set_bit(Y,bit_x) (Y|=(1<<bit_x)) /*ativa o bit */
#define clr_bit(Y,bit_x) (Y&=~(1<<bit_x)) /*limpa o bit */
#define tst_bit(Y,bit_x) (Y&(1<<bit_x)) /*testa o bit */

// Tabela de vetores pag. 158 COPIEI ISSO DE UM CÓDIGO DA PROFESSORA
ISR(INT1_vect); // Protótipo da Interrupção externa INT1.
ISR(TIMER1_OVF_vect); // Protótipo da Interrupção TIMER1. (16 bits) 

//Entradas e Saídas (eu q fiz)
#define alerta PB1
#define motor PB2
#define buzzer PB3
#define sensor_umidade PC0 //Nessa parte fiquei com dúvida, acho que temos que olhar o código base (conecta no PD0 também???)
#define botao_inicia PD2 // O que acha?? 
#define botao_emergencia PD3 //Eu pensei em colocarmos um botão de emergência, para usar uma interrupção externa. O que acha?

//Variáveis (copiei de um código da prof)
char tempo_lavacao = 0; // tempo de lavação media e pesada
char flag_tempo = 0; // permite contar tempo
char minuto = 0;
char segundo = 0;

char status = 0; // estado das saidas da maquina;
char status_motor =0; // estado do motor (duty cycle)

int main(void)
{

//Configuração de entradas e saídas digitais (já de acordo com a config criada ali em cima)
DDRB = 0b00001110; // pinos PB1, PB2 e PB3: saidas
DDRD = 0x00; // pino PD2 e PD3: entradas
PORTD = 0b00001100; // pull up: PD3 e PD2
  
//A partir daqui estou copiando a resolução do exercício da máquina de lavar, td isso tem que ser editado :)

// Configuração do ADC
DIDR0 = 0x0C;// entrada analógica no PC2 e PC3
ADCSRA = 0x87; // ADC habilitado, prescaler = 128
  
// configuração da interrupção INT1 da tampa da máquina
UCSR0B = 0x00; // desabilitar Rx e Tx usar PORTD pg.354
EICRA = 0x00; // nivel baixo em INT1 pg. 163
EIMSK = 0x02; // Ativa INT1. pg. 164

// configuração da interrupção de timer no TC1(16 bits)
TCCR1A = 0; // timer para oper.normal OC1A e OC1B desconect.
TCCR1B = 0; //limpa registrador
TCCR1B |= (1<<CS10)|(1 << CS12); // configura prescaler 1024
TCNT1 = 0xC2F7; //valor para que estouro ocorra em 1 segundo
                // 65536-(16MHz/1024/1Hz) = 65536 - 15.625 = 49911(0xC2F7)
 TIMSK1 |= (1 << TOIE1);
  
// configuração do PWM no PD5 => TC0 => OCR0B pg.32
TCCR0A = 0b00110001; //PWM com fase corrigida, saida OC0B não invertida, modo 1 e TOP = 0xff pg.196
TCCR0B = 0x02; //PWM fase corrigida, modo 1 e prescaler = 8
              // fPWM = 4000Hz

// desliga motor
OCR0B = 0; // duty cycle = 0
  
sei(); // Liga a chave geral de interrupções.
  
while(1) //laço infinito
{
  
}

/*****************************************************************************
                     Rotina de tratamento de Interrupção
******************************************************************************/
ISR(INT1_vect)
{
status = PORTD; // salva conteúdo das saidas na PORTD
status_motor = OCR0B;
OCR0B = 0; //desliga motor
PORTD = 0x00; // desliga tudo
// verifica se a tampa foi fechada
while(!tst_bit(PIND,PD3))
 ;
PORTD = status; // retorna condição das saídas
OCR0B = status_motor; // …inclusive do motor
}

/*******************************************************************************
            Rotina de tratamento de Timer (1s)- clock interno
********************************************************************************/
ISR(TIMER1_OVF_vect)
{
// verifica flag_tempo, se igual a zero, sai da rotina
if(flag_tempo !=0)
 {
 TCNT1 = 0xC2F7; // reseta o contador para contar 1s
 segundo++;
 if (segundo == 60)
 {
 minuto ++;
 segundo = 0;
 }
 }
}
