//------------------------------------------------------------------------
// Copyright(c) 2025 Void Audio.
//------------------------------------------------------------------------

#include "compression.h"

namespace VOID {


float Compressor::process(float input) {
    /*
     * Main process function for the compressor
     */
    s = std::abs(input);


    // setCoeff();
    // smooth();
    toDb();
    softKnee();
    dbToLinear();

    output = input * s; // s is now a linenar gain factor calculated from input

    return output;
}



    
void Compressor::toDb() {
    s = 20.0f * std::log10(std::max(s, eps)); // convert to dB, avoid log(0)
}

void Compressor::dbToLinear() {
    s = std::pow(10.0f, s / 20.0f);
}

void Compressor::setCoeff(){
    // sets the smoothing coefficient based on the sample rate and attack/release times, makes release adaptive instead of hardcoded
    if (s > prevSample) {
        coeff = std::exp(-1.0f / (sampleRate * attack_time_s)); 
    } else {
        coeff = std::exp(-1.0f / (sampleRate * release_time_s))* (std::max(s,eps));
    }
}

void Compressor::smooth() {
    /* 
     * smooths the input signal based on coeffs and previous sample
     */

    s = coeff * s + (1.0f - coeff) * prevSample;
    prevSample = s;
}

void Compressor::softKnee() {
    /*
     * Applies soft knee to compression application
     */

    float signalDif = s - threshold;

    if (signalDif < -kneeWidth/2) {
        s = 0.0f;
    } else if (signalDif > kneeWidth/2) {
        s = (-signalDif) * (1.0f-1.0f/ratio);
    } else {
        s = -(1.0f-1.0f/ratio) * std::pow(signalDif + kneeWidth/2,2) / (2 * kneeWidth);
    }

}


} // namespace VOID