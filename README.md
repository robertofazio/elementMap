##element.Map##
********************

Sterescopic media server tool for video mapping.

elementMap è il primo strumento destinato alla gestione di videoproiezioni stereoscopiche di 3d mapping, sviluppato in C++ usando OpenFrameworks e dedicato a video proiezioni stereoscopiche con differenti modalità di visualizzazione.
Sviluppato principalmente sul videoproiettore Christie CP2220 ( http://www.christiedigital.com/en-us/cinema/cinema-products/digital-cinema-projectors/pages/christie-cp2220-dlp-digital-cinema-projector.aspx )

Piattaforma di sviluppo : MAC // Attuale OS : OSX 10.7.5
Scheda grafica minima : nVidia o ATI min 512MB e OPEN GL_STEREO QUAD BUFFER supportate
fare riferimento a questo link per aggiornare il sistema operativo o il vostro harwdare : http://arek.bdmonkeys.net/GLInfo/GLInfo.html
Richiesto secondo monitor o videoproiettore connesso prima di lanciare elementMap

Attualmente funziona con un video proiettore

(C) 2012 Roberto Fazio Studio - www.robertofazio.com 
Creative coders : Matteo Cinzio Riva , Eloi Maduell , Mauro Ferrario 
versione alpha 0.2.9 - 23/11/2012

MAIN FEATURES:

- Funziona con un video proiettore
- LOAD con menu a tendina per caricare file contenuti nelle cartelle "images" e "movies"
- Selezione INPUT mode: MONO, STEREO_TWO_CHANNLES, STEREO_TOP_BOTTOM, STEREO_LEFT_RIGHT
- Selezione OUTPUT mode: MONO, ANAGLIPH, STEREO_OPENGL, LEFTRIGHT, TOPBOTTOM
- zero-parallax adjustment for stereo vision
- Horizontal e vertical flip; clockwise e counter-clockwise rotation
- QuadBuffer stereoscopic mode funzionante
- Controllo del warp tramite tastiera e con mouse direttamente dalla preview
- Swap left/right frame nelle visualizzazioni stereo
- Preview in formato 16:9 o 4:3
- Fullscreen funzionante sulla finestra di output
- Blending mode su ogni livello (add, subtract, multiply)
- Quad warper and fine warper mesh fino a 16x16 suddivisioni
- salva/carica (sia warp che global settings) di una sola configurazione.
- Controllo audio
- Attualmente gestisce 3 elements , video , immagine e syphon
- Syphon supportato correttamente
- Risouluzione output si adatta correttamente a tutte le risoluzioni del secondo schermo.

KNOWN ISSUES:

- il livello video non ha trasparenza esterna -> impostare BLENDING MODE = ADD
- FRAME RATE GENERALE PERFORMANCE : soprattuto quando carichi 2 video separati non mantiene il sincronismo
- RESET GRID : per visualizzare correttamente un file caricato la sequenza è: LOAD -> INPUT TYPE -> RESET GRID
- FINE WARPING : aumentando e diminuendo la risoluzione della griglia dsi modifica la dimensione del contenuto
- IS ACTIVE BUTTON : quando lo selezioni non mantiene livello di Opacity corretto 
- STEREOSCOPIC BUTTON : non funzionano nei vari element e nella Main Preview
- SFONDO PREVIEW : mettiamo uno sfondo nero quando tutti i livelli sono OFF e la griglia è disattivata
- VOLUME : Bug sul volumene quando lo alzi non mantiene il livello
- TASTO SAVE E OPEN non salvano bene tutte le caratteristiche
- PLAY STOP : buggy bisogno premere STOP più volte per farlo riprendere
- BLEND MODE BUTTON: non deve stare sopra INPUT
- OUTPUT MODE BUTTON : non deve stare sopra i FPS
- SYPHON : bug sulla trasparenza (!) se abiliti la griglia te ne accorgi ( semrba un problema di priorità ) 
- NO MEDIA IMG : inserire NoMedia Img anche sul livello syphon e su 2Channel ( cambiare colore scritta in blu come interfaccia ) 

TO DO LIST:

- Interfaccia e usabilità
- Cambiare il nome Active Stero in OPEN GL
- Possibilità di cambiare la priorità di ogni element
- STEREO MODE PASSIVE Interlacciato orizzontale e verticale per TV Passivi
- Una area di testo con le info su : risoluzione video , img di ingresso, aspect ratio, e nome.
- CROP BUTTON: left/right top/bottom su ogni element
- CONTRAST BUTTON: per ogni element
- ROTATE FINE BUTTON: per ruotare ogni element potremmo usare quello già presente in ofxUI
- ZOOM BUTTON: per zoomare su ogni punto del fine warping
- SCALE BUTTON: importante, manca potrebbe essere con la rotella del mouse(!?) o con le frecce
- MOVE/TRANSLATE BUTTON : si abilita via interfaccia e poi col mouse si muove (!) 
- Usare il primo frame per la preview video. ofxUI.
- MEDIA LIBRARY : si potrebbe mettere sotto una media library e poi fare drag e drop sui vari element (?!)
- Splash image appena si apre elementMap per i crediti,immagini etc..

WISH LIST:

- Possibilità di salvare e caricare più configurazioni, molto utile per gestire diversi mapping.
- Possibilità di aggiungere n. quad element e possibilità di sceglierne il tipo, img, video , syphon etc
- Maschera per ogni element
- MIDI e OSC Controls : mi immagino un tasto che abilita il midi map e che in automatico riconosca i segnali MIDI ( poi ti spiego come dovrebbe funzionare ) 
- Progettazione di una main area inspector
- Edge Blending
- Multiproiezioni
- Export as SVG / PNG / PDF - MAD MAPPER
- Spacial Scanner to turn your video-projector into a 2d scanner - MAD MAPPER

===========

ISTRUZIONI:

- premendo il tasto omonimo verrà visualizzata un'immagine testPattern, un monoscopio che ha lo scopo di aiutare l'utente per effettuare una calibrazione colore del videoproiettore e la messa in asse.
- warp per ogni livello; nella finestra di output si seleziona il livello con i tasti '1' '2' e '3' ('0' per deselezionare), poi i comandi sono i soliti;
- 'w' abilito disabilito la funzione quad warper sull'element selezionato. Col mouse seleziono i vari vertici attivi nel riquadro rosso.
- con 'arrow_key' muovo il vertice selezionato, '<' + 'arrow_key' aumento lo step di movimento.
- 'v' seleziono il vertice, '<' + 'arrow_key' aumento lo step di movimento.
- 'g' visualizzo mesh warper una griglia finao a 17x17 suddivisioni
- 'q' , 'a' aumento/diminuisco risoluzione vertical mesh , controlled by mouse
- 'z' , 'x' aumento/diminuisco risoluzione orizontal mesh , controlled by mouse
- 'h' selezione multipla dei punti
- 'c' clear quad warp
- 'r' resetto mesh point
- MELA 'r' resetto tutti i punti
- 'n' , 'm' next, previous point
- 't' translate element , '<' + 'arrow_key' aumento lo step di movimento.
- 's' salvo configurazione warp
- 'l' carico configurazione
- Premere 'f' sulla prima finestra per abilitare/disabilitare la GUI nella prima finestra;
- I tasti per modificare il warp funzionano solo con il mouse sulla seconda finestra;
- 'return' disabilito gui interfaccia
- La barra per play/pausa funziona nella prima finestra;
- 'backspace' stop 
- ',' '.' framebyframe
- Il tasto 'i' per l'help funziona nella prima finestra;
