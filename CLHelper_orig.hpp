//
//  CLHelper.hpp
//  cppcommon
//
//  Created by Andrew W.E. McDonald
//  Put into class structure by Sean Grimes on 11/21/15.
//  Copyright © 2015. All rights reserved.
//

#pragma once

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <iostream>


/*
 - Automatically select the most powerful GPU with the default c'tor
 - Keep the other c'tor for manual selection
 - Add a method that returns the possible devices on the machine
 - Return this as a map of ints --> vector<string> where the vector has device information
 NEXT
 - Create and build the program in opencl
 - Create the kernel in opencl
 - Error handling for those two things
 - Creating and pushing buffers, pulling buffers, setting kernel args, enqueue the kernel
 - Can check out blade to accomplish the above stuff
*/


/**
    \brief Removing some OpenCL setup boiler plate
    \author Andrew McDonald
    \author Sean Grimes
    \date 11-21-15
    \note Currently under development, no guarantees made. 
*/
class CLHelper{
public:
    
    /**
        \brief default c'tor, setup for use with MBP on macOS
        \details Will default to 3 possible devices, selecting the discrete GPU by default
    */
    CLHelper() : CLHelper(3, 2) {}
    
    /**
        \brief c'tor to select which compute device to attempt to use
        \details Will try to use 'deviceToUse', on a MBP the devices are:\n
        0 == CPU \n
        1 == Integrated GPU \n
        2 == Discrete GPU
        @param maxDevices The number of OpenCL compute devices on the machine
        @param deviceToUse The OpenCL device to use
    */
    CLHelper(int maxDevices, int deviceToUse)
        : err{}
        , correct{}
        , maxDevices{maxDevices}
        , deviceToUse{deviceToUse}
    {
        device_ids = new cl_device_id[maxDevices];
    }

    /**
        \brief Move c'tor
    */
    CLHelper(CLHelper &&clh) 
        : CLHelper() 
    {
        swap(*this, clh);
    }

    /**
        \brief Move assignment
    */
    CLHelper &operator=(CLHelper &&clh){
        swap(*this, clh);
        return *this;
    }

    /**
        \brief Copy c'tor
    */
    CLHelper(const CLHelper &clh) 
        : err{clh.err}
        , correct{clh.correct}
        , maxDevices{clh.maxDevices}
        , device_ids(maxDevices ? new cl_device_id[maxDevices] : nullptr)
        , deviceToUse{clh.deviceToUse}
        , device_id{clh.device_id}
        , context{clh.context}
        , commands{clh.commands}
        , num_devices{clh.num_devices}
    {
        std::copy(clh.device_ids, clh.device_ids + maxDevices, device_ids);
    }

    /**
        \brief Copy assignment
    */
    CLHelper &operator=(CLHelper clh){
        swap(*this, clh);
        return *this;
    }

    /**
        \brief d'tor
    */
    ~CLHelper(){delete[] device_ids;}

    void swap(CLHelper &l, CLHelper &r){
        using std::swap;
        swap(l.err, r.err);
        swap(l.correct, r.correct);
        swap(l.maxDevices, r.maxDevices);
        swap(l.deviceToUse, r.deviceToUse);
        swap(l.device_ids, r.device_ids);
        swap(l.device_id, r.device_id);
        swap(l.context, r.context);
        swap(l.commands, r.commands);
        swap(l.num_devices, r.num_devices);
    }


    /**
        \brief See details
        \details Sets index of OpenCL device number to use. Default is 2.
    */
    void setDeviceToUse(int deviceIndex){
        deviceToUse = deviceIndex;
    }   
    
