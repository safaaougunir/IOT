# IOT

Partie Arduino :
On a créé un client MQTT (Publisher) Arduino constitué de deux parties

Partie luminosité :

À partir d’un capteur de luminosité on capte tout changement de la luminosité afin de publier la valeur de cette dernière dans un topic (building1/room1/light).

Partie Contrôle de Brightness :

À partir d’un potentiomètre on peut changer la luminosité des hues

CONFIGURATION 

 La première étape qu’on a fait c’est la configuration de la carte raspberry-pi qui a comme fonction le partage de connexion avec le routeur via un câble Ethernet en utilisant la librairie DNS Mask.

Après on a choisis une architecture MQTT vu qu’elle est adaptée aux projets IOT, donc on a commencé par créer un Broker sur la plateforme CloudMqtt et créer un topic pour chaque objet.

Puis on a créé un client MQTT (Subscriber) sur  une deuxième carte raspberry-Pi  en utilisant une librairie Paho dans python afin de contrôler les ampoules connectées  (Philips Hue) par des requêtes http.

Les ampoules Philips Hue sont connectées avec le bridge qui joue le rôle d’une passerelle entre les ampoules et l’internet. 

