#pragma once

#include "communication.h"

class InternalCommunication : public CommunicationBase {
 public:
    int receive_data(ReceiveData * const data) { 
        if (receive_data_received_) {
            *data = receive_data_;
            receive_data_received_ = false;
            return sizeof(ReceiveData);
        } 
        return 0;
    }
    void send_data(const SendData &data) { send_data_ = data;}

    // call from non interrupt context at low frequency than receiver
    void set_receive_data(ReceiveData * const data) { 
        receive_data_ = *data; 
        receive_data_received_ = true;
    }

    // todo this would need a buffer, but right now I don't care too much about
    // send data
    const SendData & get_send_data() const { return send_data_; }

 private:
    bool receive_data_received_ = true;
    ReceiveData receive_data_ = {};
    SendData send_data_ = {};
};
