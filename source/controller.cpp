//------------------------------------------------------------------------
// Copyright(c) 2025 Void Audio.
//------------------------------------------------------------------------

#include "controller.h"
#include "cids.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "base/source/fstreamer.h"
#include "public.sdk/source/vst/vstaudioeffect.h"

using namespace Steinberg;


namespace VOID {

//------------------------------------------------------------------------
// SwellController Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API SwellController::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated

	//---do not forget to call parent ------
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

	// PARAMETERS ---------------------------- !

	    /*
	 *
	 * Register all the parameters we plan on using
	 *
	 */

	parameters.addParameter(
	STR16("distortionAmount"),         // parameter title/name
	nullptr,                 // units (nullptr if none)
	0,                       // no step count (continuous)
	0.0,                     // default normalized value (0.5)
	Vst::ParameterInfo::kCanAutomate,  // flags (can automate)
	10                // a unique parameter ID (uint32)
    );

	parameters.addParameter(
	STR16("driveAmount"),         // parameter title/name
	nullptr,                 // units (nullptr if none)
	0,                       // no step count (continuous)
	0.0,                     // default normalized value (0.5)
	Vst::ParameterInfo::kCanAutomate,  // flags (can automate)
	11                // a unique parameter ID (uint32)
    );

	parameters.addParameter(
	STR16("extraParam"),         // parameter title/name
	nullptr,                 // units (nullptr if none)
	0,                       // no step count (continuous)
	0.0,                     // default normalized value (0.5)
	Vst::ParameterInfo::kCanAutomate,  // flags (can automate)
	12                // a unique parameter ID (uint32)
    );




	// ----------- Bypass ---------------------------------
	parameters.addParameter(
    STR16("Bypass"),                 // parameter title/name
    nullptr,                         // units (nullptr if none)
    2,                               // step count: 2 (Off/On)
    0.0,                             // default normalized value (0 = Off)
    Vst::ParameterInfo::kCanAutomate,// flags (can automate)
    0                                // unique parameter ID (uint32)
	);

	

	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SwellController::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!

	//---do not forget to call parent ------
	return EditControllerEx1::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API SwellController::setComponentState (IBStream* state)
{
	// Here you get the state of the component (Processor part)
	if (!state)
		return kResultFalse;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SwellController::setState (IBStream* state)
{
	// Here you get the state of the controller
    Steinberg::IBStreamer s(state, kLittleEndian);
    double val = 0.0;
    if (!s.readDouble(val))
        return kResultFalse;
    distortionAmountMix = val;       // keep adding for params

	if (!s.readDouble(val))
        return kResultFalse;
    driveAmountMix = val; 

	if (!s.readDouble(val))
        return kResultFalse;
    extraParamAmountMix = val; 



	// bypass
    if (!s.readDouble(val))
        return kResultFalse;
    bypassValue = val;

    return kResultOk;
        
}

//------------------------------------------------------------------------
tresult PLUGIN_API SwellController::getState (IBStream* state)
{
	// Here you are asked to deliver the state of the controller (if needed)
	// Note: the real state of your plug-in is saved in the processor
	Steinberg::IBStreamer s(state, kLittleEndian);
    s.writeDouble(distortionAmountMix); 
	s.writeDouble(driveAmountMix);
	s.writeDouble(extraParamAmountMix);     // keep adding for params

	// bypass
	s.writeDouble(bypassValue); 
	return kResultOk;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API SwellController::createView(FIDString name)
{
    if (FIDStringsEqual(name, Vst::ViewType::kEditor))
    {
        // "view" is the template name in your .uidesc
        // "editor.uidesc" should be your actual UI description file name
        auto* view = new VSTGUI::VST3Editor(this, "view", "editor.uidesc");
        return view;
    }
    return nullptr;
}

//------------------------------------------------------------------------

tresult PLUGIN_API SwellController::setParamNormalized(Steinberg::Vst::ParamID pID, Steinberg::Vst::ParamValue value)
	{
	if (pID == 10) // Distortion Amount param
	{
		// clamp, check change, assign, notify
		if (value < 0.0) value = 0.0;
		else if (value > 1.0) value = 1.0;

		if (distortionAmountMix != value)
		{
			distortionAmountMix = value;
			performEdit(pID, distortionAmountMix);
		}
	}

	if (pID == 11) // Drive Amount param
	{
		if (value < 0.0) value = 0.0;
		else if (value > 1.0) value = 1.0;

		if (driveAmountMix != value)
		{
			driveAmountMix = value;
			performEdit(pID, driveAmountMix);
		}
	}

	if (pID == 12) // Extra Param Amount param
	{
		if (value < 0.0) value = 0.0;
		else if (value > 1.0) value = 1.0;

		if (extraParamAmountMix != value)
		{
			extraParamAmountMix = value;
			performEdit(pID, extraParamAmountMix);
		}
	}

	if (pID == 0) // Bypass param
	{
		if (value < 0.0) value = 0.0;
		else if (value > 1.0) value = 1.0;

		if (bypassValue != value)
		{
			bypassValue = value;
			performEdit(pID, bypassValue);     // For any other parameters, pass it up the chain ADD MORE IF NEEDED 
		}
	}
	else
	{
		
		return EditControllerEx1::setParamNormalized(pID, value);
	}

    return kResultOk;
}

Steinberg::Vst::ParamValue PLUGIN_API SwellController::getParamNormalized(Steinberg::Vst::ParamID pID)
{
    switch (pID)
    {
        case 10:
            return distortionAmountMix;
		case 11:
            return driveAmountMix;
		case 12:
            return extraParamAmountMix;  // add more params here


		// bypass	
		case 0:
            return bypassValue;

        default:
            return EditControllerEx1::getParamNormalized(pID);
    }
}

} // namespace VOID
