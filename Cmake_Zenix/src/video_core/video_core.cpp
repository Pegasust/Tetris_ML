#include "video_core.h"

void VideoCore::VideoHandler::pause_displaying()
{
	keep_displaying_data = false;
}

void VideoCore::VideoHandler::resume_displaying()
{
	keep_displaying_data = true;
}

void VideoCore::VideoHandler::stop_displaying()
{
	exit = true;
	video_thread.join();
}

VideoCore::VideoHandler::VideoHandler():exit(false), keep_displaying_data(true)
{
	display_data = Renderer::MainRenderer::RenderData();
}

void VideoCore::VideoHandler::start_async_display(VideoHandler* h, const ::Tetris::GameModule& game)
{
	keep_displaying_data = true;
	exit = false;
	/*	video_thread = std::thread(&VideoCore::VideoHandler::thread_main,
		1.0 / 60.0, this->display_data, this->keep_displaying_data, this->exit);*/
	//update_thread = std::thread(Renderer::MainRenderer::try_update(game, h->display_data));
	video_thread = std::thread(&thread_main_ptr, 1.0 / 60.0, h);

}

void VideoCore::VideoHandler::thread_main(const double& frametime_sec, const Renderer::MainRenderer::RenderData& display_data, const bool& keep_displaying_data, const bool& exit)
{
	do
	{
		auto next_time = std::chrono::high_resolution_clock::now()
			+ std::chrono::duration<double, std::milli>(frametime_sec);

		if (keep_displaying_data)
		{
			Renderer::MainRenderer::try_display(display_data);
		}
		std::this_thread::sleep_until(next_time);
	} while (!exit);
}

void VideoCore::VideoHandler::thread_main_ptr(const double& frametime_s, const VideoHandler* handler_ptr)
{
	do
	{
		auto next_time = std::chrono::high_resolution_clock::now()
			+ std::chrono::duration<double, std::milli>(frametime_s);

		if (handler_ptr->keep_displaying_data)
		{
			Renderer::MainRenderer::try_display(handler_ptr->display_data);
		}
		std::this_thread::sleep_until(next_time);
	} while (!handler_ptr->exit);
}

