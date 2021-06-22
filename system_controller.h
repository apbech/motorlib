#pragma once

#include "internal_communication.h"

// An optional outer control loop to use rather than external communication
// This is an example and can be copied and modified for an application
class SystemController {
 public:
    SystemController(InternalCommunication &communication) 
        : communication_(communication) {}
    void loop() {
        MotorStatus status = communication_.get_send_data();
        MotorCommand command = {};
        static struct { uint32_t seconds; uint32_t milliseconds; } t = {};
        static float f = -2;
        command.mode_desired = POSITION;
        f += .001;
        command.position_desired = f;
        communication_.set_receive_data(&command);

        t.milliseconds++;
        if (t.milliseconds == 1000) {
            t.seconds++;
            t.milliseconds = 0;
        }

        if (t.seconds % 10 == 0) {
            __WFI(); // sleep, not deep
        }
    }

 private:
    InternalCommunication &communication_;

};