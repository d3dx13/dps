#include <iostream>
#include <unistd.h>
#include <vector>

// dma_buf
#include <stdlib.h>
#include <signal.h>
#include <memory>

using namespace std;

class SimpleClass {
    public:
        SimpleClass() {
            cout << "Create\n";
        }
        ~SimpleClass() {
            cout << "Destroy\n";
        }
};

class SimpleContainer {
    public:
        SimpleContainer() {
            for (int i = SIGHUP; i <= SIGTERM; i++){
                signal(i, signalHandler);
            }
            if (object != nullptr) {
                raise(SIGSEGV);
            }
            object = this;

            this->cls = make_unique<SimpleClass>();
            
            _cls.push_back(this->cls.get());
        }

        ~SimpleContainer() {

        }

        static void signalHandler(int signum) {
            if (signum > 0) {
                std::cout << "Interrupt signal (" << signum << ") received, pid " << getpid() << "\n";
            } else {
                std::cout << "Calling Node Destructor, pid " << getpid() << "\n";
            }

            object->~SimpleContainer();
            
            exit(0);
        };
    
    protected:
        unique_ptr<SimpleClass> cls;

        vector<SimpleClass *> _cls;

    private:
        static SimpleContainer * object;
};
SimpleContainer* SimpleContainer::object;


int main(int argc, char *argv[])
{
    SimpleContainer container;

    raise(SIGSEGV);
    return 0;
}