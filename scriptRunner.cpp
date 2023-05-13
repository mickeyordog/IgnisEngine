#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>
#include "scriptRunner.h"

// IMPORTANT, BASICALLY EVERY MONO OBJECT POINTER MUST BE FREED TOO
// e.g. mono_method_desc_free
// can raise exception with mono_error_raise_exception (&error);

// need __stdcall before * on windows
typedef int (*GetNumber) (int, MonoException**);
typedef void (*ObjectConstructor) (MonoObject*, float, MonoException**);
typedef void (*UpdateObject) (MonoObject*, float, MonoException**);
typedef void (*Echo) (MonoObject*, MonoString*, MonoException**);
typedef int (*Test) (int, MonoException**);



// on Windows this needs __dllexport
void DoSomething() {
    printf("Hello from C!\n");
}

int PassInt(int i) {
    return i * 2;
}

char* PassString(char* str) {
    return str;
}

MonoObject* createObjectUninitialized(MonoDomain* domain, MonoImage* image, const char* namespaceName, const char* className) {
    MonoClass *my_class = mono_class_from_name (image, namespaceName, className);
    if (!my_class) {
        printf("Class not created.\n");
        return nullptr;
    }
    MonoObject *my_class_instance = mono_object_new (domain, my_class);
    if (!my_class_instance) {
        printf("Class instance not created.\n");
        return nullptr;
    }
    mono_runtime_object_init (my_class_instance);
    if (!my_class_instance) {
        printf("Object not created.\n");
        return nullptr;
    }
    return my_class_instance;
}

void printAllMethodsInClass(MonoClass* monoClass) {
    printf("Printing methods in class %s\n", mono_class_get_name(monoClass));
    void* iter = NULL;
    for (MonoMethod* method = mono_class_get_methods(monoClass, &iter); method != NULL; method = mono_class_get_methods(monoClass, &iter))
    {
        printf("%s\n", mono_method_full_name(method, 1));
    }
    printf("\n");
}

ScriptRunner::ScriptRunner(const char* assembliesPath, const char* assemblyFilePath, const char* configPath) {
    unsetenv("TERM"); // needed for Mono bug
    mono_set_assemblies_path(assembliesPath);
    mono_config_parse(configPath);

    this->domain = mono_jit_init("Ignis Domain");
    if (!this->domain) {
        printf("Error creating domain.\n");
        return;
    }
    this->assembly = mono_domain_assembly_open(domain, assemblyFilePath);
    if (!this->assembly) {
        printf("Error creating assembly.\n");
        return;
    }
    this->image = mono_assembly_get_image(this->assembly);
    if (!this->image) {
        printf("Error creating image.\n");
        return;
    }
    char* h = const_cast<char*>("argv");
    int retval = mono_jit_exec (domain, assembly, 1, &h);
    if (retval != 0) {
        printf("Error executing assembly.\n");
        return;
    }
}

ScriptRunner::~ScriptRunner()
{
    // mono_jit_cleanup(domain);
}

// Object will not be initialized if you just call this one!
MonoObject* ScriptRunner::createObjectUninitialized(const char* namespaceName, const char* className) {
    MonoClass *monoClass = mono_class_from_name (image, namespaceName, className);
    if (!monoClass) {
        printf("Class not created.\n");
        return nullptr;
    }
    MonoObject *classInstance = mono_object_new (domain, monoClass);
    if (!classInstance) {
        printf("Class instance not created.\n");
        return nullptr;
    }
    return classInstance;
}

MonoObject* ScriptRunner::createObjectEmptyConstructor(const char *namespaceName, const char *className) {
    MonoObject* classInstance = this->createObjectUninitialized(namespaceName, className);
    if (!classInstance) {
        printf("Object not created.\n");
        return nullptr;
    }
    mono_runtime_object_init (classInstance);
    return classInstance;
}

// Remember float must be written as single
void* ScriptRunner::getMethodThunkFromObject(MonoObject *object, const char *methodSignature)
{
    MonoMethodDesc* methodDesc = mono_method_desc_new (methodSignature, true);
    if (!methodDesc) {
        printf("mono_method_desc_new failed\n");
        exit(-1);
    }
    
    // Remember this needs to be mono_free_method()'d!
    // Should add logging system so I can say whether to print every step or not
    MonoMethod* method = mono_method_desc_search_in_class(methodDesc, mono_object_get_class(object));
    if (!method) {
        printf("mono_method_desc_search_in_image failed for signature %s in class %s\n", methodSignature, mono_class_get_name(mono_object_get_class(object)));
        return nullptr;
    }
    else {
        printf("Found method with name %s\n", mono_method_full_name(method, true));
    }

    void* thunk = mono_method_get_unmanaged_thunk(method);
    if (!thunk) {
        printf("mono_method_get_unmanaged_thunk failed\n");
        return nullptr;
    }

    mono_method_desc_free(methodDesc);
    // mono_free_method(method);
    return thunk;
}

