#pragma once
template <bool threaded, uint64_t time_out, typename ChronoType>
int Common::KeyboardListener<threaded, time_out, ChronoType>::get_char() {
    if (time_out == 0) {
        // Block program
        return _getch();
    }
    auto invoke_tm = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point tm_out = invoke_tm + ChronoType(time_out);

    while (!_kbhit()) {
        if (std::chrono::high_resolution_clock::now() > tm_out) {
            return -1;
        }
    }
    int actual_char = _getch();
    while (_kbhit()) {
        actual_char = _getch();
    }
    return actual_char;
}

template <uint64_t time_out, typename ChronoType>
class Common::KeyboardListener<true, time_out, ChronoType> {
private:
    static bool exit;
    static bool initialized;
    static std::thread input_thr;

public:
    static void exit_synchronously();
    static inline int get_key() {
        return get_char();
    }
    static std::string get_cin_line(const std::string& default_val);

protected:
    static int get_char();
};

template <uint64_t time_out, typename ChronoType>
bool Common::KeyboardListener<true, time_out, ChronoType>::exit = false;

template <uint64_t time_out, typename ChronoType>
bool Common::KeyboardListener<true, time_out, ChronoType>::initialized = false;

template <uint64_t time_out, typename ChronoType>
std::thread Common::KeyboardListener<true, time_out, ChronoType>::input_thr = std::thread();

template <uint64_t time_out, typename ChronoType>
void Common::KeyboardListener<true, time_out, ChronoType>::exit_synchronously() {
    exit = true;
    initialized = false;
    if (input_thr.joinable()) {
        input_thr.join();
    }
}

template <uint64_t time_out, typename ChronoType>
int Common::KeyboardListener<true, time_out, ChronoType>::get_char() {
    static InputBufferQueue input_buffer;
    if (!initialized) {
        // Create a thread that keep on adding to the buffer queue.
        input_thr = std::thread([]() {
            while (!exit) {
                if (time_out == 0) {
                    // The thread just blocks itself until it gets
                    // an input. (It is unclear if the thread sleeps or not).
                    input_buffer.push(_getch());
                } else {
                    // Just sleep until the next update comes
                    static const auto sleep_dur = ChronoType(time_out);
                    std::this_thread::sleep_for(sleep_dur);
                    // After sleeping, record all of the keystrokes recorded
                    // by the OS.
                    while (_kbhit()) {
                        input_buffer.push(_getch());
                    }
                }
            }
        });
        initialized = true;
    }
    // Automatically returns NO INPUT if
    if (input_buffer.empty()) {
        return -1;
    }
    int ret_val = input_buffer.front();
    input_buffer.pop();
    return ret_val;
}

template <bool threaded, uint64_t timeout, typename ChronoType>
std::string Common::KeyboardListener<threaded, timeout, ChronoType>::get_cin_line(
    const std::string& default_val) {
    std::string retval;
    std::thread wait_thread = std::thread([&]() {
        // Wait some seconds
        auto unpause_timepoint = Common::GameClock::Clock::now();
        unpause_timepoint += ChronoType(timeout);
        // std::cout << "Time out: " << timeout << std::endl;
        do {
            // std::cout << "update" << std::endl;
            // std::this_thread::yield();
            if (std::cin.peek() != EOF) {
                break;
            }
        } while (Common::GameClock::Clock::now() < unpause_timepoint);
        // std::cout << "The wait is over. " << std::endl;
        std::getline(std::cin, retval);
        if (retval.length() == 0) {
            retval = default_val;
        }
    });
    wait_thread.join();
    return retval;
}

template <uint64_t timeout, typename ChronoType>
std::string Common::KeyboardListener<true, timeout, ChronoType>::get_cin_line(
    const std::string& default_val) {
    return KeyboardListener<true, timeout, ChronoType>::get_cin_line(default_val);
}