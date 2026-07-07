# Smart Christmas Crib with ESP32

Un presepe natalizio intelligente realizzato con ESP32, in grado di controllare illuminazione, musica e cascata sia localmente che da remoto.

Il sistema può essere gestito tramite:

- pulsanti fisici;
- interfaccia web sviluppata con HTML, CSS e JavaScript;
- applicazione mobile realizzata in Flutter;
- assistente vocale Alexa.

## Obiettivo
L'obiettivo del progetto è integrare elettronica, programmazione e domotica in un unico sistema semplice da utilizzare.

## Funzionalità
- Accensione e spegnimento delle luci del presepe.
- Controllo della pompa dell'acqua per la cascata.
- Riproduzione di musica tramite scheda SD.
- Controllo locale tramite pulsanti fisici.
- Controllo remoto tramite rete Wi-Fi.
- Integrazione con Alexa per i comandi vocali.

## Materiale utilizzato
### Hardware
- ESP32
- DFPlayer Mini
- Amplificatore PAM8406
- Moduli relè
- LED e stringa luminosa
- Mini pompa ad acqua
- Altoparlanti 3W 8Ω
- Pulsanti
- Breadboard
- Resistenze
- Fusibili di protezione
- Alimentatore 5V USB-C
### Software
- Arduino IDE
- C++
- HTML
- CSS
- JavaScript
- Flutter

## Collegamenti
collegare l'alimentatore alla breadboard.
pin VCC e GND di dfplayer, amplificatore, rele e esp32 all'alimentazione su breadboard.
pin BUSY dfplayer al GND (in caso non dovesse funzionare il GND da solo)
pin DAC_R e DAC_L ai pin RIN e LIN dell'amplificatore
pin ROUT+, ROUT- dell'amplificatore vanno collegati all'altoparlante destro (- nero e + rosso)
pin LOUT+, LOUT- dell'amplificatore vanno collegati all'altoparlante sinistro (- nero e + rosso) 
pin RX dfplayer al pin TX2 esp32
pin TX dfplayer al pin RX2 esp32 con una resistenza in mezzo da 1kohm
i pin D21, D22, D32, D33, D27 e D14 vanno collegati ai positivi dei 6 pulsanti fisici. i negativi dei pulsanti vanno al GND comune della breadboard.
i pin D26 e D25 vanno all'ingresso IN dei rele. VCC e GND di questi ultimi vanno collegati all'alimentazione comune.
l'altro lato dei rele va collegato ai tre fili della pompa e delle luci rispettando gli ingressi inserendo in serie un fusibile da 1A in un portafusibile per la sicurezza.
Caricare infine il codice Arduino sull'esp32 e verificare il corretto funzionamento di tutto.

## Come funziona
La scheda SD contiene le canzoni in formato MP3, il dfplayer legge queste canzoni e le riproduce mandando il segnale audio all'amplificatore e quindi ilsuono esce tramite gli artoparlanti. per controllare tramite wifi, l'esp32 riceve i comandi di input dall'app, dai pulsanti fisici, dal sito web o dall'assistente vocale e li trasmette al dfplayer per la musica o controlla i rele per accendere o spegnere le luci o la pompa. stessa cosa tramite i pulsanti fisici dove il sergnale passa prima dall'esp32 e poi viene trasmesso ai componenti opportuni.
## Foto

![Circuito](Images/circuito.jpg)

![Montaggio](Images/montaggio.jpg)

## Codice

Il codice si trova nella cartella Arduino.

## Video

Video dimostrativo nella cartella Videos.

## Autore

Emanuele
