#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

MonoDomain *domain;

domain = mono_jit_init (domain_name);