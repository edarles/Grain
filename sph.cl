__kernel void integrate (__global float* x, __global float vel* y, __global float* a, float dt)
{
    const int i = get_global_id (0);
    vel[i*3] += a[i*3] * dt;
    vel[(i*3)+1] += a[(i*3)+1] * dt;
    vel[(i*3)+2] += a[(i*3)+2] * dt;
    pos[i*4] += vel[i*3] * dt;
    pos[(i*4)+1] += vel[(i*3)+1] * dt;
    pos[(i*4)+2] += vel[(i*3)+2] * dt;
}