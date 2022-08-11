#include "CPU.h"

#include "RAM.h"
#include "Display.h"
#include "Keyboard.h"

#include <memory>
#include <time.h>

#define HASH_MAXIMUM 134
#define HASH_FUNCTION(B1, B2) (((B1) << 15) | (B2)) % HASH_MAXIMUM
#define REGISTER_OPCODE(B1, B2, FUNCTION) RegisterOpcode(HASH_FUNCTION(B1, B2), FUNCTION)

CPU::CPU(std::shared_ptr<RAM> ram, std::shared_ptr<Display> display, std::shared_ptr<Keyboard> keyboard) :
	m_ram(ram),
	m_display(display),
	m_keyboard(keyboard),
	m_opcodes(HASH_MAXIMUM),
	m_masks(0x10)
{
	Reset();
	SetupHashtables();
}

void CPU::TickCPU()
{
	// shouldn't tick if paused
	if (m_isPaused || m_HasExited) return;
	
	// saving the opcode
	uint16_t opcode = *m_ram->GetPointer<uint16_t>(m_programCounter);

	// swapping opcode bytes order
	opcode = (opcode >> 8) | ((opcode & 0xFF) << 8);
	
	// saving the original opcode
	m_opcode = opcode;

	// masking the opcode to calculate it's hash (passes by reference)
	uint8_t opcodeA = opcode >> 12;
	m_masks[opcodeA](opcode);

	// calculating hash of instruction
	uint8_t hash = HASH_FUNCTION(opcodeA, opcode & 0x00FF);
	
	// executing instruction
	m_programCounter += (this->*m_opcodes[hash])();
}

void CPU::TickTimers()
{
	// shouldn't tick if paused
	if (m_isPaused || m_HasExited) return;

	if (m_delayTimer > 0) m_delayTimer--;
	if (m_soundTimer > 0) m_soundTimer--;
}

void CPU::Reset()
{
#if CPU_RANDOM_SEED
	srand(CPU_RANDOM_SEED);
#else
	srand((unsigned int)time(NULL));
#endif
	std::memset(m_registers, 0, sizeof(m_registers));
	std::memset(m_stack, 0, sizeof(m_stack));
	
	m_programCounter = 0x200;
	m_stackPointer = 0;
	m_HasExited = false;
	m_opcode = 0;

	m_delayTimer = 0;
	m_soundTimer = 0;
}

void CPU::SetProgramCounter(uint16_t programCounter)
{
	m_programCounter = programCounter;
}

const uint16_t& CPU::GetProgramCounter() const
{
	return m_programCounter;
}

void CPU::RegisterOpcode(size_t hash, t_AsmInstruction function)
{
	if (hash >= m_opcodes.size())
	{
		// resize hashtable to fit new hash
		m_opcodes.resize(hash);
	}
	else
	{
		assert(m_opcodes[hash] == &CPU::OP_UNKN && "The hash has collided, use some other has function");
		m_opcodes[hash] = function;
	}
}

void CPU::MaskA000(uint16_t& opcode)
{
	opcode &= 0xF000;
}

void CPU::MaskA00D(uint16_t& opcode)
{
	opcode &= 0xF00F;
}

void CPU::MaskA0CD(uint16_t& opcode)
{
	opcode &= 0xF0FF;
}

