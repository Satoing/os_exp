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

int callFunc(const char* name, const char* funcName="print") {
    void* handle = dlopen(name, RTLD_LAZY);
    if(handle == 0) {
        cout<<"dlopen error"<<'\n';
        return 0;
    }
    typedef void(*Fun)();
    
    Fun fun = (Fun)dlsym(handle, funcName);
    if(fun == 0) {
        cout<<"dlsym error"<<'\n';
        return 0;
    }
    fun();
    dlclose(handle);
}
int callIntFunc(const char* name, const char* funcName="getId") {
    void* handle = dlopen(name, RTLD_LAZY);
    if(handle == 0) {
        cout<<"dlopen error"<<'\n';
        return 0;
    }
    typedef int(*Fun)();
    
    Fun fun = (Fun)dlsym(handle, funcName);
    if(fun == 0) {
        cout<<"dlsym error"<<'\n';
        return 0;
    }
    int res = fun();
    dlclose(handle);
    return res;
}

int main(int argc, char* argv[]) {
    vector<string> plugins;
    pluginSearcher(plugins);

    if(argc == 1) {
        for(auto plugin:plugins) {
            // cout<<"调用插件名："<<plugin.c_str()<<'\n';
            callFunc(plugin.c_str());
        }
        return 0;
    }
    
    if(argc == 2) {
        if(strcmp(argv[1], "help") == 0) {
            for(int i=0;i<plugins.size();i++) {
                cout<<callIntFunc(plugins[i].c_str(), "getId")<<"  ";
                callFunc(plugins[i].c_str(), "help");
            }
            // int id = 0;
            // cout<<"请选择id：";
            // cin>>id;
            // return callFunc(plugins[id].c_str());
        }
        else {
            int id = atoi(argv[1]);
            for(int i=0;i<plugins.size();i++) {
                if(id == callIntFunc(plugins[i].c_str(), "getId"))
                    return callFunc(plugins[i].c_str());
            }
        }
    }
    return 0;
}