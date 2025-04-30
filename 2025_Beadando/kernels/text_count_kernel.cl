// text mint a file egésze
// global_histogram mint a teljes számok
// text_size mint a karakter szám
// blocksize mint egyes részek nagysága

// TODO: ChatGPT által generált, ellenőrizendő
__kernel void text_count_kernel(__global const char* text, __global int* global_histogram, int text_size, int block_size) {
    int id = get_global_id(0);
    int start = id * block_size;
    int end = min(start + block_size, text_size);
   __local int local_hist[256];

    for (int i = get_local_id(0); i < 256; i+= get_local_size(0)) {
        local_hist[i]=0;
    }
    
    barrier(CLK_LOCAL_MEM_FENCE);
    
    for(int i = start; i < end; i++){
        if(i<text_size){
        atomic_add(&local_hist[(int)text[i]],1);

        }
    }
    
    barrier(CLK_LOCAL_MEM_FENCE);
    for (int i = get_local_id; i < 256; i+=get_local_size(0)) {
        atomic_add(&global_histogram[i], local_hist[i]);
    }
}