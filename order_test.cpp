#include "order.h"
#include <thread>
#include <stdint.h>
#include <stdio.h> 
#include <iostream>

void fill_order(order *myorder)
{
    for (int i=1; i<101; ++i) myorder->new_entry(i);
    myorder->list_entry();
    for (int i=1; i<91; ++i) myorder->del_entry(i);
}

int main()
{
    order myorder;
    std::thread t1([]{std::cout << "Hello C++11 from lambda function." << std::endl;});
    t1.detach();
//    fill_order(&myorder);
    std::thread t2(fill_order,&myorder);
    t2.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    myorder.list_entry();
    
}
