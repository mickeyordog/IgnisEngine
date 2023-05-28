#include <iostream>

#include "pythonEngine.h"
#include "moduleIgnis.h"

PythonEngine::PythonEngine()
{
    PyImport_AppendInittab("emb", &PyInit_emb);

    Py_Initialize();

    // Is this alright to add?
    PyObject *sys = PyImport_ImportModule("sys");
    PyObject *path = PyObject_GetAttrString(sys, "path");
    PyList_Append(path, PyUnicode_FromString("../src/script"));
    // need to dec ref?
}

PythonEngine::~PythonEngine()
{
    if (Py_FinalizeEx() < 0)
    {
        std::cout << "Failed to finalize Python" << std::endl;
    }
}

bool PythonEngine::invokeFunction(const char* moduleName, const char* functionName, int argc, int argv[]) const
{
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;

    pName = PyUnicode_DecodeFSDefault(moduleName);
    if (pName == NULL)
    {
        fprintf(stderr, "Error decoding filename\n");
        return false;
    }

    pModule = PyImport_Import(pName);
    if (pModule == NULL)
    {
        PyErr_Print();
        fprintf(stderr, "Error importing module\n");
        Py_DECREF(pName);
        return false;
    }
    Py_DECREF(pName);

    if (pModule != NULL)
    {
        pFunc = PyObject_GetAttrString(pModule, "get_component_classes");
        if (pFunc && PyCallable_Check(pFunc))
        {
            // pValue = PyObject_CallObject(pFunc, NULL);
            // if (pValue != NULL)
            // {
            //     PyObject *obj = PyList_GET_ITEM(pValue, 0);
            //     PyObject *repr = PyObject_Repr(pValue);
            //     PyObject *str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
            //     const char *bytes = PyBytes_AS_STRING(str);

            //     printf("REPR: %s\n", bytes);
            // }
            // else
            // {
            //     PyErr_Print();
            //     printf("Error calling object\n");
            // }
        }
        else
        {
            PyErr_Print();
            printf("Error getting component classes\n");
        }

        pFunc = PyObject_GetAttrString(pModule, functionName);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc))
        {
            pArgs = PyTuple_New(argc);
            for (int i = 0; i < argc; ++i)
            {
                pValue = PyLong_FromLong(argv[i]);
                if (!pValue)
                {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL)
            {
                printf("Result of call: %ld\n", PyLong_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else
            {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr, "Call failed\n");
                return 1;
            }
        }
        else
        {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", functionName);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else
    {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", moduleName);
        return 1;
    }
    return true;
}

bool PythonEngine::invokeMethod(PyObject *self, PyObject *name, PyObject *arg)
{
    PyObject_CallMethodOneArg(self, name, arg);
    // PyObject_CallMethod()
    return true;
}
