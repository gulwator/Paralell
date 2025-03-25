// text mint a file egésze
// global_histogram mint a teljes számok
// text_size mint a karakter szám
// blocksize mint egyes részek nagysága

// TODO: ChatGPT által generált, ellenőrizendő
__kernel void text_count_kernel(__global const char* text, __global int* global_histogram, int text_size, int block_size) {
    int id = get_global_id(0);
    int start = id * block_size;
    int end = min(start + block_size, text_size);
    int local_hist[256] = {0};

    for (int i = start; i < end; i++) {
        atomic_inc(&local_hist[(int)text[i]]);
    }

    for (int j = 0; j < 256; j++) {
        atomic_add(&global_histogram[j], local_hist[j]);
    }
}