/*
 * The core that displays information to screen. This is the "render to screen"
 * process. In order to render, display_data needs to be linked with a render
 * buffer like the one from "../renderer/game_renderer.h"
*/
#pragma once
#include "../renderer/game_renderer.h"
#include "../verbosity/framerate.h"
namespace VideoCore
{
	class VideoHandler
	{
	public:
		Renderer::MainRenderer::RenderData display_data;
		std::thread video_thread;
		// This value can be set to another from another thread,
		// make sure not to skip steps.
        volatile bool keep_displaying_data;
        // This value can be set to another from another thread,
        // make sure not to skip steps.
		volatile bool exit;
		
		void pause_displaying();
		void resume_displaying();
		void stop_displaying();
		VideoHandler();
		void start_async_display(VideoHandler* h, const ::Tetris::GameModule& game);
		inline void start_async_display(const ::Tetris::GameModule& game) {
            start_async_display(this, game);
		}
	private:
		//static void thread_main(const double& frametime_sec, 
		//	const Renderer::MainRenderer::RenderData& display_data,
		//	const bool& keep_displaying_data,
		//	const bool& exit);
        /**
		 * frametime_ms: The amount of time to wait before each update.
		 * This can be understood as the max frametime in seconds.
		 * Set this value a lot higher than what you expect, as
		 * each iteration actually tries to synchronize with
		 * with visual update.
		 */
		static void thread_main_ptr(const int& frametime_ms,
			VideoHandler* handler_ptr);
	};
}