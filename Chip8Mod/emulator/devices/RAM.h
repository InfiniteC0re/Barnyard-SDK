#pragma once
#include <cstdint>
#include <cassert>

#define RAM_SIZE 4096

class RAM
{
public:
	RAM();
	RAM(const RAM&) = delete;

	/* Returns pointer of a value at address */
	template<typename T>
	T* GetPointer(uint16_t address);

	/* Returns a value at address */
	template<typename T>
	const T& GetValue(uint16_t address);

	/* Clears RAM but doesn't clear the fonts data */
	void Reset();

private:
	uint8_t m_memory[RAM_SIZE];
};


template<typename T>
inline T* RAM::GetPointer(uint16_t address)
{
	assert(address >= 0 && address < RAM_SIZE && "Wrong RAM address");
	return reinterpret_cast<T*>(&m_memory[address]);
}

template<typename T>
inline const T& RAM::GetValue(uint16_t address)
{
	assert(address >= 0 && address < RAM_SIZE && "Wrong RAM address");
	return const_cast<T&>(m_memory[address]);
}
