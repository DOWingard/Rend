//------------------------------------------------------------------------
// Copyright(c) 2025 Void Audio.
//------------------------------------------------------------------------

#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

#include "DspFilters/RBJ.h"      // filters


#include <cmath>
#include <numbers>
namespace VOID {



//------------------------------------------------------------------------
//  SwellProcessor
//------------------------------------------------------------------------
class SwellProcessor : public Steinberg::Vst::AudioEffect {

public:

	SwellProcessor ();
	~SwellProcessor () SMTG_OVERRIDE;

    // Create function
	static Steinberg::FUnknown* createInstance (void* /*context*/) 
	{ 
		return (Steinberg::Vst::IAudioProcessor*)new SwellProcessor; 
	}
	 
	//--- ---------------------------------------------------------------------
	// AudioEffect overrides:
	//--- ---------------------------------------------------------------------
	/** Called at first after constructor */
	Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
	
	/** Called at the end before destructor */
	Steinberg::tresult PLUGIN_API terminate () SMTG_OVERRIDE;

	/*# Called to set busses properly */
	Steinberg::tresult PLUGIN_API setBusArrangements (Steinberg::Vst::SpeakerArrangement* inputs, Steinberg::int32 numIns,
                                                        Steinberg::Vst::SpeakerArrangement* outputs, Steinberg::int32 numOuts) SMTG_OVERRIDE;
	
	/** Switch the Plug-in on/off */
	Steinberg::tresult PLUGIN_API setActive (Steinberg::TBool state) SMTG_OVERRIDE;

	/** Will be called before any process call */
	Steinberg::tresult PLUGIN_API setupProcessing (Steinberg::Vst::ProcessSetup& newSetup) SMTG_OVERRIDE;
	
	/** Asks if a given sample size is supported see SymbolicSampleSizes. */
	Steinberg::tresult PLUGIN_API canProcessSampleSize (Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;

	/** Here we go...the process call */
	Steinberg::tresult PLUGIN_API process (Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;
		
	/** For persistence */
	Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;


	float clip(float signal, float threshhold);
	float bitcrush(float signal, float mix);
// helper template filter function
	template <typename FilterType>
	void applyFilter(
		std::vector<float*>& filteredPtrs,
		std::vector<std::vector<float>>& filteredSignal,
		float** inputBuffers,
		int numSamples,
		int numChannels,
		FilterType& filter) {
   /***********************************************************************\
	Applies given filter to a deep copy of the samples and returns the copy
   \***********************************************************************/
		for (int ch = 0; ch < numChannels; ++ch) {
			memcpy(filteredSignal[ch].data(), inputBuffers[ch], numSamples * sizeof(float));
		}
		for (int ch = 0; ch < numChannels; ++ch) {
			filteredPtrs[ch] = filteredSignal[ch].data();
		}

		filter.process(numSamples, filteredPtrs.data());

	}
	// filters
	Dsp::SimpleFilter <Dsp::RBJ::LowShelf, 2>  lowShelfFilter;
	Dsp::SimpleFilter <Dsp::RBJ::HighShelf, 1> highShelfFilter;
	Dsp::SimpleFilter <Dsp::RBJ::BandStop, 1>  bandStopFilter;


//------------------------------------------------------------------------
protected:



//------------------------------------------------------------------------

private:
static constexpr float inv_pi = 0.31830988618379067154f;
double distortionAmountMix    = 0.0; 
double bypassValue            = 0.0; 
double driveAmountMix         = 0.0;
double extraParamAmountMix    = 0.0;
double switchstate            = 1.0;
float noiseAmount             = 0.0001f;

std::vector<float*> filteredPtrs;
std::vector<std::vector<float>> filteredSignal;








};

//------------------------------------------------------------------------
} // namespace VOID
