// Cmake_Zenix.h : Include file for standard system include files,
// or project specific include files.

#pragma once

//#include <iostream>

// TODO: Reference additional headers your program requires here.

#if !defined(CMAKE_N_PROCESSOR_DETECTED)
#define N_PROCESSOR_DETECTED 1 //defaults to single core performance
#else
#define N_PROCESSOR_DETECTED CMAKE_N_PROCESSOR_DETECTED
#endif
