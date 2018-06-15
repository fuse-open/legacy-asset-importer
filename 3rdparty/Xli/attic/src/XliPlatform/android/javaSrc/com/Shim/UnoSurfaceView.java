package com.Shim;

import android.app.NativeActivity;
import android.util.Log;
import android.view.MotionEvent;
import android.view.MotionEvent.PointerCoords;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class UnoSurfaceView extends SurfaceView implements SurfaceHolder.Callback
{
    NativeActivity nativeActivity;
    int width = 0;
    int height = 0;
    
	public UnoSurfaceView(NativeActivity context) {
		super(context);
        pointerCoords = new PointerCoords();
        nativeActivity = context;
        getHolder().addCallback(this);
        setZOrderOnTop(false);
	}

	public void updateDimensions(int w, int h) {
		if (width!=w || height!=h) {
			width=w;
			height=h;
			com.Shim.XliJ.RootSurfaceChangedDimensions(w, h);
		}
	}
	
	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		Log.d("XliApp","surfaceCreated");
        com.Shim.XliJ.RootSurfaceChanged(holder.getSurface());
        updateDimensions(getWidth(), getHeight());
        //nativeActivity.surfaceCreated(holder);
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
		Log.d("XliApp","surfaceChanged");
		updateDimensions(width, height);
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		Log.d("XliApp","surfaceDestroyed");		
	}

    static PointerCoords pointerCoords;
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int action = event.getAction();
		event.getPointerCoords(0,pointerCoords);
		
		switch (action) {
		case MotionEvent.ACTION_MOVE:
			com.Shim.XliJ.XliJ_OnSurfaceTouch(0, (int)pointerCoords.x, (int)pointerCoords.y, 0);
			break;
		case MotionEvent.ACTION_DOWN:
			com.Shim.XliJ.XliJ_OnSurfaceTouch(0, (int)pointerCoords.x, (int)pointerCoords.y, 1);
			break;
	    case MotionEvent.ACTION_UP:
	    	com.Shim.XliJ.XliJ_OnSurfaceTouch(0, (int)pointerCoords.x, (int)pointerCoords.y, 2);
	        performClick();
	        break;
	    }
		return true;
	}
	
	@Override
	public boolean performClick()
	{
		return super.performClick();
	}
}
