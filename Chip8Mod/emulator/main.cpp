#include "devices/Chip8.h"

#include <iostream>
#include <thread>
#include <mutex>

#define WINDOW_SCALE 8

GLFWwindow* g_window;
bool g_isRunning = true;

void Chip8Thread(Chip8& chip, std::mutex& chip_mutex)
{
	double lastTimerTick = 0;
	double lastCPUTick = 0;
	
	double nextSecond = glfwGetTime() + 1.0;
	int times = 0;

	while (g_isRunning)
	{
		double currTime = glfwGetTime();
		double cpuTickDiff = currTime - lastCPUTick;
		double timerTickDiff = currTime - lastTimerTick;

		// checking performance
		if (currTime >= nextSecond)
		{
			printf("Current CPU Rate: %d Hz (target clock is %d Hz)\n", times, CPU_HZ);
			nextSecond = glfwGetTime() + 1.0;
			times = 0;
		}

		// updating CPU 
		if (cpuTickDiff >= CPU_HZ_SECONDS)
		{
			lastCPUTick = currTime;
			times++;

			chip_mutex.lock();
			chip.GetCPU().TickCPU();
			chip_mutex.unlock();
		}

		// updating timers
		if (timerTickDiff >= CPU_TIMER_HZ_SECONDS)
		{
			lastTimerTick = currTime;

			chip_mutex.lock();
			chip.GetCPU().TickTimers();
			chip_mutex.unlock();
		}
	}
}

int main(int argc, const char** argv)
{
	const char* executableFilepath = *(argv++);
	const char* romFilepath = nullptr;
	
	// check arguments
	if (*argv == nullptr)
	{
		std::cout << "Usage: " << executableFilepath << " rom_filepath" << std::endl;
		std::cout << "Example: " << executableFilepath << " './roms/Chip8 emulator Logo [Garstyciuks].ch8'" << std::endl;
		return 0;
	}
	else
	{
		romFilepath = *argv;
	}

	if (!glfwInit())
	{
		std::cerr << "glfw: can't initialize" << std::endl;
		return -1;
	}

	g_window = glfwCreateWindow(DISPLAY_WIDTH * WINDOW_SCALE, DISPLAY_HEIGHT * WINDOW_SCALE, "Chip 8 Emulator", nullptr, nullptr);

	if (!g_window)
	{
		std::cerr << "glfw: can't create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(g_window);

	GLenum glewStatus = glewInit();
	if (glewStatus == GLEW_OK)
	{
		std::cout << "using glew " << glewGetString(GLEW_VERSION) << std::endl;

		// turning VSync on
		glfwSwapInterval(1);

		Chip8 chip;
		chip.LoadProgram(romFilepath);

		std::mutex chip_mutex;
		std::thread chip_thread(Chip8Thread, std::ref(chip), std::ref(chip_mutex));

		while (!glfwWindowShouldClose(g_window))
		{
			chip_mutex.lock();

			// updating keyboard and the screen
			chip.GetKeyboard().Update(g_window);
			chip.UpdateDisplay();
			
			// rendering
			chip.GetDisplay().Render();

			// reset emulator if needed
			if (glfwGetKey(g_window, GLFW_KEY_ENTER) == GLFW_PRESS) { chip.Reset(true); }

			chip_mutex.unlock();

			glfwSwapBuffers(g_window);
			glfwPollEvents();
		}

		// waiting for the Chip8 thread to finish
		g_isRunning = false;
		chip_thread.join();
	}
	else
	{
		std::cerr << "glew: error: " << glewGetErrorString(glewStatus) << std::endl;
	}

	glfwTerminate();

	return 0;
}