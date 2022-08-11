#pragma once
#include <vector>
#include <cstdint>
#include <memory>

// uncomment line below to use custom random seed
// #define CPU_RANDOM_SEED 1234865

#define CPU_HZ 500
#define CPU_STACK_SIZE 16
#define CPU_TIMER_HZ 60

class Keyboard;
class Display;
class RAM;

constexpr auto CPU_HZ_SECONDS = 1.0 / CPU_HZ;
constexpr auto CPU_TIMER_HZ_SECONDS = 1.0 / CPU_TIMER_HZ;

typedef enum
{
	V0, V1, V2, V3, V4, V5, V6, V7,
	V8, V9, VA, VB, VC, VD, VE, VF,
	COUNT
} Register;

class CPU
{
public:
	friend class InstructionExecutor;

public:
	CPU(std::shared_ptr<RAM> ram, std::shared_ptr<Display> display, std::shared_ptr<Keyboard> keyboard);
	CPU(const CPU&) = delete;
	~CPU() = default;

	/* Executes next instruction */
	void TickCPU();

	/* Ticks timer */
	void TickTimers();

	/* Resets the state of CPU */
	void Reset();

	/* Sets program counter */
	void SetProgramCounter(uint16_t programCounter);

	/* Returns program counter */
	const uint16_t& GetProgramCounter() const;

	/* Sets new value for m_isPaused */
	inline void SetPaused(bool new_state) { m_isPaused = new_state; };

	/* Returns true if paused */
	inline const bool IsPaused() const { return m_isPaused; };

	/* Returns true if exited program */
	inline const bool HasExited() const { return m_HasExited; };

private:
	/* Function templates */
	typedef uint8_t(CPU::* t_AsmInstruction)();
	typedef void(*t_MaskFunction)(uint16_t&);

	void RegisterOpcode(size_t hash, t_AsmInstruction function);

	/* Mask Functions */

	static void MaskA000(uint16_t& opcode);
	static void MaskA00D(uint16_t& opcode);
	static void MaskA0CD(uint16_t& opcode);

	/* CPU Opcodes */

	/* UNKNOWN OPCODE */
	uint8_t OP_UNKN();

	/* CLS */
	uint8_t OP_00E0();
	/* RET */
	uint8_t OP_00EE();
	/* JP addr */
	uint8_t OP_1000();
	/* CALL addr */
	uint8_t OP_2000();
	/* SE Vx, byte */
	uint8_t OP_3000();
	/* SNE Vx, byte */
	uint8_t OP_4000();
	/* SE Vx, Vy */
	uint8_t OP_5000();
	/* LD Vx, byte */
	uint8_t OP_6000();
	/* ADD Vx, byte */
	uint8_t OP_7000();
	/* LD Vx, Vy */
	uint8_t OP_8000();
	/* OR Vx, Vy */
	uint8_t OP_8001();
	/* AND Vx, Vy */
	uint8_t OP_8002();
	/* XOR Vx, Vy */
	uint8_t OP_8003();
	/* ADD Vx, Vy */
	uint8_t OP_8004();
	/* SUB Vx, Vy */
	uint8_t OP_8005();
	/* SHR Vx {, Vy} */
	uint8_t OP_8006();
	/* SUBN Vx, Vy */
	uint8_t OP_8007();
	/* SHL Vx {, Vy} */
	uint8_t OP_800E();
	/* SNE Vx, Vy */
	uint8_t OP_9000();
	/* LD I, addr */
	uint8_t OP_A000();
	/* JP V0, addr */
	uint8_t OP_B000();
	/* RND Vx, byte */
	uint8_t OP_C000();
	/* DRW Vx, Vy, nibble */
	uint8_t OP_D000();
	/* SKP Vx */
	uint8_t OP_E09E();
	/* SKNP Vx */
	uint8_t OP_E0A1();
	/* LD Vx, DT */
	uint8_t OP_F007();
	/* LD Vx, K */
	uint8_t OP_F00A();
	/* LD DT, Vx */
	uint8_t OP_F015();
	/* LD ST, Vx */
	uint8_t OP_F018();
	/* ADD I, Vx */
	uint8_t OP_F01E();
	/* LD F, Vx */
	uint8_t OP_F029();
	/* LD B, Vx */
	uint8_t OP_F033();
	/* LD [I], Vx */
	uint8_t OP_F055();
	/* LD Vx, [I] */
	uint8_t OP_F065();

	/* Call it on init to setup the hashtable */
	void SetupHashtables();

private:
	/* Linked devices */
	std::shared_ptr<Keyboard> m_keyboard;
	std::shared_ptr<Display> m_display;
	std::shared_ptr<RAM> m_ram;

	/* Registers */
	uint8_t m_registers[Register::COUNT];
	uint16_t m_indexRegister = 0;

	/* Stack */
	uint16_t m_stack[CPU_STACK_SIZE];
	uint8_t m_stackPointer = 0;

	/* Timers */
	uint8_t m_delayTimer = 0;
	uint8_t m_soundTimer = 0;
	
	/* Counters */
	uint16_t m_programCounter = 0;

	/* Current state */
	bool m_isPaused = true;
	bool m_HasExited = false;
	uint16_t m_opcode = 0;

	/* Hashtables with function pointers */
	std::vector<t_AsmInstruction> m_opcodes;
	std::vector<t_MaskFunction> m_masks;
};

