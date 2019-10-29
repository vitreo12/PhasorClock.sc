//Copied over from gen~ exported stuff

//With sc_sample == float, it goes out of sync due to the use of long in recalculating the phases. With double, it works just as Max's one.
#define sc_sample double
		
inline sc_sample genlib_isnan(sc_sample v) { return (((((uint32_t *)&(v))[1])&0x7fe00000)==0x7fe00000); }

inline sc_sample safediv(sc_sample num, sc_sample denom) {
    return denom == 0. ? (sc_sample)0. : (sc_sample)(num/denom);
}

#define GENLIB_QUANT(f1,f2) sc_sample(floor((f1)*(f2)+0.5)/(f2))

struct SCRate {
    sc_sample phase, diff, mult, invmult, prev;
    int wantlock, quant;
    
    SCRate() { reset(); }
    
    inline void reset() {
        phase = diff = prev = 0;
        mult = invmult = 1;
        wantlock = 1;
        quant = 1;
    }
    
    inline sc_sample perform_lock(sc_sample in1, sc_sample in2) {
        // did multiplier change?
        if (in2 != mult && !genlib_isnan(in2)) {
            mult = in2;
            invmult = safediv(1., mult);
            wantlock = 1;
        }
        sc_sample diff = in1 - prev;
        
        if (diff < sc_sample(-0.5)) {
            diff += sc_sample(1);
        } else if (diff > sc_sample(0.5)) {
            diff -= sc_sample(1);
        }
        
        if (wantlock) {
            // recalculate phase
            phase = (in1 - GENLIB_QUANT(in1, quant)) * invmult
            + GENLIB_QUANT(in1, quant * mult);
            diff = 0;
            wantlock = 0;
        } else {
            // diff is always between -0.5 and 0.5
            phase += diff * invmult;
        }
        
        if (phase > sc_sample(1.) || phase < sc_sample(-0.)) {
            phase = phase - (long)(phase);
        }
        
        prev = in1;
        
        return phase;
    }
    
    inline sc_sample perform_cycle(sc_sample in1, sc_sample in2) {
        // did multiplier change?
        if (in2 != mult && !genlib_isnan(in2)) {
            mult = in2;
            invmult = safediv(1., mult);
            wantlock = 1;
        }
        sc_sample diff = in1 - prev;
        
        if (diff < sc_sample(-0.5)) {
            if (wantlock) {
                wantlock = 0;
                phase = in1 * invmult;
                diff = sc_sample(0);
            } else {
                diff += sc_sample(1);
            }
        } else if (diff > sc_sample(0.5)) {
            if (wantlock) {
                wantlock = 0;
                phase = in1 * invmult;
                diff = sc_sample(0);
            } else {
                diff -= sc_sample(1);
            }
        }
        
        // diff is always between -0.5 and 0.5
        phase += diff * invmult;
        
        if (phase > sc_sample(1.) || phase < sc_sample(-0.)) {
            phase = phase - (long)(phase);
        }
        
        prev = in1;
        
        return phase;
    }
    
    inline sc_sample perform_off(sc_sample in1, sc_sample in2) {
        // did multiplier change?
        if (in2 != mult && !genlib_isnan(in2)) {
            mult = in2;
            invmult = safediv(1., mult);
            wantlock = 1;
        }
        sc_sample diff = in1 - prev;
        
        if (diff < sc_sample(-0.5)) {
            diff += sc_sample(1);
        } else if (diff > sc_sample(0.5)) {
            diff -= sc_sample(1);
        }
        
        phase += diff * invmult;
        
        if (phase > sc_sample(1.) || phase < sc_sample(-0.)) {
            phase = phase - (long)(phase);
        }
        
        prev = in1;
        
        return phase;
    }
};