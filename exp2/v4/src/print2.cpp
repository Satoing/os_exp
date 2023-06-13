#include <iostream>
#include "../include/interface.h"

class Plugin2 : public Interface {
private:
    const int id = 2;
public:
    void print() {
        std::cout<<"Hello China!"<<'\n';
    }
    void help() {
        std::cout<<"This function will print \"Hello China!\""<<'\n';
    }
    int getId() {
        return id;
    }
};

extern "C" Interface* getInterface() {
    static Plugin2 p;
    return &p;
}