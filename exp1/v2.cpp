#include <iostream>
#include <fstream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

class A {
private:
    int _port;
    const char* _ip;
public:
    A() {}
    A(const char* ip, int port):_ip(ip), _port(port) {}
    void show() {
        cout << _ip << ":" << _port << '\n';
    }
    bool serialize(const char* pFilePath) const {
        int f = open(pFilePath, O_RDWR);
        if(f < 0) return false;
        if(write(f, (const char*)this, sizeof(A)) < 0)
            return false;
        return true;
    }
    bool deserialize(const char* pFilePath) {
        int f = open(pFilePath, O_RDWR);
        if(read(f, (char*)this, sizeof(A)) < 0) 
            return false;
        return true;
    }
    // 重载为传递fd
    bool serialize(int f) const {
        if(write(f, (const char*)this, sizeof(A)) < 0) 
            return false;
        return true;
    }
    bool deserialize(int f) {
        int r = read(f, (char*)this, sizeof(A));
        if(r == 0 || r == -1) return false;
        return true;
    }
};

class Serializer {
public:
    bool serialize(const char* pFilePath, const vector<A*> &v) {
        int f = open(pFilePath, O_RDWR|O_APPEND|O_TRUNC);
        if(f < 0) return false;
        for(auto a:v) a->serialize(f);
        return true;
    }
	bool deserialize(const char* pFilePath, vector<A*> &v) {
        int f = open(pFilePath, O_RDWR);
        if(f < 0) return false;
        while(true) {
            A* a = new A();
            if(a->deserialize(f) == true) v.push_back(a);
            else break;
        }
        close(f);
        return true;
    }
};

int main() {
    {
        A a("127.0.0.1", 80), b("120.76.41.137", 8080);
        vector<A*> v;
        v.push_back(&a);
        v.push_back(&b);
        Serializer s;
        s.serialize("a.txt", v);
    }
    {
        vector<A*> v;
        Serializer s;
        s.deserialize("a.txt", v);
        for(auto a:v) {
            a->show();
        }
    }
    return 0;
}