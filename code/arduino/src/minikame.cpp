#include "minikame.h"

MiniKame::MiniKame(){
    board_pins[0] = D1;
    board_pins[1] = D4,
    board_pins[2] = D8;
    board_pins[3] = D6;
    board_pins[4] = D7;
    board_pins[5] = D5;
    board_pins[6] = D2;
    board_pins[7] = D3;

    trim[0] = 1;
    trim[1] = -8;
    trim[2] = 6;
    trim[3] = 4;
    trim[4] = 2;
    trim[5] = -5;
    trim[6] = 6;
    trim[7] = 2;

    for(int i=0; i<8; i++) servo[i].attach(board_pins[i]);
    for(int i=0; i<8; i++) oscillator[i].setTrim(trim[i]);
    home();
}

void MiniKame::turnR(float steps, float T=600){
    int x_amp = 15;
    int z_amp = 15;
    int ap = 15;
    int hi = 15;
    float period[] = {T, T, T, T, T, T, T, T};
    int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp};
    int offset[] = {90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi};
    int phase[] = {0,180,90,90,180,0,90,90};

    execute(steps, period, amplitude, offset, phase);
}

void MiniKame::turnL(float steps, float T=600){
    int x_amp = 15;
    int z_amp = 15;
    int ap = 15;
    int hi = 15;
    float period[] = {T, T, T, T, T, T, T, T};
    int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp};
    int offset[] = {90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi};
    int phase[] = {180,0,90,90,0,180,90,90};

    execute(steps, period, amplitude, offset, phase);
}

void MiniKame::dance(float steps, float T=600){
    int x_amp = 0;
    int z_amp = 40;
    int ap = 30;
    int hi = 20;
    float period[] = {T, T, T, T, T, T, T, T};
    int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp};
    int offset[] = {90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi};
    int phase[] = {0,0,0,270,0,0,90,180};

    execute(steps, period, amplitude, offset, phase);
}

void MiniKame::run(float steps, float T=5000){
    int x_amp = 15;
    int z_amp = 15;
    int ap = 15;
    int hi = 15;
    int front_x = 6;
    float period[] = {T, T, T, T, T, T, T, T};
    int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp};
    int offset[] = {    90+ap-front_x,
                        90-ap+front_x,
                        90-hi,
                        90+hi,
                        90-ap-front_x,
                        90+ap+front_x,
                        90+hi,
                        90-hi
                    };
    int phase[] = {0,0,90,90,180,180,90,90};

    execute(steps, period, amplitude, offset, phase);
}

void MiniKame::walk(float steps, float T=5000){
    int x_amp = 15;
    int z_amp = 15;
    int ap = 20;
    int hi = 15;
    int front_x = 12;
    float period[] = {T, T, T/2, T/2, T, T, T/2, T/2};
    int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp};
    int offset[] = {    90+ap-front_x,
                        90-ap+front_x,
                        90-hi,
                        90+hi,
                        90-ap-front_x,
                        90+ap+front_x,
                        90+hi,
                        90-hi
                    };
    int  phase[] = {90, 90, 270, 90, 270, 270, 90, 270};

    for (int i=0; i<8; i++){
        oscillator[i].reset();
        oscillator[i].setPeriod(period[i]);
        oscillator[i].setAmplitude(amplitude[i]);
        oscillator[i].setPhase(phase[i]);
        oscillator[i].setOffset(offset[i]);
    }

    unsigned long final = millis() + period[0]*steps;
    unsigned long init = millis();
    while (millis() < final){
        int side = (int)((millis()-init) / (period[0]/2)) % 2;
        servo[0].writeMicroseconds(angToUsec(oscillator[0].refresh()));
        servo[1].writeMicroseconds(angToUsec(oscillator[1].refresh()));
        servo[4].writeMicroseconds(angToUsec(oscillator[4].refresh()));
        servo[5].writeMicroseconds(angToUsec(oscillator[5].refresh()));
        if (side == 0){
            servo[3].writeMicroseconds(angToUsec(oscillator[3].refresh()));
            servo[6].writeMicroseconds(angToUsec(oscillator[6].refresh()));
        }
        else{
            servo[2].writeMicroseconds(angToUsec(oscillator[2].refresh()));
            servo[7].writeMicroseconds(angToUsec(oscillator[7].refresh()));
        }
    }
}

void MiniKame::home(){
    int ap = 20;
    int hi = 35;
    int position[] = {90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi};
    for (int i=0; i<8; i++) servo[i].writeMicroseconds(angToUsec(position[i])+trim[i]);
}

void MiniKame::zero(){
    for (int i=0; i<8; i++){
        servo[i].writeMicroseconds(angToUsec(90+trim[i]));
    }
}

int MiniKame::angToUsec(float value){
    int usecs = value/180 * (MAX_PULSE_WIDTH-MIN_PULSE_WIDTH) + MIN_PULSE_WIDTH;
    return usecs;
}

void MiniKame::execute(float steps, float period[8], int amplitude[8], int offset[8], int phase[8]){
    for (int i=0; i<8; i++){
        oscillator[i].setPeriod(period[i]);
        oscillator[i].setAmplitude(amplitude[i]);
        oscillator[i].setPhase(phase[i]);
        oscillator[i].setOffset(offset[i]);
    }

    unsigned long final = millis() + period[0]*steps;
    while (millis() < final){
        for (int i=0; i<8; i++){
            servo[i].writeMicroseconds(angToUsec(oscillator[i].refresh()));
        }
    }
}
