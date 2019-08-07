/*
symtab.c

Copyright (c) Jun 5, 2019 Manwoo Yi

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

/**************************
 * Include system headers
 **************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/**************************
 * Include project headers
 **************************/
#include "symtab.h"
#include "virtual_machine.h"
#include "ported_lib.h"

/**************************
 * Data types, Constants
 **************************/
typedef struct _string_literal_tab_t_ {
    uint32_t idx;
    char*    string_literal_buffer;
    struct _string_literal_tab_t_   *next;
} string_literal_tab_t;

typedef struct _string_literal_tab_linkedlist_t_ {
    string_literal_tab_t*    head;
    string_literal_tab_t*    tail;
    uint32_t                 count;
} str_literal_tab_linkedlist_t;

typedef struct _symtab_t_ {
    uint32_t    idx;
    uint32_t    type_idx;
    char*       name;
    object_t    value;
    struct _symtab_t_   *next;
} symtab_t;

typedef struct _symtab_linkedlist_t_ {
    symtab_t*   head;
    symtab_t*   tail;
    uint32_t    count;
} symtab_linkedlist_t;

#define TYPE_SIZE_LEN (2048)
typedef struct _type_info_tab_t_ {
    char*         type_name;
    uint32_t      size_byte;
    object_type_t obj_type;
} type_info_tab_t;

#define MAX_DEPTH (128)

/**************************
 * Private variables
 **************************/
static str_literal_tab_linkedlist_t s_string_literal_tab_linkedlist = {0};
static symtab_linkedlist_t s_symtab_linkedlist[MAX_DEPTH] = {0};
static type_info_tab_t s_type_info_table[TYPE_SIZE_LEN] = {
    {"num_t", 8, object_type_number},
    {"str_t", 8, object_type_string},
    {0}
};
static uint32_t s_block_depth = {0};

/**************************
 * Private function prototypes
 **************************/
static bool compare_string(const char* str1, const char* str2);
static string_literal_tab_t* find_string_literal(const char* string_literal);
static string_literal_tab_t* insert_string_literal(const char* string_literal);
static symtab_t* find_symtab(const char* symbol);
static symtab_t* find_symtab_by_idx(uint32_t idx);
static symtab_t* insert_symtab(const char* symbol, uint32_t type_idx);
static void remove_symtab_by_sym_node(symtab_t* sym_node);
static bool check_type_index_valid(uint32_t type_idx);
static void remove_all_symtab(symtab_t* head);


/**************************
 * Public functions
 **************************/
char* Symtab_add_string_literal(const char* parser_str_ptr, uint32_t* out_idx)
{
    string_literal_tab_t* string_literal_node = find_string_literal(parser_str_ptr);

    if (string_literal_node == NULL)
    {
        string_literal_node = insert_string_literal(parser_str_ptr);
    }

    *out_idx = string_literal_node->idx;
    return string_literal_node->string_literal_buffer;
}

char* Symtab_get_string_literal_by_idx(uint32_t idx)
{
    string_literal_tab_t* iter = s_string_literal_tab_linkedlist.head;

    while (iter != NULL)
    {
        if (iter->idx == idx)
        {
            return iter->string_literal_buffer;
        }
        iter = iter->next;
    }

    return NULL;  
}

uint32_t Symtab_get_idx_of_type(const char* type_str_ptr)
{
    for(uint32_t i = 0 ; i < TYPE_SIZE_LEN ; i++)
    {
        if (s_type_info_table[i].type_name != 0)
        {
            if (compare_string(s_type_info_table[i].type_name, type_str_ptr))
            {
                return i;
            }
        }
    }

    return TYPE_SIZE_LEN;
}

uint32_t Symtab_add_variable(uint32_t type_idx, const char* ident_str_ptr)
{
    symtab_t* symtab_item = find_symtab(ident_str_ptr);

    if (check_type_index_valid(type_idx) == false)
    {
        VirtualMachine_add_error_msg(error_code_undefined_type);
        return 0;
    }

    if (symtab_item == NULL)
    {
        symtab_item = insert_symtab(ident_str_ptr, type_idx);
    }

    return symtab_item->idx;
}

uint32_t Symtab_get_idx_variable_by_identifier(const char* ident_str_ptr)
{
    symtab_t* symtab_item = find_symtab(ident_str_ptr);

    if (symtab_item != NULL)
    {
        return symtab_item->idx;
    }

    return 0;
}

bool Symtab_is_exist_variable(const char* ident_str_ptr)
{
    symtab_t* symtab_item = find_symtab(ident_str_ptr);
    return (symtab_item != NULL);
}

void Symtab_store_value_to_symtab(uint32_t symtab_idx, object_t value, bool fail_del_sym)
{
    symtab_t* symtab_item = find_symtab_by_idx(symtab_idx);
    if (symtab_item == NULL)
    {
        VirtualMachine_add_error_msg(error_code_not_found_symbol);
    }

    type_info_tab_t* type_info = &s_type_info_table[symtab_item->type_idx];

    if (type_info->obj_type == value.type)
    {
        symtab_item->value = value;
    }
    else
    {
        if (fail_del_sym)
        {
            remove_symtab_by_sym_node(symtab_item);
        }

        VirtualMachine_add_error_msg(error_code_type_mismatch);
    }
}

