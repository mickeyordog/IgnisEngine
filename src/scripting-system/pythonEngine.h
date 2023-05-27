#pragma once

class PythonEngine {
public:
    PythonEngine();
    ~PythonEngine();

    static bool invokeMethod(const char *moduleName, const char *functionName, int argc, char** argv);
};