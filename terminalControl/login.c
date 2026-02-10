#include <stdio.h>
#include <unistd.h>

#include "terminal_control.h"

#define ARROW_UP    65
#define ARROW_DOWN  66
#define ARROW_RIGHT 67
#define ARROW_LEFT  68
#define ENTER       13

#define MAIN_MENU_REGISTER 1
#define MAIN_MENU_LOGIN 2
#define MAIN_MENU_EXIT 3

#define BUFFER_SIZE 64

void print_menu(char **options, int quantity);
void handle_choice(int choice);
int read_input();

int main(void){
    
    int cursor_y = 1;
    int exit = 0;

    char *main_menu_options[3] = {"Registrati", "Login", "Esci"};

    enter_screen();
    set_echo_off();
    set_raw_input_mode();

    do{
        print_menu(main_menu_options, 3);
        set_cursor(0, cursor_y);
        printf(">");

        switch (read_input())
        {
            case ARROW_UP:
                cursor_y  <= 1 ? cursor_y = 1 : cursor_y--;
                break;
            case ARROW_DOWN:
                cursor_y  >= 3 ? cursor_y = 3 : cursor_y++;
                break;
            case ENTER:
                exit = 1;
                break;
            default:
                break;
        }
        usleep(100000);
    }while(!exit);

    handle_choice(cursor_y);

    clear_screen();
    set_cooked_input_mode();
    set_echo_on();

    exit_screen();
    return 0;
}

void print_menu(char **options, int quantity){

    clear_screen();
    for(int i = 0; i < quantity; i++){
        set_cursor(0, i+1);
        printf(" %s\n", options[i]);
    }
}

int read_input(){

    char c;

    if ((c = getchar()) == '\x1B') {
        getchar(); // skip the [
        return getchar(); //the real arrow value
    }

    return c;

}

//TODO: gestisci la scelta di registrarsi o di fare il login senza riempire lo stack
void handle_choice(int choice){
   
    char username[BUFFER_SIZE], password[BUFFER_SIZE];
    int size;

    switch (choice)
    {
        case MAIN_MENU_REGISTER:
            clear_screen();

            printf("Username: ");
            fflush(stdout);
            set_cooked_input_mode();
            set_echo_on();
            size = read(STDIN_FILENO, username, BUFFER_SIZE);
            username[size - 1] = '\0';

            printf("Password: ");
            fflush(stdout);
            set_echo_off();
            size = read(STDIN_FILENO, password, BUFFER_SIZE);
            password[size - 1] = '\0';

            break;
        case MAIN_MENU_LOGIN:

            break;
        case MAIN_MENU_EXIT:
            break;
        default:
            break;
    }

}