declare name "3ToQuadri";
declare version "1.0";
declare author "Laurent Di Biase";
declare license "BSD";
declare copyright "(c)CICM 2021";

import("stdfaust.lib");


r1 = hslider("Radius1", 1.0, 0, 5, 0.001) : si.smooth(ba.tau2pole(0.02));
a1 = hslider("Angle1", 0, ma.PI*(-2), ma.PI*2, 0.001) : si.smooth(ba.tau2pole(0.02));

r2 = hslider("Radius2", 1.0, 0, 5, 0.001) : si.smooth(ba.tau2pole(0.02));
a2 = hslider("Angle2", ma.PI*(2/3), ma.PI*(-2), ma.PI*2, 0.001) : si.smooth(ba.tau2pole(0.02));

r3 = hslider("Radius3", 1.0, 0, 5, 0.001) : si.smooth(ba.tau2pole(0.02));
a3 = hslider("Angle3", ma.PI*(-2/3), ma.PI*(-2), ma.PI*2, 0.001) : si.smooth(ba.tau2pole(0.02));


process(sig1, sig2, sig3) = ho.map(1, sig1, r1, a1), ho.map(1, sig2, r2, a2), ho.map(1, sig3, r3, a3):> ho.optimInPhase(1) <: (ho.decoder(1, 4));

