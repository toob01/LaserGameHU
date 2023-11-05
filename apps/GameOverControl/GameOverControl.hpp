#include <CleanRTOS.h>



namespace crt {
    class gameOver : public Task {
        private:
            Flag startFlag;

        public:
            gameOver( const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber, int timer) :
                Task( taskName, taskPriority, taskSizeBytes, taskCoreNumber), startFlag(this)
            {}

            void start(){}
            void main(){}




    };
};