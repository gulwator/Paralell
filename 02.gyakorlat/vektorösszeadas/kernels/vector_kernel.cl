__kernel void vector_kernel(__global float* beVector1, __global float* beVector2, __global float* kiVector, int n)
{

    int i=get_group_id(0);
    if(i<n){
    // printf("%d\n",n);
    // printf("%d\n",i);
    kiVector[i]=beVector1[i]+beVector2[i];
    // printf("%f + %f = %f\n ",beVector1[i],beVector2[i],kiVector[i]);
    }

    // int value = 11;
    // #ifdef SET_ME
    // value = SET_ME;
    // #endif
    // if (get_global_id(0) < n) {
    //     buffer[get_global_id(0)] = value;
    // }
}
