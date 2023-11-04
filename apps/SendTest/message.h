#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>

namespace crt{

struct Message{
    uint64_t msg;
    int nofBytes;

    Message(uint64_t msg=0, int nofBytes=0):
    msg(msg), nofBytes(nofBytes){
        ESP_LOGI("Message", "Created  message with msg : %llu, nbytes : %d", msg, nofBytes);
    }

    uint8_t getByte(int byteNum){
        if(byteNum <= nofBytes){
            return msg >> ((nofBytes-byteNum)*8) & 0xFF;
        }
        ESP_LOGE("Message", "Byte index out of range");
        return 0;
    }
};
}
#endif //MESSAGE_H