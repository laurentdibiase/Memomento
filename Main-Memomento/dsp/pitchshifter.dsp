declare name 		"pitchshifter";
declare version 	"1.0";
declare author 		"Laurent Di Biase";
declare license 	"BSD";
declare copyright 	"(c)Laurent Di Biase 2020";

/* ========= DESCRITPION ==========
Pitchshifting - Mono - A simple pitchshifter based on 2 delay lines .transpose 
is a standard Faustfunction, with bypass and Wet/Dry effect.

l: the window length (samples)
x: crossfade duration duration (samples),fixed value 4096 or not
s: shift (semitones)

*/										   

import("stdfaust.lib");					   
																					  
pitchshift =  _ : ef.transpose(l, x, s) : _;
											  
					
l = hslider ("[2] Length [osc:/ptch/0]",1024,10,8192,1): si.smooth(0.999);
x = hslider ("[1] Duration [osc:/ptch/1]",10,10,4096,1): si.smooth(0.999);
s = hslider ("[3] Shift [osc:/ptch/2]",0,-24,24,1): si.smooth(0.999);	
w = hslider("[4] Dry/Wet [osc:/ptch/0]", 1, 0, 1, 0.01): si.smooth(0.999);											  

fx = _ <:pitchshift, _ : (*(w), *(1-w)) :> _ ;	

process = fx;

