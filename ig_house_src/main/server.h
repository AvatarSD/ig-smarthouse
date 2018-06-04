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


void run();



#endif