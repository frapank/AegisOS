void bootenv_entry(void)
{
    volatile unsigned short* vga = (unsigned short*)0xB8000;

    vga[0] = (0x3F << 8) | 'H';
    vga[1] = (0x3F << 8) | 'i';
}
