# How I Designed the Interrupt System (and Why)

In this section I explain the design pattern I used to implement interrupt handling in a clean and efficient way.
Most of the concepts apply even in the IRQ system.

## ISR Design

The main idea is to avoid a large `switch` statement for all the CPU exceptions.
Instead, I maintain an **array of function pointers**, where each index corresponds directly to the exception number.
This allows the dispatcher to simply index into the array and jump to the correct handler with no branching overhead.

### Default handlers via symbol aliases

Not all exceptions require a custom handler.
Instead of defining many empty functions that would only call a shared “default handler”, I use **GCC symbol aliases** to map all unimplemented ISR handlers directly to a single function:

```c
void isr13_man(struct regs *r) __attribute__((alias("default_isr_handler")));
```

This does **not** generate a wrapper function.
It does **not** create “fake” functions.
It simply creates additional *names* that all point to the exact same code address.
So when the dispatcher jumps to `isr13_man`, it actually jumps straight into `default_isr_handler` with zero overhead.

This design keeps the binary smaller and makes the dispatcher faster, because no extra calls or wrappers are needed.
