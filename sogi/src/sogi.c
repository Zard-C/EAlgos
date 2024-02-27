#include <stdio.h>
#include <math.h>
#include <unistd.h>

// SOGI参数
#define SAMPLING_FREQUENCY 20000 // 采样频率, 例如20kHz
#define GRID_FREQUENCY     50    // 电网频率, 例如50Hz
#define OMEGA              (2.0 * 3.14159 * GRID_FREQUENCY) // ω
#define K                  0.15  // 增益

// 状态变量
double q_v = 0; // 正交分量的状态
double q_v_last = 0; // 上一次的正交分量状态 
double v = 0;   // 同相分量的状态
double v_last = 0; // 上一次的同相分量状态
double amplitude = 0; // 电网电压幅值
double omega = OMEGA;    // 初始电网频率
double gain = 1.2; // 增益
double step = 1.0 / SAMPLING_FREQUENCY;


void sogi_update(double input) 
{
    double delta = input - v;
    delta = delta * gain;
    delta = delta - q_v;
    delta = delta * omega;
    q_v_last = q_v;
    v_last = v;
    v = v_last + step* delta;   
    q_v = q_v_last + step* omega * v;   
    // // add frequency lock loop
    // omega = omega + 0.01 * step * q_v_last;
    amplitude = sqrt(v * v   + q_v * q_v);
}

int main() {
    for (int i = 0; i < 200 * SAMPLING_FREQUENCY /GRID_FREQUENCY ; i++) {
        double input = 220 * sin(2.0 * 3.14159 * GRID_FREQUENCY * i / SAMPLING_FREQUENCY);
        sogi_update(input);
        printf("omega: %lf, input: %lf,  v: %lf, q_v: %lf, amplitude: %lf, phase = %lf\n", omega, input, v_last, q_v_last, amplitude, atan2(q_v_last, v_last));
        usleep(5000);
    }
    return 0;
}