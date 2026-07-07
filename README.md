# Progetto elettronico Arduino

Un progetto elettronico usando Arduino per rendere smart un presepe di natale, tramite componenti e programmazione.

## Obiettivo

Controllare da remoto luci, musica e cascata in un presepe, utilizzando assistente vocale come Alexa, sito web con linguaggi HTML, CSS e JavaScript ed app mobile in Flutter. 

## Materiale utilizzato

- Modulo WiFi e Bluethoot ESP32 con ingresso type C
- DFPlayer Mini
- Amplificatore PAM8406
- 2 moduli relè per Arduino.
- Resistenze
- LED RGB
- Altoparlanti 3W 8Ω
- 2 Breadboard
- Cavi jumper (maschio-maschio, femmina-femmina, maschio-femmina)
- Mini pompa per acqua per la cascata.
- Stringa luci LED
- 6 bottoni ON/OFF.
- Fusibili 1A
- Portafusibili
- Pinza spellacavi
- Multimetro
- Connettori a leva per fili
- Scatola di derivazione per alloggiare il circuito.
- Un paio di metri di cavo rosso e nero per allungamenti.
- Alimentatore 5V con cavo usb type C

## Collegamenti
collegare l'alimentatore alla breadboard.
pin VCC e GND di dfplayer, amplificatore, rele e esp32 all'alimentazione su breadboard.
pin BUSY dfplayer al GND (in caso non dovesse funzionare il GND da solo)
pin DAC_R e DAC_L ai pin RIN e LIN dell'amplificatore
pin ROUT+, ROUT- dell'amplificatore vanno collegati all'altoparlante destro (- nero e + rosso)
pin LOUT+, LOUT- dell'amplificatore vanno collegati all'altoparlante sinistro (- nero e + rosso) 
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
