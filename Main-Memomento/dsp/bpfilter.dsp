
/*
(fi.)bandpass

Order 2*Nh Butterworth bandpass filter made using the transformation s <- s + wc^2/s on lowpass(Nh), 
where wc is the desired bandpass center frequency. 
The lowpass(Nh) cutoff w1 is half the desired bandpass width. 
bandpass is a standard Faust function.

_ : bandpass(Nh,fl,fu) : _

    Nh: HALF the desired bandpass order (which is therefore even)
    fl: lower -3dB frequency in Hz
    fu: upper -3dB frequency in Hz Thus, the passband width is fu-fl, and its center frequency is (fl+fu)/2.
*/

import("stdfaust.lib");

//lower frequency//
fl = vslider("low", 30, 20, 250, 1);
//upper frequency//
fu = vslider("high", 16000, 2000, 20000, 1);

//Block of 6 filters
bandp = par(i, 6, fi.bandpass(3,fl,fu));

process = bandp : _,_,_,_,_,_;
