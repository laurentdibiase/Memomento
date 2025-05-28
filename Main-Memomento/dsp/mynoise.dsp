
import("stdfaust.lib");

random  = +(12345)~*(1103515245);
noise   = random/2147483647.0;
process = noise * hslider("vol", 0.5, 0, 1, 0.01);

