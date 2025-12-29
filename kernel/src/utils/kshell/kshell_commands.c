#include "utils/kshell/kshell_commands.h"
#include "core/print.h"
#include "utils/kshell/kshell.h"
#include "core/memory.h"

static char* about_msg[] = {
    "==================================\n"
    "           LainOS KShell          \n"
    "  Type help to view all commands  \n"
    "==================================\n"
};

static int about_msg_len = sizeof(about_msg) / sizeof(about_msg[0]);

/* Commands execute */
void cmd_clear(void)
{
    clear_screenk();
}

void cmd_exit(void)
{
    clear_screenk();
    kshell_stop();
}

void cmd_about(void)
{
    for(int i = 0; i < about_msg_len; i++) {
        printk(about_msg[i]);
    }
}

void cmd_binfo(void)
{
    printk_color("Build info list:\n", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    printk_color(" > Build date: ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    printk("%s\n", __DATE__);
    printk_color(" > Build time: ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    printk("%s\n", __TIME__);
}

void cmd_echo(void)
{
    char command_buffer[50] = {0};
    size_t index = 0;
    printk("Insert: ");
    for (;;){
        char c = getch();

        // Enter: Process comands
        if (c == '\n') {
            if(command_buffer[0] == '\0') break;
            new_linek();
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
    printk("%s\n", command_buffer);
}
