#Nom de l'application
APPLICATION = Projet_IOT_Alarme_Polytech

#Référence de la carte Lora
BOARD ?= lora-e5-dev

#ADC pour capteur de flame/metal
FEATURES_REQUIRED = periph_adc

#FEATURES_REQUIRED = periph_gpio_irq
USEMODULE += periph_gpio_irq

#Capteur DHT22 humidity et temperature
USEMODULE += dht
USEMODULE += fmt

#Capteur de Co2 air quality ccs811
USEMODULE += ccs811

#DO NOT REMOVE, NEED FOR LORA AUTO INIT
USEMODULE += lm75a
USEMODULE += sx126x_stm32wl

# Default LoRa region is Europe and default band is 868MHz
LORA_REGION ?= EU868

#Connection Lora cayenne
USEPKG += semtech-loramac
USEMODULE += auto_init_loramac
USEPKG += cayenne-lpp

#Directory utilisé
RIOTBASE ?= $(CURDIR)/../../RIOT

include $(RIOTBASE)/Makefile.include
