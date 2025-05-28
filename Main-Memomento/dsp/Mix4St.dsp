/*

4 Tracks Stereo Mixer with Panner, Mute and Master

*/

import("stdfaust.lib");

input(v) = vgroup("[%v]%a", *(1-checkbox("[2]Mute")) : *(vslider("[3]Level", 0, 0, 1, 0.01))<: *(1-pan),*(pan))
with {
        a = v+1; 
        pan = hslider("[1]Pan [style:knob]",0.5,0,1,0.01) : si.smoo;
    };
master = vgroup("[3]Master", vslider("Level", 0, 0, 1, 0.01));

process  = hgroup("Stereo Mixer", par(i, 4, input(i)):> * (master),* (master));

