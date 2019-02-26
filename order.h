/*


*/
#ifndef _ORDER_H_   
#define _ORDER_H_
#include <stdint.h>

class order {

    
//private:
public:
    
struct order_t {
	uint16_t 	    orderno;   		// the orderno as primary key for our message for the nodes
	uint16_t       	node;   		// the destination node
	unsigned char  	type;      		// becomes networkheader.type
	unsigned int   	flags;     		// some flags as part of payload
	unsigned char  	channel1;		// the channel for the sensor 1
	float		    value1;    		// the information that is send to sensor 1
	unsigned char  	channel2;  		// the channel for the sensor 2
	float		    value2;   	 	// the information that is send to sensor 2
	unsigned char  	channel3;  		// the channel for the sensor 3
	float		    value3;    		// the information that is send to sensor 3
	unsigned char  	channel4;  		// the channel for the sensor 1
	float		    value4;    		// the information that is send to sensor 4
	uint64_t		entrytime;      // timestamp for the entrytime
	uint64_t	  	last_send;		// timestamp for last sending of this record
	order_t        *next;           // poiter to the next record
};

    order_t *initial_order_ptr;
//protected:
    order_t* new_entry(uint16_t orderno);
    int del_entry(uint16_t orderno);
    void list_entry(void);
 
//public:
    
    order(void);

};

/*
struct test_struct {
int a;
double b;
test_struct ∗ next;
};

class SSD1306_OLED : public Print {
    private:
		byte px, py, inv;
		void sendCommand(byte command);
		void sendData(byte data);
		void scrollBuffer(void);
		void setPageMode(void);
		void setCursor(byte posX, byte posY);
		size_t write(uint8_t c);
		void printBuffer(void);
    protected:
        int oberklasseProt;	
	public:
		SSD1306_OLED(void);
		SSD1306_OLED(byte _ssd1306_min_row);
		SSD1306_OLED(byte _ssd1306_min_row, byte _ssd1306_max_row);
		void begin(void);
		void setBlackBackground(void);
		void setWhiteBackground(void);
		void setDisplayOff(void);
		void setDisplayOn(void);
		void setFont(byte _myfont);
		void clear(void);
};

*/




#endif // _ORDER_H_
