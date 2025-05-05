#include "kernel_loader.h"
#include "txt_loader.h"

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>




int main(void)
{
    cl_int err;

    // Szöveg betöltése
    int txt_length = 0;
    char* filename = "mid-doc.txt";
    char* text = load_text_file(filename, &txt_length);
    if (!text) {
        printf("Nem sikerult beolvasni a fajlt!\n");
        return 1;
    }

    printf("Szoveg hossza: %d karakter\n", txt_length);


    // Platform lekérdezése
    cl_platform_id platform_id;
    cl_uint n_platforms;
    err = clGetPlatformIDs(1, &platform_id, &n_platforms);

    // Eszköz lekérdezése (GPU)
    cl_device_id device_id;
    cl_uint n_devices;
    err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &n_devices);

    // Kontextus és parancssor létrehozása
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
    cl_command_queue queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &err);

    // Kernel betöltése és fordítása
    int error_code;
    const char* kernel_code = load_kernel_source("kernels/text_count_kernel.cl", &error_code);
    if (error_code != 0) {
        printf("Nem sikerult betolteni a kernelt!\n");
        return 1;
    }

    cl_program program = clCreateProgramWithSource(context, 1, &kernel_code, NULL, &err);
    err = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    if (err != CL_SUCCESS) {
        size_t log_size;
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char* log = (char*)malloc(log_size);
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
        printf("Build hiba:\n%s\n", log);
        free(log);
        return 1;
    }

    cl_kernel kernel = clCreateKernel(program, "text_count_kernel", &err);

    // Buffer létrehozása
    cl_mem text_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, txt_length * sizeof(char), text, &err);
    cl_mem result_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, 256 * sizeof(int), NULL, &err);

    
    // Eredmény buffer inicializálása nullára
    int zeros[256] = {0};
    clEnqueueWriteBuffer(queue, result_buffer, CL_TRUE, 0, 256 * sizeof(int), zeros, 0, NULL, NULL);
    clFinish(queue); 

    // Kernel argumentumok beállítása
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &text_buffer);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &result_buffer);
    clSetKernelArg(kernel, 2, sizeof(int), &txt_length);

    // Kernel futtatása
    size_t global_work_size = txt_length;
    cl_event event;
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, NULL, 0, NULL, &event);
    clFinish(queue);

    
    // Időmérés
    cl_ulong start, end;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
    double elapsed_time = (double)(end - start) / 1e9;
    printf("Parhuzamos futasi idő: %.6f masodperc\n", elapsed_time);

    // Eredmény visszaolvasása
    int result[256];
    clEnqueueReadBuffer(queue, result_buffer, CL_TRUE, 0, sizeof(result), result, 0, NULL, NULL);
    clFinish(queue); 
    // Kiírás
    printf("\nKaraktergyakorisag:\n");
    for (int i = 0; i < 256; ++i) {
        if (result[i] > 0) {
            //olvasható karakterek kiírása
            if (i >= 32 && i <= 126)
                printf("'%c' (%d): %d\n", i, i, result[i]);
            else
                printf("[0x%02x] (%d): %d\n", i, i, result[i]);
        }
    }

    // Erőforrások felszabadítása
    clReleaseMemObject(text_buffer);
    clReleaseMemObject(result_buffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free(text);

    return 0;
}
