#pragma once
#include <conio.h>
#include <vector>
#include <stdint.h>
namespace keyboard_input 
{
	//https://www.daniweb.com/programming/software-development/code/216732/reading-scan-codes-from-the-keyboard
	//correct for Windows 10 both laptop and external keyboard.
	const int
		UP_ARROW = 72,
		DOWN_ARROW = 80,
		RIGHT_ARROW = 77,
		LEFT_ARROW = 75,
		ESCAPE = 27,
		ENTER = 13,
		DELETE = 83;
	class KeyboardInputProcessing
	{
	public:
		typedef float FP32Vector2D[2];
		static const uint8_t BUFFER_SIZE = 2;
		//Denotes the amount of increment on each side
		const float INPUT_INCREMENT = 0.01f;
		const float DEADZONE = 0.30f;
		FP32Vector2D input_buffer[BUFFER_SIZE];
	private:
		 uint8_t current_buffer = 0;
	};
}
