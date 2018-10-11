#include "rotary_encoder.h"

rotary_encoder::rotary_encoder(
         PinName channelA,
         PinName channelB,
         PinName index,
         int pulsesPerRev,
         Encoding encoding) : channelA_(channelA), channelB_(channelB),
        index_(index) {

    pulses_       = 0;
    revolutions_  = 0;
    pulsesPerRev_ = pulsesPerRev;
    encoding_     = encoding;
    deg           = 0.0;
    rad           = 0.0;

    int chanA = channelA_.read();
    int chanB = channelB_.read();

    currState_ = (chanA << 1) | (chanB);
    prevState_ = currState_;

    channelA_.rise(this, &rotary_encoder::encode);
    channelA_.fall(this, &rotary_encoder::encode);

    if (encoding == X4_ENCODING) {
        channelB_.rise(this, &rotary_encoder::encode);
        channelB_.fall(this, &rotary_encoder::encode);
        magnification=4;
    }
    else{
        magnification=2;
    }

    if (index !=  NC) {
        index_.rise(this, &rotary_encoder::index);
    }

}

void rotary_encoder::reset(void) {

    pulses_      = 0;
    revolutions_ = 0;
    deg          = 0.0;
    rad          = 0.0;

}

int rotary_encoder::getCurrentState(void) {

    return currState_;

}

int rotary_encoder::getPulses(void) {

    return pulses_;

}

float rotary_encoder::getDeg(void) {
    float resolution_deg = (360.0/((float)pulsesPerRev_*magnification));//分解能
    
    deg = pulses_*resolution_deg;
    
    return deg;

}

float rotary_encoder::getRad(void){
    float resolution_rad = (2*3.14159/((float)pulsesPerRev_*magnification));//分解能
    
    rad = pulses_*resolution_rad;
    
    return rad;
}

    

void rotary_encoder::encode(void) {

    int change = 0;
    int chanA  = channelA_.read();
    int chanB  = channelB_.read();

    currState_ = (chanA << 1) | (chanB);

    if (encoding_ == X2_ENCODING) {
        if ((prevState_ == 0x3 && currState_ == 0x0) ||
                (prevState_ == 0x0 && currState_ == 0x3)) {
            pulses_++;
        }
        else if ((prevState_ == 0x2 && currState_ == 0x1) ||
                 (prevState_ == 0x1 && currState_ == 0x2)) {
            pulses_--;
        }
    } else if (encoding_ == X4_ENCODING) {
        if (((currState_ ^ prevState_) != INVALID) && (currState_ != prevState_)) {
            change = (prevState_ & PREV_MASK) ^ ((currState_ & CURR_MASK) >> 1);
            if (change == 0) {
                change = -1;
            }
           pulses_ -= change;
        }
    }
    prevState_ = currState_;
}


void rotary_encoder::index(void) {

    revolutions_++;

}
