#pragma once
#include <cstdint>
#include "../st_device.h"
#include "../../gpio.h"
#include "../../torque_sensor.h"
#include "i2c_dma.h"

// A two reading torque source
class I2CTorque final : public TorqueSensorBase {
 public:
    I2CTorque(I2C_DMA i2c, uint8_t address = 0, uint8_t decimation = 50) : 
        i2c_(i2c), decimation_(decimation) {
            address_ = 0x28 + address;
    }

    void trigger() {
        count_++;
        if (count_ < decimation_) {
            return;
        }
        count_ = 0;

       data_out_[0] = 0x40;
       i2c_.write(address_, 1, data_out_);
       i2c_.read(address_, 6*4, data_in_);
    }

    float read() {
        if (count_ == 0) {
            // wait until dma complete
            while(!i2c_.ready())

            // process result
            result0_ = (uint32_t) data_in_[3] << 24 | (uint32_t) data_in_[2] << 16 | (uint16_t) data_in_[1] << 8 | data_in_[0];
            result1_ = (uint32_t) data_in_[23] << 24 | (uint32_t) data_in_[22] << 16 | (uint16_t) data_in_[21] << 8 | data_in_[20];
            int32_t diff = result0_ - result1_;
            sum_ = result0_ + result1_;
            float sum = (float) result0_ + (float) result1_;
            float tcomp = sum * k_temp_;
            if (sum != 0) {
                torque_ = diff/sum * gain_ + bias_ + tcomp;
            }
        }
        return torque_;
    }


 //private:
    I2C_DMA &i2c_;
    uint8_t address_;
    uint8_t data_out_[1] = {};
    uint8_t data_in_[6*4] = {};
    uint32_t result0_ = 0;
    uint32_t result1_ = 0;
    uint32_t sum_;
    float torque_ = 0;
    uint8_t count_ = 0;
    uint8_t decimation_;

    friend class System;
};