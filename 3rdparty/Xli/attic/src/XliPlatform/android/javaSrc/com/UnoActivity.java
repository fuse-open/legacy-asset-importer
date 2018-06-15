package $(Android.PackageName);
$(UnoCore.UnoActivity.Top.Statement:Join())

import android.app.NativeActivity;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import com.Shim.UnoSurfaceView;

public class $(Android.LibraryName) extends android.app.NativeActivity
{
    static 
    {
        $(Android.Java.LoadLibraries)
    }
        
    public static NativeActivity RootActivity;
    public static LinearLayout RootLayout;
    public static UnoSurfaceView RootView;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d("XliApp", "Caching Native Activity");
    	RootActivity = this;
    	com.Shim.XliJ.CacheActivity(this);        
    	Log.d("XliApp", "Successfully Cached Native Activity");
        
        super.onCreate(savedInstanceState);
        
        RootView = new UnoSurfaceView(RootActivity);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
        RootView.setLayoutParams(layoutParams);
        
        RootLayout = new LinearLayout(RootActivity);
        RootLayout.setOrientation(LinearLayout.VERTICAL);
        RootLayout.addView(RootView);
        com.Shim.XliJ.CacheRootUILayout(RootLayout);
        
        getWindow().takeSurface(null);
        getWindow().setContentView(RootLayout);
    }

    //------------------------------------------------
    
    $(UnoCore.UnoActivity.Body.Statement:Join())
}
