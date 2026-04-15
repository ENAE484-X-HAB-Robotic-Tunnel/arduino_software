#pragma once
#include <Arduino.h>

// number of motors and id of sender and buffer size
#define NUM_MOTORS      6
#define SENDER_JETSON   0
#define SENDER_ARDUINO  1
#define RX_BUF_SIZE     64

// jetson message struct
typedef struct {
    uint8_t     sender_id; // sender id
    float       angles[NUM_MOTORS]; // angle to spin
    uint8_t     seq; // nth command
    bool        valid;
} MotorCommunication;

// arduino message struct
// arduino response status
typedef enum {
    STATUS_QUEUED   = 'Q',
    STATUS_DONE     = 'D',
    STATUS_NAK      = 'N',
    STATUS_ERROR    = 'E',
} ResponseStatus;

// acknowledgment message struct
typedef struct {
    uint8_t         sender_id;
    ResponseStatus  status; // current arduino status
    uint8_t         seq; // nth command
    bool            valid;
} MotorResponse;


// declear functions
MotorCommunication readSerial();
void sendAck(uint8_t seq, bool motorStatus[NUM_MOTORS]);
