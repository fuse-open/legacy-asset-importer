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
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;

@SuppressLint("ViewConstructor")
public class Hidden extends View implements View.OnKeyListener { //used to extend view
    InputConnection fic;
    NativeActivity activity;

    public Hidden(Context context, NativeActivity activity) {        	
        super(context);
        this.activity = activity;
        init(context);
        setFocusableInTouchMode(true);
        setFocusable(true);
        setVisibility(View.VISIBLE);
        requestFocus();
    }

    private void init(Context context) {
    	this.setOnKeyListener(this);
        DummyEditable.PopulateDummyString();
    }
    
    public void Disable(Context context, NativeActivity activity)
    {
    	this.setOnKeyListener(null);
        setFocusableInTouchMode(false);
        setFocusable(false);
    }
    
    @Override
    public boolean onKeyPreIme(int keyCode, KeyEvent keyEvent) {
        if (keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_ESCAPE)
        {
            if (keyEvent.getAction()==KeyEvent.ACTION_DOWN)
            {
            	if (keyCode == KeyEvent.KEYCODE_BACK) {
            		XliJ.HideKeyboard();
            		return true;
            	}
            	XliJ.XliJ_OnKeyDown(keyCode);
            } else if (keyEvent.getAction()==KeyEvent.ACTION_UP) {
            	XliJ.XliJ_OnKeyUp(keyCode);
            }
            return true;
        }
        return super.onKeyPreIme(keyCode, keyEvent);
    }
    @Override
    public boolean onKey(View view, int keyCode, KeyEvent keyEvent) {
        return super.onKeyPreIme(keyCode, keyEvent);
    }

    @Override
    public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
        fic = new HiddenInputConnection(this, false);
        outAttrs.actionLabel = null;
        //outAttrs.inputType = InputType.TYPE_NULL;
        outAttrs.imeOptions = EditorInfo.IME_ACTION_NEXT;
        return fic;
    }

    @Override
    public boolean onCheckIsTextEditor() { return true; }
}
