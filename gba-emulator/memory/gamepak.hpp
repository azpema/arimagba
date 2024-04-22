#ifndef _GAMEPAK_ 
#define _GAMEPAK_ 

#include <string>
#include <fstream>
#include <iostream>

class GamePak {
	public:
		GamePak(std::string filePath);
		~GamePak();
		uint32_t read(uint32_t addr, uint8_t bytes);

		int readHeader();
		int readEntryPoint();
		int readNintendoLogo();
		int readGameTitle();
		int readGameCode();
		int readMakerCode();
		int readFixedValue();
		int readMainUnitCode();
		int readDeviceType();
		int readReservedArea1();
		int readSoftwareVersion();
		int readComplementCheck();
		int readReservedArea2();
		int calcComplementCheck();

		void printHeader();

	private:
		uint32_t fileSize;
		std::ifstream fileStream;

		uint16_t *gameMem;

		const static int ENTRY_POINT_OFF = 0x000;
		const static int ENTRY_POINT_SIZE = 4;
		const static int NINTENDO_LOGO_OFF = 0x004;
		const static int NINTENDO_LOGO_SIZE = 156;
		const static int GAME_TITLE_OFF = 0x0A0;
		const static int GAME_TITLE_SIZE = 12;
		const static int GAME_CODE_OFF = 0x0AC;
		const static int GAME_CODE_SIZE = 4;
		const static int MAKER_CODE_OFF = 0x0B0;
		const static int MAKER_CODE_SIZE = 2;
		const static int FIXED_VALUE_OFF = 0x0B2;
		const static int FIXED_VALUE_SIZE = 1;
		const static int MAIN_UNIT_CODE_OFF = 0x0B3;
		const static int MAIN_UNIT_CODE_SIZE = 1;
		const static int DEVICE_TYPE_OFF = 0x0B4;
		const static int DEVICE_TYPE_SIZE = 1;
		const static int RESERVED_AREA_1_OFF = 0x0B5;
		const static int RESERVED_AREA_1_SIZE = 7;
		const static int SOFTWARE_VERSION_OFF = 0x0BC;
		const static int SOFTWARE_VERSION_SIZE = 1;
		const static int COMPLEMENT_CHECK_OFF = 0x0BD;
		const static int COMPLEMENT_CHECK_SIZE = 1;
		const static int RESERVED_AREA_2_OFF = 0x0BE;
		const static int RESERVED_AREA_2_SIZE = 2;

		uint32_t entryPoint;
		uint8_t nintendoLogo[GamePak::NINTENDO_LOGO_SIZE];
		uint8_t gameTitle[GamePak::GAME_TITLE_SIZE + 1];
		uint8_t gameCode[GamePak::GAME_CODE_SIZE + 1];
		uint8_t makerCode[GamePak::MAKER_CODE_OFF + 1];
		uint8_t fixedValue;
		uint8_t mainUnitCode;
		uint8_t deviceType;
		uint8_t reservedArea1[GamePak::RESERVED_AREA_1_SIZE];
		uint8_t softwareVersion;
		uint8_t complementCheck;
		uint8_t reservedArea2[GamePak::RESERVED_AREA_2_SIZE];

		// TODO: Multiboot header
};

#endif