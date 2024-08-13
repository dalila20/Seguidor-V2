#pragma once
//Sensor
#define SENSOR1 10
#define SENSOR2 9
#define SENSOR3 A0
#define SENSOR4 A1
#define SENSOR5 A3
#define SENSOR6 A4
#define SENSOR7 A2
#define SENSOR8 12
#define SENSOR9 8

/* Ordem das cores:
 * Vermelho: 5V
 * Laranja: GND
 * Amarelo: 10
 * Verde: A0
 * Azul: 9
 * Roxo: A1
 * Branco: 12
 * Branco: A3
 * Preto: A2
 * Marrom: 8
 * Vermelho: A4
*/

#define NUM_SENSORES 9

#define BUZZER 11
#define TENSAO A5

const int sensores[NUM_SENSORES] = {SENSOR1, SENSOR2, SENSOR3, SENSOR4,
                                    SENSOR5, SENSOR6, SENSOR7, SENSOR8, SENSOR9};

void sensoresSetup()
{
  for (int i = 0; i < NUM_SENSORES; i++)
  {
    pinMode(sensores[i], INPUT);
  }
}

//Ponte H
#define ENA 3
#define IN1 2
#define IN2 4
#define IN3 7
#define IN4 6
#define ENB 5

void ponteHSetup()
{
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
}

// Controle
// 190 - 147
// 150 - 116
// 120 - 93
#define nominalSetSPDA 150
#define nominalSetSPDB 116
#define velocidadeDesafio 80

#define maxSetSPDA 150
#define maxSetSPDB 116

#define MaxSpeedA 150
#define MaxSpeedB 116

#define kp 30
#define kd 30
#define ki 0
int setSpeedA = nominalSetSPDA;
int setSpeedB = nominalSetSPDB;
#define MaxIntegral 70
#define PIDRefreshRate 30 //tempo (MS)

//Fazer curvas
#define velocidadeDeCurva 120
