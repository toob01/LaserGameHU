#pragma once

namespace crt {
    class IReadyUpListener {
        public:
            virtual void sendReady()=0;
    };
};
