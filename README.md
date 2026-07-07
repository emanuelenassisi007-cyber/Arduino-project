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

## Architettura del sistema

L'ESP32 rappresenta il cuore del progetto.
Riceve i comandi provenienti dai pulsanti fisici oppure tramite la rete Wi-Fi (sito web, applicazione mobile o Alexa) e gestisce i vari dispositivi collegati.
Per la musica comunica con il DFPlayer Mini, che legge i file MP3 presenti sulla scheda SD.
Il segnale audio viene inviato al PAM8406, che pilota i due altoparlanti.
Le luci e la pompa della cascata vengono controllate attraverso i moduli relè.

In questo modo tutte le modalità di controllo utilizzano la stessa logica centralizzata, garantendo un funzionamento coerente del sistema.

## Schema dei collegamenti

I collegamenti principali sono:
- alimentazione comune a ESP32, DFPlayer, amplificatore e relè;
- DFPlayer collegato all'ESP32 tramite UART;
- uscita audio del DFPlayer collegata all'amplificatore PAM8406;
- pulsanti collegati agli ingressi digitali dell'ESP32;
- relè collegati alle uscite digitali dell'ESP32 per il controllo di luci e pompa.

## Immagini
Lo schema completo è riportato nella cartella Images assieme ad altri schemi

## Codice

Il codice si trova nella cartella Arduino.

## Video

Video dimostrativo nella cartella Videos.

## Possibili sviluppi futuri
- Controllo dell'intensità delle luci.
- Programmazione automatica degli orari di accensione.
- Gestione di effetti luminosi personalizzati.
- Monitoraggio dello stato del sistema tramite notifiche.
- Progettazione di un PCB (Printed Circuit Board) personalizzato per sostituire la breadboard, rendendo il circuito più compatto, affidabile e adatto a un     utilizzo permanente. Nella cartella Images è riportato un semplice schema non ultimato di come potrebbe risultare il circuito su PCB.

## Autore

Emanuele
