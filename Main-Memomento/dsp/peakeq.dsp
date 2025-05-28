declare name "Peak-EQ";
declare description "3Peak-EQ";

import("stdfaust.lib");

/*
(fi.)peak_eq
Second order "peaking equalizer" section (gain boost or cut near some frequency) Also called a "parametric equalizer" section. peak_eq is a standard Faust function.

Usage
_ : peak_eq(Lfx,fx,B) : _;
Where:

Lfx: level (dB) at fx (boost Lfx>0 or cut Lfx<0)
fx: peak frequency (Hz)
B: bandwidth (B) of peak in Hz

*/

EqH = fi.peak_eq(Lfx1,fx1,B1);

Lfx1 = hslider("levelh", 0, -30, 30, 1) : si.smooth(0.999);
fx1 = 12000;
B1 = 4000;

EqM = fi.peak_eq(Lfx2,fx2,B2);

Lfx2 = hslider("levelm", 0, -30, 30, 1) : si.smooth(0.999);
fx2 = hslider("freq", 200, 200, 5000, 1) : si.smooth(0.999);
B2 = hslider("Q", 100, 100, 2000, 1) : si.smooth(0.999);

EqL = fi.peak_eq(Lfx3,fx3,B3);

Lfx3 = hslider("levell", 0, -60, 60, 1) : si.smooth(0.999);
fx3 = 80;
B3 = 10;

process = EqH : EqM : EqL;