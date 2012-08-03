##element.map##
********************

Sterescopic media server tool for video mapping.

elementMap è il primo strumento destinato alla gestione di videoproiezioni stereoscopiche di 3d mapping, sviluppato in OpenFrameworks e dedicato a video proiezioni stereoscopiche con tecnologia attiva sydebyside.
elementMap attualmente è stato sviluppato per piattaforme basate su OS X 10.7 ma non è escluso lo sviluppo per Windows e Linux.

E' possibile utilizzare più sorgenti esterne Syphon

versione alpha 0.1.4 - 02/08/2012

MAIN NEW FEATURES:

- warp per ogni livello; nella finestra di output si seleziona il livello con i tasti '1' '2' e '3' ('0' per deselezionare), poi i comandi sono i soliti;
- salva/caica funzionante sui singoli livelli;
- aggiunto un bordo rosso per evidenziare il livello selezionato
- l'opacità è tornata in funzione, e ora è gestita dal warper (che ha preso alcune funzioni del mixer..)
- frame by frame ripristinato correttamente: ',' e '.' per avanzare/retrocedere di un frame nel video
- controllo volume funzionante (by Mauro)
- Attualmente gestisce 3 elements , video , immagine e syphon
- Quad warper and fine warper mesh fino a 17x17 suddivisioni
- Possibilità di salvare e caricare una sola configurazione 
- Opacità per ogni element

KNOWN ISSUES:

- il livello video non ha trasparenza esterna
- Syphon problema di risoluzione 
- RISOLTO: bug quando seleziono il quad senza aver premuto W si muove lo stesso
- RISOLTO: se ci sono più quad warp nello stesso punto li muovo entrambi
- Picolo bug sul volume, quando si spegne e si muove lo slider si riattiva
- RISOLTO: In fullscreen ci va solo se si disattivano i quad, se premi 1-2-3 non fa più correttamente in fullscreen. Testato con un solo monitor.

TO DO LIST:

- Progettazione di un'area inspector
- Possibilità di aggiungere n. quad element e possibilità di sceglierne il tipo, img, video , syphon etc
- Implementare la funzione Rotate e Zoom per ogni warp.
- Controlli R,G,B per ogni element
- Opacità generale per andare a nero nel mixer.
- Eliminare la preview di destra per ottimizzare risorse e lasciare solo quella di sinistra.
- possibilità di settare la preview da 4/3 a 16/9 per una questione di eleganza. Non output esterna ma preview!
- Possibilità di salvare e caricare più configurazioni, molto utile per gestire diversi mapping.
- Abilitare Blending Mode, solo 4 , i più classici, normal, add, etc..
- Flipping options, per flippare l'immagine
- Maschera per ogni element
- Inserire uno slider opacità sul mixer finale , è utile in proiezione.
- Interfaccia
- Stereoscopia Anaglifo
- Stereoscopia Attiva sydebyside Bottom/Top e Left/Right
- Edge Blending
- Multiproiezioni
- Export as SVG / PNG / PDF - MAD MAPPER
- Spacial Scanner to turn your video-projector into a 2d scanner - MAD MAPPER

===========


updated on 19/07/2012

- aggiunta seconda finestra!

ISTRUZIONI:

- Nel file "outputRes.xml" settare larghezza e altezza del secondo monitor o proiettore. 
- Fare partire l'applicazione;
- La seconda finestra dovrebbe già trovarsi nel secondo monitor;
- Cliccare sulla finestra principale e premere 'f' per mandare in fullscreen la seconda finestra. Una volta in fullscreen la seconda finestra non tornerà più in modalità normale.
- Premere 'f' sulla prima finestra per abilitare/disabilitare la GUI nella prima finestra;
- I tasti per modificare il warp funzionano solo con il mouse sulla seconda finestra;
- La barra per play/pausa funziona nella prima finestra;
- Il tasto 'i' per l'help funziona nella prima finestra;

updated on 11/07/2012

- fullscreen funzionante (a discapito di qualche fps...)
- interfaccia player video: play, pause e rewind
- aggiunto controllo frame by frame 
- in fullscreen: barra spazio=play/pause e backspace=rewind
- ordine degli element (dal primo piano allo sfondo): syphon, video, image
- salvataggio progetto (cioè dati del warp) su xml
- selezione multipla punti della griglia
- spostamento veloce o lento con le frecce
- elementWarp adesso è una classe applicabile a ogni livello indipendentemente
 
