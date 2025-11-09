ENTRY(_start)

SECTIONS
{
    . = 0x2000;

    .text : {
        _stext = .;
        *(.text*)
        *(.rodata*)
        _etext = .;
    }

    .data : {
        _sdata = .;
        *(.data*)
        _edata = .;
    }

    .bss : {
        _sbss = .;
        *(COMMON)
        *(.bss*)
        _ebss = .;
    }

    . = ALIGN(16);
    _heap_start = .;
    . += 0x10000;  
    _heap_end = .;

    _stack_top = . + 0x4000; 
}

