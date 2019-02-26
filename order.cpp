#include "order.h"
#include <stdio.h> 
#include <iostream>

order::order(void)
{
    initial_order_ptr = NULL;
}


order::order_t* order::new_entry(uint16_t orderno)
{
    order::order_t *akt_order_ptr;
    if (! initial_order_ptr) 
    {
        initial_order_ptr = new order::order_t;
        initial_order_ptr->orderno = orderno;
        initial_order_ptr->next = NULL;
        akt_order_ptr = initial_order_ptr;
    } 
    else
    {
        akt_order_ptr = initial_order_ptr;
        while (akt_order_ptr->next) {
            akt_order_ptr = akt_order_ptr->next;
        }
        akt_order_ptr->next = new order::order_t;
        if ( ! initial_order_ptr->next ) initial_order_ptr->next = akt_order_ptr->next;
        akt_order_ptr = akt_order_ptr->next;
        akt_order_ptr->orderno = orderno;
        akt_order_ptr->next = NULL;
    }
    return akt_order_ptr;
}

int order::del_entry(uint16_t orderno)
{
    order::order_t *akt_order_ptr, *last_order_ptr;
    akt_order_ptr = initial_order_ptr;
    while (akt_order_ptr) 
    {
        if (akt_order_ptr->orderno != orderno )
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
    order::order_t *akt_order_ptr;
    akt_order_ptr = initial_order_ptr;
    while (akt_order_ptr) {
        printf("OrderNo: %u\n",akt_order_ptr->orderno);
        akt_order_ptr = akt_order_ptr->next;
    }    
}

