#include "sts3032.h"
#include "stdint.h"

uint8_t BufOut[256];

uint8_t PingWrite(uint8_t ID, Sts3032connect_t* sts)
{
    uint8_t data_out[6];
    data_out[0] = 0xFF;
    data_out[1] = 0xFF;
    data_out[2] = ID;
    data_out[3] = 2;
    data_out[4] = 1;
    ChecSumm(data_out);
    sts->transmit(data_out, 6);
    uint8_t data_in[6];
    sts->recieve(data_in, 6);
    if(data_in[2] == ID)
    {
        return(1);
    }
    else
    {
        return(0);
    }
}

void Write(uint8_t ID, uint8_t command, uint8_t size, uint8_t *param, Sts3032connect_t* sts)
{
    uint8_t data_out[10];
    data_out[0] = 0xFF;
    data_out[1] = 0xFF;
    data_out[2] = ID;
    data_out[3] = size + 3;
    data_out[4] = 3;
    data_out[5] = command;
    data_out[6] = *param;
    if(size==2)
    {
        param++;
        data_out[7] = *param;
    }
    ChecSumm(data_out);
    sts->transmit(data_out, size+7);
}

uint8_t SetPosition(uint8_t ID, uint16_t pos, Sts3032connect_t* sts) {
    uint8_t buf[2];
    buf[0] = (uint8_t)(pos >> 8);
    buf[1] = (uint8_t)(pos & 0xFF);
    Write(ID, TARGET_POSITION, 2, buf, sts);
}

uint8_t SetMode(uint8_t ID, uint8_t Mode, Sts3032connect_t* sts) {
    uint8_t buf;
    buf = Mode;
    Write(ID, OPERATION_MODE, 1, &buf, sts);
}

uint8_t SetVelocite(uint8_t ID, uint16_t Speed, Sts3032connect_t* sts) {
    uint8_t buf[2];
    buf[0] = (uint8_t)(Speed >> 8);
    buf[1] = (uint8_t)(Speed & 0xFF);
    Write(ID, RUNNING_SPEED, 2, buf, sts);
}

uint8_t SetAcceleration(uint8_t ID, uint16_t Accel, Sts3032connect_t* sts) {
    uint8_t buf[2];
    buf[0] = (uint8_t)(Accel >> 8);
    buf[1] = (uint8_t)(Accel & 0xFF);
    Write(ID, RUNNING_SPEED, 2, buf, sts);
}

uint8_t ChecSumm(uint8_t *messag)
{
    // uint8_t T;
    uint32_t summ=0;
    messag+=2;
    summ += *messag;
    messag++;
    summ += *messag;
    uint8_t l = *messag;
    // T = sprintf(BufOut, "%d\n\r", summ);
    // SerialWrite(BufOut, T);
    messag++;
    for(uint8_t i=0; i<(l-1); i++)
    {
        summ += *messag;
        // T = sprintf(BufOut, "%d\n\r", *messag);
        // SerialWrite(BufOut, T);
        messag++;
        
    }
    *messag = (uint8_t)(~(summ&0xFF));
    // T = sprintf(BufOut, "%d\n\r", *messag);
    // SerialWrite(BufOut, T);
}