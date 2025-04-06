#include "sts3032.h"
#include "stdint.h"

uint8_t BufOut[256];

uint8_t PingWrite(uint8_t ID, uint8_t* Ret, Sts3032connect_t* sts)
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
    Ret[0] = 0;
    sts->receive(data_in, 6);
    if((data_in[0] ==0xFF) & (data_in[2] == ID))
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
    uint8_t data_out[12];
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

void Read(uint8_t ID, uint8_t Addr, uint8_t size, uint8_t* buf, Sts3032connect_t* sts) {
    uint8_t data_out[12];
    data_out[0] = 0xFF;
    data_out[1] = 0xFF;
    data_out[2] = ID;
    data_out[3] = 4;
    data_out[4] = 2;
    data_out[5] = Addr;
    data_out[6] = size;
    ChecSumm(data_out);
    sts->transmit(data_out, 8);
    sts->receive(buf, 6 + size);
}

void WriteRegister(uint8_t ID, uint8_t Addr, uint8_t size, int16_t Data, Sts3032connect_t* sts) {
    uint8_t buf[2];
    uint16_t DataAbs;
    DataAbs = abs(Data);
    if(Data < 0) {
        DataAbs |= 0x8000;
    }
    buf[1] = (uint8_t)((DataAbs >> 8) & 0xFF);
    buf[0] = (uint8_t)(DataAbs & 0xFF);
    Write(ID, Addr, size, buf, sts);
}

uint16_t ReadRegister(uint8_t ID, uint8_t Addr, uint8_t size, Sts3032connect_t* sts) {
    uint8_t buf[16];
    uint16_t data;
    Read(ID, Addr, size, buf, sts);
    data = buf[5];
    if(size == 2) {
        //data = data << 8;
        data += buf[6] << 8;
    }
    return (data);
}

void Action(uint8_t ID, Sts3032connect_t* sts)
{
    uint8_t data_out[12];
    data_out[0] = 0xFF;
    data_out[1] = 0xFF;
    data_out[2] = ID;
    data_out[3] = 2;
    data_out[4] = 5;
    ChecSumm(data_out);
    sts->transmit(data_out,6);
}

uint8_t SetPosition(uint8_t ID, uint16_t pos, Sts3032connect_t* sts) {
    uint8_t buf[2];
    buf[1] = (uint8_t)(pos >> 8);
    buf[0] = (uint8_t)(pos & 0xFF);
    Write(ID, TARGET_POSITION, 2, buf, sts);
}

uint8_t SetMode(uint8_t ID, uint8_t Mode, Sts3032connect_t* sts) {
    uint8_t buf;
    buf = Mode;
    Write(ID, OPERATION_MODE, 1, &buf, sts);
}

uint8_t LockingEprom(uint8_t ID, uint8_t Lock, Sts3032connect_t* sts) {//1 - Locked/ 0 - Unlock
    uint8_t buf;
    buf = Lock;
    Write(ID, WRITE_LOCK, 1, &buf, sts);
}

uint8_t SetID(uint8_t LastID, uint8_t NewID, Sts3032connect_t* sts) {
    uint8_t buf;
    buf = NewID;
    Write(LastID, ID, 1, &buf, sts);
}

uint8_t SetVelocite(uint8_t ID, int16_t Speed, Sts3032connect_t* sts) {
    uint8_t buf[2];
    uint16_t SpeedAbs;
    SpeedAbs = abs(Speed);
    if(Speed < 0) {
        SpeedAbs |= 0x8000;
    }
    buf[1] = (uint8_t)((SpeedAbs >> 8) & 0xFF);
    buf[0] = (uint8_t)(SpeedAbs & 0xFF);
    Write(ID, RUNNING_SPEED, 2, buf, sts);
}

uint8_t SetAcceleration(uint8_t ID, uint8_t Accel, Sts3032connect_t* sts) {
    uint8_t buf[2];
    buf[0] = Accel;
    Write(ID, TARGET_ACCELERATION, 1, buf, sts);
}

void SetTorque(uint8_t ID, uint16_t torque, Sts3032connect_t* sts) {
    WriteRegister(ID, TORQUE_LIMIT, 2, (int16_t)torque, sts);
}

void Reset(uint8_t ID, Sts3032connect_t* sts) {
    uint8_t data_out[12];
    data_out[0] = 0xFF;
    data_out[1] = 0xFF;
    data_out[2] = ID;
    data_out[3] = 2;
    data_out[4] = 0x0A;
    ChecSumm(data_out);
    sts->transmit(data_out,6);
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