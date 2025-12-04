# **Keyboard Driver Documentation (`keyboard.c`)**

## **Overview**

This file implements a minimal PS/2 keyboard driver for the kernel.
It provides:

* Low-level scancode reading from the PS/2 controller
* A ring buffer for storing processed key events
* Modifier-key state tracking
* Scancode → ASCII translation (with and without Shift)
* A simple blocking `getch()` API for retrieving characters

The driver is interrupt-driven: the keyboard IRQ handler reads raw scancodes and converts them into characters pushed into the buffer.

---

## **Architecture**

### **Hardware Interaction**

* The PS/2 keyboard sends raw scancodes to **I/O port `0x60`**.
* Each key press and release generates a scancode; release scancodes have bit `7` set.
* The interrupt handler (`keyboard_interrupt_handler`) is invoked on IRQ1.

### **Modifier Keys**

The driver tracks:

* Left Shift
* Right Shift
* Control (currently stored, not fully used)
* Alt (declared but not used yet)
* CapsLock (declared but not used yet)

For now, only **Shift** affects ASCII translation.

### **Scancode Translation**

Two lookup tables are provided:

* `scancode_to_ascii[]` — normal characters
* `scancode_to_ascii_shift[]` — Shift-modified characters

Only the first 0x40 scancodes are mapped, but this covers typical ASCII keys.

---

## **Ring Buffer**

The keyboard input is stored in a simple circular buffer:

* Size: `255` characters
* Elements: 16-bit values (`u16`)
* `kbd_head` → next position to write
* `kbd_tail` → next position to read

### **Concurrency Considerations**

* The interrupt handler writes to the ring buffer.
* User-space reading functions (`getch()`) pop from it.
* To avoid race conditions:

  * `kbd_buffer_pop()` temporarily disables interrupts (`cli()` / `sti()`).
  * `kbd_buffer_push()` does *not* disable IRQs; overflow is silently ignored.

---

## **API Description**

### **`keyboard_interrupt_handler()`**

Triggered by IRQ1:

1. Reads raw scancode from port 0x60.
2. Detects press/release events.
3. Updates modifier-key state (currently only Shift).
4. Converts scancode to ASCII using the correct table.
5. Pushes resulting character into the ring buffer.

### **`kbd_buffer_push(u16 char_code)`**

Pushes a character into the ring buffer unless full.

### **`kbd_buffer_pop()`**

Returns the next character or `-1` if empty.
Interrupts are temporarily disabled to ensure safe concurrent access.

### **`getch()`**

Blocking character read:

```
while buffer is empty:
    sti()
    hlt()
return next char
```

This is efficient: the CPU sleeps (`hlt`) until the next interrupt (keyboard IRQ) wakes it.
