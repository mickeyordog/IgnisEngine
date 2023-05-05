#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

typedef int (*GetNumber) (int, MonoException**);

// gint32 HashCode(MonoObject *obj) {
//     return 0;
// }

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

MonoObject* createObject(MonoDomain* domain, MonoImage* image, const char* namespace, const char* className) {
    MonoClass *my_class = mono_class_from_name (image, namespace, className);
    MonoObject *my_class_instance = mono_object_new (domain, my_class);
    mono_runtime_object_init (my_class_instance);
}

int main( int argc, char* argv[] ) {
    unsetenv("TERM"); // also needed this
    // remember to set env var for pkg config path too, prob do that in code
    // didn't need this
    // mono_set_dirs ("/Library/Frameworks/Mono.framework/Versions/6.12.0/lib", "/Library/Frameworks/Mono.framework/Versions/6.12.0/etc");
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
    char* h = "test.exe";
    int retval = mono_jit_exec (domain, assembly, 1, &h);
    printf("retval: %d\n", retval);

    MonoImage* image;
	image = mono_assembly_get_image(assembly);


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
    // printf("End\n");
}
// are we looking for mono-2.0.a? or something else like .dylib
// if we need the .dylib, how could I fix that file universal problem?

// clang src/monoTest.c `pkg-config --cflags --libs mono-2` -o test
// try setting path to pkg-config of the one in root mac directory, that might've been the dif that fixed it when I went from cmake to console
// clang monoTest.c -rdynamic `pkg-config --cflags --libs mono-2` -o test