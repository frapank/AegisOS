;-------
; Title
lain_line1 db "#      ", "  ##  ", " ###  ", " #    #", 0Dh,0Ah,0
lain_line2 db "#      ", " #  # ", "  #   ", " ##   #", 0Dh,0Ah,0
lain_line3 db "#      ", "#    #", "  #   ", " # #  #", 0Dh,0Ah,0
lain_line4 db "#      ", "######", "  #   ", " #  # #", 0Dh,0Ah,0
lain_line5 db "###### ", "#    #", " ###  ", " #   ##", 0Dh,0Ah,0

;-------
; Text related
line_down db 0Dh, 0Ah, 0
msg db "Welcome to the lain boot loader :D",0Dh,0Ah,0
msg_wait db "Press a key to continue...",0Dh,0Ah,0

;-------
; Other
boot_disk db 0
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
KERNEL_LOCATION equ 0x1000
