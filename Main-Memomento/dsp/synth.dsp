
import("stdfaust.lib");


normMIDI(mv)  = mv/127.0; 
vol  = normMIDI(hslider("Ctrl Value IN (Ctrl 1) [midi:ctrl 1]", 60, 0, 127, 1)) ;


f1 = nentry("freq",196,20,2000,0.01);
f2 = nentry("freq",261.63,20,2000,0.01);
f3 = nentry("freq",261.63,20,2000,0.01);
f4 = nentry("freq",130.81,20,2000,0.01);
bend = nentry("bend",1,0,10,0.01) : si.polySmooth(t,0.999,1);
g = nentry("gain",1,0,1,0.01);
t = button("gate");
freq = (f1,f2,f3,f4) : (*bend),(*bend),(*bend),(*bend);
envelope = t*g*vol : si.smoo;

process = os.sawtooth(freq)*envelope <: _,_;
