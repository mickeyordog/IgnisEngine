#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

class PythonEngine {
public:
    PythonEngine();
    ~PythonEngine();

    bool invokeFunction(const char *moduleName, const char *functionName, int argc, int argv[]) const;
    bool invokeMethod(PyObject *self, PyObject *name, PyObject *arg);
};