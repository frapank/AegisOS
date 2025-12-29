#include "utils/kshell/kshell_commands.h"
#include "core/print.h"
#include "utils/kshell/kshell.h"

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
