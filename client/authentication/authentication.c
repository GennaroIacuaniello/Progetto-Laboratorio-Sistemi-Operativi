#include "authentication.h"

#include "network.h" 
#include "session.h" 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <limits.h>


void handle_login(int socket_fd){

    char message[MAX_LENGHT];
    char username[64], password[64];

    //Receive username request
    recv_string(socket_fd, message, MAX_LENGHT, 0);

    //CLEAR_SCREEN and CURSOR_HOME to put the cursor at the top left corner of the screen
    printf("\033[2J\033[H");

    printf("%s", message);

    scanf("%63s", username);

    //Cleans the buffer
    int c; 
    while ((c = getchar()) != '\n' && c != EOF);

    //Send username
    send_all(socket_fd, username, strlen(username) + 1);

    //Receive password request
    recv_string(socket_fd, message, MAX_LENGHT, 0);

    printf("%s", message);

    scanf("%63s", password);

    //Cleans the buffer 
    while ((c = getchar()) != '\n' && c != EOF);

    //Send password
    send_all(socket_fd, password, strlen(password) + 1);

    uint32_t status_received, status_code = 1;

    ssize_t r =recv_all(socket_fd, &status_received, sizeof(status_received), 0);

    if (r == sizeof(status_received))
        status_code = ntohl(status_received);


    //Receive login result message
    recv_string(socket_fd, message, MAX_LENGHT, 0);

    if(status_code == 0){

        //CLEAR_SCREEN and CURSOR_HOME to put the cursor at the top left corner of the screen
        printf("\033[2J\033[H");

        printf("%s", message);
        sleep(1);
        handle_session(socket_fd);

    }else{

        int done = 0;

        while (done == 0){

            //CLEAR_SCREEN and CURSOR_HOME to put the cursor at the top left corner of the screen
            printf("\033[2J\033[H");

            printf("%s", message);

            uint32_t option;
            uint32_t option_to_send;

            if (scanf("%u", &option) != 1) {
                option = UINT_MAX;     //Garbage value to send and so receive "Option not valid"
            }

            //Cleans the buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);


            option_to_send = htonl(option);

            send_all(socket_fd, &option_to_send, sizeof(option_to_send));

            switch (option){
                    case 1:
                        done = 1;
                        handle_login(socket_fd);
                        return;
                    case 2:
                        done = 1;
                        handle_registration(socket_fd);
                        return;
                    case 3:
                        done = 1;

                        recv_string(socket_fd, message, MAX_LENGHT, 0);

                        close(socket_fd);

                        printf("\033[?1049l"); // EXIT_ALT_SCREEN
                        fflush(stdout);

                        printf("%s", message);
                        
                        exit(0);
                        break;

                    default:
                        
                        recv_string(socket_fd, message, MAX_LENGHT, 0);
                        break;
            }

        }


    }

}

void handle_registration(int socket_fd){

    char message[MAX_LENGHT];
    char username[64], password[64];

    //Receive username request
    recv_string(socket_fd, message, MAX_LENGHT, 0);

    //CLEAR_SCREEN and CURSOR_HOME to put the cursor at the top left corner of the screen
    printf("\033[2J\033[H");

    printf("%s", message);

    scanf("%63s", username);

    //Cleans the buffer
    int c; 
    while ((c = getchar()) != '\n' && c != EOF);

    //Send username
    send_all(socket_fd, username, strlen(username) + 1);

    //Receive password request
    recv_string(socket_fd, message, MAX_LENGHT, 0);

    printf("%s", message);

    scanf("%63s", password);

    //Cleans the buffer
    while ((c = getchar()) != '\n' && c != EOF);

    //Send password
    send_all(socket_fd, password, strlen(password) + 1);

    uint32_t status_received, status_code = 1;

    ssize_t r =recv_all(socket_fd, &status_received, sizeof(status_received), 0);

    if (r == sizeof(status_received))
        status_code = ntohl(status_received);


    //Receive registration result message
    recv_string(socket_fd, message, MAX_LENGHT, 0);

    if(status_code == 0){

        //CLEAR_SCREEN and CURSOR_HOME to put the cursor at the top left corner of the screen
        printf("\033[2J\033[H");

        printf("%s", message);
        sleep(1);
        handle_session(socket_fd);

    }else{

        int done = 0;

        while (done == 0){

            //CLEAR_SCREEN and CURSOR_HOME to put the cursor at the top left corner of the screen
            printf("\033[2J\033[H");

            printf("%s", message);

            uint32_t option;
            uint32_t option_to_send;

            if (scanf("%u", &option) != 1) {
                option = UINT_MAX;     //Garbage value to send and so receive "Option not valid"
            }

            //Cleans the buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);


            option_to_send = htonl(option);

            send_all(socket_fd, &option_to_send, sizeof(option_to_send));

            switch (option){
                    case 1:
                        done = 1;
                        handle_registration(socket_fd);
                        return;
                    case 2:
                        done = 1;
                        handle_login(socket_fd);
                        return;
                    case 3:
                        done = 1;

                        recv_string(socket_fd, message, MAX_LENGHT, 0);

                        close(socket_fd);

                        printf("\033[?1049l"); // EXIT_ALT_SCREEN
                        fflush(stdout);

                        printf("%s", message);
                        
                        exit(0);
                        break;

                    default:
                        
                        recv_string(socket_fd, message, MAX_LENGHT, 0);
                        break;
            }

        }


    }


}
