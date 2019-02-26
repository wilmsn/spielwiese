/*


*/
#ifndef _ORDER_H_   
#define _ORDER_H_

class order {

    
//private:
public:
    
    struct order_struct {
        int index;
        order_struct* next;
    };

    order_struct *initial_order_ptr;
//protected:
    order_struct* new_entry(int idx);
    void del_entry(int idx);
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
