declare name "QuadCompressor";
declare author "Laurent Di Biase";	
										   
/* ========= DESCRITPION ==========
Parametric quad compressor based on co.compressor_mono

    ratio: compression ratio (1 = no compression, >1 means compression)
    thresh: dB level threshold above which compression kicks in (0 dB = max level)
    att: attack time = time constant (sec) when level & compression going up
    rel: release time = time constant (sec) coming out of compression
*/										   

import("stdfaust.lib");					   
										
	
compressor = _ :co.compressor_mono(ratio,thresh,att,rel) : _;

ratio = hslider ("[2]ratio",3,1,70,1);
thresh = hslider ("[1]threshold",-12,-70,0,1);
att = hslider ("[3]attack",0.01,0.0001,1,0.0001);
rel = hslider ("[4]release",0.22,0.001,1,0.0001);
gain = hslider("[5]gain", 2, -60, 12, 0.01) : ba.db2linear : si.smooth(ba.tau2pole(0.1));
										   
quadcomp = compressor*gain, compressor*gain, compressor*gain, compressor*gain;

process = quadcomp;