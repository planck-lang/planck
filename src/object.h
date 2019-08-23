/*
object.h

Copyright (c) Jun 3, 2019 Manwoo Yi

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

#ifndef SRC_OBJECT_H_
#define SRC_OBJECT_H_

/**************************
 * Includes
 **************************/
#include "planck_types.h"

/**************************
 * Public function prototypes
 **************************/
object_t Obj_to_string(object_t obj);
object_t Obj_conc_string(object_t str_obj1, object_t str_obj2);
object_t Obj_rept_string(object_t str_obj, object_t num_obj);
int      Obj_comp_string(object_t str_obj, object_t num_obj);
list_t*  Obj_conv_list_item(object_t obj);
char*    Obj_combined_to_str(char* str, object_t obj, bool auto_free);

#endif /* SRC_OBJECT_H_ */
