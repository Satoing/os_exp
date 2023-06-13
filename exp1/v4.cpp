#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

class Wrapper {
public:
    virtual bool serialize(int f) = 0;
    virtual Wrapper* deserialize(int f) = 0;
    virtual int get_type() = 0;
    // virtual void show() = 0;
};

class A : public Wrapper {
private:
    int _port;
    const char* _ipv4;
public:
    A() {}
    A(const char* ip, int port):_ipv4(ip), _port(port) {}
    void show() {
        cout << _ipv4 << ":" << _port << '\n';
    }
    bool serialize(int f) {
        if(write(f, (const char*)this, sizeof(A)) < 0) 
            return false;
        return true;
    }
    Wrapper* deserialize(int f) {
        A* a = new A();
        read(f, (void*)a, sizeof(A));
        return a;
    }
    int get_type() {
        return 1;
    }
};

class B : public Wrapper {
private:
    int _port;
    int _ipv6;
public:
    B() {}
    B(int name, int age):_ipv6(name), _port(age) {}
    void show() {
        cout << _ipv6 << ":" << _port << '\n';
    }
    bool serialize(int f) {
        if(write(f, (const char*)this, sizeof(B)) < 0) 
            return false;
        return true;
    }
    Wrapper* deserialize(int f) {
        B* a = new B();
        int r = read(f, (void*)a, sizeof(B));
        return a;
    }
    int get_type() {
        return 2;
    }
};

class Serializer {
public:
    bool serialize(const char* pFilePath, const vector<Wrapper*> &v) {
        int f = open(pFilePath, O_RDWR|O_APPEND|O_TRUNC);
        if(f < 0) return false;
        for(auto a:v) {
            int nType = a->get_type();
            if(write(f, &nType, sizeof(int))<0) return false;
	        a->serialize(f);
        } return true;
    }	
    bool deserialize(const char* pFilePath, vector<Wrapper*> &v) {
        int f = open(pFilePath, O_RDWR);
        if(f < 0) return false;
        while(true) {
            int nType;
            int r = read(f, &nType, sizeof(int));
            if((r == 0 || r == -1)) break;
            int type;
            for(auto t:_types) {
                type = t->get_type();
                if(type == nType) {
                    Wrapper* w = t->deserialize(f);
                    if(w != nullptr) v.push_back(w);
                }
            }
        }
        close(f);
        return true;
    }
    void Register(Wrapper* t) {
	    _types.push_back(t);
    }

private:
    std::vector<Wrapper*> _types;
};

int main() {
    {   
        // 封装wrapper
        A a1("127.0.0.1", 80);
        A a2("192.168.0.103", 8080);
        B b1(10, 21);
        B b2(100, 22);
        vector<Wrapper*> v;
        v.push_back(&a1);
        v.push_back(&a2);
        v.push_back(&b1);
        v.push_back(&b2);
        // 序列化Wrapper
        Serializer s;
        s.serialize("a.txt", v);
    }
    {
        Serializer s;
        A a;
        B b;
        // 注册需要反序列化的类
        s.Register(&a);
        s.Register(&b);

        vector<Wrapper*> v;
        s.deserialize("a.txt", v);

        for(auto i:v) {
            // i->show();
            A *p = dynamic_cast<A*>(i);
            if(p != NULL) p->show();

            B *q = dynamic_cast<B*>(i);
            if(q != NULL) q->show();
        }
    }
    return 0;
}
