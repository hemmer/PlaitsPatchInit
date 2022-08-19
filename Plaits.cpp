#include "daisy_patch_sm.h"
#include "daisysp.h"
#include "plaits/dsp/voice.h"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

DaisyPatchSM hw;

plaits::Voice voice;
plaits::Patch patch;
plaits::Modulations modulations;

#define BLOCK_SIZE 16

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	hw.ProcessAllControls();

	plaits::Voice::Frame output[BLOCK_SIZE];

	voice.Render(patch, modulations, output, BLOCK_SIZE);


	for (size_t i = 0; i < size; i++)
	{
		OUT_L[i] = output[i].out / 32768.f;				
		OUT_R[i] = output[i].aux / 32768.f;				
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(BLOCK_SIZE); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);
	while(1) {}
}
