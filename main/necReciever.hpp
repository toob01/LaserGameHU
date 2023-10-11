#include <crt_CleanRTOS.h>

namespace crt{

const uint64_t T_LEADSIGNAL_MIN_US = 7000;
const uint64_t T_LEADSIGNAL_MAX_US = 11000;
const uint64_t T_LEADPAUSE_MIN_US = 3000;
const uint64_t T_LEADPAUSE_MAX_US = 6000;
const uint64_t T_BITPAUSE_MIN_US = 200;
const uint64_t T_BITPAUSE_MAX_US = 2000;
const uint64_t T_BITPAUSE_THRESHOLD_ZERO_ONE = 1100;

class necReciever : public Task
{
private:

    Queue<uint64, 10> signalChannel;
    Queue<uint64, 10> pauseChannel;

	enum state_necReciever_t = {waitingForLeadSignal, waitingForLeadPause, waitingForBitPause}
	state_necReciever_t state_necReciever = state_necReciever::waitingForLeadSignal;
	
public:
    necReceiver()

    void extractMessage (uint64& msg, uint& nofBytes, uint64 m, uint n){
        msg = 0; mloc=m;
        for (int i=0; i<n; i++){
            msg<<(mloc&1);
            mloc=mloc>>1;
        }
        nofBytes = n%8;
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

		for(;;)

		{
			switch (state_necReciever){

				case state_necReciever_t::waitingForLeadSignal:
					t_signalUs = signalChannel.read();
                    if((t_signalUs>T_LEADSIGNAL_MIN_US) && (t_signalUs<T_LEADSIGNAL_MAX_US)){
                        state_necReciever = state_necReciever_t::waitingForLeadPause;
                        break;
                    }else{
                        break;
                    }
                
                case state_necReciever_t::waitingForLeadPause:
                    t_pauseUs = pauseChannel.read();
                    if((t_pauseUs>T_LEADPAUSE_MIN_US) && (t_signalUs<T_LEADPAUSE_MAX_US)){
                        n=0; 
                        m=0;
                        state_necReciever = state_necReciever_t::waitingForBitPause;
                        break;
                    }else{
                        state_necReciever = state_necReciever_t::waitingForLeadSignal;
                        break;
                    }

                case state_necReciever_t::waitingForBitPause:
					t_pauseUs = pauseChannel.read();
                    if((t_pauseUs > T_BITPAUSE_MIN) && (t_pauseUS < T_BITPAUSE_MAX)){
                        m<<1;
                        if(t_pause_us > T_BITPAUSE_THRESHOLD_ZERO_ONE){
                            m |= 1;
                            n++;
                            break;
                        }else{
                            n++;
                            break;
                        }
                    }else{
                        extractMessage(msg,nofBytes,m,n);
                        messageReceiver.messageReceived(msg,nofBytes);
                        state_necReciever = state_necReciever_t::waitingForLeadSignal;
                        break;
                    }

				default:
					assert(false); // Report error: no valid state.
					break;
			}
		}
	}
}
}