// Promise + exception
// scenario: one thread waits on future until another thread setsit's value, 
// if exception thrown in other thread, propagate excepotion to waiting thread so we can throw exception there a
// well, std::promise offers that as well
// 
// set_exception() : comes to the rescure in exception
// 
// set_exception_at_thread_exit()
// 
// https://www.udemy.com/course/modern-cpp-concurrency-in-depth/learn/lecture/7670580   
//

#pragma warning (disable : 4244)

#include <iostream>
#include <thread>
#include <future>
#include <stdexcept>

void throw_exception()
{
    //this is the invalid object exception throw    
    throw  std::invalid_argument("input cannot be negative");
}

void calc_sqrt(std::promise<int>& prom)
{
    int x = 1;
    std::cout << "please enter an int value: ";
    try
    {
        std::cin >> x;
        if (x < 0)
        {
            throw_exception();
        }
        prom.set_value(std::sqrt(x));
    }
    catch (std::exception&)
    {
        prom.set_exception(std::current_exception());

    }
}

void print_result(std::future<int>& fut)
{
    try
    {
        int x = fut.get();
        std::cout << "Value: " << x << "\n";
    }
    catch (std::exception& e)
    {
        std::cout << "[Exception Caught: " << e.what() << "]\n";
    }
}

void runcode()
{
    //create promise
    std::promise<int> prom;
    //acquire
    std::future<int> fut = prom.get_future();

    std::thread printing_thread(print_result, std::ref(fut));
    std::thread calc_thread(calc_sqrt, std::ref(prom));

    printing_thread.join();
    calc_thread.join();

}

int main()
{
    //std::cout << "Hello World!\n";


    runcode();
}
