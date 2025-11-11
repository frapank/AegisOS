#include "utils/kshell/kshell.h"

/*
* Minimal kernel shell implementation.
* Used for debugging and executing simple commands from keyboard input.
* Each command currently accepts no arguments.
*/

#include "core/print.h"
#include "drivers/keyboard.h"
#include "utils/string.h"
#include "utils/kshell/kshell_commands.h"
#include "core/memory.h"
#include <stddef.h>

static inline void print_shell(char* msg){
    printk_color("> %s", VGA_COLOR_GREEN, VGA_COLOR_BLACK, msg);
    new_line();
}

struct command {
    const char* identifier; 
    const char* help;
    void (*function)();
};

static const struct command commands_list[] = {
    { "clear", "Clear the console",  cmd_clear },
    { "exit",  "Exit the OS",        cmd_exit  },
    { "help",  "Print help",         cmd_help  },
};

#define COMMAND_LIST_SIZE sizeof(commands_list)/sizeof(commands_list[0])

static inline void check_command(char* cmd_name){
    for(size_t i = 0; i < COMMAND_LIST_SIZE; i++){
        struct command cmd = commands_list[i];
        if(streql(cmd_name, cmd.identifier)){
            cmd.function();
            return;
        }
    }
    print_shell("Command not found");
}

static int running = 0;

void kshell_start() {
    clear_screen();
    print_shell("Welcome to the minimal shell of shell! Type help for all the commands_list");
    running = 1;
    char command_buffer[256];
    size_t index = 0;
    while(running == 1){
        for (;;){
            char c = getch();

            if(c == '\n'){
                new_line();
                if(index < sizeof(command_buffer) - 1){
                    check_command(command_buffer);
                }
                index = 0;
                memset(command_buffer, 0, sizeof(command_buffer));
                break;
            }
            // FIXME: '\r' breaks input handling
            command_buffer[index] = c;
            index++;
            printk("%c", c);
        }
    }
}

void kshell_stop(){
    running = 0;
}