object_t Symtab_load_value_from_symtab(uint32_t symtab_idx)
{
    symtab_t* symtab_item = find_symtab_by_idx(symtab_idx);
    if (symtab_item == NULL)
    {
        VirtualMachine_add_error_msg(error_code_not_found_symbol);
    }

    return symtab_item->value;
}

void Symtab_set_block_input(bool enterBlock)
{
    if (enterBlock)
    {
        s_block_depth++;
    }
    else
    {
        if (s_block_depth > 0)
        {
            remove_all_symtab(s_symtab_linkedlist[s_block_depth].head);
            memset(&s_symtab_linkedlist[s_block_depth], 0, sizeof(symtab_linkedlist_t));
            s_block_depth--;
        }
    }
}

/**************************
 * Private functions
 **************************/
static bool compare_string(const char* str1, const char* str2)
{
    if (str1 == NULL || str2 == NULL)
    {
        return false;
    }

    if (strncmp(str1, str2, strlen(str1)) == 0)
    {
        if (strlen(str1) == strlen(str2))
        {
            return true;
        }
    }

    return false;
}

static string_literal_tab_t* find_string_literal(const char* string_literal)
{
    string_literal_tab_t* iter = s_string_literal_tab_linkedlist.head;

    while (iter != NULL)
    {
        if (compare_string(iter->string_literal_buffer, string_literal))
        {
            return iter;
        }
        iter = iter->next;
    }

    return NULL;
}

static string_literal_tab_t* insert_string_literal(const char* string_literal)
{
    string_literal_tab_t* new_node = (string_literal_tab_t*)new_malloc(sizeof(string_literal_tab_t));

    new_node->idx = ++s_string_literal_tab_linkedlist.count;
    new_node->string_literal_buffer = str_dup(string_literal, strlen(string_literal));
    new_node->next = NULL;

    if (s_string_literal_tab_linkedlist.head == NULL)
    {
        s_string_literal_tab_linkedlist.head = new_node;
    }
    else
    {
        s_string_literal_tab_linkedlist.tail->next = new_node;
    }
    
    s_string_literal_tab_linkedlist.tail = new_node;

    return new_node;
}

static symtab_t* find_symtab(const char* symbol)
{
    for (int32_t depth_index = (int32_t)s_block_depth ; depth_index >= 0 ; depth_index--)
    {
        symtab_t* iter = s_symtab_linkedlist[depth_index].head;
        while (iter != NULL)
        {
            if (compare_string(iter->name, symbol))
            {
                return iter;
            }
            iter = iter->next;
        }
    }

    return NULL;
}

static symtab_t* find_symtab_by_idx(uint32_t idx)
{
    for (int32_t depth_index = (int32_t)s_block_depth ; depth_index >= 0 ; depth_index--)
    {
        symtab_t* iter = s_symtab_linkedlist[depth_index].head;
        while (iter != NULL)
        {
            if (iter->idx == idx)
            {
                return iter;
            }
            iter = iter->next;
        }
    }

    return NULL;
}

static symtab_t* insert_symtab(const char* symbol, uint32_t type_idx)
{
    symtab_t* new_node = (symtab_t*)new_malloc(sizeof(symtab_t));

    new_node->idx = ++s_symtab_linkedlist[s_block_depth].count;
    new_node->type_idx = type_idx;
    new_node->name = str_dup(symbol, strlen(symbol));
    new_node->next = NULL;

    if (s_symtab_linkedlist[s_block_depth].head == NULL)
    {
        s_symtab_linkedlist[s_block_depth].head = new_node;
    }
    else
    {
        s_symtab_linkedlist[s_block_depth].tail->next = new_node;
    }
    
    s_symtab_linkedlist[s_block_depth].tail = new_node;

    return new_node;
}

static void remove_symtab_by_sym_node(symtab_t* sym_node)
{
    symtab_t* iter = s_symtab_linkedlist[s_block_depth].head;

    if (s_symtab_linkedlist[s_block_depth].head == sym_node)
    {
        release_mem(sym_node->name);
        release_mem(sym_node);
        s_symtab_linkedlist[s_block_depth].head = NULL;
        return;
    }

    while (iter != NULL)
    {
        if (iter->next == sym_node)
        {
            if (s_symtab_linkedlist[s_block_depth].tail == sym_node)
            {
                s_symtab_linkedlist[s_block_depth].tail = iter;
            }

            iter->next = sym_node->next;
            release_mem(sym_node->name);
            release_mem(sym_node);
            return;
        }
        iter = iter->next;
    }
}

static void remove_all_symtab(symtab_t* head)
{
    symtab_t* iter = head;

    while (iter != NULL)
    {
        symtab_t* next = iter->next;
        release_mem(iter->name);
        release_mem(iter);
        iter = next;
    }
}

static bool check_type_index_valid(uint32_t type_idx)
{
    return (type_idx < TYPE_SIZE_LEN);
}
