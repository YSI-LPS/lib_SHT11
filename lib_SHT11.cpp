/** SHT11 class
*
* @purpose       library for SHT11 humidity and temperature sensor
*
* Utilisée pour mesurer la temperature et l'humidite
*
* https://www.sensirion.com/products/humidity-sensor/
*
* Copyright (c) 2012, cstyles (http://mbed.org)
*
* Exemple:
* @code
* #include "mbed.h"
* #include "lib_SHT11.h" 
* 
* SHT11 capteur_HT;
* //SHT11 capteur_HT(p30, p29, 2.0);   //data, sck, periode_cycle_mesure
* Serial pc(USBTX, USBRX);
* 
* int main()
* {
*     float Temperature_mesuree=0.0, Humidite_mesuree=0.0;
* 
*     while(1)
*     {
*         if((Temperature_mesuree != capteur_HT.lire_T()) || (Humidite_mesuree != capteur_HT.lire_H()))
*         {
*             capteur_HT.lire_T_H(&Temperature_mesuree, &Humidite_mesuree);
*             pc.printf("T=%f\tH=%f\r\n", Temperature_mesuree, Humidite_mesuree);
*         }
*     }
* }
* @endcode
* @file          lib_SHT11.cpp 
* @date          Jun 2012
* @author        Yannic Simon
*/

#include "lib_SHT11.h"


SHT11::SHT11(PinName pin_DATA, PinName pin_SCK, float T_C_mesure) : DATA_HT(pin_DATA), SCK_HT(pin_SCK), Temps_cycle_mesure(T_C_mesure)
{
    if(Temps_cycle_mesure < 2)  Temps_cycle_mesure = 2;
    init();
}

void SHT11::start(void)
{
    DATA_HT.output();
    DATA_HT=1;
    SCK_HT=0;
    wait_us(1);
    SCK_HT=1;
    wait_us(1);
    DATA_HT=0;
    wait_us(1);
    SCK_HT=0;
    wait_us(1);
    SCK_HT=1;
    wait_us(1);
    DATA_HT=1;
    wait_us(1);
    SCK_HT=0;
    wait_us(1);
    DATA_HT=0;
}

void SHT11::changer_timer_cycle(float new_Temps_cycle_mesure)
{
    if(new_Temps_cycle_mesure < 2)  Temps_cycle_mesure = 2;
    else Temps_cycle_mesure = new_Temps_cycle_mesure;
    if(etat_cycle)  desactiver();   init();
}

void SHT11::init(void)
{
//          ____________________________________________  
// DATA_HT                                              |__
//            __   __   __   __   __   __   __   __   __
// SCK_HT   _|  |_|  |_|  |_|  |_|  |_|  |_|  |_|  |_|  |_
    unsigned char i;
    DATA_HT.output();
    DATA_HT=1;
    SCK_HT=0;
    for (i=0;i<9;i++)
    {
        SCK_HT=1;
        wait_us(1);
        SCK_HT=0;
        wait_us(1);
    }
        etat_sht11=0;
        etat_cycle=1;
        Timer_cycle_mesure.attach(callback(this, &SHT11::gestion_HT), Temps_cycle_mesure);
}

void SHT11::desactiver(void)
{
        etat_cycle=0;
        Timer_cycle_mesure.detach();
}

void SHT11::demande_mesure(short octet_de_commande)
{
    unsigned int i;

    DATA_HT.output();
    for(i=0;i<8;i++)
    {              
        if ((octet_de_commande)&(0x80)) DATA_HT=1;
        else DATA_HT=0;
        wait_us(1);
        SCK_HT=1;
        wait_us(1);
        SCK_HT=0;
        octet_de_commande=octet_de_commande*2;
    }
    DATA_HT.input();
    wait_us(1);
    SCK_HT=1;
    wait_us(1);
    SCK_HT=0;
    wait_us(1);
    DATA_HT.output();
    DATA_HT=1;
    DATA_HT.input();
}

void SHT11::mesure_T(void)
{
    start();
    demande_mesure(3);
}

void SHT11::mesure_H(void)
{
    start();
    demande_mesure(5);
}

