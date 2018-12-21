package com.phuket.tour.mp3encoder;

import com.phuket.tour.studio.Mp3Encoder;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;

import java.util.List;

import pub.devrel.easypermissions.EasyPermissions;

public class MainActivity extends Activity implements  EasyPermissions.PermissionCallbacks{

	static {
		System.loadLibrary("audioencoder");
	}
	public static final int PERMISSION_STORAGE_CODE = 0x001;
	public static final String PERMISSION_STORAGE_MSG = "此app需要获取SD卡读取权限";
	public static final String[] PERMISSION_STORAGE = new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE,Manifest.permission.READ_EXTERNAL_STORAGE};


	private Button mp3_encoder_btn;
	private final String TAG = "MainActivity";
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		findViewById(R.id.mp3_encoder_btn).setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				if(hasStoragePermission(MainActivity.this)){
					encode();
				}else{
					EasyPermissions.requestPermissions(MainActivity.this,PERMISSION_STORAGE_MSG,PERMISSION_STORAGE_CODE,PERMISSION_STORAGE);
				}
			}
		});
	}

    private void encode()
	{
		Mp3Encoder encoder = new Mp3Encoder();
		String pcmPath = "/mnt/sdcard/a_songstudio/vocal.pcm";
		int audioChannels = 1;//2;
		int sampleRate = 16000;//44100;
		int bitRate = sampleRate*audioChannels*16;//128 * 1024;
		String mp3Path = "/mnt/sdcard/a_songstudio/vocal.mp3";
		int ret = encoder.init(pcmPath, audioChannels, bitRate, sampleRate, mp3Path);
		if(ret >=0) {
			encoder.encode();
			encoder.destroy();
			Log.i(TAG, "Encode Mp3 Success");
		} else {
			Log.i(TAG, "Encoder Initialized Failed...");
		}
	}

	public static boolean hasPermissions(Context context, String... permissions) {
		return EasyPermissions.hasPermissions(context, permissions);
	}

	/**
	 * 是否有SD卡权限
	 *
	 * @param context
	 * @return
	 */
	public static boolean hasStoragePermission(Context context) {
		return hasPermissions(context, PERMISSION_STORAGE);
	}


	@Override
	public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
		super.onRequestPermissionsResult(requestCode, permissions, grantResults);
		// Forward results to EasyPermissions
		EasyPermissions.onRequestPermissionsResult(requestCode, permissions, grantResults, this);
	}

	@Override
	public void onPermissionsGranted(int requestCode, List<String> list) {
	   encode();
	}

	@Override
	public void onPermissionsDenied(int requestCode, List<String> list) {

		Toast.makeText(MainActivity.this, "权限拒绝", Toast.LENGTH_SHORT).show();
	}
}
