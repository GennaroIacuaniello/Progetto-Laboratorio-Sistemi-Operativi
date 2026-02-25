#include "session.h"
#include "network.h"
#include "match.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <limits.h>


void handle_session(int socket_fd){

    char welcome_message_and_menu[MAX_LENGHT];

    recv_string(socket_fd, welcome_message_and_menu, MAX_LENGHT, 0);

    int done = 0;

    while (done == 0){

        //CLEAR_SCREEN and CURSOR_HOME to put the cursor at the top left corner of the screen
        printf("\033[2J\033[H");

        printf("%s", welcome_message_and_menu);

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
                    lobby_creation(socket_fd);
                    return;
                case 2:
                    done = 1;
                    join_lobby(socket_fd);
                    return;
                case 3:
                    done = 1;

                    recv_string(socket_fd, welcome_message_and_menu, MAX_LENGHT, 0);

                    close(socket_fd);

                    printf("\033[?1049l"); // EXIT_ALT_SCREEN
                    fflush(stdout);

                    printf("%s", welcome_message_and_menu);
                    
                    exit(0);
                    break;

                default:
                    recv_string(socket_fd, welcome_message_and_menu, MAX_LENGHT, 0);
                    break;
        }

    }
    

}

void lobby_creation(int socket_fd){

    char size_request[MAX_LENGHT];

    recv_string(socket_fd, size_request, MAX_LENGHT, 0);

    int done = 0;

    while (done == 0){

        //CLEAR_SCREEN and CURSOR_HOME to put the cursor at the top left corner of the screen
        printf("\033[2J\033[H");

        printf("%s", size_request);

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

        if(option == 0 || option > 3)
            recv_string(socket_fd, size_request, MAX_LENGHT, 0);
        else
            done = 1;

    }

    handle_being_in_lobby(socket_fd);

}

void join_lobby(int socket_fd){

    char message_with_matches_info[MAX_LENGHT];

    recv_string(socket_fd, message_with_matches_info, MAX_LENGHT, 0);

    uint32_t status_received, status_code = 1;

    ssize_t r =recv_all(socket_fd, &status_received, sizeof(status_received), 0);

    if (r == sizeof(status_received))
        status_code = ntohl(status_received);


    if(status_code == 1){

        //CLEAR_SCREEN and CURSOR_HOME to put the cursor at the top left corner of the screen
        printf("\033[2J\033[H");

        printf("%s", message_with_matches_info);

        sleep(1);

        handle_session(socket_fd);
        return;

    }

    int done = 0;

    while (done == 0){

        //CLEAR_SCREEN and CURSOR_HOME to put the cursor at the top left corner of the screen
        printf("\033[2J\033[H");

        printf("%s", message_with_matches_info);

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

        if(option == 0){

            done = 1;

            //CLEAR_SCREEN and CURSOR_HOME to put the cursor at the top left corner of the screen
            printf("\033[2J\033[H");

            handle_session(socket_fd);
            return;
            
        }

        r = recv_all(socket_fd, &status_received, sizeof(status_received), 0);

        if (r == sizeof(status_received))
            status_code = ntohl(status_received);

        if(status_code == 0){
            done = 1;
            handle_being_in_lobby(socket_fd);
            return;
        }

        recv_string(socket_fd, message_with_matches_info, MAX_LENGHT, 0);

        r = recv_all(socket_fd, &status_received, sizeof(status_received), 0);

        if (r == sizeof(status_received))
            status_code = ntohl(status_received);


        if(status_code == 1){

            done = 1;

            //CLEAR_SCREEN and CURSOR_HOME to put the cursor at the top left corner of the screen
            printf("\033[2J\033[H");

            printf("%s", message_with_matches_info);

            sleep(1);

            handle_session(socket_fd);
            return;

        }

    }


}


