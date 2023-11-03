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
};
}
#endif //MESSAGE_H