#include <iostream>
#include <vector>
#include <string>
#include <fstream>

 #ifdef __APPLE__
 	#include "OpenCL/opencl.h"
 #else
 	#include "CL/cl.h"
 #endif

 extern std::string GetPlatformName (cl_platform_id id);

 extern std::string GetDeviceName (cl_device_id id);

 extern void CheckError (cl_int error);