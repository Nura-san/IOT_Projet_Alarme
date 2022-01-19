# IOT_Projet_Alarme
Projet d'IOT IESE5, création d'une alarme LoRaWan.

# Introduction
Dans le cadre du module Internet Of Thing (IOT) nous avons du réaliser un projet se basant sur une connexion LoRaWAN pour la sécurité des biens et des personnes.

# Organisation du projet

## Architecture matérielle
![image](https://user-images.githubusercontent.com/95240260/149894063-2e02e975-fa58-4515-8b38-b3df7deee33d.png)

Notre alarme est consituée:
- 1 carte LoRa-E5,
- 1 antenne,
- 1 boîte à piles,
- 2 Piles de 1.5V et 2900 mAh,
- 1 capteur DHT22 pour la température et l'humidité,
- 1 capteur flamme,
- 1 buzzer pour le son de l'alarme,
- 1 bouton pour déclancher l'alarme,
- 10 pins pour relier les capteurs à la cartes.

## Coût de BOM du produit 
Pour calculer le coût de la BOM de notre produit, nous avons besoin de lister tous les éléments composants notre produit. Pour la création d'un produit, nous avons besoin:
- 1 carte LoRa-E5 en kit contenant une antenne, une boite pour piles de 3.3V pour 21.90 dollars
- 1 capteur DHT22 (température et humidité) pour 7.60 dollars
- 1 capteur de flamme pour 3.63 dollars
- 1 buzzer pour 1.50 dollars
- 2*1 bouton pour 2 * 1.50 dollars
- 10*1 pins pour 10 * 0.0848 dollars
- 2*1 piles pour 2 * 0.0999 dollars
    
On a donc un coût totale de 38.6778 dollars par kit d'alarme.
Cela représente donc un coût de 193 389 dollars pour 5000 alarmes.
    
On pourrait ajouter le coût de la boite en bois. Avec une découpe laser en bois, on peut imaginer un prix de matières premières de 5 dollars de bois et 5 euros de mains d'oeuvres par boite. Ce qui ajoute 10 dollars par boite et donc 50 000 dollars pour le montant totale.
On arrive donc à un prix de 243 399 dollars pour 5000 alarmes avec leur boites en bois.

Références :
- Kit LoRa-E5 : https://www.seeedstudio.com/LoRa-E5-Dev-Kit-p-4868.html
- DHT22 : https://www.seeedstudio.com/Grove-Temperature-Humidity-Sensor-Pro-AM2302-DHT22.html
- Flame sensor : https://fr.aliexpress.com/item/2041897963.html?gatewayAdapt=glo2fra&gatewayAdapt=glo2fra
- Buzzer : https://www.seeedstudio.com/Grove-Buzzer.html
- Bouton : https://www.seeedstudio.com/buttons-c-928/Grove-Button.html
- Pins : https://www.seeedstudio.com/1-Pin-Female-Male-Jumper-Wire-125mm-50pcs-pac-p-1319.html
- Piles : https://www.manomano.fr/p/emmerich-industrial-lr06-pile-lr6-aa-alcalines-2900-mah-50-pcs-x858911-10016815?model_id=10016815


## Coût des certifications
https://www.etsi.org
https://lora-alliance.org/lorawan-certification/

## Logiciel embarqué de l'objet sirène
![image](https://user-images.githubusercontent.com/95240260/149896280-4887042d-da6d-4000-a3d2-f997cd3d59cb.png)

## Implémentation du logiciel embarqué
Initialisation :
- Delay des threads
- Calibration du capteur flamme
- Calibration du capteur DHT22
- Connexion à cayenne via LoRa
- Définition des GPIOs :
- -Capteurs flamme et DHT22
- -Bouton user_button et panique_button
- -Buzzer et LEDs

Variables globales :
- Variable des données :
- -Températures 
- -Humidité
- -Flamme
- Flags :
- -Alarme
- -LED_mode

Threads :
- fire_sensor
- DHT22_sensor
- alarme
- led
- LoRa

Interruption :
- user_button
- panique_button

Exemple d'utilisation:

Initialisation des composants :
pin_user_button = GPIO_PIN(1,13);
gpio_init_int (pin_user_button, GPIO_IN_PU, GPIO_RISING , user_button_interrupt,(void*)0);

Création des threads :
thread_led_pid = thread_create(thread_led_stack, sizeof(thread_led_stack), THREAD_PRIORITY_MAIN - 1,0, thread_led, NULL, "thread_led");

Fonction d’interruption des boutons :
static void user_button_interrupt(void *arg){
    (void)arg;
    Alarme = 0;
}

## Métriques du logiciel embarqué
Notre code compte environs 400 lignes.
Taille du fichier firmware (.bin) : 31 Mbytes. Ce fichier qui peut être optimisé en modifiant les includes. 
Pour l'instant, pour gérer les capteurs, nous avons includes des gros fichiers avec de nombreuses possibilités qui dans notre cas ne sont pas utiles.

## Architecture réseau
Nous ne possédions qu'une seule carte donc la définition de l'architecture du "réseau" à proprement parlé n'est pas spécifiqie.

## Format LPP des messages
Données envoyé dans le buffer lpp : Total = 80 bits = 10 bytes

cayenne_lpp_add_analog_input(&lpp, 0, data_fire/10);  = 8 + 8 bits = 16 bits
cayenne_lpp_add_temperature(&lpp, 1, (float)data_temp/10); = 8 + 16 bits = 24 bits
cayenne_lpp_add_relative_humidity(&lpp, 2, (float)data_hum/10); = 8 + 16 bits = 24 bits
cayenne_lpp_add_digital_input(&lpp, 3, Alarme); = 8 + 8 bits = 16 bits               

## Sécurité globale (clé de chiffrage)
Nous n'avons pas eu le temps d'implémenter de chiffrage pour nos données.

## Changement de comportement de l'objet
Notre alarme envoie des données sur cayenne via LoRa toutes les 10 minutes:
- La température,
- Le taux d'humidité,
- La detection d'une flamme ou non,
- L'alarme est activé ou non.

Notre alarme est sensible à plusieurs facteurs:
- Pression sur le panique bouton,
- Detection d'une flamme,
- Température trop élevé ou trop faible,
- Changement trop brutale de température,
- Humidité trop élevé ou trop faible,
- Changement trop brutale de l'humidié.

Lorsque l'alarme est activé par l'un des facteurs précédent, celle-ci envoie des messages LoRa pour indiquer que l'alarme vient de s'activé.
Pour déactiver l'alarme, il faut appuyer sur le bouton de la carte LoRa pour indiquer que tout est revenue dans l'ordre.

Logs: 
![image](https://user-images.githubusercontent.com/95240260/149896839-87c61edb-2d7a-4073-a1b7-aa62b8a8c65f.png)
![image](https://user-images.githubusercontent.com/95240260/149896902-519fdd2b-c3c6-4851-88ba-74e3b45db3a6.png)

## Durée de vie de la batterie
Spreading factor SP7: 
![image](https://user-images.githubusercontent.com/95240260/149814342-742e4284-521d-4476-b4bf-93b66ca6966c.png)

Spreading factor SP9:
![image](https://user-images.githubusercontent.com/95240260/149814483-8882b934-26a7-4058-b4d6-fffba9ab2c40.png)

Spreading factor SP12:
![image](https://user-images.githubusercontent.com/95240260/149814533-9eed5746-0aa0-415e-9479-9f78574d555e.png)

Nous avons décidé d'utiliser notre alarme en classe C. Nous voulons que la durée de vie de la batterie soit la plus longue possible. De plus, nous n'avons pas besoin d'envoyer de message à l'alarme, nous souhaitons uniquement envoyer un message à l'alarme lorsque celle-ci est déclanché pour éteindre le buzzer.
Avec cette classe, nous pouvons donc atteindre une durée de vie de plus de 8 ans ce qui est acceptable pour notre alarme.

## Analyse du cycle de vie du produit
Naissance:
- Extraction des matières première => Polution des sols + coût énergétique de l'extraction
- Fabrication: transformation, assemblage => Coût en énergie + Main d'oeuvre
- Transport: Fabrication des pièces en chine => Carburants

Vie:
- Utilisation => Changement des piles + coût énergétique des gateways pour la communication

Mort:
- Fin de vie => Recyclage de certain matériaux

## Avantage/inconvénients des produits concurrents
Notre objet:
![image](https://user-images.githubusercontent.com/95240260/149820720-83e8b41f-1e96-473f-8058-fa0a66408d6a.png)
![image](https://user-images.githubusercontent.com/95240260/149820348-a62c2ff8-4323-4860-a4b0-fd936ff70066.png)

SmartVOX:
![RAC-SMARTVOX-1--pi-hi500-li500](https://user-images.githubusercontent.com/95240260/149818151-417a6de1-8d8c-418b-9250-1f97fe0cd0fa.png)
![image](https://user-images.githubusercontent.com/95240260/149820369-8fb98e45-8031-4319-ad96-2e16c0033ad0.png)

R602A:
![image](https://user-images.githubusercontent.com/95240260/149818785-3375fda7-298e-4d37-b281-52c862b51168.png)
![image](https://user-images.githubusercontent.com/95240260/149820307-9dff00b6-dae0-4c9b-9dce-ff02e667177f.png)

MClimate:
![image](https://user-images.githubusercontent.com/95240260/149819220-10144dd1-df6f-475f-9864-91b3df395062.png)
![image](https://user-images.githubusercontent.com/95240260/149820392-1d6723fc-307d-4060-bbdc-b23b4a8431db.png)

Ineo-sense ACS Switch Buzz®:
![image](https://user-images.githubusercontent.com/95240260/149819941-102da9ef-4b10-4fee-80ef-4ad98dc2c36f.png)
![image](https://user-images.githubusercontent.com/95240260/149820421-00a5fa13-d8b7-4ea5-b317-88df4afe99e3.png)

## Solution pour localiser l'objet
Triangularisation du message reçu par les bornes LoRa.

## Intégrations effectuées
Affichage des valeurs des capteurs sur cayenne.
![image](https://user-images.githubusercontent.com/95240260/149893927-6cf6a3f8-5a40-4d06-b73e-20aff2f3b018.png)

# Conclusion

Pour conclure, ce projet a été très enrichissant et nous avons pu apprendre à utiliser RiotOS et la communication LoRa. Nous avons au total travaillé 12h sur place et environ 30h sur notre temps personnel.









