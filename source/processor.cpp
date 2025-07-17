//------------------------------------------------------------------------
// Copyright(c) 2025 Void Audio.
//------------------------------------------------------------------------

#include "processor.h"
#include "cids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

#include <cmath>

using namespace Steinberg;

namespace VOID {
//------------------------------------------------------------------------
// SwellProcessor
//------------------------------------------------------------------------
SwellProcessor::SwellProcessor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kSwellControllerUID);
}

//------------------------------------------------------------------------
SwellProcessor::~SwellProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API SwellProcessor::initialize (FUnknown* context)
{
/*
 * The host initiates this then resets with setBusArrangements() if necessary
 */
    tresult result = AudioEffect::initialize (context);
    if (result != kResultOk)
        return result;

    // main stereo signal
    addAudioInput  (STR16("Stereo In"),  Steinberg::Vst::SpeakerArr::kStereo);
    addAudioOutput (STR16("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);






    return kResultOk;
}

tresult PLUGIN_API SwellProcessor::setBusArrangements (Steinberg::Vst::SpeakerArrangement* inputs, Steinberg::int32 numIns,
                                                            Steinberg::Vst::SpeakerArrangement* outputs, Steinberg::int32 numOuts)
/*
 * The host automatically calls setBusArrangement() from the base class, so we can override it
 * to handle the exact cases we need
 */
{
    // Confirm proper format
    if (numIns == 2 && numOuts == 1) {
        int32 chin1  = Steinberg::Vst::SpeakerArr::getChannelCount (inputs[0]);
        int32 chout1 = Steinberg::Vst::SpeakerArr::getChannelCount (outputs[0]);

        // handle case mono in/out/sidechain
        if (chin1 == 1 && chout1 == 1 ) {
            // initiate the correct buffers
            removeAudioBusses();
            addAudioInput(STR16("Mono In"),  inputs[0]);
            addAudioOutput(STR16("Mono Out"), outputs[0]);
            addAudioInput(STR16("Mono Sidechain"), inputs[1], Steinberg::Vst::kAux, 0);
            return kResultOk;
        }
        // handle case stereo in/out mono sidechain
        if (chin1 == 2 && chout1 == 2) {
            // initiate the correct buffers
            removeAudioBusses();
            addAudioInput(STR16("Mono In"),  inputs[0]);
            addAudioOutput(STR16("Mono Out"), outputs[0]);
            return kResultOk;
        }
        // handle case stereo in/out/sidechain
        if(chin1 == 2 && chout1 == 2 ) {
            // initiate the correct buffers
            removeAudioBusses();
            addAudioInput(STR16("Mono In"),  inputs[0]);
            addAudioOutput(STR16("Mono Out"), outputs[0]);
            return kResultOk;
        }
        // handle case mono in/out stereo sidechain (who tf would ever do this? idk but we gotta make it stupid proof)
        if(chin1 == 1 && chout1 == 1) {
            // initiate the correct buffers
            removeAudioBusses();
            addAudioInput(STR16("Mono In"),  inputs[0]);
            addAudioOutput(STR16("Mono Out"), outputs[0]);
            return kResultOk;
        }
    }
    return kResultFalse;
}


//------------------------------------------------------------------------
tresult PLUGIN_API SwellProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API SwellProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API SwellProcessor::process(Vst::ProcessData& data) 
{
	// Handle parameter changes
	if (data.inputParameterChanges)
	{
		int32 numParamsChanged = data.inputParameterChanges->getParameterCount();
		for (int32 index = 0; index < numParamsChanged; ++index)
		{
			if (auto* paramQueue = data.inputParameterChanges->getParameterData(index))
			{
				Vst::ParamID id = paramQueue->getParameterId();
				int32 numPoints = paramQueue->getPointCount();

				if (numPoints > 0)
				{
					Vst::ParamValue value;
					int32 sampleOffset;
					if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
					{
						if        (id == 10) {
							distortionAmountMix = value;
						} else if (id == 0) {              // PARAMSSSSSSSSSSSSS for each
							bypassValue = value;
						}
					}
				}
			}
		}
	}

float mix    = static_cast<float>(distortionAmountMix);
float bypass = static_cast<float>(bypassValue);
if (data.numSamples > 0)
    {
        int32 minBus = std::min (data.numInputs, data.numOutputs);
        for (int32 i = 0; i < minBus; i++)
        {
            int32 minChan = std::min (data.inputs[i].numChannels, data.outputs[i].numChannels);
            for (int32 c = 0; c < minChan; c++)
            {
                float* in  = data.inputs[i].channelBuffers32[c];
                float* out = data.outputs[i].channelBuffers32[c];

                for (int32 s = 0; s < data.numSamples; s++)
                {
					if (bypass == 1.0) { out[s] = in[s]; } else {
                    	out[s] = in[s] * (1.0f - mix) + std::tanh( in[s] * 50.0f) * mix;
					}

                }
            }

            data.outputs[i].silenceFlags = data.inputs[i].silenceFlags;

            // clear the remaining output channels
            for (int32 c = minChan; c < data.outputs[i].numChannels; c++)
            {
                memset(data.outputs[i].channelBuffers32[c], 0, data.numSamples * sizeof(Vst::Sample32));
                data.outputs[i].silenceFlags |= ((uint64)1 << c);
            }
        }

        // clear the remaining output buses
        for (int32 i = minBus; i < data.numOutputs; i++)
        {
            for (int32 c = 0; c < data.outputs[i].numChannels; c++)
            {
                memset(data.outputs[i].channelBuffers32[c], 0, data.numSamples * sizeof(Vst::Sample32));
            }
            data.outputs[i].silenceFlags = ((uint64)1 << data.outputs[i].numChannels) - 1;
        }
    }

    return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SwellProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API SwellProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SwellProcessor::setState(IBStream* state)
{
    IBStreamer streamer(state, kLittleEndian);
    double val = 0.0;

    if (!streamer.readDouble(val))
        return kResultFalse;
    distortionAmountMix = val;

    if (!streamer.readDouble(val))    //  
        return kResultFalse;          //  add these blocks for each param
    bypassValue = val;                //

    return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SwellProcessor::getState(IBStream* state)
{
    IBStreamer streamer(state, kLittleEndian);
    streamer.writeDouble(distortionAmountMix);
    streamer.writeDouble(bypassValue);    // just add lines for param
    return kResultOk;
}

//------------------------------------------------------------------------

void SwellProcessor::setDistortionAmount(double value)
{
    distortionAmountMix = value;
}
} // namespace VOID


