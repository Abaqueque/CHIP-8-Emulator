#include "Chip8.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>


bool Chip8::loadROM(const std::string& path) {
	
	
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	
	if (!file) {
		std::cout << "ROM could not be opened\n";
		return false;
	}


	auto size = file.tellg();
	
	if (size < 0 || size > 3584) {
		std::cout << "ROM exceeds permissible size\n";
		return false;
	}

	reset();

	file.seekg(0, std::ios::beg);

	char* destination_ptr = reinterpret_cast<char*>(memory.data() + 0x200);

	if (!file.read(destination_ptr, size)) {
		std::cout << "ROM could not be written to memory\n";
		return false;
	}

	return true;

}


void Chip8::reset() {
	I = 0;
	PC = 0x200;

	memory.fill(0);
	V.fill(0);
	stack.fill(0);
	display.fill(0);

	delay_timer = 0;
	sound_timer = 0;
	stack_ptr = 0;
}


uint16_t Chip8::fetch() {
	if (PC > 0xFFE) {
		throw std::runtime_error("PC out of bounds\n");
	}

	uint16_t opcode = (static_cast<uint16_t>(memory[PC]) << 8) | memory[PC + 1];
	
	std::cout << "PC: 0x" << std::uppercase << std::hex << PC << "\n";
	std::cout << "Opcode: 0x" << std::uppercase << std::hex << opcode << "\n";

	PC += 2;

	return opcode;
}

void Chip8::execute(uint16_t opcode) {
	uint8_t op = (opcode & 0xF000) >> 12;
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;
	uint8_t N = opcode & 0x000F;
	uint8_t NN = opcode & 0x00FF;
	uint16_t NNN = opcode & 0x0FFF;
	

	switch (op) {
	case 0x6:
		V[X] = NN;
		break;
	case 0x7:
		V[X] += NN;
		break;
	case 0x1:
		PC = NNN;
		break;
	case 0xA:
		I = NNN;
		break;
	case 0x3:
		if (V[X] == NN)
			PC += 2;
		break;
	case 0x4:
		if (V[X] != NN)
			PC += 2;
		break;
	case 0x5:
		if (N != 0)
			throw std::runtime_error("Unknown 0x5XY0 opcode\n");
		if (V[X] == V[Y])
			PC += 2;
		break;
	case 0x9:
		if (N != 0)
			throw std::runtime_error("Unknown 0x9XY0 opcode\n");
		if (V[X] != V[Y])
			PC += 2;
		break;
	case 0x2:
		if (stack_ptr >= 16)
			throw std::runtime_error("Stack overflow\n");

		stack[stack_ptr] = PC;
		stack_ptr += 1;
		PC = NNN;
		break;
	case 0x0:
		if (opcode != 0x00EE)
			throw std::runtime_error("Unknown 0x00EE opcode\n");
		if (stack_ptr <= 0)
			throw std::runtime_error("Stack underflow\n");
		stack_ptr -= 1;
		PC = stack[stack_ptr];
		break;
	case 0x8:
		switch (N) {
		case 0:
			V[X] = V[Y];
			break;
		case 1:
			V[X] = V[X] | V[Y];
			break;
		case 2:
			V[X] = V[X] & V[Y];
			break;
		case 3:
			V[X] = V[X] ^ V[Y];
			break;
		default:
			throw std::runtime_error("Unknown 0x8XYN opcode\n");
			break;
		}
		break;
	default:
		throw std::runtime_error("Unknown opcode\n");
		break;
	}

	//std::cout << "Operation: 0x" << std::uppercase << std::hex << static_cast<int>(op) << "\n";
	//std::cout << "X: 0x" << std::uppercase << std::hex << static_cast<int>(X) << "\n";
	//std::cout << "NN: 0x" << std::uppercase << std::hex << static_cast<int>(NN) << "\n";
}


void Chip8::cycle() {
	uint16_t opcode = fetch();
	execute(opcode);
}



void Chip8::test() {
	for (int i = 0; i < 4; i++)
		cycle();
	return;
}

void Chip8::assign_opcode(uint16_t address, uint16_t opcode) {
	memory[PC] = static_cast<uint8_t>(opcode >> 8);
	memory[PC + 1] = static_cast<uint8_t>(opcode && 0x00FF);
}