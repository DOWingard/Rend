package com.example.androidndksample

import android.os.Bundle
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.app.AlertDialog
import androidx.recyclerview.widget.LinearLayoutManager
import com.example.androidndksample.databinding.ActivityMainBinding
import com.licensespring.android.LicenseSpring
import org.json.JSONArray
import org.json.JSONObject

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding
    private lateinit var featuresAdapter: FeaturesAdapter

    companion object {
        init {
            System.loadLibrary("androidndksample")
        }
    }

    // JNI entry points
    external fun nativeInitLicenseHandler(): String
    external fun nativeActivateKeyBased(key: String): String
    external fun nativeActivateUserBased(username: String, password: String): String
    external fun nativeGetTrial(email: String): String
    external fun nativeGetLicenseInfo(): String
    external fun nativeProductDetails(): String
    external fun nativeSyncConsumption(): String
    external fun nativeUpdateConsumption(amount: Int): String
    external fun nativeGetFeatures(): Array<String>?
    external fun nativeCheckLicense(): String
    external fun nativeDeactivate(): String

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        LicenseSpring.initialize(this);

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.rvFeatures.layoutManager = LinearLayoutManager(this)
        featuresAdapter = FeaturesAdapter(emptyList())
        binding.rvFeatures.adapter = featuresAdapter

        val licenseExists = nativeInitLicenseHandler();
        if (licenseExists.startsWith("license exists"))
            showInfoView();

        // activation window
        binding.btnActivateKey.setOnClickListener {
            val out = nativeActivateKeyBased(binding.etKey.text.toString().trim())
            Toast.makeText(this, out, Toast.LENGTH_SHORT).show()
            if (!out.startsWith("error")) showInfoView()
        }
        binding.btnActivateUser.setOnClickListener {
            val out = nativeActivateUserBased(
                binding.etUser.text.toString().trim(),
                binding.etPass.text.toString()
            )
            Toast.makeText(this, out, Toast.LENGTH_SHORT).show()
            if (!out.startsWith("error")) showInfoView()
        }

        binding.btnGetTrial.setOnClickListener {
            val out = nativeGetTrial(binding.etEmail.text.toString().trim())
            Toast.makeText(this, out, Toast.LENGTH_SHORT).show()
            if (!out.startsWith("error")) showInfoView()
        }

        // license window
        binding.btnProductDetails.setOnClickListener {
            val json = nativeProductDetails()
            if (json.startsWith("error")) {
                Toast.makeText(this, json, Toast.LENGTH_LONG).show()
                return@setOnClickListener
            }

            val o = JSONObject(json)
            val sb = StringBuilder().apply {
                appendLine("Name: ${o.optString("product_name")}")
                appendLine("Code: ${o.optString("product_code")}")
                appendLine("Trial allowed: ${o.optBoolean("trial_allowed")}")
                appendLine("Trial period: ${o.optInt("trial_period")} days")
                appendLine("VM allowed: ${o.optBoolean("vm_allowed")}")
                appendLine("Floating timeout: ${o.optInt("floating_timeout")} minutes")
                appendLine("Latest version: ${o.optString("latest_version")}")
                appendLine("Metadata: ${o.optString("metadata")}")
            }

            AlertDialog.Builder(this)
                .setTitle("Product Details")
                .setMessage(sb.toString())
                .setPositiveButton("OK", null)
                .show()
            Toast.makeText(this, nativeProductDetails(), Toast.LENGTH_SHORT).show()
        }
        binding.btnSyncConsumption.setOnClickListener {
            Toast.makeText(this, nativeSyncConsumption(), Toast.LENGTH_SHORT).show()
            populateLicenseInfo()
        }
        binding.btnUpdateConsumption.setOnClickListener {
            val n = binding.etConsumption.text.toString().toIntOrNull() ?: 0
            Toast.makeText(this, nativeUpdateConsumption(n), Toast.LENGTH_SHORT).show()
            populateLicenseInfo()
        }
        binding.btnCheckLicense.setOnClickListener {
            Toast.makeText(this, nativeCheckLicense(), Toast.LENGTH_SHORT).show()
            populateLicenseInfo()
            populateLicenseFeatures()
        }
        binding.btnDeactivate.setOnClickListener {
            val out = nativeDeactivate();
            Toast.makeText(this, out, Toast.LENGTH_SHORT).show()
            if (!out.startsWith("error"))
                showActivationView()
            else
                populateLicenseInfo()
        }
    }

    private fun showInfoView() {
        binding.activateView.visibility = View.GONE
        binding.infoView.visibility     = View.VISIBLE
        populateLicenseInfo()
        populateLicenseFeatures()
    }

    private fun showActivationView() {
        binding.infoView.visibility     = View.GONE
        binding.activateView.visibility = View.VISIBLE
    }

    private fun populateLicenseInfo() {
        val json = nativeGetLicenseInfo()
        if (json.startsWith("error")) {
            Toast.makeText(this, json, Toast.LENGTH_LONG).show()
            return
        }
        val o = JSONObject(json)
        binding.tvKey.text   = o.optString("key")
        binding.tvUser.text  = o.optString("user")
        binding.tvStatus.text  = o.optString("status")
        binding.tvIsOfflineActivated.text   = o.optString("is_offline_activated")
        binding.tvIsTrial.text  = o.optString("is_trial")
        binding.tvIsFloating.text  = o.optString("is_floating")
        binding.tvFloatingTimeout.text  = o.optString("floating_timeout")
        binding.tvValidityPeriod.text   = o.optString("validity_period")
        binding.tvMaintenancePeriod.text  = o.optString("maintenance_period")
        binding.tvLastChecked.text  = o.optString("last_checked")
        binding.tvTotalConsumptions.text  = o.optString("total_consumptions")
        binding.tvMaxConsumptions.text  = o.optString("max_consumptions")
    }

    private fun populateLicenseFeatures() {
        nativeGetFeatures()?.let { features ->
            featuresAdapter.updateData(features.toList())
        }
    }
}