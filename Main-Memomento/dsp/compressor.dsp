
declare author "Laurent Di Biase";	
										   
/* ========= DESCRITPION ==========
Parametric mono compressor - co.compressor_mono

    ratio: compression ratio (1 = no compression, >1 means compression)
    thresh: dB level threshold above which compression kicks in (0 dB = max level)
    att: attack time = time constant (sec) when level & compression going up
    rel: release time = time constant (sec) coming out of compression
*/										   

import("stdfaust.lib");					   
								
	
compressor = co.compressor_mono(ratio,thresh,att,rel);

ratio = hslider ("ratio",1,1,70,1);
thresh = hslider ("threshold",0,-70,0,0.1);
att = hslider ("attack",0.001,0.0001,1,0.0001);
rel = hslider ("release",0.022,0.001,1,0.0001);
gain = hslider("ampdb", 0, -30, 30, 0.01) : ba.db2linear : si.smooth(ba.tau2pole(0.1));
										   
process = compressor: * (gain);