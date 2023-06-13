#include <iostream>

const int id = 1;

extern "C" void print() {
    std::cout<<"Hello World!"<<'\n';
}
extern "C" void help() {
    std::cout<<"This function will print \"Hello World!\""<<'\n';
}

extern "C" int getId() {
    return id;
}