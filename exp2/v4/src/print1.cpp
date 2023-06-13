#include <iostream>
#include "../include/interface.h"

class Plugin1 : public Interface {
private:
    const int id = 1;
public:
    void print() {
        std::cout<<"Hello World!"<<'\n';
    }
    void help() {
        std::cout<<"This function will print \"Hello World!\""<<'\n';
    }
    int getId() {
        return id;
    }
};

extern "C" Interface* getInterface() {
    static Plugin1 p;
    return &p;
}