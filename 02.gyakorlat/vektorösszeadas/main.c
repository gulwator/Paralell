#include "kernel_loader.h"

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int SAMPLE_SIZE = 30000000;

int main(void)
{
    int i;
    cl_int err;
    int error_code;

    // Dinamikusan lefoglalunk egy 1,000,000 elemű tömböt double típusú elemekhez
    
    double* aVector = (double*)malloc(SAMPLE_SIZE * sizeof(double));
    double* bVector = (double*)malloc(SAMPLE_SIZE * sizeof(double));
    double* eredmenyVector = (double*)malloc(SAMPLE_SIZE * sizeof(double));
    double* secVector = (double*)malloc(SAMPLE_SIZE * sizeof(double));
    
    if (aVector == NULL) {
        printf("Nem sikerült lefoglalni a memóriát.\n");
        return 1;
    }
    for (size_t i = 0; i < SAMPLE_SIZE; i++) {
        aVector[i] = (double)rand() / RAND_MAX;  // Véletlenszám 0 és 1 között
        bVector[i] = (double)rand() / RAND_MAX;  // Véletlenszám 0 és 1 között
        }

    // Példa: az első 10 szám kiírása (ha szükséges)
    for (size_t i = 0; i < 10; i++) {
        printf("%f   %f\n", aVector[i],bVector[i]);
    }

    
    // for (int i = 0; i < meret; i++)
    // {
    //     printf("%f \n",eredmenyVector[i]);
    // }
    
    // printf("%d\n",meret);

    // Get platform
    cl_uint n_platforms;
	cl_platform_id platform_id;
    err = clGetPlatformIDs(1, &platform_id, &n_platforms);
	if (err != CL_SUCCESS) {
		printf("[ERROR] Error calling clGetPlatformIDs. Error code: %d\n", err);
		return 0;
	}

    // Get device
	cl_device_id device_id;
	cl_uint n_devices;
	err = clGetDeviceIDs(
		platform_id,
		CL_DEVICE_TYPE_GPU,
		1,
		&device_id,
		&n_devices
	);
	if (err != CL_SUCCESS) {
		printf("[ERROR] Error calling clGetDeviceIDs. Error code: %d\n", err);
		return 0;
	}

    // Create OpenCL context
    cl_context context = clCreateContext(NULL, n_devices, &device_id, NULL, NULL, NULL);

    // Build the program
    const char* kernel_code = load_kernel_source("kernels/vector_kernel.cl", &error_code);
    if (error_code != 0) {
        printf("Source code loading error!\n");
        return 0;
    }
    cl_program program = clCreateProgramWithSource(context, 1, &kernel_code, NULL, NULL);
    const char options[] = "-D SET_ME=1234";
    err = clBuildProgram(
        program,
        1,
        &device_id,
        options,
        NULL,
        NULL
    );
    if (err != CL_SUCCESS) {
        printf("Build error! Code: %d\n", err);
        size_t real_size;
        err = clGetProgramBuildInfo(
            program,
            device_id,
            CL_PROGRAM_BUILD_LOG,
            0,
            NULL,
            &real_size
        );
        char* build_log = (char*)malloc(sizeof(char) * (real_size + 1));
        err = clGetProgramBuildInfo(
            program,
            device_id,
            CL_PROGRAM_BUILD_LOG,
            real_size + 1,
            build_log,
            &real_size
        );
        // build_log[real_size] = 0;
        printf("Real size : %d\n", real_size);
        printf("Build log : %s\n", build_log);
        free(build_log);
        return 0;
    }
    size_t sizes_param[10];
    size_t real_size;
    err = clGetProgramInfo(
        program,
        CL_PROGRAM_BINARY_SIZES,
        10,
        sizes_param,
        &real_size
    );
    printf("Real size   : %d\n", real_size);
    printf("Binary size : %d\n", sizes_param[0]);
    cl_kernel kernel = clCreateKernel(program, "vector_kernel", NULL);

    // Create the host buffer and initialize it
    int* host_buffer = (int*)malloc(SAMPLE_SIZE * sizeof(int));
    for (i = 0; i < SAMPLE_SIZE; ++i) {
        host_buffer[i] = i;
    }

    // Create the device buffer
    cl_mem device_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, SAMPLE_SIZE * sizeof(float), NULL, NULL);
    cl_mem device_bufferBE = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, SAMPLE_SIZE * sizeof(float), aVector, NULL);
    cl_mem device_bufferBE2 = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, SAMPLE_SIZE * sizeof(float), bVector, NULL);
    // cl_mem device_bufferVectorMeret = clCreateBuffer(context, CL_MEM_READ_WRITE, SAMPLE_SIZE * sizeof(int), meret, NULL);
    cl_mem device_bufferKI = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR , SAMPLE_SIZE * sizeof(int), eredmenyVector, NULL);
    
    // Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&device_bufferBE);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&device_bufferBE2);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&device_bufferKI);
    // clSetKernelArg(kernel, 3, sizeof(cl_mem), (void*)&device_bufferVectorMeret);
    clSetKernelArg(kernel, 3, sizeof(int), (void*)&SAMPLE_SIZE);
    
    
    // Create the command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, NULL);
    
    //A vektor átadása
    // Host buffer -> Device buffer
    clEnqueueWriteBuffer(
        command_queue,
        device_bufferBE,
        CL_FALSE,
        0,
        SAMPLE_SIZE * sizeof(int),
        aVector,
        0,
        NULL,
        NULL
    );
    
    //B vektor átadása
    // Host buffer -> Device buffer
    clEnqueueWriteBuffer(
        command_queue,
        device_bufferBE2,
        CL_FALSE,
        0,
        SAMPLE_SIZE * sizeof(int),
        bVector,
        0,
        NULL,
        NULL
    );
    
    // //Vektor méret
    // // Host buffer -> Device buffer
    // clEnqueueWriteBuffer(
    //     command_queue,
    //     device_bufferVectorMeret,
    //     CL_FALSE,
    //     0,
    //     SAMPLE_SIZE * sizeof(int),
    //     host_buffer,
    //     0,
    //     NULL,
    //     NULL
    // );
    clFinish(command_queue);

    // Size specification
    size_t local_work_size = 1;
    size_t n_work_groups = (SAMPLE_SIZE + local_work_size + 1) / local_work_size;
    size_t global_work_size = n_work_groups * local_work_size;

    // Apply the kernel on the range
    cl_event event;
    err = clEnqueueNDRangeKernel(
        command_queue,
        kernel,
        1,
        NULL,
        &global_work_size,
        &local_work_size,
        0,
        NULL,
        &event
    );
