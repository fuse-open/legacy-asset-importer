package com.Shim;

import java.util.ArrayList;

import android.os.Handler;
import android.os.Message;
import android.util.Log;

public class XliCppThreadHandler extends Handler {
	public static final int REPEATING_MESSAGE = 10;
	private int repeatCount = 0;
	private ArrayList<Integer> repeatingMessageIndex;
	
	@Override
	public void handleMessage(Message msg)
	{
		Log.e("XliApp","Message: " + msg);
		switch (msg.what) {
		case REPEATING_MESSAGE:
			handleRepeating(msg);
			break;
		default:
			break;
		}
	}
	
	private void handleRepeating(Message msg)
	{
		if (repeatingMessageIndex.contains((Integer)msg.arg1)) {
			XliJ.XliJ_TimerCallback(msg.arg1);
			this.sendMessageDelayed(msg, msg.arg2);
		}
	}
	
	public void unregisterRepeating(int repeater_id)
	{
		repeatingMessageIndex.remove((Integer)repeater_id);
	}
	
	public int registerRepeating(int millisecondsDelay)
	{
		int i = repeatCount+=1;
		repeatingMessageIndex.add(i);
		Message msg = Message.obtain();
		msg.what = REPEATING_MESSAGE;
		msg.arg1 = i;
		msg.arg2 = millisecondsDelay;
		this.sendMessageDelayed(msg, millisecondsDelay);
		return i;
	}
}
