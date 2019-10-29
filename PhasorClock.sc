PhasorClock : UGen {
    *ar { arg inPhasor, multiplier = 1.0, sync = 0.0;
        ^this.multiNew('audio', inPhasor, multiplier, sync)
    }
}