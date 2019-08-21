#ifndef IG_SERVER
#define IG_SERVER

/* Stack
 *  Functional objects
 *      diffrent usr/pass
 *      methods
 *      properties
 *      signals
 *  Function object
 *      facade of all objects
 *      name/version/location/device model/etc
 *  Protocol server
 *  LwIP
 * 
 * Init
 *  Turn on wifi
 *  Connect to ssid
 *  Get IP addr
 *  Create tasts
 *      1. Listen broadcast port json-rpc
 *      2. Listen port for lson-rpc
 */

#include "inttypes.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// #include <memory>
// #include <vector>
/*
class Socket
{

};

class RawMessage
{
public:

private:
    std::shared_ptr<Socket> soket;
    std::vector<uint8_t> data; 
};

template<typename T>
class Queue
{
public:
    Queue(){}
    ~Queue(){}
    void put(T&data){

    }
    T& get(){

    } 
private:
    QueueHandle_t xQueue;
};


class Server
{
    Server(bool is_bcast, Queue<RawMessage> *out_queue);
};

class Client
{
    void send();
};

template<class T_Server, class T_Client>
class NetWorker
{
public:
    NetWorker(){}

private:
    Queue<RawMessage> inQueue;
    Queue<RawMessage> outQueue;

    T_Server server;
};
*/
class JsonServer// : Server
{
public:
    JsonServer(bool is_bcast, QueueHandle_t out_queue);
    ~JsonServer();


    int start(uint16_t port);
    void stop();

    int run();

private:
    QueueHandle_t out_queue;
    int s;
};




#endif