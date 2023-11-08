#pragma once

namespace crt {
    class IReceivingHitListener {
        public:
            virtual void decrementHealth(int amount)=0;
    };
};