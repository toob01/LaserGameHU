#include <CleanRTOS.h>



namespace crt {
    class gameOver : public Task {
        private:
            Flag startFlag;

            enum GameOverState_t {Idle, EndGame};
            GameOverState_t GameOverState = GameOverState_t::Idle;


        public:
            gameOver( const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber, int timer) :
                Task( taskName, taskPriority, taskSizeBytes, taskCoreNumber), startFlag(this)
            {
                start();
            }

            void _start(){
                startFlag.set();
            }

            void main(){
                for(;;){
                    switch(GameOverState){
                        case GameOverState_t::Idle:
                            wait(startFlag);
                            GameOverState = GameOverState_t::EndGame;
                        break;
                        case GameOverState_t::EndGame:
                            ShootingControl.disable();
                            ReceivingHitControl.disable();
                            Speaker.gameOverSet();
                            DisplayControl.gameOverSet();

                            wait(gameTimer);
                            GameOverState = GameOverState_t::Idle;
                            SendPostDataGameDataControl._start();

                        break;
                    }
                }
            }




    };
};