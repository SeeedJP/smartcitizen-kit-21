#pragma once

#include <Arduino.h>
#include <RTCZero.h>
#include <SPI.h>
#include "ArduinoLowPower.h"
#include "SdFat.h"
#include <SoftwareSerial.h>

#include "SckLed.h"
#include "SckPot.h"
#include "Shared.h"
#include "Config.h"
#include "Commands.h"
#include "Sensors.h"
#include "SckUrban.h"

// Output
enum OutLevels { OUT_SILENT, OUT_NORMAL, OUT_VERBOSE, OUT_COUNT	};
enum PrioLevels { PRIO_LOW, PRIO_MED, PRIO_HIGH };

// Forward declaration
// class AllCommands;

class SckBase {
private:

	const uint8_t pinPwrSave = 38; 		// PA13 -- TPS63001 PS/SYNC (Enable/disable power-save mode (1 disabled, 0 enabled)

	// Input
	String serialBuff;

	// Button
	const uint8_t pinBUTTON = 7;		// PA21
	const uint16_t buttonLong = 5000;
	const uint16_t buttonVeryLong = 13000;
	enum ButtonStage { BUT_STARTED, BUT_LONG_PRESS, BUT_VERY_LONG };
	ButtonStage buttonStage = BUT_STARTED;
	bool alarmRunning_TC4 = false;
	uint32_t buttonLastEvent = 0;
	
	// Configuration
	Configuration config;

	// ESP8266
	const uint8_t pinPOWER_ESP = 30;	// PB22
	const uint8_t pinESP_CH_PD = 17;	// PA04 - A3
	const uint8_t pinESP_GPIO0 = 18;	// PA05 - A4
	// const uint32_t ESP_FLASH_SPEED = 921600;
	const uint32_t ESP_FLASH_SPEED = 115200;

	// Sd card (to be revised)
	const uint8_t pinCS_SDCARD = 2;		// PA14 -- SPI Select SDcard
	// SdFat sd;

	// Urban board
	bool urbanPresent = false;
	SckUrban urban;
	bool urbanBoardDetected();

	// To ORGANIZE
	bool onUSB = true;



public:

	void setup();
	void update();

	// Peripherals
	SckLed led;
	friend class SckButton;
	RTCZero rtc;

	// Input
	void inputUpdate();

	// Output
	const char *outLevelTitles[OUT_COUNT] PROGMEM = { "Silent",	"Normal", "Verbose"	};
	OutLevels outputLevel = OUT_VERBOSE;
	char outBuff[240];
	void sckOut(String strOut, PrioLevels priority=PRIO_MED, bool newLine=true);	// Accepts String object
	void sckOut(const char *strOut, PrioLevels priority=PRIO_MED, bool newLine=true);	// Accepts constant string
	void sckOut(PrioLevels priority=PRIO_MED, bool newLine=true);
	void prompt();

	// Button
	void buttonEvent();
	void buttonStillDown();
	void setAlarm_TC4(uint16_t lapse=0);

	// Commands
	AllCommands commands;

	// ESP control
	enum ESPcontrols { ESP_OFF, ESP_FLASH, ESP_ON, ESP_REBOOT };
	void ESPcontrol(ESPcontrols myESPControl);
	bool flashingESP = false;
	uint32_t espStarted;
	void reset();

};

void ISR_button();

