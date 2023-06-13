#include <dlfcn.h>
#include <iostream>

int main() {
    void* handle = dlopen("./libprint.so", RTLD_LAZY);
    if(handle == 0) {
        std::cout<<"dlopen error"<<'\n';
        return 0;
    }
    typedef void(*Fun)();
    Fun fun = (Fun)dlsym(handle, "print");
    if(fun == 0) {
        std::cout<<"dlsym error"<<'\n';
        return 0;
    }
    fun();
    dlclose(handle);
    return 0;
}