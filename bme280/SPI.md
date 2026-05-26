# SPI (Serial Peripheral Interface)

> Inspiré du tutoriel SparkFun SPI et enrichi avec des notes pratiques pour ESP32 / Arduino / STM32.
>
> Source principale : [SparkFun SPI Tutorial](https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi/all?utm_source=chatgpt.com)

---

# 1. Qu’est-ce que le SPI ?

Le **SPI** ( *Serial Peripheral Interface* ) est un protocole de communication série synchrone très utilisé en électronique embarquée.

Il sert à faire communiquer :

* microcontrôleurs
* capteurs
* écrans TFT/OLED
* cartes SD
* mémoires Flash
* convertisseurs ADC/DAC
* shift registers

Le SPI est :

* **rapide**
* **simple**
* **full duplex** (émission et réception simultanées)

---

# 2. Principe général

Contrairement à l’UART, le SPI utilise une  **horloge partagée** .

Le contrôleur génère un signal d’horloge :

* chaque impulsion = 1 bit transféré

Cela permet :

* des vitesses très élevées
* moins d’erreurs de synchronisation

---

# 3. Les lignes SPI

## Signaux principaux

| Signal    | Nouveau nom  | Rôle                                  |
| --------- | ------------ | -------------------------------------- |
| SCK / CLK | —           | Horloge SPI                            |
| MOSI      | PICO / COPI  | Données contrôleur → périphérique |
| MISO      | POCI / CIPO  | Données périphérique → contrôleur |
| CS / SS   | Slave Select | Sélection du périphérique           |

---

## Explication rapide

### SCK

**Serial ClocK**

Horloge générée par le contrôleur.

Le périphérique lit les données selon :

* front montant
* ou front descendant

---

### MOSI

**Master Out Slave In**

Données envoyées vers le périphérique.

Exemple :

* commandes écran TFT
* écriture mémoire Flash

---

### MISO

**Master In Slave Out**

Données retournées au contrôleur.

Exemple :

* lecture capteur
* lecture carte SD

---

### CS / SS

Permet de choisir le périphérique actif.

Actif en général à l’état bas :

<pre class="overflow-visible! px-0!" data-start="1697" data-end="1773"><div class="relative w-full mt-4 mb-1"><div class=""><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-token-bg-elevated-secondary corner-superellipse/1.1 overflow-clip rounded-3xl lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>CS = LOW  -> périphérique actif</span><br/><span>CS = HIGH -> périphérique ignoré</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></pre>

---

# 4. Exemple de connexion

<pre class="overflow-visible! px-0!" data-start="1807" data-end="2044"><div class="relative w-full mt-4 mb-1"><div class=""><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-token-bg-elevated-secondary corner-superellipse/1.1 overflow-clip rounded-3xl lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>ESP32                 Capteur SPI</span><br/><span>------                ------------</span><br/><span>SCK   --------------> SCK</span><br/><span>MOSI  --------------> MOSI</span><br/><span>MISO  <-------------- MISO</span><br/><span>CS    --------------> CS</span><br/><span>GND   --------------> GND</span><br/><span>3.3V  --------------> VCC</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></pre>

---

# 5. Fonctionnement d’un transfert

Exemple :

Le contrôleur veut envoyer :

<pre class="overflow-visible! px-0!" data-start="2128" data-end="2150"><div class="relative w-full mt-4 mb-1"><div class=""><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-token-bg-elevated-secondary corner-superellipse/1.1 overflow-clip rounded-3xl lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>0b10110010</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></pre>

Processus :

1. CS passe LOW
2. le contrôleur génère les clocks
3. les bits sortent sur MOSI
4. le périphérique peut répondre sur MISO
5. CS repasse HIGH

---

# 6. SPI = Full Duplex

Pendant qu’un bit est envoyé :

* un autre peut être reçu simultanément

Donc :

<pre class="overflow-visible! px-0!" data-start="2417" data-end="2451"><div class="relative w-full mt-4 mb-1"><div class=""><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-token-bg-elevated-secondary corner-superellipse/1.1 overflow-clip rounded-3xl lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>TX et RX en même temps</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></pre>

C’est une différence importante avec l’I2C.

---

# 7. Modes SPI (CPOL / CPHA)

Le SPI possède 4 modes.

Ils définissent :

* niveau de repos de l’horloge
* front utilisé pour lire les données

| Mode | CPOL | CPHA |
| ---- | ---- | ---- |
| 0    | 0    | 0    |
| 1    | 0    | 1    |
| 2    | 1    | 0    |
| 3    | 1    | 1    |

---

## En pratique

Le périphérique impose le mode.

Toujours vérifier le datasheet.

Exemple Arduino :

