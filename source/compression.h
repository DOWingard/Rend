//------------------------------------------------------------------------
// Copyright(c) 2025 Void Audio.
//------------------------------------------------------------------------
#pragma once

#include <cmath>
#include <algorithm>


namespace VOID {
   /***********************************************************************\
	Simple downward compression implimentation with smoothing, fast attack, and adaptive release
   \***********************************************************************/

class Compressor  {
public:
    /* overload constructors*/

    Compressor() = default;
    
    Compressor(float sr, float thresh)
        : Compressor(sr, 0.01f, 0.207f, 12.0f, -thresh, 3.0f) {}

    Compressor(float sr, float attack_s, float release_s, float knee_w, float thresh, float rat)
    : sampleRate(sr),
        attack_time_s(attack_s),
        release_time_s(release_s),
        knee_width(knee_w),
        threshold(thresh),
        ratio(rat)
    {}


    ~Compressor() = default;

    float process(float input); // main process function

    float ratio;
    
private:

    void toDb();        // convnert to dB
    void dbToLinear(); // dB to linear gain
    void setCoeff();   // sets coeffs for compressor process
    void smooth();     // smooths the input signal
    void softKnee();// soft knee function 


protected:

    float output;
    float sampleRate;
    float coeff;              // envelope smoothing coefficient
    float s;                  // abs(sample)
    float attack_time_s;
    float release_time_s;  
    float knee_width;         // knee width in dB (controlls softness of compression kick-in)
    float threshold;          // threshold in dB
              // compression ratio
    float prevSample = 0;     // previous sample for smoothing
    float eps = 1e-12f;       // avoid /0
    float kneeWidth = 12.0f;  // knee width in dB 

};




//------------------------------------------------------------------------
} // namespace VOID