void CPU::SetupHashtables()
{
	// mask hashtable
	m_masks[0x0] = &MaskA0CD;
	m_masks[0x1] = &MaskA000;
	m_masks[0x2] = &MaskA000;
	m_masks[0x3] = &MaskA000;
	m_masks[0x4] = &MaskA000;
	m_masks[0x5] = &MaskA00D;
	m_masks[0x6] = &MaskA000;
	m_masks[0x7] = &MaskA000;
	m_masks[0x8] = &MaskA00D;
	m_masks[0x9] = &MaskA00D;
	m_masks[0xA] = &MaskA000;
	m_masks[0xB] = &MaskA000;
	m_masks[0xC] = &MaskA000;
	m_masks[0xD] = &MaskA000;
	m_masks[0xE] = &MaskA0CD;
	m_masks[0xF] = &MaskA0CD;

	// opcode hashtable
	for (size_t i = 0; i < m_opcodes.size(); i++)
	{
		m_opcodes[i] = &CPU::OP_UNKN;
	}

	REGISTER_OPCODE(0x0, 0xE0, &CPU::OP_00E0);
	REGISTER_OPCODE(0x0, 0xEE, &CPU::OP_00EE);
	REGISTER_OPCODE(0x1, 0x00, &CPU::OP_1000);
	REGISTER_OPCODE(0x2, 0x00, &CPU::OP_2000);
	REGISTER_OPCODE(0x3, 0x00, &CPU::OP_3000);
	REGISTER_OPCODE(0x4, 0x00, &CPU::OP_4000);
	REGISTER_OPCODE(0x5, 0x00, &CPU::OP_5000);
	REGISTER_OPCODE(0x6, 0x00, &CPU::OP_6000);
	REGISTER_OPCODE(0x7, 0x00, &CPU::OP_7000);
	REGISTER_OPCODE(0x8, 0x00, &CPU::OP_8000);
	REGISTER_OPCODE(0x8, 0x01, &CPU::OP_8001);
	REGISTER_OPCODE(0x8, 0x02, &CPU::OP_8002);
	REGISTER_OPCODE(0x8, 0x03, &CPU::OP_8003);
	REGISTER_OPCODE(0x8, 0x04, &CPU::OP_8004);
	REGISTER_OPCODE(0x8, 0x05, &CPU::OP_8005);
	REGISTER_OPCODE(0x8, 0x06, &CPU::OP_8006);
	REGISTER_OPCODE(0x8, 0x07, &CPU::OP_8007);
	REGISTER_OPCODE(0x8, 0x0E, &CPU::OP_800E);
	REGISTER_OPCODE(0x9, 0x00, &CPU::OP_9000);
	REGISTER_OPCODE(0xA, 0x00, &CPU::OP_A000);
	REGISTER_OPCODE(0xB, 0x00, &CPU::OP_B000);
	REGISTER_OPCODE(0xC, 0x00, &CPU::OP_C000);
	REGISTER_OPCODE(0xD, 0x00, &CPU::OP_D000);
	REGISTER_OPCODE(0xE, 0x9E, &CPU::OP_E09E);
	REGISTER_OPCODE(0xE, 0xA1, &CPU::OP_E0A1);
	REGISTER_OPCODE(0xF, 0x07, &CPU::OP_F007);
	REGISTER_OPCODE(0xF, 0x0A, &CPU::OP_F00A);
	REGISTER_OPCODE(0xF, 0x15, &CPU::OP_F015);
	REGISTER_OPCODE(0xF, 0x18, &CPU::OP_F018);
	REGISTER_OPCODE(0xF, 0x1E, &CPU::OP_F01E);
	REGISTER_OPCODE(0xF, 0x29, &CPU::OP_F029);
	REGISTER_OPCODE(0xF, 0x33, &CPU::OP_F033);
	REGISTER_OPCODE(0xF, 0x55, &CPU::OP_F055);
	REGISTER_OPCODE(0xF, 0x65, &CPU::OP_F065);
}

/* opcodes start from here */

uint8_t CPU::OP_UNKN()
{
	printf("Unknown opcode: 0x%04hX\n", m_opcode);
	//assert(0 && "Unknown opcode");
	return 2;
}

uint8_t CPU::OP_00E0()
{
	m_display->Clear();
	return 2;
}

uint8_t CPU::OP_00EE()
{
	m_programCounter = m_stack[--m_stackPointer];
	return 2;
}

uint8_t CPU::OP_1000()
{
	uint16_t gotoAddress = m_opcode & 0x0FFF;

	if (gotoAddress == m_programCounter)
	{
		printf("Execution paused because we've encountered infinite jmp loop\n");
		m_HasExited = true;
	}

	m_programCounter = gotoAddress;
	return 0;
}