if(err!=CL_SUCCESS){
    printf("az baj\n");
}

    // Show profiling information 
    cl_ulong ns;
    clFinish(command_queue);
    err=clGetEventProfilingInfo(
        event,
        CL_PROFILING_COMMAND_QUEUED,
        sizeof(ns),
        &ns,
        NULL
    );
    if (err == CL_PROFILING_INFO_NOT_AVAILABLE) {
        printf("Profiling info not available!\n");
        return 0;
    } else if (err != CL_SUCCESS) {
        printf("Error code: %d\n", err);
        return 0;
    }
    printf("Queued : %lu\n", ns);
    double start_time_par= ns;
    clGetEventProfilingInfo(
        event,
        CL_PROFILING_COMMAND_END,
        sizeof(ns),
        &ns,
        NULL
    );
    printf("End    : %lu\n", ns);
    double end_time_par = ns;
    // double elapsed_timepar = ((double)(end_time_par - start_time_par)) / CLOCKS_PER_SEC;
    double elapsed_timepar = ((double)(end_time_par - start_time_par)) /1000000000  ;

    // Kiírjuk az eltelt időt
    printf("Parallel Futasi ido: %f masodperc\n", elapsed_timepar);

    // Host buffer <- Device buffer
    clEnqueueReadBuffer(
        command_queue,
        device_bufferKI,
        CL_TRUE,
        0,
        SAMPLE_SIZE * sizeof(float),
        eredmenyVector,
        0,
        NULL,
        NULL
    );

    clFinish(command_queue);
    
    if (err != CL_SUCCESS) {
        printf("Error in clEnqueueReadBuffer: %d\n", err);
    }
    for (i = 0; i < SAMPLE_SIZE; ++i) {
        //ITT IRATOM KI
        // printf("[%d] = %d, ", i, host_buffer[i]);
    }

    for(int j =0; j < SAMPLE_SIZE;j++){
        // printf("[%f + %f] = %f \n",aVector[j],bVector[j],eredmenyVector[j]);
    }

    // Release the resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseDevice(device_id);

    clock_t start_time = clock();
    for (size_t i = 0; i < SAMPLE_SIZE; i++) {
        secVector[i] = aVector[i]+bVector[i];  // Véletlenszám 0 és 1 között
          // Véletlenszám 0 és 1 között
        }
    clock_t end_time = clock();

    // Az eltelt idő kiszámítása másodpercekben
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // Kiírjuk az eltelt időt
    printf("Szekvencialis Futasi ido: %f masodperc\n", elapsed_time);


    free(host_buffer);
    free(aVector);
    free(bVector);
    free(eredmenyVector);
    free(secVector);
}
