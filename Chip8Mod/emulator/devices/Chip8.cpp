#define _CRT_SECURE_NO_WARNINGS

#include "Chip8.h"

#include "CPU.h"
#include "RAM.h"

#include <fstream>
#include <iostream>

Chip8::Chip8()
{
	m_keyboard = std::make_shared<Keyboard>();
	m_display = std::make_shared<Display>();
	m_ram = std::make_shared<RAM>();

	m_cpu = std::make_shared<CPU>(m_ram, m_display, m_keyboard);
}

void Chip8::LoadProgram(std::string filepath)
{
	auto& cpu = GetCPU();

	std::ifstream file;
	file.open(filepath, std::ios::binary);

	if (file.is_open())
	{
		// getting file size
		file.seekg(0, std::ios_base::end);
		long fileSize = static_cast<long>(file.tellg());
		file.seekg(0, std::ios_base::beg);

		// saving the program in RAM at address 0x200
		cpu.SetProgramCounter(0x200);
		uint8_t* ramStart = GetRAM().GetPointer<uint8_t>(cpu.GetProgramCounter());
		file.read((char*)ramStart, fileSize);
		
		file.close();
		m_lastProgram = filepath;
	}
	else
	{
		std::cerr << "Cannot open file " << filepath << std::endl;
		exit(1);
	}
}

void Chip8::Reset(bool reload_program)
{
	// resetting cpu and ram
	GetCPU().Reset();
	GetRAM().Reset();

	// reload the program if needed
	if (reload_program) { LoadProgram(m_lastProgram); }

	GetDisplay().Clear();
	//GetDisplay().Update();
}

CPU& Chip8::GetCPU() const
{
	return *m_cpu;
}

RAM& Chip8::GetRAM() const
{
	return *m_ram;
}

Display& Chip8::GetDisplay() const
{
	return *m_display;
}

Keyboard& Chip8::GetKeyboard() const
{
	return *m_keyboard;
}