void SHT11::calcul_Temperature_Humidite(void)
{
    Temperature_mesuree=(Temperature*0.01-40);
    Humidite_mesuree=((float)Temperature_mesuree-25.0)*(0.01+(0.00008*Humidite))-4+0.0405*Humidite-0.0000028*Humidite*Humidite;
}

void SHT11::reception_mesure_T(void)
{
    unsigned int i;
    Temperature=0;

        DATA_HT.input();
    for (i=0;i<8;i++)
    {
        SCK_HT=1;
        wait_us(1);
        Temperature *=2;
        if (DATA_HT==1) Temperature++;
        SCK_HT=0;
        wait_us(1);
    }
    DATA_HT.output();
    DATA_HT=0;
    SCK_HT=1;
    wait_us(1);
    SCK_HT=0;
    wait_us(1);
    DATA_HT.input();
    for (i=0;i<8;i++)
    {
        SCK_HT=1;
        wait_us(1);
        Temperature *=2;
        if (DATA_HT==1) Temperature++;
        SCK_HT=0;
        wait_us(1);
    }
    DATA_HT.output();
    DATA_HT=1;
    SCK_HT=1;
    wait_us(1);
    SCK_HT=0;
    wait_us(1);
}

void SHT11::reception_mesure_H(void)
{
    unsigned int i;
    Humidite=0;

        DATA_HT.input();
    for (i=0;i<8;i++)
    {
        SCK_HT=1;
        wait_us(1);
        Humidite *=2;
        if (DATA_HT==1) Humidite++;
        SCK_HT=0;
        wait_us(1);
    }
    DATA_HT.output();
    DATA_HT=0;
    SCK_HT=1;
    wait_us(1);
    SCK_HT=0;
    wait_us(1);
    DATA_HT.input();
    for (i=0;i<8;i++)
    {
        SCK_HT=1;
        wait_us(1);
        Humidite *=2;
        if (DATA_HT==1) Humidite++;
        SCK_HT=0;
        wait_us(1);
    }
    DATA_HT.output();
    DATA_HT=1;
    SCK_HT=1;
    wait_us(1);
    SCK_HT=0;
    wait_us(1);
}

void SHT11::gestion_HT(void)
{   
    switch (etat_sht11)
    {
        case 0:
        Timer_cycle_mesure.detach();
        mesure_T();
        etat_sht11=1;
        Timer_cycle_mesure.attach_us(callback(this, &SHT11::gestion_HT), 10000);
        break;
        
        case 1:
        DATA_HT.input();
        if (DATA_HT==0)
        {
            Timer_cycle_mesure.detach();
            etat_sht11=2;
            Timer_cycle_mesure.attach_us(callback(this, &SHT11::gestion_HT), 1);
        }
        break;
        
        case 2:
        Timer_cycle_mesure.detach();
        reception_mesure_T();
        etat_sht11=3;
        Timer_cycle_mesure.attach_us(callback(this, &SHT11::gestion_HT), 1);
        break;
        
        case 3:
        Timer_cycle_mesure.detach();
        mesure_H();
        etat_sht11=4;
        Timer_cycle_mesure.attach_us(callback(this, &SHT11::gestion_HT), 10000);
        break;
        
        case 4:
        DATA_HT.input();
        if (DATA_HT==0)
        {
            Timer_cycle_mesure.detach();
            etat_sht11=5;
            Timer_cycle_mesure.attach_us(callback(this, &SHT11::gestion_HT), 1);
        }
        break;

        case 5:
        Timer_cycle_mesure.detach();
        reception_mesure_H();
        calcul_Temperature_Humidite();
        etat_sht11=0;
        Timer_cycle_mesure.attach(callback(this, &SHT11::gestion_HT), Temps_cycle_mesure);
        break;

        default:
        etat_sht11=0;
        break;
    }
}

float SHT11::lire_T(void)
{
    return Temperature_mesuree;
}

float SHT11::lire_H(void)
{
    return Humidite_mesuree;
}

void SHT11::lire_T_H(float *pointeur_Temperature, float *pointeur_Humidite)
{
    *pointeur_Temperature = Temperature_mesuree;
    *pointeur_Humidite = Humidite_mesuree;
}
