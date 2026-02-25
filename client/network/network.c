#include "network.h"
#include "match.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>


ssize_t send_all(int socket_fd, const void* buf, size_t n){

      size_t sent = 0;
      const char* ptr = (const char*)buf;

      while(sent < n){

            ssize_t w = send(socket_fd, ptr + sent, (size_t)(n - sent), 0);

            if (w < 0) { 

                  if (errno == EINTR) 
                        continue; 
            
                  if (errno == EPIPE) {
                        perror("Server morto\n");
                        close(socket_fd);
                        exit(1);
                  }

                  perror("send"); 
                  close(socket_fd); 
                  exit(1);
            }

            sent += (size_t)w;
      }

      return (ssize_t)sent;
      
}

ssize_t send_all_in_match(int socket_fd, const void* buf, size_t n){

      size_t sent = 0;
      const char* ptr = (const char*)buf;

      while(sent < n){

            ssize_t w = send(socket_fd, ptr + sent, (size_t)(n - sent), 0);

            if (w < 0) { 

                if (errno == EINTR) 
                    continue; 
        
                if (errno == EPIPE) {
                    perror("Server morto\n");
                    close(socket_fd);
                    sleep(2);
                    disable_terminal_game_mode();
                    printf("\033[?1049l"); // EXIT_ALT_SCREEN
                    fflush(stdout);
                    exit(1);
                }

                perror("send"); 
                close(socket_fd);
                sleep(2);
                disable_terminal_game_mode();
                printf("\033[?1049l"); // EXIT_ALT_SCREEN
                fflush(stdout);
                exit(1);

            }

            sent += (size_t)w;
      }

      return (ssize_t)sent;
      
}

ssize_t recv_all(int socket_fd, void* buf, size_t n, int flags){

      size_t received = 0;
      char* ptr = (char*)buf;

      while(received < n){

            ssize_t r = recv(socket_fd, ptr + received, (size_t)(n - received), flags);

            if( r < 0 ){

                if(errno == EINTR)
                    continue;

                if(errno == EAGAIN || errno == EWOULDBLOCK) {
                    if (received == 0) {
                            return -1;
                    } else {
                            continue;
                    }
                }
                  
                perror("recv"); 
                close(socket_fd); 
                exit(1); 

            }

            if( r == 0 )       //connection closed
                  return (ssize_t)received;


            received += (size_t)r;


      }

      return (ssize_t)received;

}

ssize_t recv_all_in_match(int socket_fd, void* buf, size_t n, int flags){

      size_t received = 0;
      char* ptr = (char*)buf;

      while(received < n){

            ssize_t r = recv(socket_fd, ptr + received, (size_t)(n - received), flags);

            if( r < 0 ){

                if(errno == EINTR)
                    continue;

                if(errno == EAGAIN || errno == EWOULDBLOCK) {
                    if (received == 0) {
                            return -1;
                    } else {
                            continue;
                    }
                }
                    
                perror("recv"); 
                close(socket_fd);
                sleep(2);
                disable_terminal_game_mode();
                printf("\033[?1049l"); // EXIT_ALT_SCREEN
                fflush(stdout);
                exit(1); 

            }

            if( r == 0 )       //connection closed
                  return (ssize_t)received;


            received += (size_t)r;


      }

      return (ssize_t)received;

}

ssize_t recv_string(int socket_fd, char* buf, size_t max_len, int flags){

    size_t received = 0;
    char c;

    //Read until the max is reached, leaving 1 place for string terminator '\0'
    while (received < max_len - 1) {
        
        ssize_t r = recv(socket_fd, &c, (size_t)1, flags); // Reads 1 char

        if (r < 0) {

            if (errno == EINTR) 
                continue;
            
            
            if(errno == EAGAIN || errno == EWOULDBLOCK) {
                if (received == 0) {

                    return -1; 

                } else {
                    
                    usleep(5000);   //5 ms

                    continue;
                }
            }

            //Real error
            perror("recv");
            close(socket_fd);
            exit(1);
            
        }

        if (r == 0) {
            //connection closed
            printf("\nConnessione chiusa dal server.\n");
            exit(0);
        }

        buf[received] = c;
        received++;

        //Stops if receives string terminator '\0'
        if (c == '\0') {
            break;
        }
    }

    buf[received] = '\0';

    return received;

}

ssize_t recv_string_in_match(int socket_fd, char* buf, size_t max_len, int flags){

    size_t received = 0;
    char c;

    //Read until the max is reached, leaving 1 place for string terminator '\0'
    while (received < max_len - 1) {
        
        ssize_t r = recv(socket_fd, &c, (size_t)1, flags); // Reads 1 char

        if (r < 0) {

            if (errno == EINTR) 
                continue;
            
            
            if(errno == EAGAIN || errno == EWOULDBLOCK) {
                if (received == 0) {

                    return -1; 

                } else {
                    
                    usleep(5000);   //5 ms

                    continue;
                }
            }

            //Real error
            perror("recv");
            close(socket_fd);
            sleep(2);
            disable_terminal_game_mode();
            printf("\033[?1049l"); // EXIT_ALT_SCREEN
            fflush(stdout);
            exit(1);
            
        }

        if (r == 0) {
            //connection closed
            printf("\nConnessione chiusa dal server.\n");
            sleep(2);
            disable_terminal_game_mode();
            printf("\033[?1049l"); // EXIT_ALT_SCREEN
            fflush(stdout);
            exit(0);
        }

        buf[received] = c;
        received++;

        //Stops if receives string terminator '\0'
        if (c == '\0') {
            break;
        }
    }

    buf[received] = '\0';

    return received;

}
