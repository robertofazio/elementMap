element.map
===========

versione alpha 0.1.4 - 02/08/2012

MAIN NEW FEATURES:

- warp per ogni livello; nella finestra di output si seleziona il livello con i tasti '1' '2' e '3' ('0' per deselezionare), poi i comandi sono i soliti;
- salva/caica funzionante sui singoli livelli;
- aggiunto un bordo rosso per evidenziare il livello selezionato
- l'opacità è tornata in funzione, e ora è gestita dal warper (che ha preso alcune funzioni del mixer..)
- frame by frame ripristinato correttamente: ',' e '.' per avanzare/retrocedere di un frame nel video
- controllo volume funzionante (by Mauro)

KNOWN ISSUES:
- il livello video non ha trasparenza esterna



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
 
