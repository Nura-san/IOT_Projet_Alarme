# IOT_Projet_Alarme
Projet d'IOT IESE5, création d'une alarme LoRaWan.

## Introduction
Dans le cadre du module Internet Of Thing (IOT) nous avons du réaliser un projet se basant sur une connexion LoRaWAN pour la sécurité des biens et des personnes.

## Organisation du projet

### Architecture matérielle
- 2 Piles de 1.5V et 2900 mAh 

### Coût de BOM du produit 
Pour calculer le coût de la BOM de notre produit, nous avons besoin de lister tous les éléments composants notre produit. Pour la création d'un produit, nous avons besoin:
- 1 carte LoRa-E5 en kit contenant une antenne, une batterie de 3.3V pour 21.90 dollars
- 1 capteur DHT22 (température et humidité) pour 7.60 dollars
- 1 capteur de flamme pour 3.63 dollars
- 1 buzzer pour 1.50 dollars
- 1 bouton pour 1.50 dollars
- 10*1 pins pour 10 * 0.0848 dollars
- 2*1 piles pour 2 * 0.0999 dollars
    
On a donc un coût totale de 37.1778 dollars par kit d'alarme.
Cela représente donc un coût de 185 889 dollars pour 5000 alarmes.
    
On pourrait ajouter le coût de la boite en bois. Avec une découpe laser en bois, on peut imaginer un prix de matières premières de 5 dollars de bois et 5 euros de mains d'oeuvres par boite. Ce qui ajoute 10 dollars par boite et donc 50 000 dollars pour le montant totale.
On arrive donc à un prix de 235 899 dollars pour 5000 alarmes avec leur boites en bois.

Références :
- Kit LoRa-E5 : https://www.seeedstudio.com/LoRa-E5-Dev-Kit-p-4868.html
- DHT22 : https://www.seeedstudio.com/Grove-Temperature-Humidity-Sensor-Pro-AM2302-DHT22.html
- Flame sensor : https://fr.aliexpress.com/item/2041897963.html?gatewayAdapt=glo2fra&gatewayAdapt=glo2fra
- Buzzer : https://www.seeedstudio.com/Grove-Buzzer.html
- Bouton : https://www.seeedstudio.com/buttons-c-928/Grove-Button.html
- Pins : https://www.seeedstudio.com/1-Pin-Female-Male-Jumper-Wire-125mm-50pcs-pac-p-1319.html
- Piles : https://www.manomano.fr/p/emmerich-industrial-lr06-pile-lr6-aa-alcalines-2900-mah-50-pcs-x858911-10016815?model_id=10016815


### Coût des certifications
estimer le coût de certification ETSI du produit, le coût de certification LoRa Alliance du produit ...
https://www.etsi.org
https://lora-alliance.org/lorawan-certification/

### Logiciel embarqué de l'objet sirène
définir le logiciel embarqué de l’objet sirène.

### Implémentation du logiciel embarqué
![image](https://user-images.githubusercontent.com/95240260/149816411-1660ffd6-fafe-4919-ba3b-6ef624a9c384.png)

### Métriques du logiciel embarqué
donner les métriques logiciel du logiciel embarqué (nombre de lignes de code, taille du binaire du firmware ie le fichier .bin)

Notre code compte environs 400 lignes.

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
Spreading factor SP7: 
![image](https://user-images.githubusercontent.com/95240260/149814342-742e4284-521d-4476-b4bf-93b66ca6966c.png)

Spreading factor SP9:
![image](https://user-images.githubusercontent.com/95240260/149814483-8882b934-26a7-4058-b4d6-fffba9ab2c40.png)

Spreading factor SP12:
![image](https://user-images.githubusercontent.com/95240260/149814533-9eed5746-0aa0-415e-9479-9f78574d555e.png)

Nous avons décidé d'utiliser notre alarme en classe C. Nous voulons que la durée de vie de la batterie soit la plus longue possible. De plus, nous n'avons pas besoin d'envoyer de message à l'alarme, nous souhaitons uniquement envoyer un message à l'alarme lorsque celle-ci est déclanché pour éteindre le buzzer.
Avec cette classe, nous pouvons donc atteindre une durée de vie de plus de 8 ans ce qui est acceptable pour notre alarme.

### Analyse du cycle de vie du produit
Naissance:
- Extraction des matières première => Polution des sols + coût énergétique de l'extraction
- Fabrication: transformation, assemblage => Coût en énergie + Main d'oeuvre
- Transport: Fabrication des pièces en chine => Carburants

Vie:
- Utilisation => Changement des piles + coût énergétique des gateways pour la communication

Mort:
- Fin de vie => Recyclage de certain matériaux

### Avantage/inconvénients des produits concurrents
Notre objet:
![image](https://user-images.githubusercontent.com/95240260/149820348-a62c2ff8-4323-4860-a4b0-fd936ff70066.png)

SmartVOX:
![RAC-SMARTVOX-1--pi-hi500-li500](https://user-images.githubusercontent.com/95240260/149818151-417a6de1-8d8c-418b-9250-1f97fe0cd0fa.png)
![image](https://user-images.githubusercontent.com/95240260/149820369-8fb98e45-8031-4319-ad96-2e16c0033ad0.png)

![image](https://user-images.githubusercontent.com/95240260/149818785-3375fda7-298e-4d37-b281-52c862b51168.png)
![image](https://user-images.githubusercontent.com/95240260/149820307-9dff00b6-dae0-4c9b-9dce-ff02e667177f.png)

![image](https://user-images.githubusercontent.com/95240260/149819220-10144dd1-df6f-475f-9864-91b3df395062.png)
![image](https://user-images.githubusercontent.com/95240260/149820392-1d6723fc-307d-4060-bbdc-b23b4a8431db.png)

![image](https://user-images.githubusercontent.com/95240260/149819941-102da9ef-4b10-4fee-80ef-4ad98dc2c36f.png)
![image](https://user-images.githubusercontent.com/95240260/149820421-00a5fa13-d8b7-4ea5-b317-88df4afe99e3.png)

### Solution pour localiser l'objet
définir les solutions utilisables pour localiser l’objet sirène (à des fins d’inventaire)
    
Triangularisation du message reçu par les bornes LoRa.

### Intégrations effectuées
Affichage des valeurs des capteurs sur cayenne.

## Conclusion











