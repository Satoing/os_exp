#include <iostream>

const int func_id = 2;

extern "C" void print() {
    std::cout<<"Hello China!"<<'\n';
}

extern "C" void help() {
    std::cout<<"This function will print \"Hello China!\""<<'\n';
}