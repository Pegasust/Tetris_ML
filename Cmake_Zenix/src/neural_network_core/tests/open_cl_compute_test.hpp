#pragma once
#include "../../common/common.hpp"

//#include "../../test_utils/test_utils.hpp"

bool sample_code() {
    const char* hello_str = "__kernel void hello(void) { }";
    cl_int err = CL_SUCCESS;
    try {
        // Get all platforms that support OpenCL
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        if (platforms.size() == 0) {
            std::cout << "No OpenCL compatible platform available " << std::endl;
            return -1;
        }
        std::cout << "Found " << platforms.size() << " available platforms." << std::endl;

        for (int i = 0; i < platforms.size(); i++) {
            std::cout << i << ". " << platforms[i].getInfo<CL_PLATFORM_NAME>() <<std::endl;
        }
        cl_context_properties properties[] = {CL_CONTEXT_PLATFORM,
                                              (cl_context_properties)(platforms[0](), 0)};
        // First device of CPU?

        cl::Context context(CL_DEVICE_TYPE_CPU, properties);
        // Retrieve the devices based on the context we given, which is the
        // first CPU we found?
        std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
        // Compile the kernel of hello_str
        cl::Program::Sources source(1, std::make_pair(hello_str, strlen(hello_str)));
        cl::Program hello_str_program = cl::Program(context, source);
        hello_str_program.build(devices);

        cl::Kernel kernel(hello_str_program, "hello", &err);

        cl::Event cl_event;
        cl::CommandQueue command_q(context, devices[0], 0, &err);

        command_q.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(4, 4), cl::NullRange,
                                       NULL, &cl_event);
        cl_event.wait();
        std::cout << "cl_event exited." << std::endl;
    } catch (cl::Error err) {
        std::cerr << "ERROR: " << err.what() << "(" << err.err() << ")" << std::endl;
        return false;
    }
    return true;
}

bool sample_compute() {
    cl_int err = CL_SUCCESS;
    try {

        // get all platforms (drivers), e.g. NVIDIA
        std::vector<cl::Platform> all_platforms;
        cl::Platform::get(&all_platforms);

        if (all_platforms.size() == 0) {
            std::cout << " No platforms found. Check OpenCL installation!\n";
            exit(1);
        }
        cl::Platform default_platform = all_platforms[0];
        std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";

        // get default device (CPUs, GPUs) of the default platform
        std::vector<cl::Device> all_devices;
        default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
        if (all_devices.size() == 0) {
            std::cout << " No devices found. Check OpenCL installation!\n";
            exit(1);
        }

        //// use device[1] because that's a GPU; device[0] is the CPU;
        // false, platform is GPU or CPU, device should be 0.
        cl::Device default_device = all_devices[0];
        std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";

        // a context is like a "runtime link" to the device and platform;
        // i.e. communication is possible
        cl::Context context({default_device});

        // create the program that we want to execute on the device
        cl::Program::Sources sources;

        // calculates for each element; C = A + B
        std::string kernel_code =
            "   void kernel simple_add(global const int* A, global const int* B, global int* C, "
            "                          global const int* N) {"
            "       int ID, Nthreads, n, ratio, start, stop;"
            ""
            "       ID = get_global_id(0);"
            "       Nthreads = get_global_size(0);"
            "       n = N[0];"
            ""
            "       ratio = (n / Nthreads);" // number of elements for each thread
            "       start = ratio * ID;"
            "       stop  = ratio * (ID + 1);"
            ""
            "       for (int i=start; i<stop; i++)"
            "           C[i] = A[i] + B[i];"
            "   }";
        sources.push_back({kernel_code.c_str(), kernel_code.length()});

        cl::Program program(context, sources);
        if (program.build({default_device}) != CL_SUCCESS) {
            std::cout << "Error building: "
                      << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << std::endl;
            return false;
        }

        // apparently OpenCL only likes arrays ...
        // N holds the number of elements in the vectors we want to add
        constexpr int n = 100;
        int N[1] = {n};
        // int n = N[0];

        // create buffers on device (allocate space on GPU)
        cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(int) * n);
        cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(int) * n);
        cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof(int) * n);
        cl::Buffer buffer_N(context, CL_MEM_READ_ONLY, sizeof(int));

        // create things on here (CPU)
        int A[n], B[n];
        // std::vector<int> A(n), B(n);
        for (int i = 0; i < n; i++) {
            A[i] = i;
            B[i] = n - i - 1;
        }
        // create a queue (a queue of commands that the GPU will execute)
        cl::CommandQueue queue(context, default_device);

        // push write commands to queue
        queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(int) * n, A);
        queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(int) * n, B);
        queue.enqueueWriteBuffer(buffer_N, CL_TRUE, 0, sizeof(int), N);

        // RUN ZE KERNEL
        // cl::KernelFunctor simple_add(cl::Kernel(program, "simple_add"), queue, cl::NullRange,
        //                             cl::NDRange(10), cl::NullRange);
        cl::Kernel simple_add_kernel(program, "simple_add", &err);
        // simple_add(buffer_A, buffer_B, buffer_C, buffer_N);
        simple_add_kernel.setArg(0, buffer_A);
        simple_add_kernel.setArg(1, buffer_B);
        simple_add_kernel.setArg(2, buffer_C);
        simple_add_kernel.setArg(3, buffer_N);
        queue.enqueueNDRangeKernel(simple_add_kernel, cl::NullRange, cl::NDRange(10),
                                   cl::NullRange);
        queue.finish();

        // std::vector<int> C(n);
        int C[n];
        // read result from GPU to here
        queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(int) * n, C);

        std::cout << "result: {";
        for (int i = 0; i < n; i++) {
            std::cout << C[i] << " ";
        }
        std::cout << "}" << std::endl;

        return true;
    } catch (cl::Error e) {
        std::cerr << "Error: " << e.err() << " (" << e.what() << ")." << std::endl;
        return false;
    }
}
int open_cl_compute_test() {
    TEST_INITIALIZE;
    DO_TEST(sample_code);
    DO_TEST(sample_compute);
    TEST_RESULTS;
    return 1;
}