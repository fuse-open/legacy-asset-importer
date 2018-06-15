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

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.util.Arrays;

import android.annotation.SuppressLint;
import android.os.AsyncTask;
import android.os.Build;
import android.util.Log;


public class StreamHelper {
   public static String InputStreamToString(InputStream stream) throws IOException, UnsupportedEncodingException
    {
		@SuppressWarnings("resource")
		java.util.Scanner s = new java.util.Scanner(stream).useDelimiter("\\A");
        return s.hasNext() ? s.next() : "";
    }

	public static int AsyncInputStreamToString(final InputStream stream, final long requestPointer) throws IOException, UnsupportedEncodingException
    {
    	final int taskKey = XliJ.ReserveObject();
    	XliJ.nActivity.runOnUiThread(new Runnable() { @SuppressLint("NewApi")
		public void run() {
    		AsyncTask<Object, Void, String> task = new AsyncInputStreamToStringTask();
    		XliJ.PopulateReservedObject(taskKey, task);
    	    if (Build.VERSION.SDK_INT > 10) {    	    	
    	    	task.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR, stream, requestPointer);
    	    } else {
    	    	task.execute(stream, requestPointer);
    	    }
    	}});
        return taskKey;
    }
	
	public static int AsyncProgressiveInputStreamToByteArray(final InputStream stream, final long requestPointer)
    {
    	final int taskKey = XliJ.ReserveObject();
    	XliJ.nActivity.runOnUiThread(new Runnable() { @SuppressLint("NewApi")
		public void run() {
    		AsyncTask<Object, Void, Void> task = new AsyncInputStreamToBytesTask();
    		XliJ.PopulateReservedObject(taskKey, task);
    	    if (Build.VERSION.SDK_INT > 10) {    	    	
    	    	task.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR, stream, requestPointer);
    	    } else {
    	    	task.execute(stream, requestPointer);
    	    }
    	}});
        return taskKey;
    }

    public static byte[] ReadAllBytesFromInputStream(InputStream stream) throws IOException
    {    	
		ByteArrayOutputStream buffer = new ByteArrayOutputStream();
		int nRead;
		byte[] data = new byte[16384];
		while ((nRead = stream.read(data, 0, data.length)) != -1) {
		  buffer.write(data, 0, nRead);
		}
		buffer.flush();
		return buffer.toByteArray();
    }


    public static byte[] ReadBytesFromInputStream(BufferedInputStream stream, int bytesToRead)
    {    	
    	byte[] buffer = new byte[bytesToRead];
    	try {
			int bytesRead = stream.read(buffer);
			if (bytesRead>-1)
			{
				return Arrays.copyOf(buffer, bytesRead); //{TODO} only copy in c++?
			} else {
				return null;
			}
		} catch (IOException e) {
			Log.e("XliApp", "read from stream crashed: "+e.getLocalizedMessage());
				return null;	
		}	
    }
}
