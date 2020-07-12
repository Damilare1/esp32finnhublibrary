#include "Arduino.h"
#include "FinnhubStock.h"
#include <Arduino_JSON.h>

Stock::Stock(int _gainIndicator, int _lossIndicator, long _baudRate)
{
    gainIndicator = _gainIndicator;
    lossIndicator = _lossIndicator;
    baudRate = _baudRate;
}

void Stock::onMessageCallback(WebsocketsMessage message) {
    Serial.print("Got Message: ");
    Serial.println(message.data());
}

void Stock::onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}
/*!
       @function init
       @abstract Initialize the PinModes, initial value of the Pins and the baudrate for serial communication
       @discussion This function initializes the output pins that would be used as indicators. It also initializes the serial communication port.
    */

Stock::init()
{
    setOutputPin(this->lossIndicator);
    setOutputPin(this->gainIndicator);
    setPinLow(this->lossIndicator);
    setPinLow(this->gainIndicator);

    Serial.begin(this->baudRate);
}

/*!
      @function setOutputPin
      @abstract Set Pin to OutPut pin mode
      @discussion This function sets the mode of a pin to output.
      @param pin The desired pin
    */

Stock::setOutputPin(int pin)
{
    pinMode(pin, OUTPUT);
}

/*!
      @function setPinLow
      @abstract Set Pin digital value to Low
      @discussion This function sets the pin digital value to give a low output.
      @param pin The desired pin
    */

Stock::setPinLow(int pin)
{
    digitalWrite(pin, LOW);
}

/*!
      @function setPinHigh
      @abstract Set Pin digital value to Low
      @discussion This function sets the pin digital value to give a high output.
      @param pin The desired pin
    */

Stock::setPinHigh(int pin)
{
    digitalWrite(pin, HIGH);
}

/*!
      @function gain
      @abstract Set the gain indicator and unset loss indicator
      @discussion This function displays a gain in price by setting the gain indicator and unsetting the loss indicator
    */

Stock::gain()
{
    setPinHigh(this->gainIndicator);
    setPinLow(this->lossIndicator);
}

/*!
      @function loss
      @abstract Set the loss indicator and unset gain indicator
      @discussion This function displays a loss in price by setting the loss indicator and unsetting the gain indicator
    */

Stock::loss()
{
    setPinHigh(this->lossIndicator);
    setPinLow(this->gainIndicator);
}

/*!
      @function comparePrice
      @abstract This function determines the status of a trade day
      @discussion This function determines whether the status of the current trade by comparing with the open price for the day
      @param openPrice The price at market open
      @param currentPrice The current market price
    */

Stock::comparePrice(int openPrice, int currentPrice)
{
    if ((currentPrice - openPrice) > 0)
    {
        gain();
    }
    else
    {
        loss();
    }
}

/*!
      @function getStockValue
      @abstract Retrieve the stock information
      @discussion This function retrieves the stock information from the API
      @param apiUrl API URL
    */

Stock::getStockValue(String apiUrl)
{
    client.connect(apiUrl.c_str());

   // this->responseCode = getHttpResponseCode();
   // parseData(this->responseCode);
   client.onMessage(onMessageCallback);
}

/*!
      @function getHttpResponseCode
      @abstract This function gets the status code of the request
      @discussion This function gets the status code of the request (possible status codes are 200 OK, 404 Not found, etc)
    */

Stock::getHttpResponseCode()
{
    int httpResponseCode = this->http.GET();
    return httpResponseCode;
}

/*!
      @function getKeys
      @abstract This function returns the keys of the payload Object
      @discussion This function returns the keys of the payload object.
    */

Stock::getKeys()
{
    return this->payloadObject.keys();
}

/*!
      @function parseData
      @abstract This function tears down the response object
      @discussion This function parses the response object and compares the value.
      @param code The HTTP response code
    */

Stock::parseData(int code)
{
    if (code > 0)
    {
        this->payloadObject = JSON.parse(http.getString());

        if (JSON.typeof(this->payloadObject) == "undefined")
        {
            Serial.println("Parsing input failed!");
            return;
        }

        this->keys = getKeys();

        int currentPrice = this->payloadObject["c"];
        int openPrice = this->payloadObject["o"];

        comparePrice(openPrice, currentPrice);

        for (int i = 0; i < keys.length(); i++)
        {
            this->list[i] = double(this->payloadObject[keys[i]]);
        }
    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(code);
    }
}