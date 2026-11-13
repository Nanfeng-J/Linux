#include"thread.hpp"
using namespace Threadmodel;
 void func1()
    {
        int cnt=5;
        while(cnt--)
        {
            std::cout<<"我是新线程"<<std::endl;
        }
    }
int main()
{
   

    Thread t(func1);
    t.start();
    t.detach();
    t.stop();
    t.join();
    return 0;
}
