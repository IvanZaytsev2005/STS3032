#include "stdint.h"

#define BlockRead

enum STSRegisters
{
    FIRMWARE_MAJOR           = 0x00,
    FIRMWARE_MINOR           = 0x01,
    SERVO_MAJOR              = 0x03,
    SERVO_MINOR              = 0x04,
    ID                       = 0x05,
    BAUDRATE                 = 0x06,
    RESPONSE_DELAY           = 0x07,
    RESPONSE_STATUS_LEVEL    = 0x08,
    MINIMUM_ANGLE            = 0x09,
    MAXIMUM_ANGLE            = 0x0B,
    MAXIMUM_TEMPERATURE      = 0x0D,
    MAXIMUM_VOLTAGE          = 0x0E,
    MINIMUM_VOLTAGE          = 0x0F,
    MAXIMUM_TORQUE           = 0x10,
    UNLOADING_CONDITION      = 0x13,
    LED_ALARM_CONDITION      = 0x14,
    POS_PROPORTIONAL_GAIN    = 0x15,
    POS_DERIVATIVE_GAIN      = 0x16,
    POS_INTEGRAL_GAIN        = 0x17,
    MINIMUM_STARTUP_FORCE    = 0x18,
    CK_INSENSITIVE_AREA      = 0x1A,
    CCK_INSENSITIVE_AREA     = 0x1B,
    CURRENT_PROTECTION_TH    = 0x1C,
    ANGULAR_RESOLUTION       = 0x1E,
    POSITION_CORRECTION      = 0x1F,
    OPERATION_MODE           = 0x21,
    TORQUE_PROTECTION_TH     = 0x22,
    TORQUE_PROTECTION_TIME   = 0x23,
    OVERLOAD_TORQUE          = 0x24,
    SPEED_PROPORTIONAL_GAIN  = 0x25,
    OVERCURRENT_TIME         = 0x26,
    SPEED_INTEGRAL_GAIN      = 0x27,
    TORQUE_SWITCH            = 0x28,
    TARGET_ACCELERATION      = 0x29,
    TARGET_POSITION          = 0x2A,
    RUNNING_TIME             = 0x2C,
    RUNNING_SPEED            = 0x2E,
    TORQUE_LIMIT             = 0x30,
    WRITE_LOCK               = 0x37,
    CURRENT_POSITION         = 0x38,
    CURRENT_SPEED            = 0x3A,
    CURRENT_DRIVE_VOLTAGE    = 0x3C,
    CURRENT_VOLTAGE          = 0x3E,
    CURRENT_TEMPERATURE      = 0x3F,
    ASYNCHRONOUS_WRITE_ST    = 0x40,
    STATUS                   = 0x41,
    MOVING_STATUS            = 0x42,
    CURRENT_CURRENT          = 0x45,
};

typedef struct STS3032 {
    uint8_t (*transmit) (uint8_t* data_out, uint8_t Len);
    #ifdef BlockRead
    // first mode - blocking mode, wait for responce
    uint8_t (*recieve) (uint8_t* data_in, uint8_t Len);
    #else
    uint8_t* data_in;
    #endif
}Sts3032connect_t;

uint8_t PingWrite(uint8_t ID, Sts3032connect_t* sts);
void Write(uint8_t ID, uint8_t command, uint8_t size, uint8_t *param, Sts3032connect_t* sts);
uint8_t SetPosition(uint8_t ID, uint16_t pos, Sts3032connect_t* sts);
uint8_t SetMode(uint8_t ID, uint8_t Mode, Sts3032connect_t* sts);
uint8_t SetVelocite(uint8_t ID, uint16_t Speed, Sts3032connect_t* sts);
uint8_t SetAcceleration(uint8_t ID, uint16_t Accel, Sts3032connect_t* sts);
uint8_t ChecSumm(uint8_t *messag);