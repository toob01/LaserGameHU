#include "crt_CleanRTOS.h"

namespace crt{
class signalPauseDetector : public Task
{
private:

    

	enum state_signalPauseDetector_t = {waitingForPause, waitingForSignal};
	state_signalPauseDetector_t state_signalPauseDetector = state_signalPauseDetector_t::waitingForPause;
	
public:
    signalPauseDetector(){

    }

	void main()
	{	
        uint64_t t_signalUs = 0;
		for(;;)
		{
			switch (state_signalPauseDetector){
				case state_signalPauseDetector_t::waitingForPause:
					vTaskDelay(100);
                    if(tsopReciever.isSignalPresent()){
                        t_signalUs+=100;
                        break;
                    }else{
                        necReceiver.SignalDetected(t_signalUs);
                        t_pauzeUs=0;
                        state_signalPauseDetector = state_signalPauseDetector_t::waitingForSignal;
                        break;
                    }
                
                case state_signalPauseDetector_t::waitingForSignal:
                    wait_us(100);
                    if(tsopReciever.isSignalPresent()){
                        t_pauseUs+=100;
                        if(t_pauzeUs > T_MAX_PAUSE_US){
                            necReceiver.PauseDetected(t_pauseUs);
                            t_pause_Us = 0;
                            break;
                        }else{
                            break;
                        }
                    }else{
                        necReceiver.PauseDetected(t_pauseUs);
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