/*
 * File:   CODE.c
 * Author: _ddd_2801_(nickname ??c ??p trai nh?t th? gi?i)
 *
 * Created on Ngày 01 tháng 11 n?m 2024, 14:08
 */
#include <xc.h>
#include <stdint.h>
#include <math.h>

#define PWM_FREQUENCY 10000    // T?n s? PWM 10 kHz
#define TABLE_SIZE 256         // S? ?i?m trong b?ng tra c?u sin
#define PI 3.14159265          // S? pi

#define OFFSET 512             // Offset cho c?m bi?n dòng ?i?n (tu? ch?nh theo c?m bi?n)
#define SCALE_FACTOR 0.1       // H? s? chuy?n ??i ADC -> dòng ?i?n (tu? ch?nh theo c?m bi?n)

uint16_t sinTable[TABLE_SIZE];  // T?o B?ng tra c?u sin

// Hàm kh?i t?o b?ng sin
void InitSinTable(void) 
{
    for (int i = 0; i < TABLE_SIZE; i++) 
    {
        sinTable[i] = (uint16_t)((sin(2 * PI * i / TABLE_SIZE) * 0.5 + 0.5) * 100);
    }
}

// Hàm t?o ?? tr? (??n gi?n hóa b?ng vòng l?p)
void delay_ms(int milliseconds) 
{
    for (volatile int i = 0; i < milliseconds * 1000; i++);  // Vòng l?p tr?
}

// Kh?i t?o PWM
void PWM_Init(void)
{
    T2CONbits.ON = 0;            // T?t Timer2 tr??c khi c?u hình
    T2CONbits.TCKPS = 0b010;     // Chia t?n s? b? ??m (prescaler) 1:4
    T2CONbits.T32 = 0;           // Ch? ?? 16-bit timer
    T2CONbits.TCS = 0;           // S? d?ng xung clock n?i
    PR2 = (40000000 / (PWM_FREQUENCY * 4)) - 1;  // Tính giá tr? PR2 cho t?n s? PWM

    OC6CONbits.OCM = 0b110;      // Ch? ?? PWM cho OC6 (PWM W+)
    OC6CONbits.OCTSEL = 0;       // S? d?ng Timer2 cho OC6
    OC6R = 0;
    OC6RS = 0;
    OC6CONbits.ON = 1;

    OC7CONbits.OCM = 0b110;      // Ch? ?? PWM cho OC7 (PWM W-)
    OC7CONbits.OCTSEL = 0;       // S? d?ng Timer2 cho OC7
    OC7R = 0;
    OC7RS = 0;
    OC7CONbits.ON = 1;

    OC5CONbits.OCM = 0b110;      // Ch? ?? PWM cho OC5 (PWM V+)
    OC5CONbits.OCTSEL = 0;       // S? d?ng Timer2 cho OC5
    OC5R = 0;
    OC5RS = 0;
    OC5CONbits.ON = 1;

    OC8CONbits.OCM = 0b110;      // Ch? ?? PWM cho OC8 (PWM V-)
    OC8CONbits.OCTSEL = 0;       // S? d?ng Timer2 cho OC8
    OC8R = 0;
    OC8RS = 0;
    OC8CONbits.ON = 1;

    OC12CONbits.OCM = 0b110;     // Ch? ?? PWM cho OC12 (PWM U+)
    OC12CONbits.OCTSEL = 0;      // S? d?ng Timer2 cho OC12
    OC12R = 0;
    OC12RS = 0;
    OC12CONbits.ON = 1;

    OC13CONbits.OCM = 0b110;     // Ch? ?? PWM cho OC13 (PWM U-)
    OC13CONbits.OCTSEL = 0;      // S? d?ng Timer2 cho OC13
    OC13R = 0;
    OC13RS = 0;
    OC13CONbits.ON = 1;

    T2CONbits.ON = 1;
}

