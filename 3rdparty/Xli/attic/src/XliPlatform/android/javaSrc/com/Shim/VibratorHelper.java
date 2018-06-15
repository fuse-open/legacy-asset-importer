/*
 * Copyright (C) 2010-2014 Outracks Technologies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

package com.Shim;

import android.annotation.SuppressLint;
import android.app.NativeActivity;
import android.content.Context;
import android.os.Vibrator;


public class VibratorHelper {
    @SuppressLint("NewApi")
	public static boolean HasVibrator(final NativeActivity activity)
    {
    	Vibrator vibrator = (Vibrator)activity.getSystemService(Context.VIBRATOR_SERVICE);
    	if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.HONEYCOMB)
    	{
    		return vibrator.hasVibrator();
    	} else {
    		return true;
    	}
    }
    
    public static void VibrateUsingPattern(long[] pattern, int repeat)
    {
    	Vibrator vibrator = (Vibrator)XliJ.nActivity.getSystemService(Context.VIBRATOR_SERVICE);
    	vibrator.vibrate(pattern, repeat);
    }
    
    public static void VibrateForMilliseconds(int milliseconds)
    {
    	Vibrator vibrator = (Vibrator)XliJ.nActivity.getSystemService(Context.VIBRATOR_SERVICE);
    	vibrator.vibrate(milliseconds);
    }
}
