/*------------------------------Bibliothèques utiles------------------------------*/
#include <stdio.h>
#include <string.h>
//Timer
#include "xtimer.h"
//Thread
#include "thread.h"
#include "msg.h"
//Shell
#include "shell.h"
//Carte utilisée LoRaE5
#include "board.h"
//GPIO de la carte
#include "periph/gpio.h"
//#include "periph_conf.h"
//ADC header
#include "periph/adc.h"
//DHT22 headers (humidty and temperature)
#include "timex.h"
#include "fmt.h"
#include "dht.h"
#include "dht_params.h"
//CSS811 headers (Air quality CO2)
#include "ccs811.h"
#include "ccs811_params.h"
//Connection LoRa
#include "net/loramac.h"
#include "semtech_loramac.h"
//Cayenne header
#include "cayenne_lpp.h"

/*------------------------------Déclaration des Paramètres------------------------------*/ 

//Délais
#define DELAY_FIRE          (60000LU * US_PER_MS) /* 1min */
#define DELAY_DHT           (60000LU * US_PER_MS) /* 1min */
#define DELAY_LED           (30000LU * US_PER_MS) /* 500 ms */
#define DELAY_LED_clignote  (40LU * US_PER_MS) /* 40 ms */
#define DELAY_LoRa          (600000LU * US_PER_MS) /* 10mins*/
#define DELAY_ALARME        (30000LU * US_PER_MS) /* 30s */

//Capteur Fire/métal
#define ADC_ID              0 //PB3 = A3
#define Resolution          ADC_RES_10BIT

    //Valeurs de calibration du sensor
const int sensorMin = 0;    
const int sensor_seuil_distant = 30;
const int sensor_seuil2_close = 600;
int data_fire;

//Capteur DHT22

dht_t dev;
int16_t data_temp, data_hum;
char data_temp_s[10];
char data_hum_s[10];
int ref_temp = 23;
int ref_hum = 50;

//User boutton reset
gpio_t pin_user_button;

//Panique Boutton
gpio_t pin_panique_buton;

//Buzzer
gpio_t pin_buzzer;

//LED
gpio_t pin_LED;




//Capteur CSS811

//Thread utilisées
static char thread_fire_sensor_stack[THREAD_STACKSIZE_MAIN];
static char thread_DHT22_sensor_stack[THREAD_STACKSIZE_MAIN];
static char thread_alarme_stack[THREAD_STACKSIZE_MAIN];
static char thread_led_stack[THREAD_STACKSIZE_MAIN];
static char thread_LoRa_stack[THREAD_STACKSIZE_MAIN];
static kernel_pid_t thread_fire_sensor_pid;
static kernel_pid_t thread_DHT22_sensor_pid;
static kernel_pid_t thread_alarme_pid;
static kernel_pid_t thread_led_pid;
static kernel_pid_t thread_LoRa_pid;

//Variables alarme flag
int Alarme;

//LED flag
int LED_mode;

//Synchronisation temps
xtimer_ticks32_t last;

//loramac descriptor
extern semtech_loramac_t loramac;
//Cayenne LPP descriptor
static cayenne_lpp_t lpp;
//Device and application informations required for OTAA activation
static const uint8_t deveui[LORAMAC_DEVEUI_LEN] = { 0x45, 0x4c, 0xe4, 0xb0, 0x02, 0x0b, 0x90, 0xf5 };
static const uint8_t appeui[LORAMAC_APPEUI_LEN] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const uint8_t appkey[LORAMAC_APPKEY_LEN] = { 0xe0, 0xa9, 0xa2, 0x23, 0xb7, 0x86, 0x2e, 0xf0, 0xa5, 0x75, 0x23, 0x43, 0xae, 0xbc, 0x34, 0xb6 };



/*------------------------------Interruption boutons------------------------------*/ 

static void user_button_interrupt(void *arg){
    (void)arg;
     printf("User_button_pressed\n");
     Alarme = 0;
 }

static void panique_button_interrupt(void *arg){
    (void)arg;
    printf("Panique_button_pressed\n");
    Alarme = 1;
}

