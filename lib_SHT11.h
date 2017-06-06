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
* @file          lib_SHT11.h 
* @date          Jun 2012
* @author        Yannic Simon
*/

#ifndef DEF_lib_SHT11_H
#define DEF_lib_SHT11_H

#include "mbed.h"

/** SHT11 class
 */
class SHT11 : public Ticker
{
public:
    /** Creer une instance SHT11
    * sur les pins data, sck avec une periode de mesure de minimum 2sec
    *
    * @param pin_DATA par defaut p30
    * @param pin_SCK par defaut p29
    * @param T_C_mesure par defaut 2.0
    */
    SHT11(PinName pin_DATA = p30, PinName pin_SCK = p29, float T_C_mesure = 2.0);

    /** renvoie la derniere temperature mesuree
    *
    * @param aucun
    * @returns la temperature en degres Celsius
    */  
    float lire_T(void);
    
    /** renvoie la derniere humidite mesuree
    *
    * @param aucun
    * @returns l'humidite relative
    */ 
    float lire_H(void);
    
    /** donne les derniere humidite et temperature mesuree
    *
    * @param T variable qui sera actualisee avec la temperature
    * @param H variable qui sera actualisee avec l'humidite
    * @returns aucun
    */ 
    void lire_T_H(float *T, float *H);
    
    /** change la periode de mesure du capteur
    *
    * @param new_Temps_cycle_mesure (minimum 2sec)
    * @returns aucun
    */
    void changer_timer_cycle(float new_Temps_cycle_mesure);
    
    /** initialisation du capteur
    *
    * @param aucun
    * @returns aucun
    */
    void init(void);
    
    /** desactivation de la mesure du capteur
    *
    * @param aucun
    * @returns aucun
    */
    void desactiver(void);
    
private:
    
    void start(void);
    void gestion_HT(void);
    void demande_mesure(short octet_de_commande);
    void mesure_T(void);
    void mesure_H(void);
    void mesure_T_H(void);
    void reception_mesure_T(void);
    void reception_mesure_H(void);
    void calcul_Temperature_Humidite(void);

    DigitalInOut DATA_HT;
    DigitalOut SCK_HT;
    Ticker Timer_cycle_mesure;

    unsigned int Temperature;
    unsigned int Humidite;
    unsigned int etat_sht11;
    unsigned int etat_cycle;
    float Temperature_mesuree;
    float Humidite_mesuree;
    float Temps_cycle_mesure;
};

#endif