<pre class="overflow-visible! px-0!" data-start="2847" data-end="2928"><div class="relative w-full mt-4 mb-1"><div class=""><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-token-bg-elevated-secondary corner-superellipse/1.1 overflow-clip rounded-3xl lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼ11">SPI</span><span>.beginTransaction(</span><br/><span></span><span class="ͼ11">SPISettings</span><span>(</span><span class="ͼy">1000000</span><span>, </span><span class="ͼ11">MSBFIRST</span><span>, </span><span class="ͼ11">SPI_MODE0</span><span>)</span><br/><span>);</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></pre>

---

# 8. Vitesse SPI

Le SPI peut être très rapide :

| MCU         | Vitesse typique           |
| ----------- | ------------------------- |
| Arduino Uno | ~8 MHz                    |
| ESP32       | 40 MHz+                   |
| STM32       | plusieurs dizaines de MHz |

---

## Attention

Plus la vitesse augmente :

* plus les câbles doivent être courts
* plus le routage PCB devient important

SPI est surtout conçu pour :

<pre class="overflow-visible! px-0!" data-start="3261" data-end="3288"><div class="relative w-full mt-4 mb-1"><div class=""><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-token-bg-elevated-secondary corner-superellipse/1.1 overflow-clip rounded-3xl lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>courte distance</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></pre>

---

# 9. Plusieurs périphériques SPI

Le bus peut être partagé.

Tous les périphériques partagent :

* SCK
* MOSI
* MISO

Mais chacun possède son propre :

* CS

---

## Exemple

<pre class="overflow-visible! px-0!" data-start="3470" data-end="3559"><div class="relative w-full mt-4 mb-1"><div class=""><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-token-bg-elevated-secondary corner-superellipse/1.1 overflow-clip rounded-3xl lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>ESP32</span><br/><span> ├── écran TFT   -> CS1</span><br/><span> ├── carte SD    -> CS2</span><br/><span> └── capteur IMU -> CS3</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></pre>

Un seul CS doit être actif à la fois.

---

# 10. Daisy Chain SPI

Certains périphériques peuvent être chaînés.

Exemple :

* shift registers
* drivers LED

<pre class="overflow-visible! px-0!" data-start="3718" data-end="3760"><div class="relative w-full mt-4 mb-1"><div class=""><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-token-bg-elevated-secondary corner-superellipse/1.1 overflow-clip rounded-3xl lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>MCU -> Chip1 -> Chip2 -> Chip3</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></pre>

Pratique pour :

* matrices LED
* longues chaînes de sorties

---

# 11. Hardware SPI vs Software SPI

## Hardware SPI

Utilise le périphérique SPI intégré au MCU.

### Avantages

* très rapide
* faible charge CPU
* fiable

### Inconvénients

* pins souvent imposés

---

## Software SPI (Bit Banging)

Le code manipule les GPIO manuellement.

### Avantages

* fonctionne sur presque n’importe quelles pins

### Inconvénients

* beaucoup plus lent
* consomme du CPU

---

# 12. SPI sur ESP32

## Pins SPI par défaut

### VSPI

| Signal | GPIO |
| ------ | ---- |
| SCK    | 18   |
| MISO   | 19   |
| MOSI   | 23   |
| CS     | 5    |

---

### HSPI

| Signal | GPIO |
| ------ | ---- |
| SCK    | 14   |
| MISO   | 12   |
| MOSI   | 13   |
| CS     | 15   |

---

## Particularités ESP32

### GPIO 34–39

* input only
* OK pour MISO
* impossible pour MOSI/SCK

---

### GPIO 6–11

Réservés à la mémoire Flash interne.

À éviter totalement.

---

# 13. Exemple Arduino / ESP32

<pre class="overflow-visible! px-0!" data-start="4685" data-end="4869"><div class="relative w-full mt-4 mb-1"><div class=""><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-token-bg-elevated-secondary corner-superellipse/1.1 overflow-clip rounded-3xl lxnfua_clipPathFallback"><div class="pointer-events-none absolute inset-x-4 top-12 bottom-4"><div class="pointer-events-none sticky z-40 shrink-0 z-1!"><div class="sticky bg-token-border-light"></div></div></div><div class="relative"><div class=""><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span class="ͼu">#include</span><span></span><span class="ͼz"><SPI.h></span><br/><br/><span class="ͼ11">void</span><span></span><span class="ͼ11">setup</span><span>() {</span><br/><span></span><span class="ͼ11">SPI</span><span>.begin();</span><br/><br/><span></span><span class="ͼ11">pinMode</span><span>(</span><span class="ͼy">5</span><span>, </span><span class="ͼ11">OUTPUT</span><span>);</span><br/><br/><span></span><span class="ͼ11">digitalWrite</span><span>(</span><span class="ͼy">5</span><span>, </span><span class="ͼ11">LOW</span><span>);</span><br/><br/><span></span><span class="ͼ11">SPI</span><span>.transfer(</span><span class="ͼy">0x42</span><span>);</span><br/><br/><span></span><span class="ͼ11">digitalWrite</span><span>(</span><span class="ͼy">5</span><span>, </span><span class="ͼ11">HIGH</span><span>);</span><br/><span>}</span><br/><br/><span class="ͼ11">void</span><span></span><span class="ͼ11">loop</span><span>() {</span><br/><span>}</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></pre>