void* ScriptRunner::getMethodThunk(const char* methodSignature)
{
    MonoMethodDesc* methodDesc = mono_method_desc_new (methodSignature, true);
    if (!methodDesc)
        printf("mono_method_desc_new failed\n");
    MonoMethod* method = mono_method_desc_search_in_image(methodDesc, image);
    if (!method) {
        printf("mono_method_desc_search_in_image failed\n");
    }
    else
        printf("Found method with name %s\n", mono_method_full_name(method, 1));

    void* thunk = mono_method_get_unmanaged_thunk(method);
    if (!thunk) {
        printf("mono_method_get_unmanaged_thunk failed\n");
        return nullptr;
    }

    mono_method_desc_free(methodDesc);
    mono_free_method(method);
    return thunk;
}


int main( int argc, char* argv[] ) {
    MonoException* exception;
    UpdateObject updateObject;

    // TODO: should make test class so I can verify functionality of some of these
    ScriptRunner runner("/Library/Frameworks/Mono.framework/Versions/6.12.0/lib", "test.exe", "/Library/Frameworks/Mono.framework/Versions/6.12.0/etc/mono/config");
    Test test = (Test)runner.getMethodThunk("GameObject:Test(int)");
    test(2, &exception);
    MonoObject* gameObject = runner.createObjectEmptyConstructor("", "GameObject");
    ObjectConstructor constructor = (UpdateObject)runner.getMethodThunkFromObject(gameObject, ":.ctor(single)");
    if (!constructor)
        printf("constructor null\n");
    if (exception != nullptr)
        mono_raise_exception(exception);
    constructor(gameObject, 1.0f, &exception);

    updateObject = (UpdateObject)runner.getMethodThunkFromObject(gameObject, ":Update(single)");
    if (!updateObject)
        printf("updateObject null\n");
    if (exception != nullptr)
        mono_raise_exception(exception);
    // Test test = (Test)runner.getMethodThunk(gameObject, ":Test(int)");
    updateObject(gameObject, 1.0f, &exception);
    // printf("Result: %d", test(2, &exception));
    // mono_raise_exception(exception);
    // try using debugger, not sure why this would cause crash
    // static void , static int works, static (int) works
    return 0;
    MonoClass* instanceClass = mono_object_get_class(gameObject);
    // Get a reference to the method in the class
    MonoMethod* metho = mono_class_get_method_from_name(instanceClass, "Update", 1);
    if (metho == nullptr)
    {
        // No method called "IncrementFloatVar" with 1 parameter in the class, log error or something
        printf("Method not found.\n");
    }

    // Call the C# method on the objectInstance instance, and get any potential exceptions
    MonoObject* exc = nullptr;
    float value = 1.0f;
    void* param = &value;
    mono_runtime_invoke(metho, gameObject, &param, &exc);
    
    return 0;

    unsetenv("TERM"); // also needed this
    // remember to set env var for pkg config path too, prob do that in code
    // didn't need this
    // mono_set_dirs ("/Library/Frameworks/Mono.framework/Versions/6.12.0/lib", "/Library/Frameworks/Mono.framework/Versions/6.12.0/etc");
    // mono_set_assemblies_path is a very bad idea as it prevents gac from working, src: http://docs.go-mono.com/?link=xhtml%3adeploy%2fmono-api-assembly.html
    mono_set_assemblies_path("/Library/Frameworks/Mono.framework/Versions/6.12.0/lib"); // this was what fixed it, had wrong path
    mono_config_parse("/Library/Frameworks/Mono.framework/Versions/6.12.0/etc/mono/config");
    MonoDomain *domain;

    domain = mono_jit_init ("Testing Mono");
    // int i = system("csc src/test.cs");


    MonoAssembly *assembly;
    assembly = mono_domain_assembly_open (domain, "test.exe");
    if (!assembly)
        printf("Assembly not found.\n");


    /*
    //Build a method description object
    MonoMethodDesc* TypeMethodDesc;
    char* TypeMethodDescStr = "Dog:Type()";
    TypeMethodDesc = mono_method_desc_new(TypeMethodDescStr, 0);
    if (!TypeMethodDesc)
    {
        printf("mono_method_desc_new failed\n");
        return 1;
    }

    //Search the method in the image
    MonoMethod* method;
    method = mono_method_desc_search_in_image(TypeMethodDesc, image);
    if (!method)
    {
        printf("mono_method_desc_search_in_image failed\n");
    }

    //run the method
    mono_runtime_invoke(method, NULL, NULL, NULL);
    printf("Ran the method\n");
    */

    // MonoDomain* appDomain = mono_domain_create_appdomain("MyAppDomain", NULL);
    // mono_domain_set(appDomain, 1);
    // printf("i: %d\n", i);
    printf("Start\n");
    char* h = const_cast<char*>("est.exe");
    int retval = mono_jit_exec (domain, assembly, 1, &h);
    printf("retval: %d\n", retval);

    MonoImage* image;
	image = mono_assembly_get_image(assembly);

    // MonoObject* object = createObject(domain, image, "", "GameObject");
    MonoClass *my_class = mono_class_from_name (image, "", "GameObject");
    if (!my_class)
        printf("Class not created.\n");
    MonoObject *my_class_instance = mono_object_new (domain, my_class);
    if (!my_class_instance)
        printf("Class instance not created.\n");
    mono_runtime_object_init (my_class_instance);
    if (!my_class_instance) {
        printf("Object not created.\n");
    }

    MonoMethodDesc* methodDesc = mono_method_desc_new ("GameObject:.ctor (single)", true);
    if (!methodDesc)
        printf("mono_method_desc_new failed\n");
    MonoMethod* method = mono_method_desc_search_in_image(methodDesc, image);
    if (!method) {
        printf("mono_method_desc_search_in_image failed\n");
    }
    else
        printf("Found method with name %s\n", mono_method_full_name(method, 1));

    mono_method_desc_free(methodDesc);
    printf("Here\n");
    ObjectConstructor objectConstructor = (ObjectConstructor)mono_method_get_unmanaged_thunk(method);
    exception;
    objectConstructor(my_class_instance, 10.5f, &exception);
    printf("Here2\n");

    methodDesc = mono_method_desc_new ("GameObject:Update(single)", true);
    if (!methodDesc)
        printf("mono_method_desc_new failed\n");
    method = mono_method_desc_search_in_image(methodDesc, image);
    if (!method) {
        printf("mono_method_desc_search_in_image failed\n");
    }
    else {
        printf("Found method with name %s\n", mono_method_full_name(method, 1));
    }
    mono_method_desc_free(methodDesc);

    updateObject = (UpdateObject)mono_method_get_unmanaged_thunk(method);
    printf("Here3\n");
    updateObject(my_class_instance, 5.5, &exception);
    printf("Here4\n");
    updateObject(my_class_instance, 50.5, &exception);

    methodDesc = mono_method_desc_new ("GameObject:Echo (string)", true);
    if (!methodDesc)
        printf("mono_method_desc_new failed\n");
    method = mono_method_desc_search_in_image(methodDesc, image);
    if (!method) {
        printf("mono_method_desc_search_in_image failed\n");
    }
    else
        printf("Found method with name %s\n", mono_method_full_name(method, 1));
    Echo echo = (Echo)mono_method_get_unmanaged_thunk(method);
    echo(my_class_instance, mono_string_new(domain, "Hello World"), &exception);


    // MonoMethodDesc* methodDesc = mono_method_desc_new ("HelloWorld:GetNumber(int)", 0);
    // if (!methodDesc)
    //     printf("mono_method_desc_new failed\n");
    // MonoMethod* method = mono_method_desc_search_in_image(methodDesc, image);
    // if (!method)
    //     printf("mono_method_desc_search_in_image failed\n");

    // last param of typedef must be MonoException**!
    // GetNumber getNumber = mono_method_get_unmanaged_thunk(method);
    // MonoException* exception;
    // printf("getNumber(): %d\n", getNumber(5, &exception));
    // printf("getNumber()");
    /* we execute methods that take one argument */
    // void *args [1];
    // int val = 10;
    /* Note we put the address of the value type in the args array */
    // args [0] = &val;
    // This is alright but unmanaged to managed thunks is better longterm
    // MonoObject *result = mono_runtime_invoke (method, NULL, args, NULL);
    /* we always get a MonoObject* from mono_runtime_invoke (), so to get
    * the integer value we need to unbox (which returns a pointer to
    * the value stored in the object) and dereference.
    */
    // int int_result = *(int*)mono_object_unbox (result);
    // printf("GetNumber(%d): %d\n", val, int_result);

    mono_jit_cleanup (domain);
    printf("End\n");
}
// are we looking for mono-2.0.a? or something else like .dylib
// if we need the .dylib, how could I fix that file universal problem?

// clang src/monoTest.c `pkg-config --cflags --libs mono-2` -o test
// try setting path to pkg-config of the one in root mac directory, that might've been the dif that fixed it when I went from cmake to console
// clang++ monoTest.cpp -rdynamic -Wall -g -o test `pkg-config --cflags --libs mono-2` 

// could also embed python, if I use linting and py3 features for typing it might be alright
// look into how python allocates memory though to see if it's alright