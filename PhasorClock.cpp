#include "SC_PlugIn.h"

#include "SCRate.hpp"

static InterfaceTable *ft;

struct PhasorClock : public Unit 
{
    SCRate rate;
};

static void PhasorClock_next_a_a(PhasorClock* unit, int inNumSamples);
static void PhasorClock_next_a_k(PhasorClock* unit, int inNumSamples);
static void PhasorClock_next_k_a(PhasorClock* unit, int inNumSamples);
static void PhasorClock_next_k_k(PhasorClock* unit, int inNumSamples);

static void PhasorClock_Ctor(PhasorClock* unit);

void PhasorClock_Ctor(PhasorClock* unit) 
{
    unit->rate.reset();

    if (INRATE(1) == calc_FullRate) 
    {
        if(INRATE(2) == calc_FullRate) { 
            Print("a_a\n");
            SETCALC(PhasorClock_next_a_a);
        }
        else {
            Print("a_k\n");
            SETCALC(PhasorClock_next_a_k);
        }
    } 
    else 
    {
        if(INRATE(2) == calc_FullRate) {
            Print("k_a\n");
            SETCALC(PhasorClock_next_k_a);
        }
        else {
            Print("k_k\n");
            SETCALC(PhasorClock_next_k_k);
        }
    }
    
    //calc one sample
    PhasorClock_next_k_k(unit, 1);
}

void PhasorClock_next_a_a(PhasorClock* unit, int inNumSamples) 
{
    float* in_phasor   = IN(0);
    float* multiplier  = IN(1);
    float* sync_ctl    = IN(2);   

    float* out = OUT(0);

    for(int i = 0; i < inNumSamples; i++)
    {
        float in_phasor_val  = in_phasor[i];
        float multiplier_val = multiplier[i];
        int sync_ctl_val   = int(sync_ctl[i]);

        if(sync_ctl_val < 0)
            sync_ctl_val = 0;
        else if(sync_ctl_val > 2)
            sync_ctl_val = 2;

        float out_val = 0;

        switch (sync_ctl_val)
        {
            //cycle (reset on reset of in_phasor cycle)
            case 0:
                out_val = unit->rate.perform_cycle(in_phasor_val, multiplier_val);
                break;
            
            //lock (reset lock right away)
            case 1:
                out_val = unit->rate.perform_lock(in_phasor_val, multiplier_val);
                break;

            //off (no locking at all)
            case 2:
                out_val = unit->rate.perform_off(in_phasor_val, multiplier_val);
                break;

            default:
                out_val = unit->rate.perform_cycle(in_phasor_val, multiplier_val);
                break;
        }

        out[i] = out_val;
    }
}

void PhasorClock_next_a_k(PhasorClock* unit, int inNumSamples) 
{
    float* in_phasor   = IN(0);
    float* multiplier  = IN(1);
    int sync_ctl_val   = int(IN0(2));   

    float* out = OUT(0);

    if(sync_ctl_val < 0)
        sync_ctl_val = 0;
    else if(sync_ctl_val > 2)
        sync_ctl_val = 2;

    switch (sync_ctl_val)
    {
        //cycle (reset on reset of in_phasor cycle)
        case 0:
            for(int i = 0; i < inNumSamples; i++)
            {
                float in_phasor_val  = in_phasor[i];
                float multiplier_val = multiplier[i];

                float out_val = unit->rate.perform_cycle(in_phasor_val, multiplier_val);

                out[i] = out_val;
            }

            break;
        
        //lock (reset lock right away)
        case 1:
            for(int i = 0; i < inNumSamples; i++)
            {
                float in_phasor_val  = in_phasor[i];
                float multiplier_val = multiplier[i];

                float out_val = unit->rate.perform_lock(in_phasor_val, multiplier_val);

                out[i] = out_val;
            }

            break;

        //off (no locking at all)
        case 2:
            for(int i = 0; i < inNumSamples; i++)
            {
                float in_phasor_val  = in_phasor[i];
                float multiplier_val = multiplier[i];

                float out_val = unit->rate.perform_off(in_phasor_val, multiplier_val);

                out[i] = out_val;
            }

            break;

        default:
            for(int i = 0; i < inNumSamples; i++)
            {
                float in_phasor_val  = in_phasor[i];
                float multiplier_val = multiplier[i];

                float out_val = unit->rate.perform_cycle(in_phasor_val, multiplier_val);

                out[i] = out_val;
            }
            
            break;
    }
}

void PhasorClock_next_k_a(PhasorClock* unit, int inNumSamples) 
{
    float* in_phasor       = IN(0);
    float  multiplier_val  = IN0(1);
    float* sync_ctl        = IN(2);   

    float* out = OUT(0);

    for(int i = 0; i < inNumSamples; i++)
    {
        float in_phasor_val  = in_phasor[i];
        int sync_ctl_val   = int(sync_ctl[i]);

        if(sync_ctl_val < 0)
            sync_ctl_val = 0;
        else if(sync_ctl_val > 2)
            sync_ctl_val = 2;

        float out_val = 0;

        switch (sync_ctl_val)
        {
            //cycle (reset on reset of in_phasor cycle)
            case 0:
                out_val = unit->rate.perform_cycle(in_phasor_val, multiplier_val);
                break;
            
            //lock (reset lock right away)
            case 1:
                out_val = unit->rate.perform_lock(in_phasor_val, multiplier_val);
                break;

            //off (no locking at all)
            case 2:
                out_val = unit->rate.perform_off(in_phasor_val, multiplier_val);
                break;

            default:
                out_val = unit->rate.perform_cycle(in_phasor_val, multiplier_val);
                break;
        }

        out[i] = out_val;
    }
}

void PhasorClock_next_k_k(PhasorClock* unit, int inNumSamples) 
{
    float* in_phasor      = IN(0);
    float multiplier_val  = IN0(1);
    int sync_ctl_val      = int(IN0(2));   

    float* out = OUT(0);

    if(sync_ctl_val < 0)
        sync_ctl_val = 0;
    else if(sync_ctl_val > 2)
        sync_ctl_val = 2;

    switch (sync_ctl_val)
    {
        //cycle (reset on reset of in_phasor cycle)
        case 0:
            for(int i = 0; i < inNumSamples; i++)
            {
                float in_phasor_val  = in_phasor[i];

                float out_val = unit->rate.perform_cycle(in_phasor_val, multiplier_val);

                out[i] = out_val;
            }

            break;
        
        //lock (reset lock right away)
        case 1:
            for(int i = 0; i < inNumSamples; i++)
            {
                float in_phasor_val  = in_phasor[i];

                float out_val = unit->rate.perform_lock(in_phasor_val, multiplier_val);

                out[i] = out_val;
            }

            break;

        //off (no locking at all)
        case 2:
            for(int i = 0; i < inNumSamples; i++)
            {
                float in_phasor_val  = in_phasor[i];

                float out_val = unit->rate.perform_off(in_phasor_val, multiplier_val);

                out[i] = out_val;
            }

            break;

        default:
            for(int i = 0; i < inNumSamples; i++)
            {
                float in_phasor_val  = in_phasor[i];

                float out_val = unit->rate.perform_cycle(in_phasor_val, multiplier_val);

                out[i] = out_val;
            }
            
            break;
    }
}

PluginLoad(PhasorClockUGens) 
{
    ft = inTable; 
    DefineSimpleUnit(PhasorClock);
}