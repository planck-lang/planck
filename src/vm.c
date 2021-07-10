
#include <stdlib.h>

#include "vm.h"

Reg_s_t g_Regs = {0};
Mem_s_t g_Mem = {0};


static void _assign_more_mem(Virtual_mem_s_t *vmem)
{
    vmem->size_byte += GRAN_MEM;
    vmem->mem = (uint8_t*)realloc(vmem->mem, vmem->size_byte);
}

void vm_init(void)
{
    _assign_more_mem(&g_Mem.data);
    _assign_more_mem(&g_Mem.inst);
    _assign_more_mem(&g_Mem.stack);
}