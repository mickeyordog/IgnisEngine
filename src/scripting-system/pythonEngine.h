#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

class PythonEngine {
public:
    PythonEngine();
    ~PythonEngine();

    bool invokeMethod(const char *moduleName, const char *functionName, int argc, int argv[]) const;
    bool addModule(PyObject* module);
};