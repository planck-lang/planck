#include <stdio.h>
#include <stdbool.h>
#include <Python.h>

#include "planck.h"

static PyObject* planck_exe(PyObject* self, PyObject* args)
{
    const char* buf;
    if (!PyArg_ParseTuple(args, "s", &buf))
    {
        return NULL;
    }
    
    object_t ret;
    bool st = Planck_do(buf, &ret);

    switch(ret.type)
    {
    case object_type_number:
        return Py_BuildValue("id", (int)st, ret.value.number);
    case object_type_string:
        return Py_BuildValue("is", (int)st, ret.value.string_ptr);
    default:
        printf("[Type Error]\n");
    }

    return Py_BuildValue("is", (int)st, NULL);
} 

static PyMethodDef planck_methods[] = { 
    {   
        "planck_do", planck_exe, METH_VARARGS,
        "Run the planck interpreter in a C extension."
    },  
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef planck_definition = { 
    PyModuleDef_HEAD_INIT,
    "planck",
    "A Python module that connect with planck interpreter from C code.",
    -1, 
    planck_methods
};

PyMODINIT_FUNC PyInit_planck(void) {
    Py_Initialize();
    return PyModule_Create(&planck_definition);
}
