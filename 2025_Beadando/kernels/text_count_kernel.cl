__kernel void text_count_kernel(__global const char* text,
                                __global int* counts,
                                const int length) {
    int id = get_global_id(0);
    if (id >= length) return;

    uchar ch = text[id]; // karakter (0–255)
    atomic_add(&counts[ch], 1);
}