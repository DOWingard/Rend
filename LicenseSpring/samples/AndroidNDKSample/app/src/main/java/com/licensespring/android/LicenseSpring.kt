package com.licensespring.android

import android.content.Context
import android.util.Log

object LicenseSpring {

    // This method decalaration exactly matches the signature declared in the LicenseSpring SDK. Don't remove it.
    external fun setAndroidContextAndIDs(context: Context)

    init {
        try {
            System.loadLibrary("LicenseSpring")
            Log.d("LicenseSpring", "Native library loaded successfully")
        } catch (e: UnsatisfiedLinkError) {
            Log.e("LicenseSpring", "Failed to load native library: ${e.message}")
        }
    }

    fun initialize(appContext: Context) {
        try {
            setAndroidContextAndIDs(appContext.applicationContext)
            Log.d("LicenseSpring", "Native context and IDs set successfully")
        } catch (e: Exception) {
            Log.e("LicenseSpring", "Error calling native method: ${e.message}")
        }
    }
}