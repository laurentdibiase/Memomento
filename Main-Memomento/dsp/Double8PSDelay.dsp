
/*

DOUBLE LOOP STEREO PITCHSHIFTER DELAY 

Transposition(1) is placed inside the first delay feedback loop.
This loop is duplicate and placed inside the first loop with the second transposition(2).
The third transposition(3) is placed at the output as a second effect in series

CAUTION: The maximum feedback value do not excided 0.5

*/

process = vgroup ("Double8PSDelay",environment{declare name "Double8PSDelay";

import("stdfaust.lib");

//duree du delay en secondes
duree1 = hslider("duree1", 0.01, 0.01, 5, 0.01):si.smoo;

//duree2 = hslider("duree2", 0.01, 0.01, 5, 0.01):si.smoo;
//reinjection du delay + transpositeur

reinj1 = hslider("reinj1", 0, 0, 0.5, 0.01):si.smoo;
reinj2 = hslider("reinj2", 0, 0, 0.5, 0.01):si.smoo;

//transposition en 1 tons de -6 octaves à +6 octaves//
tra1 = hslider("traLoop1", 0, -72, 72, 2):si.smoo;
tra2 = hslider("traLoop2", 0, -72, 72, 2):si.smoo;
tra3 = hslider("traMain", 0, -72, 72, 2):si.smoo;

ligneARetard1 = de.sdelay(262144, 1024, (duree1 * ma.SR));
//ligneARetard2 = de.sdelay(262144, 1024, (duree2 * ma.SR));

transpositeur1 = ef.transpose(8192, 4096, tra1);
transpositeur2 = ef.transpose(8192, 4096, tra2);
transpositeur3 = ef.transpose(8192, 4096, tra3);

delshift = (+ : ligneARetard1) ~ ((+ : ligneARetard1) ~ (transpositeur2*(reinj2)):(transpositeur1 *(reinj1))):transpositeur3;

process = delshift,delshift;

}.process);
