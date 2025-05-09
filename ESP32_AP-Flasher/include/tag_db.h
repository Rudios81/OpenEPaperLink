#include <Arduino.h>
#include <ArduinoJson.h>

#include <unordered_map>
#include <vector>

#pragma pack(push, 1)
#pragma once

#define RUNSTATUS_STOP 0
#define RUNSTATUS_PAUSE 1
#define RUNSTATUS_RUN 2
#define RUNSTATUS_INIT 3

#define NO_SUBGHZ_CHANNEL  255
class tagRecord {
   public:
    tagRecord() : mac{0}, version(0), alias(""), lastseen(0), nextupdate(0), contentMode(0), pendingCount(0), md5{0}, expectedNextCheckin(0), modeConfigJson(""), LQI(0), RSSI(0), temperature(0), batteryMv(0), hwType(0), wakeupReason(0), capabilities(0), lastfullupdate(0), isExternal(false), apIp(IPAddress(0, 0, 0, 0)), pendingIdle(0), rotate(0), lut(0), tagSoftwareVersion(0), currentChannel(0), dataType(0), filename(""), data(nullptr), len(0), invert(0), updateCount(0), updateLast(0) {}

    uint8_t mac[8];
    uint8_t version;
    String alias;
    uint32_t lastseen;
    uint32_t nextupdate;
    uint8_t contentMode;
    uint16_t pendingCount;
    uint8_t md5[16];
    uint32_t expectedNextCheckin;
    String modeConfigJson;
    uint8_t LQI;
    int8_t RSSI;
    int8_t temperature;
    uint16_t batteryMv;
    uint8_t hwType;
    uint8_t wakeupReason;
    uint8_t capabilities;
    uint32_t lastfullupdate;
    bool isExternal;
    IPAddress apIp;
    uint16_t pendingIdle;
    uint8_t rotate;
    uint8_t lut;
    uint16_t tagSoftwareVersion;
    uint8_t currentChannel;
    uint8_t invert;
    uint32_t updateCount;
    uint32_t updateLast;

    uint8_t dataType;
    String filename;
    uint8_t* data;
    uint32_t len;

    static tagRecord* findByMAC(const uint8_t mac[8]);
};

struct Config {
    uint8_t channel;
    uint8_t subghzchannel;
    char alias[32];
    uint8_t led;
    uint8_t tft;
    uint8_t language;
    uint8_t maxsleep;
    uint8_t stopsleep;
    volatile uint8_t runStatus;
    uint8_t preview;
    uint8_t nightlyreboot;
    uint8_t lock;
    uint8_t wifiPower;
    char timeZone[52];
    uint8_t sleepTime1;
    uint8_t sleepTime2;
    uint8_t ble;
    uint8_t discovery;
    String repo;
    String env;
    uint8_t showtimestamp;
};

struct Color {
    uint8_t r, g, b;
    Color() : r(0), g(0), b(0) {}
    Color(uint16_t value_) : r((value_ >> 8) & 0xF8 | (value_ >> 13) & 0x07), g((value_ >> 3) & 0xFC | (value_ >> 9) & 0x03), b((value_ << 3) & 0xF8 | (value_ >> 2) & 0x07) {}
    Color(uint8_t r_, uint8_t g_, uint8_t b_) : r(r_), g(g_), b(b_) {}
};

struct HwType {
    uint8_t id;
    uint16_t width;
    uint16_t height;
    uint8_t rotatebuffer;
    uint8_t bpp;
    uint8_t shortlut;
    uint8_t zlib;
    uint8_t g5;
    uint16_t highlightColor;
    std::vector<Color> colortable;
};

struct varStruct {
    String value;
    bool changed;
};

extern Config config;
extern std::vector<tagRecord*> tagDB;
extern std::unordered_map<int, HwType> hwtype;
extern std::unordered_map<std::string, varStruct> varDB;
extern String tagDBtoJson(const uint8_t mac[8] = nullptr, uint8_t startPos = 0);
extern bool deleteRecord(const uint8_t mac[8], bool allVersions = true);
extern void fillNode(JsonObject& tag, const tagRecord* taginfo);
extern void saveDB(const String& filename);
extern bool loadDB(const String& filename);
extern void destroyDB();
extern uint32_t getTagCount();
extern uint32_t getTagCount(uint32_t& timeoutcount, uint32_t& lowbattcount);
extern void mac2hex(const uint8_t* mac, char* hexBuffer);
extern bool hex2mac(const String& hexString, uint8_t* mac);
extern void clearPending(tagRecord* taginfo);
extern void initAPconfig();
extern void saveAPconfig();
extern HwType getHwType(const uint8_t id);

/// @brief Update a variable with the given key and value
///
/// @param key Variable key
/// @param value Variable value
/// @param notify Should the change be notified (true, default) or not (false)
/// @return true If variable was created/updated
/// @return false If not
extern bool setVarDB(const std::string& key, const String& value, const bool notify = true);

extern void cleanupCurrent();
extern void pushTagInfo(tagRecord* taginfo);
extern void popTagInfo(const uint8_t mac[8] = nullptr);

#pragma pack(pop)
