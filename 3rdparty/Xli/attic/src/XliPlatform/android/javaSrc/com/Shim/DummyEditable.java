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

import android.text.SpannableStringBuilder;

public class DummyEditable extends SpannableStringBuilder {
	public static String DUMMY;
    DummyEditable(CharSequence source) {
        super(source);
    }
    @Override
    public SpannableStringBuilder replace(final int start, final int end, CharSequence tb, int tbstart, int tbend) {
        if (tbend > tbstart) {
            super.replace(0, length(), "", 0, 0);
            return super.replace(0, 0, tb, tbstart, tbend);
        }
        else if (end > start) {
            super.replace(0, length(), "", 0, 0);
            return super.replace(0, 0, DUMMY, 0, DUMMY.length());
        }
        return super.replace(start, end, tb, tbstart, tbend);
    }
    public static void PopulateDummyString()
    {    	
    	DummyEditable.DUMMY = "";
        for (int i = 0; i < Math.max(0, (500 - DummyEditable.DUMMY.length())); i++)
            DummyEditable.DUMMY += "\0";
    }
}