/*------------------------------Threads utilisées------------------------------*/ 
static void *thread_fire_sensor(void *arg)
{
    (void)arg;
    printf("Fire_thread_launch\n");
    while (1) {
        data_fire = 820 - adc_sample(ADC_LINE(ADC_ID), Resolution);    
        if (data_fire < 0) {
            data_fire = sensorMin;
        } 

        //printf("ADC_LINE(%u): %i\n", ADC_ID, data_fire);

        if (data_fire > sensor_seuil2_close ){
            //printf("**CLOSE FIRE** \n");
            Alarme = 1;
        }
        else if (data_fire > sensor_seuil_distant){
            //printf("**DISTANT FIRE** \n");
        }
        else {
            //printf("**NO FIRE** \n");
        }
        xtimer_periodic_wakeup(&last, DELAY_FIRE);
    }
    return NULL;
}

static void *thread_DHT22_sensor(void *arg)
{   
    (void)arg;
    printf("DHT22_thread_launch\n");
    int past_data_temp;
    int past_data_hum;
    while (1) {
        if (dht_read(&dev, &data_temp, &data_hum) != DHT_OK) {
            puts("Error reading values");
            continue;
        }

        size_t n = fmt_s16_dfp(data_temp_s, data_temp, -1);
        data_temp_s[n] = '\0';
        n = fmt_s16_dfp(data_hum_s, data_hum, -1);
        data_hum_s[n] = '\0';

        //printf("DHT values - temp: %s%%°C - relative humidity: %s%%\n",data_temp_s, data_hum_s);
        //printf("DHT values - temp: %d,%d°C - relative humidity: %d,%d\n",data_temp/10,data_temp%10, data_hum/10,data_hum%10);

        if (data_temp > ref_temp*10 && data_hum > ref_hum*10 ){
            Alarme = 1;
        }
        else if (data_temp = past_data_temp + 30){
            Alarme = 1;
        }
        else if (data_hum = past_data_hum + 30){
            Alarme = 1;
        }

        past_data_temp = data_temp;
        past_data_hum = data_hum;

        xtimer_periodic_wakeup(&last, DELAY_DHT);

    }
    return NULL;
}

static void *thread_alarme(void *arg)
{
    (void)arg;
    printf("alarme_thread_launch\n");
    while (1) {
        if (Alarme == 1) {
            gpio_write(pin_buzzer,1);
            LED_mode = 1;
            }
        else if (Alarme == 0){ 
            gpio_write(pin_buzzer,0);
            LED_mode = 0;
        }
        printf("Alarme value = %d\n",Alarme);
        xtimer_periodic_wakeup(&last, DELAY_ALARME);
    }
    return NULL;
}

static void *thread_led(void *arg)
{
    (void)arg;
    printf("led_thread_launch\n");
    while (1) {
        if (LED_mode == 0) {       
            gpio_write(pin_LED,1); // 0 = ON et 1 = OFF
            xtimer_periodic_wakeup(&last, DELAY_LED);
            }
        else if (LED_mode == 1){ 
            gpio_write(pin_LED,0);
            xtimer_periodic_wakeup(&last, DELAY_LED);
        }
        else if (LED_mode == 2){ 
            gpio_write(pin_LED,0);
            xtimer_periodic_wakeup(&last, DELAY_LED_clignote);
            gpio_write(pin_LED,1);
            xtimer_periodic_wakeup(&last, DELAY_LED_clignote);
        }
        else {
        }
    }
    return NULL;
}

static void *thread_LoRa(void *arg)
{
    (void)arg;
    printf("LoRas_thread_launch\n");
    while (1) {
        /* Prepare cayenne lpp payload here */
        cayenne_lpp_add_digital_output(&lpp, 0, (uint8_t)data_fire);
        cayenne_lpp_add_temperature(&lpp, 0, (float)data_temp);
        cayenne_lpp_add_relative_humidity(&lpp, 0, (float)data_hum);
        cayenne_lpp_add_analog_output(&lpp, 0, (float)Alarme);

        //printf("Sending LPP data\n");

        /* send the LoRaWAN message */
        uint8_t ret = semtech_loramac_send(&loramac, lpp.buffer, lpp.cursor);
        if (ret != SEMTECH_LORAMAC_TX_DONE) {
            printf("Cannot send lpp message, ret code: %d\n", ret);
        }

        /* Clear buffer once done here */
        cayenne_lpp_reset(&lpp);
        xtimer_periodic_wakeup(&last, DELAY_LoRa);

    }


    return NULL;
}


