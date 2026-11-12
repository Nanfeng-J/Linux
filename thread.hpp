#pragma once
#include<iostream>
#include<cstring>
#include<unistd.h>
#include<pthread.h>
#include<string>
#include<functional>


namespace Threadmodel
{
    class Thread
    {
        using func_t=std::function<void()>;
        void enabledetach()
        {
            std::cout<<"线程被分离了"<<std::endl;
            _isdetached=true;
        }
        void enablerunning()
        {
            _isrunning=true;
        }
        static void* routine(void*args)
        {
            Thread* self=(Thread*)args;
            self->enablerunning();
            if(self->_isdetached)
            self->detach();
            self->_func();
            return nullptr;
        }
    public:
        Thread(func_t func1)
        :_tid(0)
        ,_isrunning(false)
        ,_isdetached(false)
        ,_res(nullptr)
        ,_func(func1)
        
        {
            _name="thread-1";
        }
        
        void detach()
        {
            if(_isdetached) return;
            if(_isrunning)
            {
                pthread_detach(_tid);
            }
            enabledetach();
        }
        bool start()
        {
            if(_isrunning)
            {
                std::cout<<"重复启动，失败"<<std::endl;
                return false;
            }
            int n=pthread_create(&_tid,nullptr,routine,this);
            if(n!=0)
            {
                std::cerr<<"n:"<<strerror(n)<<std::endl;
                return false;
            }
            else
            {
                std::cout<<"线程成功启动啦"<<std::endl;
                _isrunning=true;
                return true;
            }
        }
        bool stop()
        {
            if(_isrunning)
            {
                int n=pthread_cancel(_tid);
                if(n!=0)
                {
                    std::cerr<<"n:"<<strerror(n)<<std::endl;
                    return false;
                }
                else
                {
                    _isrunning=false;
                    return true;
                }
            }
            return false;
        }
        bool join()
        {
            if(_isdetached)
            {
                std::cout<<"线程已被分离.无法join"<<std::endl;
                return false;
            }
            int n=pthread_join(_tid,&_res);
            if(n!=0)
            {
                std::cerr<<"n:"<<strerror(n)<<std::endl;
                return false;
            }
            else
            {
                _isrunning=false;
                return true;
            }

        }

        ~Thread(){}
    private:
        pthread_t _tid;
        bool _isrunning;
        bool _isdetached;
        func_t _func;
        void* _res;
        std::string _name;
    };
}