    /**
        \brief See details
        \details Setup opencp device(s), context, and commandQueue
        @return True...well at the moment it's always true
    */
    bool setup_opencl(void){
        num_devices = 0;
        if(num_devices == 0){
            err = clGetDeviceIDs(nullptr, CL_DEVICE_TYPE_ALL, maxDevices, device_ids, &num_devices);
            check_and_print_cl_err(err);
#ifdef PRINT_LIB_ERRORS
            fprintf(stderr, "num_devices: %d\n", num_devices);
#endif
        }
#ifdef PRINT_LIB_ERRORS
        fprintf(stderr, "num_devices: %d\n", num_devices);
#endif
        
        device_id = device_ids[deviceToUse];
        print_selected_device_info(device_id, deviceToUse);
        
        context = clCreateContext(0, 1, &device_id, nullptr, nullptr, &err);
        check_and_print_cl_err(err);
        
        commands = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &err);
        check_and_print_cl_err(err);
        return true;
    }
    
    /**
        \brief Read kernel source into program buffer
        @param fname Path to kernel source
        @param programBuffer The buffer for kernel source
    */
    void read_kernels_from_file(const char *fname, char **programBuffer){
        //////* AweM - Read kernel source as an array of char's */
        /* FROM:
         -> http://dhruba.name/2012/08/16/opencl-cookbook-creating-programs-and-reading-kernels-from-a-file/
         */
        // get size of kernel source
        FILE *programHandle = fopen(fname, "r");
        fseek(programHandle, 0, SEEK_END);
        int programSize = (int)ftell(programHandle);
        rewind(programHandle);
        
        // read kernel source into buffer
        (*programBuffer) = (char*) malloc(programSize + 1);
        (*programBuffer)[programSize] = '\0';
        fread((*programBuffer), sizeof(char), programSize, programHandle);
        fclose(programHandle);
    }
    
    /**
        \brief Checks OpenCL errors and prints a more descriptive error message
        @param err The error code
    */
    void check_and_print_cl_err(int err){
        if (err == CL_SUCCESS) return;
        
        std::string err_msg = get_opencl_error(err);
#ifdef PRINT_LIB_ERRORS
        fprintf(stderr, "%s\n", err_msg.c_str());
#endif
        throw std::runtime_error(err_msg);
    }
    
    /**
        \brief error checking specific to building the kernel ('program')
        @param err The error code
        @param program Pointer to the cl_program
    */
    void check_and_print_cl_program_build_err(int err, cl_program *program){
        if (err != CL_SUCCESS){
            size_t len;
            char buffer[2048];
            clGetProgramBuildInfo(*program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
            throw std::runtime_error("Couldn't build executable");
        }
    }
    
    /**
        \brief Print device information
        @param cdi The cl_device_id
        @param device_num The OpenCL device number
    */
    void print_selected_device_info(cl_device_id cdi, int device_num){
        
        char* value;
        size_t valueSize;
        cl_uint maxComputeUnits;
        
        // print device name
        clGetDeviceInfo(cdi, CL_DEVICE_NAME, 0, NULL, &valueSize);
        value = (char*) malloc(valueSize);
        clGetDeviceInfo(cdi, CL_DEVICE_NAME, valueSize, value, NULL);
        printf("%d. Device: %s\n", device_num, value);
        free(value);
        
        // print hardware device version
        clGetDeviceInfo(cdi, CL_DEVICE_VERSION, 0, NULL, &valueSize);
        value = (char*) malloc(valueSize);
        clGetDeviceInfo(cdi, CL_DEVICE_VERSION, valueSize, value, NULL);
        printf(" %d.%d Hardware version: %s\n", device_num, 1, value);
        free(value);
        
        // print software driver version
        clGetDeviceInfo(cdi, CL_DRIVER_VERSION, 0, NULL, &valueSize);
        value = (char*) malloc(valueSize);
        clGetDeviceInfo(cdi, CL_DRIVER_VERSION, valueSize, value, NULL);
        printf(" %d.%d Software version: %s\n", device_num, 2, value);
        free(value);
        
        // print c version supported by compiler for device
        clGetDeviceInfo(cdi, CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &valueSize);
        value = (char*) malloc(valueSize);
        clGetDeviceInfo(cdi, CL_DEVICE_OPENCL_C_VERSION, valueSize, value, NULL);
        printf(" %d.%d OpenCL C version: %s\n", device_num, 3, value);
        free(value);
        
        // print parallel compute units
        clGetDeviceInfo(cdi, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(maxComputeUnits), &maxComputeUnits, NULL);
        printf(" %d.%d Parallel compute units: %d\n", device_num, 4, maxComputeUnits);
    }
    
    /**
        \brief Convert numeric error code to text
        @param err The error code
        @return The error string based on err
    */
    std::string get_opencl_error(int err){
        
        switch (err) {
            case CL_SUCCESS:
                return "No problem!";
            case CL_DEVICE_NOT_FOUND:
                return "OpenCL Error: '-1' - CL_DEVICE_NOT_FOUND - No OpenCL devices that matched given device type were found";
            case CL_DEVICE_NOT_AVAILABLE :
                return "OpenCL Error: '-2' - CL_DEVICE_NOT_AVAILABLE  - No OpenCL compatible device was found";
            case CL_COMPILER_NOT_AVAILABLE:
                return "OpenCL Error: '-3' - CL_COMPILER_NOT_AVAILABLE - OpenCL Compiler perhaps failed to configure itself, or check your OpenCL installation";
            case CL_MEM_OBJECT_ALLOCATION_FAILURE:
                return "OpenCL Error: '-4' - CL_MEM_OBJECT_ALLOCATION_FAILURE - Failed to allocate memory for buffer object";
            case CL_OUT_OF_RESOURCES:
                return "OpenCL Error: '-5' - CL_OUT_OF_RESOURCES - failure to allocate resources required by the OpenCL implementation on the device";
            case CL_OUT_OF_HOST_MEMORY:
                return "OpenCL Error: '-6' - CL_OUT_OF_HOST_MEMORY - failure to allocate resources required by the OpenCL implementation on the host";
            case CL_PROFILING_INFO_NOT_AVAILABLE:
                return "OpenCl Error: '-7' - CL_PROFILING_INFO_NOT_AVAILABLE - returned by clGetEventProfilingInfo, if the CL_QUEUE_PROFILING_ENABLE flag is not set for the command-queue and if the profiling information is currently not available";
            case CL_MEM_COPY_OVERLAP:
                return "OpenCL Error: '-8' - CL_MEM_COPY_OVERLAP - if source and destination buffers are the same buffer object and the source and destination regions overlap";
            case CL_IMAGE_FORMAT_MISMATCH:
                return "OpenCL Error: '-9' - CL_IMAGE_FORMAT_MISMATCH - src and dst image do not use the same image format";
            case CL_IMAGE_FORMAT_NOT_SUPPORTED:
                return "OpenCL Error: '-10' - CL_IMAGE_FORMAT_NOT_SUPPORTED - the image format is not supported.";
            case CL_BUILD_PROGRAM_FAILURE:
                return "OpenCL Error: '-11' - CL_BUILD_PROGRAM_FAILURE - program build error for given device, Use clGetProgramBuildInfo API call to get the build log of the kernel compilation.";
            case CL_MAP_FAILURE:
                return "OpenCL Error: '-12' - CL_MAP_FAILURE - failed to map the requested region into the host address space. This error does not occur for buffer objects created with CL_MEM_USE_HOST_PTR or CL_MEM_ALLOC_HOST_PTR";
            case CL_MISALIGNED_SUB_BUFFER_OFFSET :
                return "OpenCL Error: '-13' - CL_MISALIGNED_SUB_BUFFER_OFFSET  - no devices in given context associated with buffer for which the origin value is aligned to the CL_DEVICE_MEM_BASE_ADDR_ALIGN value";
            case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
                return "OpenCL Error: '-14' - CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST - returned by clWaitForEvents(), execution status of any of the events in event list is a negative integer value i.e., error";
            case CL_COMPILE_PROGRAM_FAILURE:
                return "OpenCL Error: '-15' - CL_COMPILE_PROGRAM_FAILURE - failed to compile the program source. Error occurs if clCompileProgram does not return until the compile has completed";
            case CL_LINKER_NOT_AVAILABLE:
                return "OpenCL Error: '-16' - CL_LINKER_NOT_AVAILABLE - Linker unavailable ";
            case CL_LINK_PROGRAM_FAILURE:
                return "OpenCL Error: '-17' - CL_LINK_PROGRAM_FAILURE - failed to link the compiled binaries and perhaps libraries";
            case CL_DEVICE_PARTITION_FAILED:
                return "OpenCL Error: '-18' - CL_DEVICE_PARTITION_FAILED - given partition name is supported by the implementation but input device couldn't be partitioned further";
            case CL_KERNEL_ARG_INFO_NOT_AVAILABLE:
                return "OpenCL Error: '-19' - CL_KERNEL_ARG_INFO_NOT_AVAILABLE - argument information is not available for the given kernel";
            case CL_INVALID_VALUE:
                return "OpenCL Error: '-30' - CL_INVALID_VALUE - values passed in the flags parameter is not valid";
            case CL_INVALID_DEVICE_TYPE:
                return "OpenCL Error: '-31' - CL_INVALID_DEVICE_TYPE - device type specified is not valid, its returned by clCreateContextFromType / clGetDeviceIDs";
            case CL_INVALID_PLATFORM:
                return "OpenCL Error: '-32' - CL_INVALID_PLATFORM - the specified platform is not a valid platform, its returned by clGetPlatformInfo /clGetDeviceIDs / clCreateContext / clCreateContextFromType ";
            case CL_INVALID_DEVICE:
                return "OpenCL Error: '-33' - CL_INVALID_DEVICE - device/s specified are not valid";
            case CL_INVALID_CONTEXT:
                return "OpenCL Error: '-34' - CL_INVALID_CONTEXT - the given context is invalid OpenCL context, or the context associated with certain parameters are not the same";
            case CL_INVALID_QUEUE_PROPERTIES:
                return "OpenCL Error: '-35' - CL_INVALID_QUEUE_PROPERTIES - specified properties are valid but are not supported by the device, its returned by  clCreateCommandQueue / clSetCommandQueueProperty";
            case CL_INVALID_COMMAND_QUEUE:
                return "OpenCL Error: '-36' - the specified command-queue is not a valid command- CL_INVALID_COMMAND_QUEUE -queue";
            case CL_INVALID_HOST_PTR:
                return "OpenCL Error: '-37' - CL_INVALID_HOST_PTR - host pointer is NULL and CL_MEM_COPY_HOST_PTR or CL_MEM_USE_HOST_PTR are set in flags or if host_ptr is not NULL but CL_MEM_COPY_HOST_PTR or CL_MEM_USE_HOST_PTR are not set in flags. returned by clCreateBuffer / clCreateImage2D / clCreateImage3D ";
            case CL_INVALID_MEM_OBJECT:
                return "OpenCL Error: '-38' - CL_INVALID_MEM_OBJECT - the passed parameter is not a valid memory, image, or buffer object";
            case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
                return "OpenCL Error: '-39' - CL_INVALID_IMAGE_FORMAT_DESCRIPTOR - image format specified is not valid or is NULL, clCreateImage2D /clCreateImage3D returns this.";
            case CL_INVALID_IMAGE_SIZE:
                return "OpenCL Error: '-40' - CL_INVALID_IMAGE_SIZE - specified image width or height are outbound (?) or 0";
            case CL_INVALID_SAMPLER:
                return "OpenCL Error: '-41' - CL_INVALID_SAMPLER - specified sampler is an invalid sampler object";
            case CL_INVALID_BINARY:
                return "OpenCL Error: '-42' - CL_INVALID_BINARY - program binary is not a valid binary for the specified device, returned by clBuildProgram / clCreateProgramWithBinary ";
            case CL_INVALID_BUILD_OPTIONS:
                return "OpenCL Error: '-43' - CL_INVALID_BUILD_OPTIONS - the given build options are not valid";
            case CL_INVALID_PROGRAM:
                return "OpenCL Error: '-44' - CL_INVALID_PROGRAM - the given program is an invalid program object, returned by clRetainProgram / clReleaseProgram / clBuildProgram / clGetProgramInfo / clGetProgramBuildInfo / clCreateKernel / clCreateKernelsInProgram ";
            case CL_INVALID_PROGRAM_EXECUTABLE:
                return "OpenCL Error: '-45' - CL_INVALID_PROGRAM_EXECUTABLE - if there is no successfully built executable for program returned by clCreateKernel, there is no device in program then returned by clCreateKernelsInProgram, if no successfully built program executable present for device associated with command queue then returned by clEnqueueNDRangeKernel / clEnqueueTask";
            case CL_INVALID_KERNEL_NAME:
                return "OpenCL Error: '-46' - CL_INVALID_KERNEL_NAME - mentioned kernel name is not found in program";
            case CL_INVALID_KERNEL_DEFINITION:
                return "OpenCL Error: '-47' - CL_INVALID_KERNEL_DEFINITION - arguments mismatch for the __kernel function definition and the passed ones, returned by clCreateKernel";
            case CL_INVALID_KERNEL:
                return "OpenCL Error: '-48' - CL_INVALID_KERNEL - specified kernel is an invalid kernel object";
            case CL_INVALID_ARG_INDEX:
                return "OpenCL Error: '-49' - CL_INVALID_ARG_INDEX - clSetKernelArg if an invalid argument index is specified";
            case CL_INVALID_ARG_VALUE:
                return "OpenCL Error: '-50' - CL_INVALID_ARG_VALUE - the argument value specified is NULL, returned by clSetKernelArg";
            case CL_INVALID_ARG_SIZE:
                return "OpenCL Error: '-51' - CL_INVALID_ARG_SIZE - the given argument size (arg_size) do not match size of the data type for an argument, returned by clSetKernelArg";
            case CL_INVALID_KERNEL_ARGS:
                return "OpenCL Error: '-52' - CL_INVALID_KERNEL_ARGS - the kernel argument values have not been specified, returned by clEnqueueNDRangeKernel / clEnqueueTask";
            case CL_INVALID_WORK_DIMENSION:
                return "OpenCL Error: '-53' - CL_INVALID_WORK_DIMENSION - given work dimension is an invalid value, returned by clEnqueueNDRangeKernel";
            case CL_INVALID_WORK_GROUP_SIZE:
                return "OpenCL Error: '-54' - CL_INVALID_WORK_GROUP_SIZE - the specified local workgroup size and number of workitems specified by global workgroup size is not evenly divisible by local workgroup size";
            case CL_INVALID_WORK_ITEM_SIZE:
                return "OpenCL Error: '-55' - CL_INVALID_WORK_ITEM_SIZE - no. of workitems specified in any of local work group sizes is greater than the corresponding values specified by CL_DEVICE_MAX_WORK_ITEM_SIZES in that particular dimension";
            case CL_INVALID_GLOBAL_OFFSET:
                return "OpenCL Error: '-56' - CL_INVALID_GLOBAL_OFFSET - global_work_offset is not NULL. Must currently be a NULL value. In a future revision of OpenCL, global_work_offset can be used but not until OCL 1.2";
            case CL_INVALID_EVENT_WAIT_LIST:
                return "OpenCL Error: '-57' - CL_INVALID_EVENT_WAIT_LIST - event wait list is NULL and (no. of events in wait list &gt; 0), or event wait list is not NULL and no. of events in wait list is 0, or specified event objects are not valid events";
            case CL_INVALID_EVENT:
                return "OpenCL Error: '-58' - CL_INVALID_EVENT - invalid event objects specified";
            case CL_INVALID_OPERATION:
                return "OpenCL Error: '-59' - CL_INVALID_OPERATION - ";
            case CL_INVALID_GL_OBJECT:
                return "OpenCL Error: '-60' - CL_INVALID_GL_OBJECT - not a valid GL buffer object";
            case CL_INVALID_BUFFER_SIZE:
                return "OpenCL Error: '-61' - CL_INVALID_BUFFER_SIZE - the value of the parameter size is 0 or exceeds CL_DEVICE_MAX_MEM_ALLOC_SIZE for all devices specified in the parameter context, returned by clCreateBuffer";
            case CL_INVALID_MIP_LEVEL :
                return "OpenCL Error: '-62' - CL_INVALID_MIP_LEVEL  - ";
            case CL_INVALID_GLOBAL_WORK_SIZE:
                return "OpenCL Error: '-63' - CL_INVALID_GLOBAL_WORK_SIZE - specified global work size is NULL, or any of the values specified in global work dimensions are 0 or exceeds the range given by the sizeof(size_t) for the device on which the kernel will be enqueued, returned by clEnqueueNDRangeKernel";
            case CL_INVALID_PROPERTY:
                return "OpenCL Error: '-64' - CL_INVALID_PROPERTY - context property name in properties is not a supported property name, returned by clCreateContext";
            case CL_INVALID_IMAGE_DESCRIPTOR:
                return "OpenCL Error: '-65' - CL_INVALID_IMAGE_DESCRIPTOR - values specified in image description are invalid";
            case CL_INVALID_COMPILER_OPTIONS:
                return "OpenCL Error: '-66' - CL_INVALID_COMPILER_OPTIONS - compiler options specified by options are invalid, returned by clCompileProgram";
            case CL_INVALID_LINKER_OPTIONS:
                return "OpenCL Error: '-67' - CL_INVALID_LINKER_OPTIONS - linker options specified by options are invalid, returned by clLinkProgram";
            case CL_INVALID_DEVICE_PARTITION_COUNT:
                return "OpenCL Error: '-68' - CL_INVALID_DEVICE_PARTITION_COUNT";
            default:
                return "No known error definition -- error code: " + std::to_string(err);
        }
    }
    
    /**
        \brief A relic from our less politically correct days
    */
    void print_dferr(const char *opt){
        fprintf(stderr, "ASCII art has been removed\n");
        if (!(opt == NULL)) {
            std::cout << opt << std::endl;
        }
        exit(256);
    }
    
public:
    int err;
    unsigned int correct;
    int maxDevices; 
    int deviceToUse;
    cl_device_id *device_ids = new cl_device_id[maxDevices];
    cl_device_id device_id;
    cl_context context;
    cl_command_queue commands;
    cl_uint num_devices;
};
