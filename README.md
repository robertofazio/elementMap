##element.Map##
********************

Sterescopic media server tool for video mapping.

elementMap è il primo strumento destinato alla gestione di videoproiezioni stereoscopiche di 3d mapping, sviluppato in C++ usando OpenFrameworks e dedicato a video proiezioni stereoscopiche con tecnologia attiva sydebyside.
os : OSX 10.7.4 
scheda grafica : nVidia min 512MB e GL_STEREO supportate

(C) 2012 Roberto Fazio Studio - www.robertofazio.com
Creative coders : Matteo Cinzio Riva , Eloi Maduell , Mauro Ferrario
versione alpha 0.2.1 - 25/08/2012

MAIN FEATURES:

- Swap left/right frame nelle visualizzazioni stereo
- Preview in formato 16:9 o 4:3
- Fullscreen funzionante sulla finestra di output
- Blending mode su ogni livello (add, subtract, multiply)
- Quad warper and fine warper mesh fino a 17x17 suddivisioni
- salva/carica (sia warp che global settings) di una sola configurazione.
- Controllo audio
- Attualmente gestisce 3 elements , video , immagine e syphon
- Syphon supportato correttamente
- Risouluzione output si adatta correttamente a tutte le risoluzioni del secondo schermo.

KNOWN ISSUES:

- il livello video non ha trasparenza esterna
- SAVE e OPEN PROJECT non funzionano correttamente ( possibilità di aprire e salvare più progetti )
- ofxUI non risponde a modifiche non dirette ( == non si aggiorna la visualizzazione di bottoni e slider)

TO DO LIST:

- Interfaccia e usabilità
- Stereoscopia Attiva sydebyside Bottom/Top e Left/Right
- Possibilità di cambiare la priorità di ogni element
- Tasto per modificare lo ZERO PARALLAX in main preview
- Menù a tendina o drag&drop per inserire i video ??
- Nell'input mode predisporre come metodi di visualizzazione, MONO 2D only left e/o right, ANAGLIPH, TOP BOTTOM, LEFT RIGHT..
- Nell'output mode predisporre come metodi di visualizzazione, LEFT VIEW, RIGHT VIEW, ANAGLIPH, TOP BOTTOM, LEFT RIGHT..
- Nella preview si devono vedere le info dei tasti premuti, va bene dove è ora non modifichiamola.
- Di fianco a Xgrid e YGrid mettere una label con le info della risoluzione della griglia. Facciamo 16x16 o 20x20 
- Una area di testo con le info su : risoluzione video , img di ingresso, aspect ratio, e nome.
- Tasto Crop left/right top/bottom su ogni element
- Usare il primo frame per la preview video. ofxUI.
e poi inserisci quelli che hai scritto nella tua agenda :P
- Splah image appena si apre elementMap per i crediti,immagini etc..

WISH LIST:

- Possibilità di salvare e caricare più configurazioni, molto utile per gestire diversi mapping.
- Implementare la funzione Rotate e Zoom per ogni warp.
- Possibilità di aggiungere n. quad element e possibilità di sceglierne il tipo, img, video , syphon etc
- Maschera per ogni element
- Flipping options, per flippare l'immagine
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
