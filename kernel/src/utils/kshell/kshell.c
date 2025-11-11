#include "utils/kshell/kshell.h"

/*
* Minimal kernel shell implementation.
* Used for debugging and executing simple commands from keyboard input.
* Each command currently accepts no arguments.
*/

#include <stddef.h>
#include "core/print.h"
#include "drivers/keyboard.h"
#include "drivers/vga/vga.h"
#include "utils/string.h"
#include "utils/kshell/kshell_commands.h"
#include "core/memory.h"

static void cmd_help_local();

struct command {
    const char* identifier; 
    const char* help;
    void (*function)();
};

static const struct command commands_list[] = {
    { "clear", "Clear the console",  cmd_clear },
    { "exit",  "Exit shell",         cmd_exit  },
    { "help",  "Print help",         cmd_help_local  },
    { "about", "Print start screen", cmd_about  },
    { "binfo", "All build info",     cmd_binfo  },
};

#define COMMAND_LIST_SIZE sizeof(commands_list)/sizeof(commands_list[0])

static inline void check_command(char* cmd_name){
    for(size_t i = 0; i < COMMAND_LIST_SIZE; i++){
        struct command cmd = commands_list[i];
        if(kstreql(cmd_name, cmd.identifier)){
            cmd.function();
            return;
        }
    }
    printk_color("Command %s not found \n", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, cmd_name);
}

static void cmd_help_local(){
    printk_color("Commands list:\n", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    for(size_t i = 0; i < COMMAND_LIST_SIZE; i++){
        struct command cmd = commands_list[i];
        printk_color(" > %s: ", VGA_COLOR_YELLOW, VGA_COLOR_BLACK, cmd.identifier);
        printk_color("%s\n", VGA_COLOR_WHITE, VGA_COLOR_BLACK, cmd.help);
    }
}

static int running = 0;

void kshell_start() {
    clear_screen();
    cmd_about(); // Welcome message
    running = 1;
    char command_buffer[10];
    size_t index = 0;
    while(running == 1){
        for (;;){
            char c = getch();

            // Enter: Process comands
            if (c == '\n') {
                knew_line();
                command_buffer[index] = '\0';
                check_command(command_buffer);
                index = 0;
                kmemset(command_buffer, 0, sizeof(command_buffer));
                break;
            }

            // Backspace: Delete char
            if (c == 0x08 || c == 0x7f) {
                if (index > 0) {
                    index--;
                    command_buffer[index] = '\0';
                    printk("\b \b");
                }
                continue;
            }

            // Normal: Add a char 
            if (index < sizeof(command_buffer) - 1) {
                if (c >= 32 && c <= 126) {
                    command_buffer[index++] = c;
                    printk("%c", c);
                }
            }
        }
    }
}

void kshell_stop(){
    running = 0;
}
