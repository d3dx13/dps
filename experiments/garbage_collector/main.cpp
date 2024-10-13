#include <iostream>
#include <unistd.h>
#include <vector>

// dma_buf
#include <stdlib.h>
#include <signal.h>
#include <memory>
#include <list>
#include <any>

using namespace std;

class SimpleClass {
    public:
        SimpleClass(string name) : name(name){
            cout << "Create" << name << "\n";
        }
        ~SimpleClass() {
            cout << "Destroy" << name << "\n";
        }
    
    protected:
        const string name;
};

class SimpleContainer {
    public:
        SimpleContainer() {
            for (int i = SIGHUP; i <= SIGTERM; i++){
                signal(i, signalHandler);
            }
            std::cout << "__node__ " << __node__ << "\n";

            if (__node__ == NULL) {
                __node__ = this;
            } else {
                std::cout << "Only one Node per process Alowed\n";
                raise(SIGABRT);
            }
        }

        ~SimpleContainer() {
            cout << "SimpleContainer DIE\n";
            
            for (auto it = this->__cls__.begin(); it != this->__cls__.end(); it++) {
                it->get()->~SimpleClass();
            }
            
            exit(0);
        }

        shared_ptr<SimpleClass> anonce(string name) {
            shared_ptr<SimpleClass> cls;
            cls = make_shared<SimpleClass>(name);

            this->__cls__.push_back(cls);

            return cls;
        }

        static void signalHandler(int signum) {
            if (signum > 0) {
                std::cout << "Interrupt signal (" << signum << ") received, pid " << getpid() << "\n";
            } else {
                std::cout << "Calling Node Destructor, pid " << getpid() << "\n";
            }

            __node__->~SimpleContainer();
        };
    
    protected:
        list<shared_ptr<SimpleClass>> __cls__;

    private:
        static SimpleContainer * __node__;
};
SimpleContainer * SimpleContainer::__node__;


class SimpleContainerChild : public SimpleContainer {
    public:
        SimpleContainerChild() : SimpleContainer() {
            t42 = this->anonce("t42");
        }
    
    shared_ptr<SimpleClass> t42;
};

int main(int argc, char *argv[])
{
    SimpleContainerChild container;

    shared_ptr<SimpleClass> t1 = container.anonce("t1");
    shared_ptr<SimpleClass> t2 = container.anonce("t2");
    shared_ptr<SimpleClass> t3 = container.anonce("t3");

    for (;;){
        usleep(1000);
    }

    raise(SIGSEGV);
    return 0;
}