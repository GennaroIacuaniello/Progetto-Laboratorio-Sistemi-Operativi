#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//Commands to see the map full screen
#define ENTER_ALT_SCREEN "\033[?1049h" // Entra nello schermo intero
#define EXIT_ALT_SCREEN  "\033[?1049l" // Torna al terminale normale
#define HIDE_CURSOR      "\033[?25l"   // Nasconde il cursore lampeggiante
#define SHOW_CURSOR      "\033[?25h"   // Mostra di nuovo il cursore

//Clear commands
#define CLEAR_SCREEN     "\033[2J"     // Pulisce tutto lo schermo
#define CURSOR_HOME      "\033[H"      // Riporta il cursore in alto a sinistra (0,0)

//Reset to go back to normal terminal background color
#define RESET       "\033[0m"

//Background colors
#define BG_BLACK    "\033[40m"   //For empty cells ('e')
#define BG_WHITE    "\033[47m"   //For walls ('w')
#define BG_BLUE     "\033[44m"   //Player 0 ('b')
#define BG_RED      "\033[41m"   //Player 1 ('r')
#define BG_GREEN    "\033[42m"   //Player 2 ('g')
#define BG_YELLOW   "\033[43m"   //Player 3 ('y')

#define FG_WHITE_BOLD "\033[1;37m"
#define FG_BLACK_BOLD "\033[1;30m"

//Array of players backgrond colors
const char* bg_colors[4] = {
    "\033[44m", //0: Blue (b)
    "\033[41m", //1: Red (r)
    "\033[42m", //2: Green (g)
    "\033[43m"  //3: yellow (y)
};


void check_terminal_size();         //Initial function to warn the user in case of too litle terminal



int main(int argc, char* argv[]) {

    //Initial terminal size check
    check_terminal_size();


    return 0;
}







void check_terminal_size() {

    struct winsize w;

    //Do nothing if ioctl fails ( TIOCGWINSZ = Terminal IOCtl Get WINdow SiZe)
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        return;
    }

    //50 rows guarantees space for the biggest map (48x48 + border/message)
    if (w.ws_row < 50 || w.ws_col < 100) {
        printf("\n======================================================\n");
        printf("[ATTENZIONE] Il tuo terminale è troppo piccolo!\n");
        printf("Attuale: %d righe x %d colonne.\n", w.ws_row, w.ws_col);
        printf("Consigliato: almeno 50 righe x 100 colonne.\n\n");
        printf("Per visualizzare correttamente la mappa di gioco più grande (48x48),\n");
        printf("ti consigliamo di ingrandire la finestra del terminale a tutto schermo\n");
        printf("oppure di rimpicciolire leggermente il font (usa Ctrl e il tasto '-' o '+').\n");
        printf("======================================================\n\n");
        
        printf("Premi INVIO per continuare comunque con la connessione...");
        getchar();
    }

}