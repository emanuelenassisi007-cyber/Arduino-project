#include <WiFi.h>
#include <WebServer.h>
#include <HardwareSerial.h>
#include "DFRobotDFPlayerMini.h"
#include <SinricPro.h>
#include <SinricProSwitch.h>

// ---------------- CONFIG ----------------
#define WIFI_SSID "TIM-33669964"
#define WIFI_PASS "G9hYRfKCKGKNcx4RXKc5ySf4"

#define API_KEY "a2c1a360-c0a1-4143-85da-6085738672bc"
#define APP_SECRET "aca2e15e-ac08-4ec0-9759-83d1f7d15711-2a251292-c84e-413e-a7bd-b878435c72f9"

// ---------------- DISPOSITIVI ----------------
#define RELAY_LUCI    25
#define RELAY_POMPA  26
#define PIN_VOL_UP   27
#define PIN_VOL_DOWN 14
#define PIN_NEXT     33
#define PIN_PREV     32
#define PIN_LUCI_BTN 21
#define PIN_POMPA_BTN 22

// ---------------- SINRIC ----------------
const char* dev_luci   = "68dac2af359ccc32ce09173d";
const char* dev_pompa  = "68dac301359ccc32ce0917a2";
const char* dev_musica = "68dac8f15918d860c09d919e";

bool statoLuci = false;
bool statoPompa = false;


// ---------------- MUSICA ----------------
String titoli[] = {
  "A Natale puoi","All I Want for Christmas Is You"," È la notte di Natale","Feliz Navidad",
  "Happy Xmas","Holly Jolly Christmas","It's Beginning to Look a Lot Like Christmas","Jingle Bell Rock",
  "Merry Christmas","Santa Claus Is Coming To Town","Snowman",
  "Un pacco per te","Tu Scendi Dalle Stelle"
};
int traccia = 0;
int volume = 15;

// ---------------- DFPLAYER ----------------
HardwareSerial mySerial(2);
DFRobotDFPlayerMini player;
bool df_ok = false;
bool df_init_done = false;

// ---------------- WEB ----------------
WebServer server(80);

// ---------------- DEBOUNCE ----------------
unsigned long lastPressVolUp=0, lastPressVolDown=0, lastPressNext=0;
unsigned long lastPressPrev=0, lastPressLuci=0, lastPressPompa=0;
const unsigned long debounceTime = 250;

// ---------------- HTML ----------------
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="it">
<head>
<meta charset="UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1.0" />
<title>Progetto ESP32</title>

<style>
* {margin:0;padding:0;box-sizing:border-box;}
body {
  font-family: system-ui,-apple-system,BlinkMacSystemFont,"Segoe UI",Roboto;
  background:#f5f5f7;
  display:flex;
  flex-direction:column;
  align-items:center;
  padding:30px;
  color:#1d1d1f;
}
h1 {font-size:2rem;font-weight:700;margin-bottom:30px;}

.card {
  width:350px;
  background:white;
  border-radius:20px;
  padding:15px 20px;
  box-shadow:0 4px 15px rgba(0,0,0,0.08);
  margin-bottom:20px;
  display:flex;
  flex-direction:column;
  gap:10px;
}

.numero {
  font-size:20px;
  font-weight:500;
  background:#e5e5ea;
  padding:5px 15px;
  border-radius:10px;
  text-align:center;
}

.title-text {
  background:#e5e5ea;
  border-radius:10px;
  padding:5px 15px;
}

.card-checkbox {
  width:350px;
  background:white;
  padding:15px 20px;
  border-radius:15px;
  box-shadow:0 4px 15px rgba(0,0,0,0.08);
  margin-bottom:20px;
}

.row {
  display:flex;
  justify-content:space-between;
  align-items:center;
}

.switch {
  position:relative;
  width:70px;
  height:29px;
}
.switch input {display:none;}
.slider {
  position:absolute;
  inset:0;
  background:#e5e5ea;
  border-radius:34px;
  cursor:pointer;
  transition:.2s;
}
.slider:before {
  content:"";
  position:absolute;
  height:25px;
  width:39px;
  left:2px;
  top:2px;
  background:white;
  border-radius:40%;
  transition:.3s;
}
input:checked + .slider {background:#34c759;}
input:checked + .slider:before {transform:translateX(26px);}

.linea {
  height:1px;
  background:#918f8f;
  border:none;
  margin:10px 0;
}

.card-buttons {
  width:350px;
  background:white;
  padding:20px;
  border-radius:15px;
  box-shadow:0 4px 15px rgba(0,0,0,0.08);
  display:flex;
  flex-direction:column;
  gap:25px;
}

.music-controls,
.volume-controls {
  display:flex;
  justify-content:center;
  align-items:center;
  gap:20px;
}

.ctrl-btn, .vol-btn {
  width:18px;
  opacity:0.7;
  cursor:pointer;
}
.ctrl-btn:active,
.vol-btn:active {transform:scale(0.9);}

#volume-slider {
  width:220px;
}
</style>
</head>

