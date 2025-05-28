process = vgroup("SLimiter",environment{declare name "Limiter";
declare author "Yann Orlarey";

/* ========= DESCRITPION ==========

- A limiter is a security that prevents a sound from saturating

*/

process =slimiter;

slimiter(w,x,y,z) =w*coeff,x*coeff,y*coeff,z*coeff

	with {
		epsilon =1/(44100*1.0);
		peak1 = max(abs(w),abs(x)):max~-(epsilon);
  		peak2 = max(abs(y),abs(z)):max~-(epsilon);
		coeff = 1.0/max(1.0,peak1&peak2);
  		
    };
		
}.process);