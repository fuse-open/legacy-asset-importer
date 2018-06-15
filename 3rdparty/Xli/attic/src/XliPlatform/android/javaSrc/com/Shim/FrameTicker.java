package com.Shim;

import android.annotation.TargetApi;
import android.os.Build;
import android.view.Choreographer;

@TargetApi(Build.VERSION_CODES.JELLY_BEAN)
public class FrameTicker implements Choreographer.FrameCallback {
	Choreographer c;
	public FrameTicker()
	{
		c = Choreographer.getInstance();
	}
	@Override
	public void doFrame(long frameTimeNanos) {
		long time = frameTimeNanos / 1000000;
		com.Shim.XliJ.XliJ_FrameTick((int)time);
        c.postFrameCallback(this);
	}
}