<body>

<h1>Impostazioni Progetto</h1>

<div class="card">
  <div>Canzone in corso:</div>
  <div class="title-text">
    <marquee id="canzone">Caricamento...</marquee>
  </div>
</div>

<div class="card">
  <div>Volume:</div>
  <div class="numero" id="vol">0</div>
</div>

<div class="card-checkbox">
  <div class="row">
    <div>Cascata:</div>
    <label class="switch">
      <input type="checkbox" id="pompa">
      <span class="slider"></span>
    </label>
  </div>
  <hr class="linea">
  <div class="row">
    <div>Luci:</div>
    <label class="switch">
      <input type="checkbox" id="luci">
      <span class="slider"></span>
    </label>
  </div>
</div>

<div class="card-buttons">
  <div class="music-controls">
    <img src="https://i.postimg.cc/Kzr0TZwp/back.png" class="ctrl-btn" id="prev">
    <img src="https://i.postimg.cc/cHMF3s2P/play.png" class="ctrl-btn" id="play">
    <img src="https://i.postimg.cc/RFQgtMyj/pause.png" class="ctrl-btn" id="pause">
    <img src="https://i.postimg.cc/26F0WzMM/next.png" class="ctrl-btn" id="next">
  </div>

  <div class="volume-controls">
    <img src="https://i.postimg.cc/B6cmFqkr/volume-down.png" class="vol-btn" id="volDown">
    <input type="range" min="0" max="30" id="volume-slider">
    <img src="https://i.postimg.cc/NM7dXgZq/volume-up.png" class="vol-btn" id="volUp">
  </div>
</div>

<script>
async function updateStatus(){
  const r = await fetch('/status');
  const d = await r.json();
  document.getElementById('canzone').textContent = d.canzone;
  document.getElementById('vol').textContent = d.volume;
  document.getElementById('volume-slider').value = d.volume;
}
setInterval(updateStatus,1000);

document.getElementById('play').onclick = () => fetch('/play');
document.getElementById('pause').onclick = () => fetch('/pause');
document.getElementById('next').onclick = () => fetch('/next');
document.getElementById('prev').onclick = () => fetch('/prev');

document.getElementById('volume-slider').onchange = e =>
  fetch('/setVolume?value=' + e.target.value);

document.getElementById('volUp').onclick = () =>
  fetch('/setVolume?value=' + (Number(vol.textContent)+1));

document.getElementById('volDown').onclick = () =>
  fetch('/setVolume?value=' + (Number(vol.textContent)-1));

document.getElementById('luci').onchange = e =>
  fetch(e.target.checked ? '/luciOn' : '/luciOff');

document.getElementById('pompa').onchange = e =>
  fetch(e.target.checked ? '/pompaOn' : '/pompaOff');
</script>

</body>
</html>
)rawliteral";

// ---------------- WEB FUN ----------------
void handleRoot(){ server.send(200,"text/html",htmlPage); }

void handleStatus(){
  String j="{";
  j+="\"volume\":"+String(volume)+",";
  j+="\"canzone\":\""+titoli[traccia]+"\"}";
  server.send(200,"application/json",j);
}

void webPlay(){ if(df_ok) player.play(traccia+1); server.send(200,"text/plain","OK"); }
void webPause(){ if(df_ok) player.pause(); server.send(200,"text/plain","OK"); }
void webNext(){ traccia=(traccia+1)%21; if(df_ok) player.play(traccia+1); server.send(200,"text/plain","OK"); }
void webPrev(){ traccia=(traccia+20)%21; if(df_ok) player.play(traccia+1); server.send(200,"text/plain","OK"); }
void webSetVolume(){
  if(server.hasArg("value")){
    volume=constrain(server.arg("value").toInt(),0,30);
    if(df_ok) player.volume(volume);
  }
  server.send(200,"text/plain","OK");
}
void webLuciOn(){ statoLuci=true; digitalWrite(RELAY_LUCI,LOW); server.send(200,"text/plain","OK"); }
void webLuciOff(){ statoLuci=false; digitalWrite(RELAY_LUCI,HIGH); server.send(200,"text/plain","OK"); }
void webPompaOn(){ statoPompa=true; digitalWrite(RELAY_POMPA,LOW); server.send(200,"text/plain","OK"); }
void webPompaOff(){ statoPompa=false; digitalWrite(RELAY_POMPA,HIGH); server.send(200,"text/plain","OK"); }

// ---------------- WIFI ----------------
void connectWiFi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID,WIFI_PASS);
  while(WiFi.status()!=WL_CONNECTED) delay(300);
  Serial.println("WiFi connesso!");
}

