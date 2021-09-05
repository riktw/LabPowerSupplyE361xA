/*
 * HPDL1414.h
 *
 *  Created on: May 12, 2021
 *      Author: rik
 */

#ifndef APPLICATION_DISPLAY_HPDL1414_H_
#define APPLICATION_DISPLAY_HPDL1414_H_

#include <stdint.h>
#include "main.h"

struct gpio {
	GPIO_TypeDef* port;
	uint16_t pin;
};

using millivolts = uint32_t;
using milliamps = uint32_t;


class HPDL1414 {
public:
	static constexpr int NUMBER_OF_DIGITS = 4;

	HPDL1414();
	void Init(gpio addr0, gpio addr1, gpio wr);
	void SetDigit(uint8_t digit, char data);
	void SetDisplay(char data[NUMBER_OF_DIGITS]);

private:
	void WriteChar(char data);
	gpio m_addr0;
	gpio m_addr1;
	gpio m_wr;
};

class Display {
public:
	static constexpr int HPDL_PER_DISPLAY = 2;
	static constexpr int NUMBER_OF_DIGITS = 8;

	Display();
	void Init(gpio addr0, gpio addr1, gpio wr1, gpio addr2, gpio addr3, gpio wr2);
	void Set(const char data[NUMBER_OF_DIGITS]);

private:
	HPDL1414 m_displays[2];
};

class BaseDisplay {
public:
	virtual void Set(int32_t data) = 0;
	virtual void Clear() = 0;
	virtual void SetText(const char* data, int size) = 0;
	virtual int LimitValue(bool range, int value) = 0;

	bool m_valueChanged = false;
	int32_t m_oldValue = 0;
	int32_t m_value = 0;
	int m_valueChangedCounter = 0;
private:
	Display m_display;

};

class VoltageDisplay: public BaseDisplay {
public:
	VoltageDisplay();
	void Set(int32_t data);
	void SetText(const char* data, int size);
	void Clear();
	int LimitValue(bool range, int value);

private:
	constexpr static int MAXIMUM_VOLTAGE = 15000;
	Display m_display;
};


class CurrentDisplay: public BaseDisplay {
public:
	CurrentDisplay();
	void Set(int32_t data);
	void SetText(const char* data, int size);
	void Clear();
	int LimitValue(bool range, int value);

private:
	constexpr static int MAXIMUM_CURRENT = 1000;
	Display m_display;
};
#endif /* APPLICATION_DISPLAY_HPDL1414_H_ */