uint8_t CPU::OP_2000()
{
	m_stack[m_stackPointer++] = m_programCounter;
	m_programCounter = m_opcode & 0x0FFF;
	return 0;
}

uint8_t CPU::OP_3000()
{
	if (m_registers[(m_opcode >> 8) & 0xF] == (m_opcode & 0x00FF)) return 4;
	return 2;
}

uint8_t CPU::OP_4000()
{
	if (m_registers[(m_opcode >> 8) & 0xF] != (m_opcode & 0x00FF)) return 4;
	return 2;
}

uint8_t CPU::OP_5000()
{
	if (m_registers[(m_opcode >> 8) & 0xF] == m_registers[(m_opcode >> 4) & 0xF]) return 4;
	return 2;
}

uint8_t CPU::OP_6000()
{
	m_registers[(m_opcode >> 8) & 0xF] = m_opcode & 0x00FF;
	return 2;
}

uint8_t CPU::OP_7000()
{
	m_registers[(m_opcode >> 8) & 0xF] += m_opcode & 0x00FF;
	return 2;
}

uint8_t CPU::OP_8000()
{
	m_registers[(m_opcode >> 8) & 0xF] = m_registers[(m_opcode >> 4) & 0xF];
	return 2;
}

uint8_t CPU::OP_8001()
{
	m_registers[(m_opcode >> 8) & 0xF] |= m_registers[(m_opcode >> 4) & 0xF];
	return 2;
}

uint8_t CPU::OP_8002()
{
	m_registers[(m_opcode >> 8) & 0xF] &= m_registers[(m_opcode >> 4) & 0xF];
	return 2;
}

uint8_t CPU::OP_8003()
{
	m_registers[(m_opcode >> 8) & 0xF] ^= m_registers[(m_opcode >> 4) & 0xF];
	return 2;
}

uint8_t CPU::OP_8004()
{
	uint8_t b = (m_opcode >> 8) & 0xF;
	uint8_t c = (m_opcode >> 4) & 0xF;

	m_registers[Register::VF] = (m_registers[b] + m_registers[c]) > 0xFF;
	m_registers[b] += m_registers[c];

	return 2;
}

uint8_t CPU::OP_8005()
{
	uint8_t b = (m_opcode >> 8) & 0xF;
	uint8_t c = (m_opcode >> 4) & 0xF;
	
	m_registers[Register::VF] = m_registers[b] >= m_registers[c];
	m_registers[b] -= m_registers[c];
	
	return 2;
}

uint8_t CPU::OP_8006()
{
	uint8_t b = (m_opcode >> 8) & 0xF;

	m_registers[Register::VF] = m_registers[b] & 1;
	m_registers[b] >>= 1;
	
	return 2;
}

uint8_t CPU::OP_8007()
{
	uint8_t b = (m_opcode >> 8) & 0xF;
	uint8_t c = (m_opcode >> 4) & 0xF;

	m_registers[Register::VF] = m_registers[c] >= m_registers[b];
	m_registers[b] = m_registers[c] - m_registers[b];

	return 2;
}

uint8_t CPU::OP_800E()
{
	uint8_t b = (m_opcode >> 8) & 0xF;

	m_registers[Register::VF] = m_registers[b] >> 7;
	m_registers[b] <<= 1;

	return 2;
}

uint8_t CPU::OP_9000()
{
	if (m_registers[(m_opcode >> 8) & 0xF] != m_registers[(m_opcode >> 4) & 0xF]) return 4;
	return 2;
}

uint8_t CPU::OP_A000()
{
	m_indexRegister = m_opcode & 0x0FFF;
	return 2;
}

uint8_t CPU::OP_B000()
{
	m_programCounter = m_opcode & 0x0FFF;
	return m_registers[Register::V0];
}

uint8_t CPU::OP_C000()
{
	m_registers[(m_opcode >> 8) & 0xF] = (rand() % 266) & m_opcode & 0x00FF;
	return 2;
}

