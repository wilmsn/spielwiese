#include "ordertest1.h"
#include <stdio.h> 
#include <iostream>
#include <thread>

order::order(void)
{
    initial_order_ptr=NULL;
}


order::order_struct* order::new_entry(int idx)
{
    order::order_struct *akt_order_ptr;
    if (! initial_order_ptr) 
    {
        initial_order_ptr = new order::order_struct;
        initial_order_ptr->index = idx;
        initial_order_ptr->next = NULL;
        akt_order_ptr = initial_order_ptr;
    } 
    else
    {
        akt_order_ptr = initial_order_ptr;
        while (akt_order_ptr->next) {
            akt_order_ptr = akt_order_ptr->next;
        }
        akt_order_ptr->next = new order::order_struct;
        if ( ! initial_order_ptr->next ) initial_order_ptr->next = akt_order_ptr->next;
        akt_order_ptr = akt_order_ptr->next;
        akt_order_ptr->index = idx;
        akt_order_ptr->next = NULL;
    }
    return akt_order_ptr;
}

void order::del_entry(int idx)
{
    order::order_struct *akt_order_ptr, *last_order_ptr;
    akt_order_ptr = initial_order_ptr;
    while (akt_order_ptr) 
    {
        if (akt_order_ptr->index != idx )
        {
            last_order_ptr = akt_order_ptr;
            akt_order_ptr=akt_order_ptr->next;
        } 
        else
        {
            if (akt_order_ptr == initial_order_ptr)
            {
                initial_order_ptr=akt_order_ptr->next;
            }
            else 
            {
                last_order_ptr->next=akt_order_ptr->next;
            }
            delete akt_order_ptr;
        }
    }
}

void order::list_entry(void)
{
    order::order_struct *akt_order_ptr;
    akt_order_ptr = initial_order_ptr;
    while (akt_order_ptr) {
        printf("Index: %d\n",akt_order_ptr->index);
        akt_order_ptr = akt_order_ptr->next;
    }    
}

void fill_order(order *myorder)
{
    for (int i=1; i<101; ++i) myorder->new_entry(i);
    myorder->list_entry();
    for (int i=1; i<91; ++i) myorder->del_entry(i);
    myorder->del_entry(95);
    myorder->del_entry(100);
}

int main()
{
    order myorder;
    std::thread t1([]{std::cout << "Hello C++11 from lambda function." << std::endl;});
    t1.detach();
    std::thread t2(fill_order,&myorder);
    t2.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    myorder.list_entry();
    
}
