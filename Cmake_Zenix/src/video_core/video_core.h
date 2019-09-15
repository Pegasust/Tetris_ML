#pragma once
#include "../renderer/game_renderer.h"
namespace VideoCore
{
	class VideoHandler
	{
	public:
		Renderer::MainRenderer::RenderData display_data;
		std::thread video_thread;
		bool keep_displaying_data;
		bool exit;
		
		void pause_displaying();
		void resume_displaying();
		void stop_displaying();
		VideoHandler();
		void start_async_display(VideoHandler* h, const ::Tetris::GameModule& game);
	private:
		static void thread_main(const double& frametime_sec, 
			const Renderer::MainRenderer::RenderData& display_data,
			const bool& keep_displaying_data,
			const bool& exit);
		static void thread_main_ptr(const double& frametime_s,
			VideoHandler* handler_ptr);
	};
}