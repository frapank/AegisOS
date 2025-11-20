#include "utils/kshell/kshell_commands.h"
#include "core/print.h"
#include "utils/kshell/kshell.h"

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
    printk_color("     LainOS Minimal Shell     \n", VGA_COLOR_BLACK, VGA_COLOR_WHITE);
    printk_color("                              \n", VGA_COLOR_BLACK, VGA_COLOR_WHITE);
    printk_color("     Type help to view all    \n", VGA_COLOR_BLACK, VGA_COLOR_WHITE);
    printk_color("          commands            \n", VGA_COLOR_BLACK, VGA_COLOR_WHITE);
}

void cmd_binfo(void)
{
    printk_color("Build info list:\n", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    printk_color(" > Build date: ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    printk("%s\n", __DATE__);
    printk_color(" > Build time: ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    printk("%s\n", __TIME__);
}
