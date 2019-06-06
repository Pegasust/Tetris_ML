#pragma once
#include "lite_renderer.h"
namespace Renderer
{
	class RenderUnit
	{
	public:
		const char BURNED_CHAR = '=';
		const char SHADOW_CHAR = '.';
		unsigned long long frames = 0;
		std::string field;
		std::string info;
		std::string upcoming;
		void update_string(const LiteGameModule::LiteModule& mod);
		void show_shadow(const LiteGameModule::LiteModule& mod);
		void show_burns(const unsigned char & n_burned, const unsigned char & burn_y, const int& time_ms);
		void render();
		RenderUnit(const LiteGameModule::LiteModule& mod);
	};
}