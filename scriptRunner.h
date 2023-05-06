#pragma once

#include <mono/metadata/assembly.h>

class ScriptRunner
{
public:
    ScriptRunner(const char* assembliesPath, const char* assemblyFilePath, const char* configPath);
    ~ScriptRunner();

    // void runScript(const char* script);
    MonoObject* createObjectEmptyConstructor(const char *namespaceName, const char *className);
    MonoObject* createObjectUninitialized(const char *namespaceName, const char *className);
    void* getMethodThunk(MonoObject* object, const char* methodSignature);

private:
    MonoDomain* domain;
    MonoAssembly* assembly;
    MonoImage* image;
};