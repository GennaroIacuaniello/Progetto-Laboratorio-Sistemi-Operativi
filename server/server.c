#include <stdlib.h>
#include <stdio.h>

/*
lista giocatori: 

lista (nodo: nickname, lobby in cui è)

mappa: 

matrice (di interi, vedi sotto) di dimensione a scelta dell'utente (16x16, 32x32, 48x48, 64x64); 

cella: 

UN INTERO gestito come segue:

000...01 muro
000...00 cella normale

      3210
[0,0] 0010 <-- cella normale colorata da giocatore 1
[1,0] 1010 <-- cella normale colorata da giocatore 1 e occupata da giocatore 1
[1,1] 0111 <-- muro scoperto da entrambi i giocatori;

per partita, lista informazione giocatore:

nodo: posizione_x, posizione_y, num celle colorate


mappe: 4 preset 16x16 scelti casualmente dal server (forse?)


*/