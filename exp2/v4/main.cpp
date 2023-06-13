#include <dlfcn.h>
#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <string.h>
#include "include/interface.h"
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

bool getAllPlugins(vector<string> plugins, vector<Interface*>& Interfaces) {
    for(auto p:plugins) {
        void* handle = dlopen(p.c_str(), RTLD_LAZY);
        if(handle == 0) {
            cout<<"dlopen error"<<'\n';
            return 0;
        }
        typedef Interface*(*Fun)();
        
        Fun fun = (Fun)dlsym(handle, "getInterface");
        if(fun == 0) {
            cout<<"dlsym error"<<'\n';
            return 0;
        }
        Interface* i = fun();
        Interfaces.push_back(fun());
    }
}

int main(int argc, char* argv[]) {
    vector<string> plugins;
    vector<Interface*> interfaces;
    pluginSearcher(plugins);
    getAllPlugins(plugins, interfaces);
    
    if(argc == 2) {
        if(strcmp(argv[1], "help") == 0) {
            for(auto i:interfaces) {
                cout<<i->getId()<<" ";
                i->print();
            }
        }
        else {
            int id = atoi(argv[1]);
            for(auto i:interfaces)
                if(id==i->getId()) i->print();
        }
    }
    return 0;
}