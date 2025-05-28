
/*

pitchshifter

-4/+4 transpose d'une tiers 


//fdelay avec réinjection, commence avec un "plus" et dont la boucle comporte un taux (fdbk)

*/


import("stdfaust.lib");

shift = vslider("shift", 0, -4, +4, 0.01);


n = 524288; //capacité de stockage de la ligne à retard en nombre d'échantillons

//on contrôle la durée dur en millisecondes du retard
tempo = vslider("tempo", 60, 1, 5000, 1) : si.smooth(0.999); //tempo comme sur un métronome
durmus = vslider("durmus", 1, 0.01, 32, 0.001): si.smooth(0.999);

//ajout du % de feedback
fdbk = vslider("fdbk", 0.01, 0.01, 0.99, 0.01): si.smooth(0.999);

d = 60 / tempo * durmus * ma.SR; //nombre d'échantillons correspondant//

process = dm.freeverb_demo : _ , ((+ : de.fdelay(n, d)) ~ (*(fdbk)) : ef.transpose(1024,1024,shift)) :> _ ; 


