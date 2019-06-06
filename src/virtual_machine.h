/*
virtual_machine.h

Copyright (c) May 29, 2019 Manwoo Yi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#ifndef SRC_VIRTUAL_MACHINE_H_
#define SRC_VIRTUAL_MACHINE_H_

#include <assert.h>

typedef enum _opcode_t_ {
    opcode_nop = 0,
    opcode_push,
    opcode_add,
    opcode_sub,
    opcode_mul,
    opcode_div,
    opcode_mod,
    opcode_halt,
    opcode_MAXNUM
} opcode_t;

static_assert((int)opcode_MAXNUM <= 256, "opcode must be less than 255");


void        VirtualMachine_run_vm(object_t* codes);
object_t    VirtualMachine_get_result(void);

#endif /* SRC_VIRTUAL_MACHINE_H_ */