uint8_t CPU::OP_D000()
{
	uint8_t* screen = m_display->GetScreen();
	uint8_t x = m_registers[(m_opcode >> 8) & 0xF] % DISPLAY_WIDTH;
	uint8_t y = m_registers[(m_opcode >> 4) & 0xF] % DISPLAY_HEIGHT;

	// set collision to 0
	m_registers[Register::VF] = 0;

	for (uint8_t i = 0; i < ((m_opcode) & 0xF); i++)
	{
		const uint8_t spriteRow = m_ram->GetValue<uint8_t>(m_indexRegister + i);

		for (uint8_t j = 0; j < SPRITE_WIDTH; j++)
		{
			if ((spriteRow & (0b10000000 >> j)) != 0)
			{
				uint16_t pixelCoords = ((x + j) + ((y + i) * DISPLAY_WIDTH)) % DISPLAY_SIZE;
				uint8_t& screenPixel = screen[pixelCoords];

				m_registers[Register::VF] |= screenPixel;
				screenPixel ^= 1;
			}
		}
	}

	return 2;
}

uint8_t CPU::OP_E09E()
{
	return m_keyboard->GetKeyState((KeyboardKey)m_registers[(m_opcode >> 8) & 0xF]) ? 4 : 2;
}

uint8_t CPU::OP_E0A1()
{
	return !m_keyboard->GetKeyState((KeyboardKey)m_registers[(m_opcode >> 8) & 0xF]) ? 4 : 2;
}

uint8_t CPU::OP_F007()
{
	m_registers[(m_opcode >> 8) & 0xF] = m_delayTimer;
	return 2;
}

uint8_t CPU::OP_F00A()
{
	for (uint8_t i = 0; i < (uint8_t)KeyboardKey::KEYCOUNT; i++)
	{
		if (m_keyboard->GetKeyState((KeyboardKey)i))
		{
			m_registers[(m_opcode >> 8) & 0xF] = i;
			return 2;
		}
	}

	return 0;
}

uint8_t CPU::OP_F015()
{
	m_delayTimer = m_registers[(m_opcode >> 8) & 0xF];
	return 2;
}

uint8_t CPU::OP_F018()
{
	// todo: sound support
	m_soundTimer = m_registers[(m_opcode >> 8) & 0xF];
	return 2;
}

uint8_t CPU::OP_F01E()
{
	m_indexRegister += m_registers[(m_opcode >> 8) & 0xF];
	return 2;
}

uint8_t CPU::OP_F029()
{
	m_indexRegister = 5 * m_registers[(m_opcode >> 8) & 0xF];
	return 2;
}

uint8_t CPU::OP_F033()
{
	uint8_t opcodeB = (m_opcode >> 8) & 0xF;
	
	auto b = m_ram->GetPointer<uint8_t>(m_indexRegister + 0);
	auto c = m_ram->GetPointer<uint8_t>(m_indexRegister + 1);
	auto d = m_ram->GetPointer<uint8_t>(m_indexRegister + 2);
	*b = static_cast<uint8_t>((m_registers[opcodeB]) * 0.01f);
	*c = static_cast<uint8_t>((m_registers[opcodeB] - (*b * 100)) * 0.1f);
	*d = static_cast<uint8_t>((m_registers[opcodeB] - (*b * 100) - (*c * 10)));
	
	return 2;
}

uint8_t CPU::OP_F055()
{
	for (uint8_t i = 0; i <= ((m_opcode >> 8) & 0xF); i++)
	{
		*m_ram->GetPointer<uint8_t>(m_indexRegister + i) = m_registers[i];
	}

	return 2;
}

uint8_t CPU::OP_F065()
{
	for (uint8_t i = 0; i <= ((m_opcode >> 8) & 0xF); i++)
	{
		m_registers[i] = *m_ram->GetPointer<uint8_t>(m_indexRegister + i);
	}

	return 2;
}
