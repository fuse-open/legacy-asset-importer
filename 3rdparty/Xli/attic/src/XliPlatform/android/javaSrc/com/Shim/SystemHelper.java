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
import android.graphics.Rect;
import android.os.Build;
import android.util.DisplayMetrics;
import android.view.Window;
import android.view.WindowManager;


public class SystemHelper {
    public static void hideStatusBar(final NativeActivity activity) {
    	Window win = activity.getWindow();
    	win.addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        win.addFlags(WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN);
    }
    public static void showStatusBar(final NativeActivity activity) {
    	Window win = activity.getWindow();
    	win.clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
    }
	@SuppressLint("NewApi")
	public static DisplayMetrics GetDisplayMetrics(final NativeActivity activity) 
	{
		DisplayMetrics metrics = new DisplayMetrics();
    	if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {    		
    		activity.getWindowManager().getDefaultDisplay().getRealMetrics(metrics);
            return metrics;
    	} else {
    		activity.getWindowManager().getDefaultDisplay().getMetrics(metrics);
            return metrics;
    	}
    }	
	public static float GetStatusBarHeight(NativeActivity activity)
	{
		/*Rect rectangle= new Rect();
		Window window= activity.getWindow();
		window.getDecorView().getWindowVisibleDisplayFrame(rectangle);
		int statusBarHeight = rectangle.top;
		//int contentViewTop = window.findViewById(Window.ID_ANDROID_CONTENT).getTop();
		//int titleBarHeight= contentViewTop - statusBarHeight; //dont need this yet but is useful code
		return statusBarHeight;*/

        int result = 0;
        int resourceId = activity.getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0)
            result = activity.getResources().getDimensionPixelSize(resourceId);
        return (float)result;
	}
}
