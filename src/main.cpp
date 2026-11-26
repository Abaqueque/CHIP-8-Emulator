#include <iostream>
#include "Chip8.hpp"
#include <filesystem>
#include <exception>

int main()
{
	Chip8 emulator;

	if (emulator.loadROM("roms/subroutine_test.ch8")) {
		std::cout << "ROM loaded successfully\n";
	}
	else {
		std::cout << "ROM loading failed\n";
	}


	try {
		emulator.test();
	}
	catch (const std::exception& e) {
		std::cerr << "Emulator error: " << e.what() << "\n";
		return 1;
	}

	return 0;
}