# IOT_Projet_Alarme
Projet d'IOT IESE5, création d'une alarme LoRaWan.

## Introduction
Dans le cadre du module Internet Of Thing (IOT) nous avons du réaliser un projet se basant sur une connexion LoRaWAN pour la sécurité des biens et des personnes.

## Organisation du projet

### Architecture matérielle
définir l’architecture matérielle de l’objet,

### Coût de BOM du produit
estimer le coût de la BOM de votre produit (composants, PCB et enclosure) pour 5000 unités produites.
    
    La nomenclature, ou Bill of Materials (BOM) en anglais, est un document détaillant tous les éléments indispensables pour mener à bien un processus de production. En règle générale, la BOM intervient aux étapes de conception, de production et de montage d'un produit. 

    Pour calculer le coût de la BOM de notre produit, nous avons besoin de lister tous les éléments composants notre produit. Pour la création d'un produit, nous avons besoin:
    - 1 carte LoRa-E5 en kit contenant une antenne, une batterie de 3.3V pour 21.90 dollars
    - 1 capteur DHT22 (température et humidité) pour 7.60 dollars
    - 1 capteur de flamme pour 3.63 dollars
    - 1 buzzer pour 1.50 dollars
    - 1 bouton pour 1.50 dollars
    - 10*1 pins pour 10 * 0.0848 dollars
    
    On a donc un coût totale de 36.978 dollars par kit d'alarme.
    Cela représente donc un coût de 184 890 dollars pour 5000 alarmes.
    
    On pourrait ajouter le coût de la boite en bois.
    
    Kit LoRa-E5 : https://www.seeedstudio.com/LoRa-E5-Dev-Kit-p-4868.html
    DHT22 : https://www.seeedstudio.com/Grove-Temperature-Humidity-Sensor-Pro-AM2302-DHT22.html
    Flame sensor : https://fr.aliexpress.com/item/2041897963.html?gatewayAdapt=glo2fra&gatewayAdapt=glo2fra
    Buzzer : https://www.seeedstudio.com/Grove-Buzzer.html
    Bouton : https://www.seeedstudio.com/buttons-c-928/Grove-Button.html
    Pins : https://www.seeedstudio.com/1-Pin-Female-Male-Jumper-Wire-125mm-50pcs-pac-p-1319.html


### Coût des certifications
estimer le coût de certification ETSI du produit, le coût de certification LoRa Alliance du produit ...
https://www.etsi.org
https://lora-alliance.org/lorawan-certification/

### Logiciel embarqué de l'objet sirène
définir le logiciel embarqué de l’objet sirène.

### Implémentation du logiciel embarqué
proposer une implémentation du logiciel embarqué de l’objet défini.

### Métriques du logiciel embarqué
donner les métriques logiciel du logiciel embarqué (nombre de lignes de code, taille du binaire du firmware ie le fichier .bin)

### Architecture réseau
définir l’architecture globale du réseau de sirènes d’alarme,

### Sécurité globale (clé de chiffrage)
définir la sécurité globale (clé de chiffrage),

### Format LPP des messages
définir le format LPP des messages LoRaWAN uplink et downlink,

### Changement de comportement de l'objet
montrer les changements de comportement de l’objet en fonction des événements (normal, incident détecté, retour à la normal).
    Les messages downlink en provenance du récepteur seront émulés par des messages downlink émis par le network serveur (TTN ou CampusIoT).
    Prévoir de montrer la trace console de l’objet pendant la démonstration.

### Durée de vie de la batterie
estimer la durée de vie de la batterie en classe A, en classe B et en classe C en utilisant un outil comme  https://www.elsys.se/en/battery-life-calculator/ 

Nous avons décidé d'utiliser notre alarme en classe C. Nous voulons que la durée de vie de la batterie soit la plus longue possible. De plus, nous n'avons pas besoin d'envoyer de message à l'alarme, nous souhaitons uniquement envoyer un message à l'alarme lorsque celle-ci est déclanché pour éteindre le buzzer.

### Analyse du cycle de vie du produit
réaliser une analyse (briève) du cycle de vie du produit (ACV)

### Avantage/inconvénients des produits concurrents
rechercher et analyser (avantages/inconvénients sous la forme d’une grille) des produits concurrents
    https://www.aet.fr/produit/avertisseur-vocal-connecte-126-db-radio-lora-xpr901111.html  
    
    SmartVOX® diffuse, avec une très haute qualité sonore, des sons et messages vocaux pré-enregistrés personnalisables

    • Très grande couverture sonore : jusqu'à 126 dB (A) à 1 m (réglable)
    • Communication sans fil longue distance ( jusqu'à 1,2 km)
    • Secouru par batterie intégrée
    • Scénarios d'alerte 100% personnalisables
    • Extrêmement simple à installer et utiliser
    • Disponible avec un feu flash (en option)
    
    https://iqflow.io/products/r602a-lorawan-wireless-siren
    
    Wireless Range Of 10 Km
    LoRaWAN™ Class A Compatible
    Frequency Hopping Spread Spectrum (FHSS)
    Improved Interference Immunity
    Improved Power Management For Longer Battery Life
    Encrypt-RF™ Security (Diffie-Hellman Key Exchange + AES-128 CBC For Sensor Data Messages)
    Battery Life*2:
    ⁻ 5 Years
     (Conditions: Ambient Temperature 25 °C, 20 Triggers/Day, Txpower = 20dBm, LoRa Spreading Factor SF = 10)
    Over-The-Air Updates (Future)
    Third-Party Online Wireless Sensor Monitoring And Notification System To Configure Sensors, View Data And Set Alerts Via SMS Text And Email (Optional)
    Available Third-Party Platform: Actility/ThingPark, TTN, MyDevices/Cayenne
    Prix = 179 dollars

### Solution pour localiser l'objet
définir les solutions utilisables pour localiser l’objet sirène (à des fins d’inventaire)
    
Triangularisation du message reçu par les bornes LoRa.

### Intégrations effectuées
en option, montrer les intégrations effectuées (cayenne, influxdb …)
Affichage des valeurs des capteurs sur cayenne.

## Conclusion











