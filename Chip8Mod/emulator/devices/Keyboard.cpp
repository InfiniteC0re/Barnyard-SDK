#include "Keyboard.h"
#include <memory>

Keyboard::Keyboard()
{
	memset(m_inputs, 0, sizeof(m_inputs));
}
