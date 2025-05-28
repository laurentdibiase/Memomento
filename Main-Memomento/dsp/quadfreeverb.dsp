/*

Freeverb version for quadriphonic listening

Using:
_ : mono_freeverb(fb1, fb2, damp, spread) : _;
Where:

fb1: coefficient of the lowpass comb filters (0-1)
fb2: coefficient of the allpass comb filters (0-1)
damp: damping of the lowpass comb filter (0-1)
spread: spatial spread in number of samples (for stereo)
*/


import("stdfaust.lib");

quadfreeverb = _,_,_,_ <: (*(g)*fixedgain,*(g)*fixedgain,*(g)*fixedgain,*(g)*fixedgain :
	monoverb,monoverb,monoverb,monoverb),
	*(1-g),*(1-g),*(1-g),*(1-g) :> _,_,_,_
with{
    monoverb = re.mono_freeverb(combfeed, allpassfeed, damping, spatSpread);
	scaleroom   = 0.28;
	offsetroom  = 0.7;
	allpassfeed = 0.5;
	scaledamp   = 0.4;
	fixedgain   = 0.1;
	origSR = 44100;

	parameters(x) = vgroup("freeverb",x);
	knobGroup(x) = parameters(vgroup("[0]",x));
	damping = knobGroup(hslider("[0] damp",0.3, 0, 1, 0.025)*scaledamp*origSR/ma.SR);
	combfeed = knobGroup(hslider("[1] room size", 0.36, 0, 1, 0.025)*scaleroom*origSR/ma.SR + offsetroom);
	spatSpread = knobGroup(hslider("[2] stereo spread",0.5,0,1,0.01)*46*ma.SR/origSR : int);
	g = parameters(hslider("[1] wet [midi:ctrl 91]", 0.31, 0, 1, 0.025));
};

process = quadfreeverb;
