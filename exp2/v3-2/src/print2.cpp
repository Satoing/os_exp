#include <iostream>

const int id = 2;

extern "C" void print() {
    std::cout<<"Hello China!"<<'\n';
}

extern "C" void help() {
    std::cout<<"This function will print \"Hello China!\""<<'\n';
}

extern "C" int getId() {
    return id;
}