#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

int main( int argc, char* args[] ) {
    MonoDomain *domain;

    domain = mono_jit_init ("test.cs");
    printf("Worked?\n");
}
// are we looking for mono-2.0.a? or something else like .dylib
// if we need the .dylib, how could I fix that file universal problem?

// clang src/monoTest.c `pkg-config --cflags --libs mono-2` -o test