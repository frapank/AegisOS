#include "utils/kshell/kshell_commands.h"
#include "core/print.h"
#include "utils/kshell/kshell.h"

/* Commands execute */
void cmd_clear(){
    clear_screen();
}

void cmd_exit(){
    clear_screen();
    kshell_stop();
}

void cmd_about(){
    printk_color("     LainOS Minimal Shell     \n", VGA_COLOR_BLACK, VGA_COLOR_WHITE);
    printk_color("                              \n", VGA_COLOR_BLACK, VGA_COLOR_WHITE);
    printk_color("     Type help to view all    \n", VGA_COLOR_BLACK, VGA_COLOR_WHITE);
    printk_color("          commands            \n", VGA_COLOR_BLACK, VGA_COLOR_WHITE);
}
