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

import android.os.AsyncTask;

public class AsyncInputStreamToBytesTask extends AsyncTask<Object, Void, Void> {
	public long requestPointer;
	@Override
	protected Void doInBackground(Object... params) {
		requestPointer = (long)((Long)params[1]);
		try {
			InputStream stream = (InputStream)params[0];

			int nRead;
			int runningTotal = 0;
			int progressThreshold = 30000;
			int steps = 1;
			byte[] data = new byte[16384];
			while ((nRead = stream.read(data, 0, data.length)) != -1) {
				XliJ.XliJ_HttpBytesDownloadedCallback(data, nRead, requestPointer);
				runningTotal+=nRead;
				if (runningTotal/progressThreshold > steps)
				{
					steps = runningTotal/progressThreshold;
					XliJ.XliJ_HttpProgressCallback(requestPointer, runningTotal, 0, false, 1);
				}
			}			
		} catch (IOException e) {				
			XliJ.XliJ_HttpErrorCallback(requestPointer, -1, "IOException (AsyncInputStreamToBytesTask): "+e.getLocalizedMessage());
		}
		return null;
	}
	@Override
	protected void onPostExecute(Void ignore)
	{    	
		XliJ.XliJ_HttpBytesDownloadedCallback(null, -1, requestPointer);
	}
}
