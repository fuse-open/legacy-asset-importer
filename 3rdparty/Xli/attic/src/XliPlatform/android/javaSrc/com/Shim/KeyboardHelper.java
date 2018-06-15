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
import android.os.Build;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.inputmethod.InputMethodManager;
import android.widget.FrameLayout;

public class KeyboardHelper {
	static ViewTreeObserver.OnGlobalLayoutListener layoutListener;
	static Hidden hiddenText;
    static int keyboardSize;
    static ViewGroup hiddenLayout;

    public static int GetKeyboardSize() { return (int)keyboardSize; }

    public static void ShowKeyboard() {
        if (hiddenText == null)
        {
        	AttachHiddenView();
        }
        DummyEditable.PopulateDummyString();
        XliJ.nActivity.runOnUiThread(new Runnable() { public void run() {
            try {
                hiddenText.setFocusableInTouchMode(true);
                hiddenText.setFocusable(true);
                InputMethodManager imm = (InputMethodManager)XliJ.nActivity.getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.showSoftInput(hiddenText, 0);
                hiddenText.requestFocus();
            } catch (Exception e) {
                Log.e("XliApp","Unable show keyboard");
            }
        }});
    }
    public static void HideKeyboard() {
        if (hiddenText == null)
        {
            Log.e("XliApp","HideKeyboard: Hidden View not available");
            return;
        }
        XliJ.nActivity.runOnUiThread(new Runnable() { public void run() {
            InputMethodManager imm = (InputMethodManager)XliJ.nActivity.getSystemService(Context.INPUT_METHOD_SERVICE);
            imm.hideSoftInputFromWindow(XliJ.nActivity.getWindow().getDecorView().getWindowToken(), 0);
            hiddenText.setFocusableInTouchMode(false);
            hiddenText.setFocusable(false);
            hiddenText.setVisibility(View.VISIBLE);
        }});
    }

    public static void AttachHiddenView()
    {
        Log.d("XliApp","Attempting to attach hidden view");
        if (hiddenLayout == null)
        {

            hiddenLayout = new FrameLayout(XliJ.nActivity);
            XliJ.nActivity.runOnUiThread(new Runnable() { public void run() {
                try {
                    com.Shim.XliJ.nRootLayout.addView(hiddenLayout);
                    hiddenText = new Hidden(XliJ.nActivity, XliJ.nActivity);
                    hiddenLayout.addView(hiddenText);
                    hiddenText.setVisibility(View.VISIBLE);
                    hiddenText.requestFocus();
                    HideKeyboard();
                    Log.i("XliApp","Successfully created input capture View.");
                } catch (Exception e) {
                    Log.e("XliApp","Unable to create Layout or View for input capture.");
                    XliJ.XliJ_JavaThrowError(-1, "Unable to create Layout or View for input capture.");
                }

                try {
                	keyboardSize = 0;
                	layoutListener = new ViewTreeObserver.OnGlobalLayoutListener(){
                        public void onGlobalLayout() {
                            int rootViewHeight = hiddenLayout.getRootView().getHeight();
                            int location[] = new int[2];
                            hiddenLayout.getLocationOnScreen(location);
                            int hiddenHeight = (int)(location[1] + hiddenLayout.getMeasuredHeight());
                            keyboardSize = rootViewHeight - hiddenHeight;
                            XliJ.XliJ_OnKeyboardResized();
                        }
                    };
                	hiddenLayout.getViewTreeObserver().addOnGlobalLayoutListener(layoutListener);
                    Log.i("XliApp","Successfully attached View Tree Observer.");
                } catch (Exception e) {
                    Log.e("XliApp","Unable to attach keyboard height monitor.");
                    XliJ.XliJ_JavaThrowError(-1, "Unable to attach keyboard height monitor.");
                }
            }});
        }
    }
}
