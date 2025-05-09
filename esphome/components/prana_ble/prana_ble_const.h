#pragma once

#include <set>

namespace esphome {
namespace prana_ble {

static const char *const TAG = "prana_ble";

static const uint16_t PRANA_MAGIC = 0xEFBE;
static const uint8_t PRANA_CMD_PREFIX = 0x04;
static const uint8_t PRANA_STATE[] = {0xBE, 0xEF, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x5A};

//Only new devices - direct fan speed control
//inlet fan
static const uint8_t cmd_fan_in_speed[10] = {
  0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28
};

//outlet fan
static const uint8_t cmd_fan_out_speed[10] = {
  0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32
};

//both fans
static const uint8_t cmd_fan_speed[10] = {
  0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C
};

struct PranaCmdPacket
{
  uint16_t magic;
  uint8_t prefix;
  uint8_t command;

  PranaCmdPacket(uint8_t command)
  {
    magic = PRANA_MAGIC;
    prefix = PRANA_CMD_PREFIX;
    this->command = command;
  }
} __attribute__((packed));

enum PranaFan : uint8_t {
  FAN_IN          = 0x01,
  FAN_OUT         = 0x02,
  FAN_BOTH        = 0x05
};

enum PranaFanMode : uint8_t {
  Manual,
  Auto,
  Auto_Plus
};

enum PranaDisplayMode : uint8_t {
  FAN         = 0,
  TEMP_IN     = 1,
  TEMP_OUT    = 2,
  CO2         = 3,
  VOC         = 4,
  HUMIDITY    = 5,
  AIR_QUALITY = 6,
  PRESSURE    = 7,
  UNUSED      = 8,
  DATE        = 9,
  TIME        = 10,
  MODE_COUNT  = 11
};

enum PranaCommand : uint8_t {
  CMD_POWER_OFF           = 0x01,
  CMD_BRIGHTNESS          = 0x02,
  CMD_HEATING             = 0x05,
  CMD_NIGHT_MODE          = 0x06,
  CMD_HIGH_SPEED          = 0x07,
  CMD_FAN_LOCK            = 0x09,
  CMD_FAN_OUT_OFF         = 0x10,
  CMD_FAN_OUT_SPEED_UP    = 0x11,
  CMD_FAN_OUT_SPEED_DOWN  = 0x12,
  CMD_WINTER_MODE         = 0x16,
  CMD_AUTO_MODE           = 0x18,
  CMD_POWER_ON            = 0x0A,
  CMD_FAN_SPEED_DOWN      = 0x0B,
  CMD_FAN_SPEED_UP        = 0x0C,
  CMD_FAN_IN_SPEED_UP     = 0x0E,
  CMD_FAN_IN_SPEED_DOWN   = 0x0F,
  CMD_FAN_IN_OFF          = 0x0D,
  CMD_DISPLAY_LEFT        = 0x19,
  CMD_DISPLAY_RIGHT       = 0x1A,



  //inlet fan
  CMD_FAN_IN_SPEED1     = 0x1F,
  CMD_FAN_IN_SPEED2     = 0x20,
  CMD_FAN_IN_SPEED3     = 0x21,
  CMD_FAN_IN_SPEED4     = 0x22,
  CMD_FAN_IN_SPEED5     = 0x23,
  CMD_FAN_IN_SPEED6     = 0x24,
  CMD_FAN_IN_SPEED7     = 0x25,
  CMD_FAN_IN_SPEED8     = 0x26,
  CMD_FAN_IN_SPEED9     = 0x27,
  CMD_FAN_IN_SPEED10    = 0x28,

  //outlet fan
  CMD_FAN_OUT_SPEED1    = 0x29,
  CMD_FAN_OUT_SPEED2    = 0x2A,
  CMD_FAN_OUT_SPEED3    = 0x2B,
  CMD_FAN_OUT_SPEED4    = 0x2C,
  CMD_FAN_OUT_SPEED5    = 0x2D,
  CMD_FAN_OUT_SPEED6    = 0x2E,
  CMD_FAN_OUT_SPEED7    = 0x2F,
  CMD_FAN_OUT_SPEED8    = 0x30,
  CMD_FAN_OUT_SPEED9    = 0x31,
  CMD_FAN_OUT_SPEED10   = 0x32,

  //both fans
  CMD_FAN_SPEED1        = 0x33,
  CMD_FAN_SPEED2        = 0x34,
  CMD_FAN_SPEED3        = 0x35,
  CMD_FAN_SPEED4        = 0x36,
  CMD_FAN_SPEED5        = 0x37,
  CMD_FAN_SPEED6        = 0x38,
  CMD_FAN_SPEED7        = 0x39,
  CMD_FAN_SPEED8        = 0x3A,
  CMD_FAN_SPEED9        = 0x3B,
  CMD_FAN_SPEED10       = 0x3C
};


struct PranaStatusPacket {
  uint16_t magic;
  uint8_t prefix[2];
  uint32_t timestamp;
  uint8_t suffix;
  uint8_t unused1; // C0
  bool enabled : 8;//[10]
  uint8_t unused2;
  uint8_t brightness : 8;//12
  uint8_t unused3;
  bool heating_on : 8; //14
  uint8_t unused4;
  bool night_mode : 8; //16
  uint8_t unused11;
  uint8_t unknown1[2];
  PranaFanMode fan_mode; //20
  uint8_t unused5;
  bool fans_locked : 8; //22
  uint8_t unknown2[3];
  uint8_t speed;//26
  uint8_t unused6;
  bool input_enabled : 8;//28
  uint8_t unknown3;
  uint8_t speed_in;//30
  uint8_t unused7;
  bool output_enabled : 8;//32
  uint8_t unused8;
  uint8_t speed_out;//34
  uint8_t unknown4[6];//35-40
  uint8_t unused9;
  bool winter_mode : 8;//42
  uint8_t unknown5[5];//47
  short temp_inside_in;//48-49
  uint8_t unknown6;
  short temp_outside_in;//51-52 //short?
  uint8_t unknown7;

  short temp_inside_out;

  uint8_t unknown8;//56
  short temp_outside_out; //57-58
  uint8_t unknown9;//59
  uint8_t humidity;//60
  short co2;//61-62 //short?
  short voc;//63-64
  uint8_t unknown10[13];//65-77
  uint8_t pressure;//78
  uint8_t unknown11[15];//79-93
  uint8_t unused10;//C0
  uint8_t unknown12;//95
  uint8_t voltage;//96
  uint8_t frequency;//97
  uint8_t unknown13;//98
  uint8_t display_mode;//99
}  __attribute__((packed));


static const std::vector<std::string> PRANA_FAN_MODES {"Manual", "Auto", "Auto+"};
static const std::vector<std::string> PRANA_DISPLAY_MODES {"Fan", "Temp inside", "Temp outside", "CO2", "VOC", "Humidity", "Air quality", "Pressure", "unused", "Date", "Time"};

}  // namespace prana_ble
}  // namespace esphome
