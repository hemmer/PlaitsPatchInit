#include "daisy_patch_sm.h"
#include "daisysp.h"
#include "plaits/dsp/voice.h"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

DaisyPatchSM hw;

plaits::Voice voice = {};
plaits::Patch patch = {};
plaits::Modulations modulations = {};
char shared_buffer[16384] = {};

#define BLOCK_SIZE 16
plaits::Voice::Frame outputPlaits[BLOCK_SIZE];


Switch button;
Switch toggle;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) {

	button.Debounce();
	toggle.Debounce();

	hw.ProcessAllControls();

	if (button.RisingEdge() || hw.gate_in_2.Trig()) {
		patch.engine = (patch.engine + 1) % voice.GetNumEngines();
	}

	float pitch = hw.GetAdcValue(CV_1) * 8.f - 4.f;

	patch.note = 60.f + pitch * 12.f;
	patch.harmonics = hw.GetAdcValue(CV_2);
	patch.timbre = hw.GetAdcValue(CV_3);
	patch.morph = hw.GetAdcValue(CV_4);
	patch.lpg_colour = 0.5f;
	patch.decay = 0.5f;
	patch.frequency_modulation_amount = 0.f;
	patch.timbre_modulation_amount = 0.f;
	patch.morph_modulation_amount = 0.f;

	float voct_cv = hw.GetAdcValue(CV_5);
	float voct    = fmap(voct_cv, 0.f, 60.f);
	modulations.note = voct;

	// todo: work out how modulations.timbre_patched etc can work
	modulations.harmonics = hw.GetAdcValue(CV_6);
	modulations.timbre = hw.GetAdcValue(CV_7);
	modulations.morph = hw.GetAdcValue(CV_8);

	if (toggle.Pressed()) {
		modulations.trigger = 5.f * hw.gate_in_1.State();
		modulations.trigger_patched = true;
	}
	else {
		modulations.trigger = 0.f;
		modulations.trigger_patched = false;
	}

	voice.Render(patch, modulations, outputPlaits, BLOCK_SIZE);

	for (size_t i = 0; i < size; i++) {
		OUT_L[i] = outputPlaits[i].out / 32768.f;
		OUT_R[i] = outputPlaits[i].aux / 32768.f;
	}
}


int main(void) {
	hw.Init();
	hw.SetAudioBlockSize(BLOCK_SIZE); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	button.Init(hw.B7);
	toggle.Init(hw.B8);

	patch.engine = 0;
	modulations.engine = 0;

	stmlib::BufferAllocator allocator(shared_buffer, sizeof(shared_buffer));
	voice.Init(&allocator);

	hw.StartAudio(AudioCallback);

	while (1) {	}
}