/*------------------------------Programme Principale------------------------------*/ 

int main(void)
{
    /*----------Initialisation----------*/ 
    printf("Initialisation alarme incendie IOT\n");
    last = xtimer_now(); //Temps  

     /*----------Composants----------*/

    //User bouton PB13 = D0 / PB10 = D10
    pin_user_button = GPIO_PIN(1,13);
    gpio_init_int (pin_user_button, GPIO_IN_PU, GPIO_RISING , user_button_interrupt,(void*)0);

    //PANIQUE Bouton PA9 = D9
    pin_panique_buton = GPIO_PIN(0,9);
    gpio_init_int (pin_panique_buton, GPIO_IN_PU, GPIO_RISING , panique_button_interrupt,(void*)0);
    
    //Buzzer Analog : PB4 = A4 
    pin_buzzer = GPIO_PIN(0,4);
    gpio_init(pin_buzzer,GPIO_OUT); 
    Alarme = 0;

    //LED PB5 = D5
    pin_LED = GPIO_PIN(1,5);
    gpio_init(pin_buzzer,GPIO_OUT);
    LED_mode = 2;

    //ADC PB3 = A3 (voir ADC_ID)
    adc_init(ADC_LINE(ADC_ID));

    //DHT22 PA0 = D0 (voir "dht_params.h") 
    printf("Initializing DHT sensor...\t");
    if (dht_init(&dev, &dht_params[0]) == DHT_OK) {
        puts("[OK]\n");
    }
    else {
        puts("[Failed]");
        return 1;
    }

    //CCS811 connexion I2C : PB15 = SCL, PA15 = SDA 
    /*ccs811_t sensor;
    printf("Initializing CCS811 sensor...\t");*/
    /* initialize the sensor with default configuration parameters */
    /*if (ccs811_init (&sensor, &ccs811_params[0]) != CCS811_OK) {
        puts("Initialization failed :\n");
        return 1;
    }*/

    /*----------LoRa----------*/

    // use a fast datarate so we don't use the physical layer too much 
    semtech_loramac_set_dr(&loramac, 5);
    // set the LoRaWAN keys 
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);
    // start the OTAA join procedure 
    puts("Starting join procedure");
    if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
        puts("Join procedure failed");
        return 1;
    }
    puts("Join procedure succeeded");

    /*------------------------------Création des threads------------------------------*/  
    thread_led_pid = thread_create(thread_led_stack, sizeof(thread_led_stack), THREAD_PRIORITY_MAIN - 1,0, thread_led, NULL, "thread_led");
    xtimer_sleep(3);
    thread_fire_sensor_pid = thread_create(thread_fire_sensor_stack, sizeof(thread_fire_sensor_stack), THREAD_PRIORITY_MAIN - 1,0, thread_fire_sensor, NULL, "thread_fire_sensor");
    xtimer_msleep(20);
    thread_DHT22_sensor_pid = thread_create(thread_DHT22_sensor_stack, sizeof(thread_DHT22_sensor_stack), THREAD_PRIORITY_MAIN - 1,0, thread_DHT22_sensor, NULL, "thread_DHT22_sensor");
    xtimer_msleep(20);
    thread_alarme_pid = thread_create(thread_alarme_stack, sizeof(thread_alarme_stack), THREAD_PRIORITY_MAIN - 1,0, thread_alarme, NULL, "thread_alarme");
    xtimer_msleep(20);
    thread_LoRa_pid = thread_create(thread_LoRa_stack, sizeof(thread_LoRa_stack), THREAD_PRIORITY_MAIN - 1,0, thread_LoRa, NULL, "thread_LoRa");

    /* Envois des données LoRa_data_sender function */
    //LoRa_data_sender();

    while (1) {
        xtimer_msleep(10);
     }

    return 0;
}
