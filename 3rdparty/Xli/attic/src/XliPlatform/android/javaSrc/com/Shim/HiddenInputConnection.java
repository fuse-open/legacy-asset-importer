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

import android.os.Build;
import android.text.Editable;
import android.text.Selection;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.BaseInputConnection;

class HiddenInputConnection extends BaseInputConnection {
    static DummyEditable mEditable;
    
    public HiddenInputConnection(View targetView, boolean fullEditor) {
        super(targetView, fullEditor);
    }

    @Override
     public String getTextBeforeCursor(int n, int flags) {
         //http://code.google.com/p/android/issues/detail?id=62306
         return DummyEditable.DUMMY;
     }

    @Override
    public Editable getEditable() {
        if (Build.VERSION.SDK_INT < 14)
            return super.getEditable();
        if (mEditable == null) {
        	mEditable = new DummyEditable(DummyEditable.DUMMY);
            Selection.setSelection(mEditable, DummyEditable.DUMMY.length());
        }
        else if (mEditable.length() == 0) {
        	mEditable.append(DummyEditable.DUMMY);
            Selection.setSelection(mEditable, DummyEditable.DUMMY.length());
        }
        return mEditable;
    }

    @Override
    public boolean deleteSurroundingText (int beforeLength, int afterLength) {
		if (beforeLength == 1 && afterLength == 0)
		{
			XliJ.XliJ_OnKeyDown(KeyEvent.KEYCODE_DEL);
			XliJ.XliJ_OnKeyUp(KeyEvent.KEYCODE_DEL);
			DummyEditable.PopulateDummyString();
			return true;
		} 
		return false;
    }

    @Override
    public boolean sendKeyEvent(KeyEvent event) {
        int keyCode = event.getKeyCode();
        if (event.getAction() == KeyEvent.ACTION_DOWN) {
        	XliJ.XliJ_OnKeyDown(keyCode);
            return false;
        } else if (event.getAction() == KeyEvent.ACTION_UP) {
        	XliJ.XliJ_OnKeyUp(keyCode);
            char unicodeChar = (char)event.getUnicodeChar();
            XliJ.XliJ_OnTextInput(""+unicodeChar);
            return false;
        } else if (event.getAction() == KeyEvent.ACTION_MULTIPLE){
            String text = event.getCharacters();
            if (text != null)
            {
            	XliJ.XliJ_OnTextInput(text);
            } else {
                char unicodeChar = (char)event.getUnicodeChar();
                XliJ.XliJ_OnTextInput(""+unicodeChar);
            }
            return false;
        }
        return false;
    }
}
