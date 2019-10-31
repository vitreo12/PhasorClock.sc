PhasorClock : UGen {
    *ar { arg inPhasor, multiplier = 1.0, sync = 0.0;
        ^this.multiNew('audio', inPhasor, multiplier, sync)
    }
}

//Combine PhasorClock and Delta. Input should be a SingleRamp (or any 0-1 ramp, like LFSaw.range(0, 1))
PhasorClockDelta {
	*ar { arg phasor, multiplier = 1, sync = 0.0, thresh = 0.5;
		var pClock = PhasorClock.ar(phasor, multiplier, sync);
		var delta  = Delta.ar(pClock, thresh);
		^delta;
	}
}

//Short version
PCDelta : PhasorClockDelta {

}