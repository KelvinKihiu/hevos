#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "memory/memory.h"
#include "keyboard/keyboard.h"
#include "disk/disk.h"
#include "string/string.h"
#include "fs/pparser.h"
#include "fs/file.h"
#include "gdt/gdt.h"
#include "config.h"
#include "task/tss.h"
#include "task/task.h"
#include "task/process.h"
#include "status.h"
#include "isr80h/isr80h.h"
#include "screen/screen.h"
#include "time/timer.h"


void terminal_write_char(char c, char color)
{
    putch(c);
}


void print(const char* str)
{
    puts(str);
}

static struct paging_4gb_chunk* kernel_chunk = 0;


void panic(const char* msg)
{
    puts(msg);
    while(1) {}
}

void kernel_page()
{
    kernel_registers();
    paging_switch(kernel_chunk);
}

struct tss tss;
struct gdt gdt_real[HEVOS_TOTAL_GDT_SEGMENTS];
struct gdt_structured gdt_structured[HEVOS_TOTAL_GDT_SEGMENTS] =
{
    { .base = 0x00, .limit = 0x00,       .type = 0x00 },                // Null segment
    { .base = 0x00, .limit = 0xffffffff, .type = 0x9a },                // Kernel code segment
    { .base = 0x00, .limit = 0xffffffff, .type = 0x92 },                // Kernel data segment
    { .base = 0x00, .limit = 0xffffffff, .type = 0xf8 },                // User code segment
    { .base = 0x00, .limit = 0xffffffff, .type = 0xf2 },                // User data segment
    { .base = (uint32_t)&tss, .limit = sizeof(tss), .type = 0xE9 }      // TSS segment
};


void kernel_main() 
{
    // initialize the terminal
    init_screen();

    memset(gdt_real, 0x00, sizeof(gdt_real));
    gdt_structured_to_gdt(gdt_real, gdt_structured, HEVOS_TOTAL_GDT_SEGMENTS);

    // Load the GDT
    gdt_load(gdt_real, sizeof(gdt_real));

    // initialize the kernel's heap
    kheap_init();

    // Initialize the file systems
    fs_init();

    // Search and initialize disk
    disk_search_and_init();

    // initialize the idt
    idt_init();

    // Setup the TSS
    memset(&tss, 0x00, sizeof(tss));
    tss.esp0 = 0x600000;
    tss.ss0 = KERNEL_DATA_SELECTOR;

    // Load the TSS
    tss_load(0x28);

    // Setup paging
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    // Switch to kernel paging chunk
    paging_switch(kernel_chunk);

    // Enable paging
    enable_paging();

    // Register kernel commands
    isr80h_register_commands();

    // Initialie all the system keyboards
    keyboard_init();

    int fd = fopen("0:/hello.txt", "w");
    if (fd)
    {
        char buf[] = "This is a new.";
        int r = fwrite(buf, sizeof(buf), 1, fd);
        if (r)
        {
            print("Buffer written\n");
        }
    }
    

    struct process* process = 0;
    int res = process_load_switch("0:/shell.elf", &process);
    if (res != HEVOS_ALL_OK)
    {
        panic("Failed to load shell.bin\n");
    }

    task_run_first_ever_task();
    
    while(1) {}
}