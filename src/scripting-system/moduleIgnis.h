#define PY_SSIZE_T_CLEAN
#include <Python.h>

static int numargs = 0;
static PyObject *ignis_numargs(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":numargs"))
        return NULL;
    return PyLong_FromLong(numargs);
}

static PyObject *ignis_multiply(PyObject *self, PyObject *args)
{
    float a, b;
    if (!PyArg_ParseTuple(args, "ff:multiply", &a, &b))
        return NULL;
    return PyFloat_FromDouble(a * b);
}

static PyObject *ignis_dot(PyObject *self, PyObject *args)
{
    float x1, y1, x2, y2;
    if (!PyArg_ParseTuple(args, "(ff)(ff):dot", &x1, &y1, &x2, &y2))
        return NULL;
    return PyFloat_FromDouble(x1 * x2 + y1 * y2);
}

static PyMethodDef IgnisMethods[] = {
    {"numargs", ignis_numargs, METH_VARARGS, "Return the number of arguments received by the process."},
    {"multiply", ignis_multiply, METH_VARARGS, "Multiply two numbers."},
    {"dot", ignis_dot, METH_VARARGS, "Dot product between two vectors."},
    {NULL, NULL, 0, NULL}};

static PyModuleDef IgnisModule = {
    PyModuleDef_HEAD_INIT, "emb", NULL, -1, IgnisMethods,
    NULL, NULL, NULL, NULL};

static PyObject *PyInit_emb(void)
{
    return PyModule_Create(&IgnisModule);
}