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

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.CookieHandler;
import java.net.CookieManager;
import java.net.CookiePolicy;
import java.nio.ByteBuffer;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.util.HashMap;

import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.SSLSession;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;

import android.os.AsyncTask;
import android.os.Build;

public class HttpHelper {
	//[TODO] this task array hack is terrible
    @SuppressWarnings({ "rawtypes", "unchecked" })
	public static int SendHttpAsync(final String url, final String method,
    								 	final HashMap<String,String> headers, final ByteBuffer body,
    								 	final int timeout, final long requestPointer, final boolean verifyHost) {
    	try
    	{
            final int taskKey = XliJ.ReserveObject();
            XliJ.nActivity.runOnUiThread(new Runnable() { public void run() {
                AsyncTask task = new AsyncHttpRequest();
                XliJ.PopulateReservedObject(taskKey, task);
 				//((AsyncTask<Object, Void, Boolean>)(task)).execute(url, method, headers, (Integer)timeout, body, (Long)requestPointer, (Boolean)verifyHost);
                if (Build.VERSION.SDK_INT>=Build.VERSION_CODES.HONEYCOMB) {
                    ((AsyncTask<Object, Void, Boolean>)(task)).executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR, url, method, headers, (Integer)timeout, body, (Long)requestPointer, (Boolean)verifyHost);
                }
                else {
                    ((AsyncTask<Object, Void, Boolean>)(task)).execute(url, method, headers, (Integer)timeout, body, (Long)requestPointer, (Boolean)verifyHost);
                }
             }});
    		return taskKey;
    	} catch (Exception e) {
    		XliJ.XliJ_HttpErrorCallback(requestPointer, -1, "Unable to build Async Http Request: "+e.getLocalizedMessage());
    		return 0;
    	}
    }

    @SuppressWarnings({ "rawtypes", "unchecked" })
	public static int SendHttpStringAsync(final String url, final String method,
    								 			final HashMap<String,String> headers, final String body,
    								 			final int timeout, final long requestPointer, final boolean verifyHost) {
    	try
    	{
            final int taskKey = XliJ.ReserveObject();
            XliJ.nActivity.runOnUiThread(new Runnable() { public void run() {
         		ByteBuffer data = null;
                AsyncTask task = new AsyncHttpRequest();
                XliJ.PopulateReservedObject(taskKey, task);
         		if (body!=null) data = ByteBuffer.wrap(body.getBytes());
 				((AsyncTask<Object, Void, Boolean>)(task)).execute(url, method, headers, (Integer)timeout, data, (Long)requestPointer, (Boolean)verifyHost);         	
             }});
    		return taskKey;
    	} catch (Exception e) {
    		XliJ.XliJ_HttpErrorCallback(requestPointer, -1, "Unable to build Async Http Request: "+e.getLocalizedMessage());
    		return 0;
    	}
    }
	
    public static byte[] ReadAllBytesFromHttpInputStream(InputStream stream, long requestPointer) throws IOException
    {    	
		ByteArrayOutputStream buffer = new ByteArrayOutputStream();
		int nRead;
		int runningTotal = 0;
		int progressThreshold = 8000;
		int steps = 1;
		byte[] data = new byte[16384];
		while ((nRead = stream.read(data, 0, data.length)) != -1) {
		  buffer.write(data, 0, nRead);
		  runningTotal+=nRead;
		  if (runningTotal/progressThreshold > steps)
		  {
			  steps = runningTotal/progressThreshold;
			  XliJ.XliJ_HttpProgressCallback(requestPointer, runningTotal, 0, false, 1);
		  }
		}
		buffer.flush();
		return buffer.toByteArray();
    }

    public static void InitDefaultCookieManager()
    {
        CookieManager cookieManager = new CookieManager(null, CookiePolicy.ACCEPT_ALL);
        CookieHandler.setDefault(cookieManager);
    }

    // always verify the host - dont check for certificate
    final static HostnameVerifier DO_NOT_VERIFY = new HostnameVerifier() {
    	public boolean verify(String hostname, SSLSession session) {
    		return true;
    	}
    };
    // Create a trust manager that does not validate certificate chains
    public static TrustManager[] trustAllCerts = new TrustManager[] { new X509TrustManager() {
		public java.security.cert.X509Certificate[] getAcceptedIssuers() { return new java.security.cert.X509Certificate[] {}; }
		public void checkClientTrusted(X509Certificate[] chain,String authType) throws CertificateException {}
		public void checkServerTrusted(X509Certificate[] chain, String authType) throws CertificateException {}
	}};
}
