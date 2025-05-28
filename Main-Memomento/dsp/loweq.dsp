declare name "Peak-EQ";
declare description "Peak-EQ";

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

Eq = fi.peak_eq(Lfx,fx,B);

Lfx = hslider("level", 0, -60, 60, 1) : si.smooth(0.999);
fx = 80;
B = 10;

process = Eq;