void handle_being_in_lobby(int socket_fd){

    int stdin_open = 1;
    fd_set rset;
    
    char recvline[MAX_LENGHT];

    uint32_t status_received, status_code = 1;

    uint32_t option;
    uint32_t option_to_send;
	
    while (1) {
    	
    	FD_ZERO(&rset);
    	
    	if (stdin_open) 
			FD_SET(STDIN_FILENO, &rset);
		
        FD_SET(socket_fd, &rset);
        
        int maxfd = (STDIN_FILENO > socket_fd ? STDIN_FILENO : socket_fd) + 1;
        
        int nready = select(maxfd, &rset, NULL, NULL, NULL);

        if (nready < 0) {

            if (errno == EINTR) 
                continue;

            perror("select");
            close(socket_fd);
            exit(1);
            break;

        }
        
        //Socket ready: read from server
        if (FD_ISSET(socket_fd, &rset)) {

            ssize_t r = recv_all(socket_fd, &status_received, sizeof(status_received), 0);

            if (r == sizeof(status_received)){
                status_code = ntohl(status_received);
            }else{
                status_code = 1;    //if an error occured, makes sure that at least does't have "host privileges"
            }

            if(status_code == 2){
                handle_being_in_match(socket_fd);
                //no return, after match ending, back in this function e continuing the loop
                sleep(1);   //To be sure server has sent the new code and message
                continue;
            }

            r = recv_string(socket_fd, recvline, sizeof(recvline), 0);
            
            if (r == 0) {                 //Server closed
                if (stdin_open)
                    fprintf(stderr, "Server terminated prematurely\n");
                
                close(socket_fd);
                exit(1);
                break;
            }

            //CLEAR_SCREEN and CURSOR_HOME to put the cursor at the top left corner of the screen
            printf("\033[2J\033[H");

            printf("%s", recvline);

        }

        // stdin pronto: leggi e invia al server
        if (stdin_open && FD_ISSET(STDIN_FILENO, &rset)) {

            char input_buffer[64];

            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                //EOF on stdin
                stdin_open = 0;
                option = UINT_MAX;     //Garbage value to send and so receive "Option not valid"

                if (shutdown(socket_fd, SHUT_WR) < 0) { 
                    perror("shutdown");

                    printf("\033[?1049l"); // EXIT_ALT_SCREEN
                    fflush(stdout);

                    exit(1);
                    break; 
                }
                
            }else {
                //Tries to extract a uint32_t from the line read
                if (sscanf(input_buffer, "%u", &option) != 1) {

                    option = UINT_MAX;      //Garbage value to send and so receive "Option not valid"

                }
            }
    
            option_to_send = htonl(option);

            //No need to clear the buffer since used fgets

            send_all(socket_fd, &option_to_send, sizeof(option_to_send));

            if(status_code == 0){
                
                switch (option){
                case 1:
                    handle_being_in_match(socket_fd);
                    break;
                case 2:
                    handle_change_map_size(socket_fd);
                    break;
                case 3:
                    handle_session(socket_fd);
                    break;
                case 4:
                    close(socket_fd);

                    printf("\033[?1049l"); // EXIT_ALT_SCREEN
                    fflush(stdout);

                    printf("%s", "\nArrivederci!\n");
                    
                    exit(0);
                    break;
                default:
                    //Doesn't do anything, the loop will read the message of option not valid in the next iteration
                    break;
                }

            }else if(status_code == 1){
                
                switch (option){
                case 1:
                    handle_session(socket_fd);
                    break;
                case 2:
                    close(socket_fd);

                    printf("\033[?1049l"); // EXIT_ALT_SCREEN
                    fflush(stdout);

                    printf("%s", "\nArrivederci!\n");
                    
                    exit(0);
                    break;
                default:
                    //Doesn't do anything, the loop will read the message of option not valid in the next iteration
                    break;
                }


            }

        }
        
    }


}

void handle_change_map_size(int socket_fd){

    char size_request[MAX_LENGHT];

    recv_string(socket_fd, size_request, MAX_LENGHT, 0);

    int done = 0;

    while (done == 0){

        //CLEAR_SCREEN and CURSOR_HOME to put the cursor at the top left corner of the screen
        printf("\033[2J\033[H");

        printf("%s", size_request);

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

        if(option == 0 || option > 3)
            recv_string(socket_fd, size_request, MAX_LENGHT, 0);
        else
            done = 1;

    }

}
