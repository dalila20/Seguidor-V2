#pragma once
//Sensor
#define SENSOR1 10
#define SENSOR2 A0
#define SENSOR3 9
#define SENSOR4 A1
#define SENSOR5 12
#define SENSOR6 A3
#define SENSOR7 A2
#define SENSOR8 8
#define SENSOR9 A4

#define NUM_SENSORES 9

#define BUZZER 11
#define TENSAO A5

const int sensores[NUM_SENSORES] = {SENSOR1, SENSOR2, SENSOR3, SENSOR4, SENSOR5, SENSOR6, SENSOR7, SENSOR8, SENSOR9};

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

//Controle
#define nominalSetSPD 150
#define velocidadeDesafio 80

#define maxSetSPD 150
#define MaxSpeed 150

#define kp 10
#define kd 0
#define ki 0
int setSpeedA = nominalSetSPD;
int setSpeedB = nominalSetSPD;
#define MaxIntegral 70
#define PIDRefreshRate 30 //tempo (MS)


//Fazer curvas
#define velocidadeDeCurva 120

