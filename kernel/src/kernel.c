#include "arch/idt.h"
#include "core/print.h"

// problema di prima
// non so come risolverlo
// ho ordinato il make ma non 
// credo che sia il problema
// l'errore parte da isr.asm
// se lo commento tutto funziona
// se esiste non funziona niente

void kernel_main() {

    idt_init();
    print("Welcome to your personal os!\n", COLOR_WHITE, COLOR_BLACK);


    return;
}
