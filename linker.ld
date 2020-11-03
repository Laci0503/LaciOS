ENTRY(_KERNEL_ENTRY)
SECTIONS
{
    .text :
    {
        *(.text._KERNEL_ENTRY);
        *(.text.handle_interrupt*);
        *(.text*);
        *(.data.*);
    }
}