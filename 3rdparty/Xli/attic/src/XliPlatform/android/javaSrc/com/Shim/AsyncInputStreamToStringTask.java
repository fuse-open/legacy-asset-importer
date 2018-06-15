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

import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;

import android.os.AsyncTask;

public class AsyncInputStreamToStringTask extends AsyncTask<Object, Void, String> {
	public long requestPointer;
    @Override
    protected String doInBackground(Object... params) {
    	requestPointer = (long)((Long)params[1]);
        try {
			return XliJ.InputStreamToString((InputStream)params[0]);
		} catch (UnsupportedEncodingException e) {
			XliJ.XliJ_HttpErrorCallback(requestPointer, -1, "UnsupportedEncodingException: "+e.getLocalizedMessage());
			return null;
		} catch (IOException e) {
			XliJ.XliJ_HttpErrorCallback(requestPointer, -1, "IOException (AsyncInputStreamToStringTask): "+e.getLocalizedMessage());
			return null;
		}
    }
    @Override
    protected void onPostExecute(String result)
    {    		
    	if (result!=null)
    		XliJ.XliJ_HttpContentStringCallback(result, requestPointer);
    }
}
