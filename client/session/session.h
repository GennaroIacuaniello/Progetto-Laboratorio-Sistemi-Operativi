#ifndef SESSION_H
#define SESSION_H

void handle_session(int socket_fd);
void lobby_creation(int socket_fd);
void join_lobby(int socket_fd);

void handle_being_in_lobby(int socket_fd);
void handle_change_map_size(int socket_fd);

#endif