---

# 14. Avantages du SPI

| Avantage     | Détail                                         |
| ------------ | ----------------------------------------------- |
| Très rapide | souvent bien plus rapide que l’I2C             |
| Simple       | protocole léger                                |
| Full duplex  | émission/réception simultanées               |
| Flexible     | compatible avec énormément de périphériques |

---

# 15. Inconvénients du SPI

| Inconvénient               | Détail                                |
| --------------------------- | -------------------------------------- |
| Beaucoup de fils            | surtout avec plusieurs périphériques |
| Pas d’adressage natif      | besoin d’un CS par périphérique     |
| Courte distance             | sensible au bruit à haute vitesse     |
| Pas standardisé totalement | modes et timings variables             |

---

# 16. Différences SPI vs I2C

| SPI             | I2C                 |
| --------------- | ------------------- |
| Très rapide    | Plus lent           |
| Plusieurs fils  | Seulement 2 fils    |
| Pas d’adresses | Adressage intégré |
| Full duplex     | Half duplex         |
| Simple hardware | Plus complexe       |

---

# 17. Débogage SPI

## Outils utiles

### Oscilloscope

Permet de voir :

* clock
* timings
* niveaux

---

### Analyseur logique

Extrêmement utile pour :

* décoder MOSI/MISO
* vérifier les modes SPI
* analyser les trames

Exemple populaire :

* Saleae Logic Analyzer

---

# 18. Erreurs fréquentes

## Mauvais mode SPI

Symptômes :

* données incohérentes
* périphérique silencieux

---

## CS oublié

Le périphérique ne répond pas.

---

## MISO/MOSI inversés

Erreur classique.

---

## Clock trop rapide

Le périphérique n’arrive plus à suivre.

---

## Pins ESP32 invalides

Exemple :

<pre class="overflow-visible! px-0!" data-start="6258" data-end="6323"><div class="relative w-full mt-4 mb-1"><div class=""><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-token-bg-elevated-secondary corner-superellipse/1.1 overflow-clip rounded-3xl lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>GPIO34 comme MOSI -> impossible</span><br/><span>GPIO6-11 -> dangereux</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></pre>

---

# 19. Notes importantes

## Tous les SPI ne sont pas identiques

Chaque périphérique peut imposer :

* mode SPI
* fréquence max
* timing CS
* ordre des bits

Toujours lire :

<pre class="overflow-visible! px-0!" data-start="6505" data-end="6529"><div class="relative w-full mt-4 mb-1"><div class=""><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-token-bg-elevated-secondary corner-superellipse/1.1 overflow-clip rounded-3xl lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>le datasheet</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></pre>

---

# 20. Résumé ultra-court

<pre class="overflow-visible! px-0!" data-start="6562" data-end="6711"><div class="relative w-full mt-4 mb-1"><div class=""><div class="relative"><div class="h-full min-h-0 min-w-0"><div class="h-full min-h-0 min-w-0"><div class="border border-token-border-light border-radius-3xl corner-superellipse/1.1 rounded-3xl"><div class="h-full w-full border-radius-3xl bg-token-bg-elevated-secondary corner-superellipse/1.1 overflow-clip rounded-3xl lxnfua_clipPathFallback"><div class="pointer-events-none absolute end-1.5 top-1 z-2 md:end-2 md:top-1"></div><div class="relative"><div class="pe-11 pt-3"><div class="relative z-0 flex max-w-full"><div id="code-block-viewer" dir="ltr" class="q9tKkq_viewer cm-editor z-10 light:cm-light dark:cm-light flex h-full w-full flex-col items-stretch ͼs ͼ16"><div class="cm-scroller"><pre class="cm-content q9tKkq_readonly m-0"><code><span>SPI = protocole série synchrone rapide</span><br/><br/><span>Lignes :</span><br/><span>- SCK</span><br/><span>- MOSI</span><br/><span>- MISO</span><br/><span>- CS</span><br/><br/><span>Très rapide</span><br/><span>Très utilisé</span><br/><span>Courte distance</span><br/><span>1 CS par périphérique</span></code></pre></div></div></div></div></div></div></div></div></div><div class=""><div class=""></div></div></div></div></div></pre>

---

# Sources

* [SparkFun SPI Tutorial](https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi/all?utm_source=chatgpt.com)
