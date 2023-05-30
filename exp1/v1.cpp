#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class A {
private:
    int _port;
    const char* _ip;
public:
    A(const char* ip = "0.0.0.0", int port = 0):_ip(ip), _port(port) {}
    void show() {
        cout << _ip << ":" << _port << '\n';
    }
    bool serialize(const char* pFilePath) const {
        ofstream ofs(pFilePath);
        ofs.write((const char*)this, sizeof(A));
        return true;
    }
    bool deserialize(const char* pFilePath) {
        ifstream ifs(pFilePath);
        ifs.read((char*)this, sizeof(A));
        return true;
    }
};

int main() {
    {
        A a("127.0.0.1", 80);
        a.serialize("a.txt");
    }
    {
        A a;
        a.deserialize("a.txt");
        a.show();
    }
    return 0;
}