// ---------------- SINRIC ----------------
bool sinricPower(const String &deviceId, bool &state) {
  if(deviceId == dev_luci) {
    digitalWrite(RELAY_LUCI, state ? LOW : HIGH);
  }
  else if(deviceId == dev_pompa) {
    digitalWrite(RELAY_POMPA, state ? LOW : HIGH);
  }
  else if(deviceId == dev_musica && df_ok) {
    state ? player.play(traccia+1) : player.pause();
  }
  return true;
}


// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);
  disableCore0WDT();
  delay(1000);

  // ================= DFPLAYER (PRIMA DI TUTTO) =================
  mySerial.begin(9600, SERIAL_8N1, 16, 17);
  delay(2000); // FONDAMENTALE

  if (player.begin(mySerial)) {
    df_ok = true;
    player.volume(volume);
    player.play(traccia + 1);
    Serial.println("DFPlayer avviato correttamente");
  } else {
    Serial.println("ERRORE: DFPlayer non risponde");
  }

  // ================= RELAY =================
  pinMode(RELAY_LUCI, OUTPUT);
  digitalWrite(RELAY_LUCI, HIGH);

  pinMode(RELAY_POMPA, OUTPUT);
  digitalWrite(RELAY_POMPA, HIGH);

  //==================PULSANTI=================
  pinMode(PIN_VOL_UP,     INPUT_PULLUP);
  pinMode(PIN_VOL_DOWN,   INPUT_PULLUP);
  pinMode(PIN_NEXT,       INPUT_PULLUP);
  pinMode(PIN_PREV,       INPUT_PULLUP);
  pinMode(PIN_LUCI_BTN,   INPUT_PULLUP);
  pinMode(PIN_POMPA_BTN,  INPUT_PULLUP);


  // ================= WIFI =================
  connectWiFi();

  // ================= WEB SERVER =================
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/play", webPlay);
  server.on("/pause", webPause);
  server.on("/next", webNext);
  server.on("/prev", webPrev);
  server.on("/setVolume", webSetVolume);
  server.on("/luciOn", webLuciOn);
  server.on("/luciOff", webLuciOff);
  server.on("/pompaOn", webPompaOn);
  server.on("/pompaOff", webPompaOff);
  server.begin();

  // ================= SINRICPRO (PER ULTIMO) =================
  SinricPro.begin(API_KEY, APP_SECRET);

  SinricProSwitch &luciDevice   = SinricPro[dev_luci];
  SinricProSwitch &pompaDevice  = SinricPro[dev_pompa];
  SinricProSwitch &musicaDevice = SinricPro[dev_musica];

  luciDevice.onPowerState(sinricPower);
  pompaDevice.onPowerState(sinricPower);
  musicaDevice.onPowerState(sinricPower);

  Serial.println("SinricPro avviato correttamente");
  Serial.println("Setup completato");
}


// ---------------- LOOP ----------------
void loop(){
  server.handleClient();
  SinricPro.handle();
  unsigned long now = millis();

  if(df_ok && player.available()){
    if(player.readType()==DFPlayerPlayFinished){
      traccia=(traccia+1)%21;
      player.play(traccia+1);
    }
  }

// -------- VOLUME + --------
if (digitalRead(PIN_VOL_UP) == LOW && now - lastPressVolUp > debounceTime) {
  lastPressVolUp = now;
  if (volume < 30) volume++;\
  if (df_ok) player.volume(volume);
}

// -------- VOLUME - --------
if (digitalRead(PIN_VOL_DOWN) == LOW && now - lastPressVolDown > debounceTime) {
  lastPressVolDown = now;
  if (volume > 0) volume--;
  if (df_ok) player.volume(volume);
}

// -------- NEXT --------
if (digitalRead(PIN_NEXT) == LOW && now - lastPressNext > debounceTime) {
  lastPressNext = now;
  traccia = (traccia + 1) % 21;
  if (df_ok) player.play(traccia + 1);
}

// -------- PREV --------
if (digitalRead(PIN_PREV) == LOW && now - lastPressPrev > debounceTime) {
  lastPressPrev = now;
  traccia = (traccia + 20) % 21;
  if (df_ok) player.play(traccia + 1);
}

// -------- LUCI --------
if (digitalRead(PIN_LUCI_BTN) == LOW && now - lastPressLuci > debounceTime) {
  lastPressLuci = now;
  statoLuci = !statoLuci;
  digitalWrite(RELAY_LUCI, statoLuci ? LOW : HIGH);
}

// -------- POMPA --------
if (digitalRead(PIN_POMPA_BTN) == LOW && now - lastPressPompa > debounceTime) {
  lastPressPompa = now;
  statoPompa = !statoPompa;
  digitalWrite(RELAY_POMPA, statoPompa ? LOW : HIGH);
}

}
