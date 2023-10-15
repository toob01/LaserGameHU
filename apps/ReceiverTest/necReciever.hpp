#pragma once
#include "crt_CleanRTOS.h"
#include "messageReceiver.hpp"

namespace crt{

const uint64_t T_LEADSIGNAL_MIN_US = 7000;
const uint64_t T_LEADSIGNAL_MAX_US = 11000;
const uint64_t T_LEADPAUSE_MIN_US = 1500;
const uint64_t T_LEADPAUSE_MAX_US = 7000;
const uint64_t T_BITPAUSE_MIN_US = 200;
const uint64_t T_BITPAUSE_MAX_US = 2000;
const uint64_t T_BITPAUSE_THRESHOLD_ZERO_ONE = 1100;

class NecReceiver : public Task
{
private:

    MessageReceiver& messageReceiver;
    
    Queue<uint64_t, 10> signalChannel;
    Queue<uint64_t, 10> pauseChannel;

    uint64_t n;
    uint64_t m;

	enum state_necReceiver_t {waitingForLeadSignal, waitingForLeadPause, waitingForBitPause};
	state_necReceiver_t state_necReceiver = state_necReceiver_t::waitingForLeadSignal;

    void extractMessage (uint64_t& msg, int& nofBytes, uint64_t m, uint64_t n){
        msg = 0;
        nofBytes = 0;
        uint64_t mloc=m;
        if(n>0){
            for (int i=0; i<n-1; i++){
                msg |= (mloc&1);
                msg = msg << 1;
                mloc=mloc>>1;
            }
            nofBytes = n/8;
        }
    }
	
public:
    NecReceiver(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber, MessageReceiver& messageReceiver):
        Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
        messageReceiver(messageReceiver), signalChannel(this), pauseChannel(this), n(0), m(0)
    {
        start();
        ESP_LOGI("NECReceiver", "Start NECReceiver");
    }

    void signalDetected(uint64_t t_us){
        if(t_us > T_LEADSIGNAL_MIN_US){
            signalChannel.clear();
            pauseChannel.clear();
            signalChannel.write(t_us);
        }
    }

    void pauseDetected(uint64_t t_us){
        pauseChannel.write(t_us);
    }

	void main()
	{
		uint64_t t_signalUs;
        uint64_t t_pauseUs;
        uint64_t msg;
        int nofBytes;
		for(;;)
		{
			switch (state_necReceiver){
				case state_necReceiver_t::waitingForLeadSignal:
                    // ESP_LOGI("NECReceiver", "State waitingForLeadSignal");

                    signalChannel.read(t_signalUs);
                    // ESP_LOGI("NECReceiver", "signalChannel reads : %llu", t_signalUs);
                    if((t_signalUs>T_LEADSIGNAL_MIN_US) && (t_signalUs<T_LEADSIGNAL_MAX_US)){
                        state_necReceiver = state_necReceiver_t::waitingForLeadPause;
                        break;
                    }else{
                        break;
                    }
                
                case state_necReceiver_t::waitingForLeadPause:
                    // ESP_LOGI("NECReceiver", "State waitingForLeadPause");
                    pauseChannel.read(t_pauseUs);
                    // ESP_LOGI("NECReceiver", "pauseChannel reads : %llu", t_pauseUs);
                    if((t_pauseUs>T_LEADPAUSE_MIN_US) && (t_pauseUs<T_LEADPAUSE_MAX_US)){
                        n=0;
                        m=0;
                        state_necReceiver = state_necReceiver_t::waitingForBitPause;
                        break;
                    }else{
                        state_necReceiver = state_necReceiver_t::waitingForLeadSignal;
                        break;
                    }

                case state_necReceiver_t::waitingForBitPause:
                    // ESP_LOGI("NECReceiver", "State waitingForBitPause");
					pauseChannel.read(t_pauseUs);
                    if((t_pauseUs > T_BITPAUSE_MIN_US) && (t_pauseUs < T_BITPAUSE_MAX_US)){
                        // ESP_LOGI("NECReceiver", "After if Bitpause");
                        m=m<<1;
                        if(t_pauseUs > T_BITPAUSE_THRESHOLD_ZERO_ONE){
                            m |= 1;
                            n++;
                            break;
                        }else{
                            n++;
                            break;
                        }
                    }else{
                        // ESP_LOGI("NECReceiver", "Before extractMessage");
                        extractMessage(msg,nofBytes,m,n);
                        // ESP_LOGI("NECReceiver", "Before messageReceived");
                        messageReceiver.messageReceived(msg,nofBytes);
                        state_necReceiver = state_necReceiver_t::waitingForLeadSignal;
                        break;
                    }

				default:
					assert(false); // Report error: no valid state.
					break;
			}
		}
	}
};
}