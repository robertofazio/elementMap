##element.map##
********************

Sterescopic media server tool for video mapping.

elementMap è il primo strumento destinato alla gestione di videoproiezioni stereoscopiche di 3d mapping, sviluppato in OpenFrameworks e dedicato a video proiezioni stereoscopiche con tecnologia attiva sydebyside.
os : OSX 10.7.4 
scheda grafica : nVidia min 512MB e GL_STEREO supportate
versione alpha 0.1.8 - 19/08/2012

MAIN FEATURES:

- Slider opacità sull'output finale; slideor opacità e RGB su ogni element
- Modalità stereoscopica anaglifo funzionante
- Blending mode attivi su ogni livello (add, subtract, multiply, alpha, screen) e selezionabili da interfaccia
- Quad warper and fine warper mesh fino a 17x17 suddivisioni
- salva/carica di una sola configurazione.
- aggiunto un bordo rosso per evidenziare il livello selezionato
- l'opacità è tornata in funzione, e ora è gestita dal warper (che ha preso alcune funzioni del mixer..)
- frame by frame ripristinato correttamente: ',' e '.' per avanzare/retrocedere di un frame nel video
- Controllo audio
- Attualmente gestisce 3 elements , video , immagine e syphon
- Possibilità di salvare e caricare una sola configurazione 
- Controllo opacità e visibilità per ogni element

KNOWN ISSUES:

- il livello video non ha trasparenza esterna
- Quando si salva con il tasto 's' si dovrebbero salvare tutte le impostazioni, di opacity, visibility,volume etc.. e non solo il warp

TO DO LIST:

- Possibilità di inserire la risoluzione prima che si apri l'app.
- Implementare la funzione Rotate e Zoom per ogni warp.
- Progettazione di un'area inspector
- Possibilità di aggiungere n. quad element e possibilità di sceglierne il tipo, img, video , syphon etc
- Possibilità di cambiare la priorità di ogni element
- possibilità di settare la preview da 4/3 a 16/9 per una questione di eleganza. Non output esterna ma preview!
- Possibilità di salvare e caricare più configurazioni, molto utile per gestire diversi mapping.
- Flipping options, per flippare l'immagine
- Maschera per ogni element
- Interfaccia
- Stereoscopia Attiva sydebyside Bottom/Top e Left/Right
- Edge Blending
- Multiproiezioni
- Export as SVG / PNG / PDF - MAD MAPPER
- Spacial Scanner to turn your video-projector into a 2d scanner - MAD MAPPER

===========

ISTRUZIONI:

- Nel file "outputRes.xml" settare larghezza e altezza del secondo monitor o proiettore. 
- Aprite elementMap;
- La seconda finestra dovrebbe già trovarsi nel secondo monitor;
- Da preferenze di sistema disattivare 'duplica monitor' e selezionare 'rileva monitor' dopodichè portare la seconda finestra 'output' nel secondo monitor ( videoproiettore )
- Premere 'f' per andare in fullscreen
- Verrà visualizzata un'immagine testPattern, un monoscopio che ha lo scopo di aiutare l'utente per effettuare una calibrazione colore del videoproiettore e la messa in asse.
- Disattivare testPatter , verranno così visualizzati i vari element.
- warp per ogni livello; nella finestra di output si seleziona il livello con i tasti '1' '2' e '3' ('0' per deselezionare), poi i comandi sono i soliti;
- Cliccare sulla finestra principale e premere 'f' per mandare in fullscreen la seconda finestra. Una volta in fullscreen la seconda finestra non tornerà più in modalità normale.
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
