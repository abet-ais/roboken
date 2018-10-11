#ifndef _rotary_encoder_H_
#define _rotary_encoder_H_

#include "mbed.h"


#define PREV_MASK 0x1 
#define CURR_MASK 0x2 
#define INVALID   0x3 
class rotary_encoder {

public:

    typedef enum Encoding {

        X2_ENCODING,
        X4_ENCODING

    } Encoding;


    rotary_encoder(PinName channelA, PinName channelB, PinName index, int pulsesPerRev, Encoding encoding = X2_ENCODING);

    void reset(void);
    int getCurrentState(void);
    int getPulses(void);
    float getDeg(void);
    float getRad(void);

private:


    void encode(void);
    void index(void);
    Encoding encoding_;
    InterruptIn channelA_;
    InterruptIn channelB_;
    InterruptIn index_;

    int          pulsesPerRev_;
    int          prevState_;
    int          currState_;
    volatile int pulses_;
    volatile int revolutions_;
    int          magnification; //角度の分解能を決めるために必要な変数
    float        deg;
    float        rad;

};

#endif 
