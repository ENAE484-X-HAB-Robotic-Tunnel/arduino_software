// #include "protocol.h"

// static char     rxBuf[RX_BUF_SIZE]; // buffer to store message, expecting about 35 characters
// static uint8_t  rxLen = 0; // length of my message
// static bool     inPacket = false; // flag if we are within a message

// /*
//  * Checksum
//  * verify that full and uncorrupted message is recieved
//  */
// static uint8_t computeChecksum(const char* fields[], uint8_t count){
//     // count is the number of fields -1, as loop goes from 0 to 7
//     uint8_t ck = 0;
//     for (uint8_t i = 0; i < count; i++){
//         const char* p = fields[i];
//         while(*p) {
//             ck ^= (uint8_t)(*p++);
//         }
//         ck ^= (uint8_t)','; // add delimiter back for checksum
//     }
//     return ck;
// }

// /*
//  * Packet Parser
//  * We call this after our buffer recieves a full message, encapsulated by <>
//  * Expected message format:
//  * id, angle1, angle2, angle3, angle4, angle5, angle6, sequence, checksum
//  * 0    1       2       3       4       5       6           7           8
//  * 9 entries
//  */

// static MotorCommunication parsePacket(char* buf){
//     // initialize com struct and initialize flag as false.
//     // We only st flag to true after verifying this is a proper message
//     MotorCommunication com = {};
//     com.valid = false;

//     const uint8_t EXPECTED_ENTRIES = 9;
//     char* fields[EXPECTED_ENTRIES];
//     uint8_t fieldCount = 0;

//     char* token = strtok(buf, ","); // split string into tokens using , as delimiter
//     while (token != nullptr && fieldCount < EXPECTED_ENTRIES){
//         fields[fieldCount++] = token; // populate fields w string token
//         token = strtok(nullptr, ","); // continue from where strtok stopped
//     }

//     // Error check to make sure message is valid
//     if (fieldCount != EXPECTED_ENTRIES){
//         Serial.println("Error: field_count");
//         return com;
//     }

//     // error check checksum
//     uint8_t recievedCk = (uint8_t)atoi(fields[8]);
//     /* convert char string to intiger, checksum is stored in the 8th index */
//     uint8_t calculatedCk = computeChecksum(fields, EXPECTED_ENTRIES - 1);
//     // exclude the recieved checksum value when calculating for the ck
//     if (recievedCk != calculatedCk){
//         Serial.print("Error: Checksum");
//         return com;
//     }

//     // Now assume message is valid, first filter to only accept messages from the Jetson
//     uint8_t sender = (uint8_t)atoi(fields[0]); // check sender id
//     if (sender != SENDER_JETSON){
//         return com; 
//         /* 
//          * drop our own messages
//          * Only populated field is com.valid = false
//          */
//     }

//     // populate message struct
//     com.sender_id   = sender;
//     com.seq         = (uint8_t)atoi(fields[7]);

//     for (uint8_t i = 0; i < NUM_MOTORS; i++){
//         com.angles[i] = atof(fields[i+1]);
//     }

//     // flag message as valid
//     com.valid = true;

//     // return successful message
//     return com;
// }

// /*
//  * Serial reader
//  * manages serial input and calling parser
//  * Only return a populated MotorCommunication when
//  * we recieve a valid packet
//  */

// MotorCommunication readSerial(){
//     // initialize empty mc
//     MotorCommunication empty = {};
//     empty.valid = false;

//     while (Serial.available()){
//         char c = (char)Serial.read();
//         // check if we are starting a message
//         if (c == '<'){
//             // (re)set message to length 0,
//             // flag message start
//             rxLen = 0;
//             inPacket = true;
//             continue;
//         }
        
//         // check to see if we are ending a message
//         if (c == '>'){
//             // check for eronous >
//             if (!inPacket) continue; // invalid message

//             // reached end of message, wrap up
//             inPacket = false;
//             rxBuf[rxLen] = '\0'; // end of char string flag
//             // call packet parser and return it's output
//             // based on the buffered message
//             return parsePacket(rxBuf);
//         }

//         if (inPacket){
//             if (rxLen < RX_BUF_SIZE - 1){
//                 rxBuf[rxLen++] = c; // populate buffer
//             } else {
//                 // somehow overflowed buffer
//                 // packet messed up, reset
//                 inPacket = false;
//                 rxLen = 0;
//                 Serial.println('Error: Buffer Overflow');
//             }
//         }

//         return empty; // return empty com struct if failed
//     }
// }

// /*
//  * Send motor success acknowledgement
//  * Let the jetson know that we have successfully moved
//  * motors to the next step
//  * id, status, sequence, ck
//  * 0    1       2         3
//  */

// void sendResponse(uint8_t seq, ResponseStatus status){
//     char fields[3][8];  // create a generous buffer to store messages
//     char* ptrs[3];      // point to index in field for checksum calculator 
    
//     // format entries to be placed into fields
//     snprintf(fields[0], sizeof(fields[0]), "%d",  SENDER_ARDUINO); // formats decible string
//     snprintf(fields[1], sizeof(fields[1]), "%c",  (char)status);
//     snprintf(fields[2], sizeof(fields[2]), "%d",  seq);

//     for (uint8_t i = 0; i < 3; i++) ptrs[i] = fields[i];
//     uint8_t ck = computeChecksum(ptrs, 3); 
//     /* pass pointer of memory adress of start of each field[i] to checksum calculator*/

//     // Send response
//     Serial.print('<');
//     for (uint8_t i = 0; i < 3; i++) {
//         Serial.print(fields[i]);
//         Serial.print(',');
//     }
//     Serial.print(ck);
//     Serial.println('>');
// }


