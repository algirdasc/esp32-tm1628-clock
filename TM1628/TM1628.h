#ifndef TM1628_h
#define TM1628_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "TM16XXFonts.h"
#include "Print.h"
//flags for turn on/off
#define ON true
#define OFF false

//flags for indicating LEDs
#define LED_SE0                 0x00
#define LED_SE1                 0x01
#define LED_SE2                 0x02
#define LED_SE3                 0x03
#define LED_SE4                 0x04
#define LED_SE5                 0x05
#define LED_SE6                 0x06
#define LED_PERCENT             0x07 // Procento kryziukas
#define LED_WIFI                0x08 // Virsutinis kairinis taskiukas
#define LED_SECONDS1            0x09 // Virsutinis sekundziu taskiukas
#define LED_SECONDS2            0x0A // Apatinis sekundziu taskiukas
#define LED_CELCIUS             0x0B // Taskiukas virsuj, prie temperaturos, celsijus
#define LED_PERCENT_DOTS        0x0C // Procentai
#define LED_FARENHEIT           0x0D // Taskiukas virsuj, prie temperaturos, farenheitai
#define LED_RET                 0x0E
#define LED_DTS                 0x0F
#define LED_DDD                 0x10
#define LED_CL1                 0x11
#define LED_CL2                 0x12


class TM1628 : public Print
{
  public:
	// init
	TM1628(byte _dio_pin, byte _clk_pin, byte _stb_pin);
	
	void begin(boolean active, int intensity);
	void update();
	void clear();
	void setSeg(byte addr, byte num);
	void setChar(byte _curpos, byte chr);
	void setCursor(byte pos);
	virtual size_t write(byte chr);
	void setTime(int hour, int min);
	void setLED(byte led);
	void setLEDon(byte led);
	void setLEDoff(byte led);
	void setLastSegment(byte num);
	byte getButtons();
  protected:
    byte receive();
    void sendData(byte addr, byte data);
    void sendCommand(byte data);
    void send(byte data);
    
    byte _dio_pin;
    byte _clk_pin;
    byte _stb_pin;
	byte _curpos;
//	byte buffer[];
//	byte seg_addr[];
};

#endif
