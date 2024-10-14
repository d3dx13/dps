#include <iostream>
#include <unistd.h>
#include <vector>

// dma_buf
#include <stdlib.h>
#include <signal.h>
#include <memory>
#include <queue>

using namespace std;

class SimpleClass {
    public:
        SimpleClass(string name) : name(name){
            cout << "Create " << name << "\n";
        }
        ~SimpleClass() {
            cout << "Destroy " << name << "\n";
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
            if (__node__ == NULL) {
                __node__ = this;
            } else {
                std::cout << "Only one Node per process Alowed\n";
                raise(SIGABRT);
            }
        }

        ~SimpleContainer() {
            cout << "SimpleContainer DIE\n";
            while (!this->__cls__.empty()) {
                this->__cls__.front().get()->~SimpleClass();
                this->__cls__.pop();
            }
            cout << "SimpleContainer DEAD\n";
        }

        shared_ptr<SimpleClass> anonce(string name) {
            shared_ptr<SimpleClass> cls;
            cls = make_shared<SimpleClass>(name);

            this->__cls__.push(cls);

            return cls;
        }

        static void signalHandler(int signum) {
            std::cout << "Interrupt signal (" << signum << ") received, pid " << getpid() << "\n";
            if (__node__ != NULL) {
                __node__->~SimpleContainer();
                __node__ = NULL;
                for (int i = SIGHUP; i <= SIGTERM; i++){
                    signal(i, SIG_DFL);
                }
                raise(signum);
            }
        };
    
    protected:
        queue<shared_ptr<SimpleClass>> __cls__;

    private:
        static SimpleContainer * __node__;
};
SimpleContainer * SimpleContainer::__node__;


class SimpleContainerChild : public SimpleContainer {
    public:
        SimpleContainerChild() : SimpleContainer() {
            t42 = this->anonce("t42");

            size_t SIZE = 1024 * 1024 * 1024;
            BIG.resize(SIZE);
            for (size_t i = 0; i < SIZE; i++) {
                BIG[i] = i;
            }
            cout << "malloc\n";

            this->temp = make_unique<SimpleClass>("WOW");
        }

        std::vector<char> BIG;

        unique_ptr<SimpleClass> temp;

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

    // raise(SIGSEGV);
    return 0;
}