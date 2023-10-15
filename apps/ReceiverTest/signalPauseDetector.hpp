#pragma once
#include "crt_CleanRTOS.h"
#include "necReciever.hpp"

namespace crt{
class SignalPauseDetector : public Task
{
private:

    NecReceiver& necReceiver;
    Timer sleepTimer;
    const int T_MAX_PAUSE_US = 6000;
    
	enum state_signalPauseDetector_t {waitingForPause, waitingForSignal};
	state_signalPauseDetector_t state_signalPauseDetector = waitingForPause;
    uint8_t tsopReceiverPin = 32;
    
public:
    SignalPauseDetector(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber, NecReceiver& necReceiver):
    Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), necReceiver(necReceiver), sleepTimer(this){
        start();
        ESP_LOGI("SignalPauseDetector", "Start SignalPauseDetector");
    }

	void main()
	{
        gpio_pad_select_gpio(tsopReceiverPin);
        gpio_set_direction((gpio_num_t)tsopReceiverPin, GPIO_MODE_INPUT);
        uint64_t t_signalUs = 0;
        uint64_t t_pauseUs = 0;
		for(;;)
		{
			switch (state_signalPauseDetector){
				case state_signalPauseDetector_t::waitingForPause:
                
                    sleepTimer.sleep_us(100);
                    if(!gpio_get_level((gpio_num_t)tsopReceiverPin)){
                        t_signalUs+=100;
                        break;
                    }else{
                        necReceiver.signalDetected(t_signalUs);
                        t_pauseUs = 0;
                        state_signalPauseDetector = state_signalPauseDetector_t::waitingForSignal;
                    }
                    break;
                
                case state_signalPauseDetector_t::waitingForSignal:
                    sleepTimer.sleep_us(100);
                    if(gpio_get_level((gpio_num_t)tsopReceiverPin)){
                        t_pauseUs+=100;
                        if(t_pauseUs > T_MAX_PAUSE_US){
                            necReceiver.pauseDetected(t_pauseUs);
                            t_pauseUs = 0;
                            break;
                        }else{
                            break;
                        }
                    }else{
                        necReceiver.pauseDetected(t_pauseUs);
                        t_signalUs=0;
                        state_signalPauseDetector = state_signalPauseDetector_t::waitingForPause;
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