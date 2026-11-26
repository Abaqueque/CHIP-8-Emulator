#pragma once

#include <cstdint>
#include <array>
#include <string>


class Chip8 {
public:
	void reset();

	bool loadROM(const std::string& path);

	void cycle();

	void test();

private:
	std::array<uint8_t, 4096> memory{};
	std::array<uint8_t, 16> V{};

	uint16_t PC = 0x200;
	uint16_t I = 0;
	
	std::array<uint16_t, 16> stack{};
	uint8_t stack_ptr = 0;
	uint8_t delay_timer = 0;
	uint8_t sound_timer = 0;

	std::array<bool, 64 * 32> display{};

	uint16_t fetch();

	void execute(uint16_t opcode);

	void assign_opcode(uint16_t address, uint16_t opcode);
};