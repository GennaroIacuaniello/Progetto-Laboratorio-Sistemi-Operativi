#ifndef NETWORK_H
#define NETWORK_H

#include <sys/types.h>

//Max messages lenght
#define MAX_LENGHT 4096

//Commands to see the map full screen
#define ENTER_ALT_SCREEN "\033[?1049h" // Entra nello schermo intero
#define EXIT_ALT_SCREEN  "\033[?1049l" // Torna al terminale normale
#define HIDE_CURSOR      "\033[?25l"   // Nasconde il cursore lampeggiante
#define SHOW_CURSOR      "\033[?25h"   // Mostra di nuovo il cursore

//Clear commands
#define CLEAR_SCREEN     "\033[2J"     // Pulisce tutto lo schermo
#define CURSOR_HOME      "\033[H"      // Riporta il cursore in alto a sinistra (0,0)


ssize_t send_all(int socket_fd, const void* buf, size_t n);
ssize_t send_all_in_match(int socket_fd, const void* buf, size_t n);

ssize_t recv_all(int socket_fd, void* buf, size_t n, int flags);
ssize_t recv_all_in_match(int socket_fd, void* buf, size_t n, int flags);

ssize_t recv_string(int socket_fd, char* buf, size_t max_len, int flags);
ssize_t recv_string_in_match(int socket_fd, char* buf, size_t max_len, int flags);

#endif