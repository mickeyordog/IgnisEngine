#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

void DoSomething() {
    printf("Hello from C!\n");
}

int main( int argc, char* argv[] ) {
    unsetenv("TERM"); // also needed this
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
    MonoImage* image;
	image = mono_assembly_get_image(assembly);

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
    mono_jit_cleanup (domain);
    printf("End\n");
}
// are we looking for mono-2.0.a? or something else like .dylib
// if we need the .dylib, how could I fix that file universal problem?

// clang src/monoTest.c `pkg-config --cflags --libs mono-2` -o test
// try setting path to pkg-config of the one in root mac directory, that might've been the dif that fixed it when I went from cmake to console