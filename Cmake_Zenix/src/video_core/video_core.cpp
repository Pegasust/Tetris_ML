#include "video_core.h"

void VideoCore::VideoHandler::pause_displaying() {
    keep_displaying_data = false;
}

void VideoCore::VideoHandler::resume_displaying() {
    keep_displaying_data = true;
}

void VideoCore::VideoHandler::stop_displaying() {
    exit = true;
    video_thread.join();
}

VideoCore::VideoHandler::VideoHandler() : exit(false), keep_displaying_data(true) {
    display_data = Renderer::MainRenderer::RenderData();
}

void VideoCore::VideoHandler::start_async_display(VideoHandler* h,
                                                  const ::Tetris::GameModule& game) {
    keep_displaying_data = true;
    exit = false;
    /*	video_thread = std::thread(&VideoCore::VideoHandler::thread_main,
        1.0 / 60.0, this->display_data, this->keep_displaying_data, this->exit);*/
    video_thread = std::thread(&thread_main_ptr, FRAMETIME_MILLIS, h);
}

// void VideoCore::VideoHandler::thread_main(const double& frametime_sec, const
// Renderer::MainRenderer::RenderData& display_data, const bool& keep_displaying_data, const bool&
// exit)
//{
//	do
//	{
//		auto next_time = std::chrono::high_resolution_clock::now()
//			+ std::chrono::duration<double, std::milli>(frametime_sec);
//
//		if (keep_displaying_data)
//		{
//			Renderer::MainRenderer::try_display(display_data);
//		}
//		std::this_thread::sleep_until(next_time);
//	} while (!exit);
//}

void VideoCore::VideoHandler::thread_main_ptr(const int& frametime_ms,
                                              VideoHandler* handler_ptr) {
    do {
        // auto next_time = std::chrono::high_resolution_clock::now()
        //	+ std::chrono::duration<double, std::ratio<1>>(frametime_s);

        if (handler_ptr->keep_displaying_data) {
            // Optimize out if there is no frame needed to be drawn.
            Renderer::MainRenderer::clear_screen();
            Renderer::MainRenderer::try_display(handler_ptr->display_data);
            handler_ptr->keep_displaying_data = false;
        }
        Verbosity::FrameLogger::draw_tm().update_frame_count();
        if (frametime_ms > 0) {
            std::this_thread::sleep_for(
                // ammount in seconds
                std::chrono::duration<int,std::milli>(frametime_ms));
        }
    } while (!handler_ptr->exit);
}