// Hàm c?p nh?t PWM duty cycle theo góc
void UpdatePWMSinWave(float angle) 
{
    static int index = 0;
    
    // Tính toán phaseShift d?a trên góc mong mu?n (angle)
    int phaseShift = (int)((angle / (2 * PI)) * TABLE_SIZE);

    // L?y giá tr? sin cho t?ng pha v?i ?? d?ch pha mong mu?n
    int phaseU = sinTable[(index + phaseShift) % TABLE_SIZE]; 
    int phaseV = sinTable[(index + phaseShift + TABLE_SIZE / 3) % TABLE_SIZE]; 
    int phaseW = sinTable[(index + phaseShift + 2 * TABLE_SIZE / 3) % TABLE_SIZE]; 

    OC6RS = (PR2 + 1) * phaseW / 100;  
    OC7RS = (PR2 + 1) * (100 - phaseW) / 100;  

    OC5RS = (PR2 + 1) * phaseV / 100;  
    OC8RS = (PR2 + 1) * (100 - phaseV) / 100;  

    OC12RS = (PR2 + 1) * phaseU / 100;  
    OC13RS = (PR2 + 1) * (100 - phaseU) / 100;  

    index = (index + 1) % TABLE_SIZE;   
}


//// C?u hình ADC cho vi?c ?o dòng ?i?n
//void ADC_Init(void) 
//{
//    ADCCON1bits.ON = 0;             // T?t ADC ?? c?u hình
//    ADCCON1bits.SSRC = 0b111;       // T? ??ng kh?i ??ng chuy?n ??i (Synchronous)
//    ADCCON1bits.FORM = 0;           // D? li?u ??u ra là d?ng integer (0: Integer)
//    ADCCON1bits.SAMP = 1;           // B?t ??u l?y m?u
//    ADCCON2bits.SVREF = 0b11;       // C?u hình ngu?n tham chi?u (n?u c?n thi?t)
//    ADCCON3bits.ADCS = 2;           // Chia t?n s? xung clock
//    ADCCON1bits.ADON = 1;           // B?t ADC
//}
//
//// Hàm ??c ADC t? kênh ch? ??nh
//uint16_t ReadADC(uint8_t channel) 
//{
//    ADCCON3bits.CHSEL = channel;    // Ch?n kênh ADC t??ng ?ng v?i channel
//    ADCCON1bits.SAMP = 1;           // B?t ??u l?y m?u
//    while (!ADCCON1bits.DONE);      // Ch? quá trình chuy?n ??i hoàn thành
//    return ADCBUF0;                 // Tr? v? giá tr? ADC
//}

// Hàm ?o dòng ?i?n t? các chân U, V, W
//void MeasureCurrent(void) 
//{
//    uint16_t currentU_ADC = ReadADC(2);  // ??c ADC t? kênh c?a pha U (AN0 -> chân 13)
//    uint16_t currentV_ADC = ReadADC(3);  // ??c ADC t? kênh c?a pha V (AN1 -> chân 14)
//    uint16_t currentW_ADC = ReadADC(4);  // ??c ADC t? kênh c?a pha W (AN2 -> chân 15)
//
//    // Chuy?n ??i giá tr? ADC sang dòng ?i?n
//    float currentU = (currentU_ADC - OFFSET) * SCALE_FACTOR;
//    float currentV = (currentV_ADC - OFFSET) * SCALE_FACTOR;
//    float currentW = (currentW_ADC - OFFSET) * SCALE_FACTOR;
//
//    // X? lý giá tr? dòng ?i?n ?o ???c n?u c?n thi?t
//}

int main(void) 
{
    TRISCbits.TRISC6 = 0; 
    TRISCbits.TRISC7 = 0; 
    TRISCbits.TRISC8 = 0; 
    TRISCbits.TRISC9 = 0; 
    TRISDbits.TRISD5 = 0; 
    TRISDbits.TRISD6 = 0; 

    InitSinTable();  
    PWM_Init();      
//    ADC_Init();      

    while (1) 
    {
        UpdatePWMSinWave(PI / 3);  // Ví d?: c?p nh?t góc kích là 1/3 pi
//        MeasureCurrent();         
        delay_ms(1);              
    }

    return 0;
}
