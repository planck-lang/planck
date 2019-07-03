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

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct _string_literal_tab_t_ {
    uint32_t idx;
    char*    string_literal_buffer;
    struct _string_literal_tab_t_   *next;
} string_literal_tab_t;

static struct _string_literal_tab_linkedlist_t_ {
    string_literal_tab_t*    head;
    string_literal_tab_t*    tail;
    uint32_t                 count;
} s_string_literal_tab_linkedlist = {0};


static string_literal_tab_t* find_string_literal(const char* string_literal);
static string_literal_tab_t* insert_string_literal(const char* string_literal);

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

static string_literal_tab_t* find_string_literal(const char* string_literal)
{
    string_literal_tab_t* iter = s_string_literal_tab_linkedlist.head;

    while (iter != NULL)
    {
        if (strncmp(iter->string_literal_buffer, string_literal, strlen(string_literal)) == 0)
        {
            if (strlen(iter->string_literal_buffer) == strlen(string_literal))
            {
                return iter;
            }
        }
        iter = iter->next;
    }

    return NULL;
}

static string_literal_tab_t* insert_string_literal(const char* string_literal)
{
    string_literal_tab_t* new_node = (string_literal_tab_t*)malloc(sizeof(string_literal_tab_t));

    new_node->idx = ++s_string_literal_tab_linkedlist.count;
    new_node->string_literal_buffer = strndup(string_literal, strlen(string_literal));
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
