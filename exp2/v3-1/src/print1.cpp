#include <iostream>

const int func_id = 1;

extern "C" void print() {
    std::cout<<"Hello World!"<<'\n';
}
extern "C" void help() {
    std::cout<<"This function will print \"Hello World!\""<<'\n';
}