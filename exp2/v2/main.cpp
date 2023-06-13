#include <dlfcn.h>
#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <string.h>
using namespace std;

bool pluginSearcher(vector<string>& plugins) {
    DIR* dir = opendir("./plugin");
    if(0 == dir) return false;
    while(true) {
        struct dirent* pentry = readdir(dir);
        if(0 == pentry) break;

        if(strcmp(pentry->d_name, ".") == 0) continue;
        if(strcmp(pentry->d_name, "..") == 0) continue;

        string str = "./plugin/";
        str += pentry->d_name;
        plugins.push_back(str);
    }
    return true;
}

int main() {
    vector<string> plugins;
    pluginSearcher(plugins);

    for(auto plugin:plugins) {
        // cout<<"调用插件名："<<plugin.c_str()<<'\n';
        void* handle = dlopen(plugin.c_str(), RTLD_LAZY);
        if(handle == 0) {
            cout<<"dlopen error"<<'\n';
            return 0;
        }
        typedef void(*Fun)();
        Fun fun = (Fun)dlsym(handle, "print");
        if(fun == 0) {
            cout<<"dlsym error"<<'\n';
            return 0;
        }
        fun();
        dlclose(handle);
    }
    return 0;
}