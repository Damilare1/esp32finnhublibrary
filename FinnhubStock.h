/*
  FinnhubStock.h library for monitoring Stock price of.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef Finnhub_h
#define Finnhub_h

#include "Arduino.h"
#include <ArduinoWebsockets.h>
#include <Arduino_JSON.h>

class Stock
{
public:
    Stock(int _gainIndicator, int _lossIndicator, long _baudRate, String _serverUrl);

public: void init();

public: void setOutputPin(int pin);

public: void setPinLow(int pin);

public: void setPinHigh(int pin);

public: void gain();

public: void loss();

public: void comparePrice(int openPrice, int currentPrice);

public: void getStockValue(String apiUrl);

public: int getHttpResponseCode();

public: JSONVar getKeys();

public: void parseData(int code);

public:
    float list[6];

private:
    int lossIndicator;

private:
    int gainIndicator;

private:
    unsigned long baudRate;

public:
    WebsocketsClient client;

public:
    int responseCode;

public:
    JSONVar payloadObject;

public:
    JSONVar keys;
};

#endif