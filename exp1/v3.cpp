#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

class A {
private:
    int _port;
    string _ipv4;
public:
    A() {}
    A(const char* ip, int port):_ipv4(ip), _port(port) {}
    void show() {
        cout << _ipv4 << ":" << _port << '\n';
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
	cout<<"Call A"<<'\n';
        if(write(f, (const char*)this, sizeof(A)) < 0) 
            return false;
        return true;
    }
    bool deserialize(int f) {
	cout<<"Read A"<<'\n';
        int r = read(f, (char*)this, sizeof(A));
        if(r == 0 || r == -1) return false;
        return true;
    }
};

class B {
private:
    int _port;
    string _ipv6; //ipv6
public:
    B() {}
    B(const char* ip, int port):_ipv6(ip), _port(port) {}
    void show() {
        cout << _ipv6 << ":" << _port << '\n';
    }
    bool serialize(const char* pFilePath) const {
        int f = open(pFilePath, O_RDWR);
        if(f < 0) return false;
        if(write(f, (const char*)this, sizeof(B)) < 0)
            return false;
        return true;
    }
    bool deserialize(const char* pFilePath) {
        int f = open(pFilePath, O_RDWR);
        if(read(f, (char*)this, sizeof(B)) < 0) 
            return false;
        return true;
    }
    // 传递fd
    bool serialize(int f) const {
	cout<<"Call B"<<'\n';
        if(write(f, (const char*)this, sizeof(B)) < 0) 
            return false;
        return true;
    }
    bool deserialize(int f) {
	cout<<"Read B"<<'\n';
        int r = read(f, (char*)this, sizeof(B));
        if(r == 0 || r == -1) return false;
        return true;
    }
};

class Wrapper {
public:
    int nType;
    void *pobj;
    Wrapper() {}
    Wrapper(A* a) {pobj = (void*)a;nType = 1;}
    Wrapper(B* b) {pobj = (void*)b;nType = 2;}
};

class Serializer {
public:
    bool serialize(const char* pFilePath, const vector<Wrapper*> &v) {
        int f = open(pFilePath, O_RDWR|O_APPEND|O_TRUNC);
        if(f < 0) return false;
        for(auto a:v) {
            if(write(f, &(a->nType), sizeof(int))<0) return false;
            if(a->nType == 1) ((A*)(a->pobj))->serialize(f);
	    else if(a->nType == 2) ((B*)(a->pobj))->serialize(f);
	    else return false;
        } return true;
    }	
    bool deserialize(const char* pFilePath, vector<Wrapper*> &v) {
        int f = open(pFilePath, O_RDWR);
        if(f < 0) return false;
        while(true) {
            int temp;
            int r = read(f, &temp, sizeof(int));
            if((r == 0 || r == -1)) break;
            // 分别调用A和B的序列化
            if(temp == 1) {
                A* a = new A();
                a->deserialize(f);
                Wrapper* w = new Wrapper();
                w->nType = 1;
                w->pobj = (void*)a;
                v.push_back(w);
            } else if(temp == 2) {
                B* b = new B();
                b->deserialize(f);
                Wrapper* w = new Wrapper();
                w->nType = 2;
                w->pobj = (void*)b;
                v.push_back(w);
            } else return false;
        }
        close(f);
        return true;
    }
};

int main() {
    {   
        // 封装wrapper
        A a("127.0.0.1", 80);
        B b("1234567890123456", 22);
        Wrapper w1(&a);
        Wrapper w2(&b);
        vector<Wrapper*> v;
        v.push_back(&w1);
        v.push_back(&w2);

        // 序列化Wrapper
        Serializer s;
        s.serialize("a.txt", v);
    }
    {
        vector<Wrapper*> v;
        Serializer s;
        s.deserialize("a.txt", v);
        for(auto a:v) {
            if(a->nType == 1) ((A*)(a->pobj))->show();
            if(a->nType == 2) ((B*)(a->pobj))->show();
        }
    }
    return 0;
}
