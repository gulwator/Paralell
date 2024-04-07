#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define NUM_DISKS 4

const char *hanoi_kernel_source =
"void move(int n, int start, int end, int temp, __global int *moves) {\n"
"   if (n == 1) {\n"
"       atomic_inc(moves);\n"
"   } else {\n"
"       move(n - 1, start, temp, end, moves);\n"
"       move(1, start, end, temp, moves);\n"
"       move(n - 1, temp, end, start, moves);\n"
"   }\n"
"}\n"
"kernel void hanoi(__global int *moves) {\n"
"   move(NUM_DISKS, 1, 3, 2, moves);\n"
"}\n";

int main() {
    // Get platform
    cl_uint n_platforms;
    cl_platform_id platform;
    err = clGetPlatformIDs(1,  &platform_id, &n_platforms);
    if(err != CL_SUCCESS){
        printf("[ERROR] Error calling clGetPlatformIDs. Error code: %d\n",err)
        return 0;
    }
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem moves_buffer;

    cl_int err;
    int moves = 0;

    // Get platform
    err = clGetPlatformIDs(1, &platform, NULL);
    // Get device
    err |= clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    // Create context
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    // Create command queue
    queue = clCreateCommandQueue(context, device, 0, &err);

    // Create buffer
    moves_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &err);

    // Build program
    program = clCreateProgramWithSource(context, 1, &hanoi_kernel_source, NULL, &err);
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    // Create kernel
    kernel = clCreateKernel(program, "hanoi", &err);

    // Set kernel arguments
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &moves_buffer);

    // Execute kernel
    size_t global_work_size = 1;
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);

    // Read buffer
    err = clEnqueueReadBuffer(queue, moves_buffer, CL_TRUE, 0, sizeof(int), &moves, 0, NULL, NULL);

    printf("Total moves: %d\n", moves);
    // Clean up
    clReleaseMemObject(moves_buffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

return 0;
}