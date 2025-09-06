#include <jni.h>
#include <string>

#include "../../../../../AppConfig.h"

#include <LicenseSpring/Configuration.h>
#include <LicenseSpring/ExtendedOptions.h>
#include <LicenseSpring/LicenseID.h>
#include <LicenseSpring/LicenseHandler.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_androidndksample_MainActivity_nativeInitLicenseHandler(
        JNIEnv* env, jobject) {
    AppConfig appConfig("Android NDK sample", "1.0");
    auto pConfiguration = appConfig.createLicenseSpringConfig();
    auto &lh = LicenseSpring::LicenseHandler::instance();
    lh.reconfigure(pConfiguration);

    // check if we already have an active license
    std::string out = (lh.isLicenseExists() ? "license exists" : "");
    return env->NewStringUTF(out.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_androidndksample_MainActivity_nativeActivateKeyBased(
        JNIEnv* env, jobject /*thiz*/, jstring jKey)
{
    const char* key = env->GetStringUTFChars(jKey, nullptr);
    auto licenseId    = LicenseSpring::LicenseID::fromKey(key);
    auto &lh = LicenseSpring::LicenseHandler::instance();
    lh.activateLicense(licenseId);
    env->ReleaseStringUTFChars(jKey, key);
    std::string out = (lh.wasError() ? "error: " + lh.getLastErrorMsg() : lh.getLicenseStatusStr());
    return env->NewStringUTF(out.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_androidndksample_MainActivity_nativeActivateUserBased__Ljava_lang_String_2Ljava_lang_String_2(
        JNIEnv* env, jobject /*thiz*/, jstring jUser, jstring jPass)
{
    const char* username = env->GetStringUTFChars(jUser, nullptr);
    const char* password = env->GetStringUTFChars(jPass, nullptr);
    auto licenseId = LicenseSpring::LicenseID::fromUser(username, password);
    auto &lh = LicenseSpring::LicenseHandler::instance();
    lh.activateLicense(licenseId);
    std::string out = (lh.wasError() ? "error: " + lh.getLastErrorMsg() : lh.getLicenseStatusStr());
    env->ReleaseStringUTFChars(jUser, username);
    env->ReleaseStringUTFChars(jPass, password);
    return env->NewStringUTF(out.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_androidndksample_MainActivity_nativeGetTrial(
        JNIEnv* env, jobject /*thiz*/, jstring jEmail)
{
    const char* email = env->GetStringUTFChars(jEmail, nullptr);
    auto &lh = LicenseSpring::LicenseHandler::instance();

    std::string out;
    auto licenseId = lh.getTrialLicense(email);
    if (lh.wasError()) {
        env->ReleaseStringUTFChars(jEmail, email);
        return env->NewStringUTF(out.c_str());
    }

    lh.activateLicense(licenseId);
    out = (lh.wasError() ? "error: " + lh.getLastErrorMsg() : lh.getLicenseStatusStr());
    env->ReleaseStringUTFChars(jEmail, email);
    return env->NewStringUTF(out.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_androidndksample_MainActivity_nativeGetLicenseInfo(
        JNIEnv* env, jobject /*thiz*/)
{
    auto &lh = LicenseSpring::LicenseHandler::instance();
    std::string out = "{";
    out += R"("key":")" + lh.getLicenseKey() +  + R"(",)";
    out += R"("user":")" + lh.getLicenseUser() +  + R"(",)";
    out += R"("status":")" + lh.getLicenseStatusStr() +  + R"(",)";
    out += R"("is_offline_activated":")" + std::to_string(lh.isLicenseOfflineActivated()) +  + R"(",)";
    out += R"("is_trial":")" + std::to_string(lh.isLicenseTrial()) +  + R"(",)";
    out += R"("is_floating":")" + std::to_string(lh.isLicenseFloating()) +  + R"(",)";
    out += R"("floating_timeout":")" + std::to_string(lh.getLicenseFloatingTimeout()) +  + R"(",)";
    out += R"("validity_period":")" + LicenseSpring::TmToString(lh.validityWithGracePeriod(), "%d-%m-%Y %H:%M:%S") +  + R"(",)";
    out += R"("maintenance_period":")" + LicenseSpring::TmToString(lh.getLicenseMaintenancePeriod(), "%d-%m-%Y %H:%M:%S") +  + R"(",)";
    out += R"("last_checked":")" + LicenseSpring::TmToString(lh.getLicenseLastCheckDate(), "%d-%m-%Y %H:%M:%S") +  + R"(",)";
    out += R"("total_consumptions":")" + std::to_string(lh.getLicenseTotalConsumption()) +  + R"(",)";
    out += R"("max_consumptions":")" + std::to_string(lh.getLicenseMaxConsumption()) +  + R"(")";
    out += "}";
    return env->NewStringUTF(out.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_androidndksample_MainActivity_nativeProductDetails(
        JNIEnv* env, jobject)
{
    auto &lh = LicenseSpring::LicenseHandler::instance();
    auto details = lh.getProductDetails();
    if (lh.wasError()) {
        std::string error = "error: " + lh.getLastErrorMsg();
        return env->NewStringUTF(error.c_str());
    }

    // Use nlohmann::json for proper JSON handling
    // We construct this one manually to reduce compile times for the sample
    std::string out = "{";
    out += R"("product_name":")" + details->productName() + R"(",)";
    out += R"("product_code":")" + details->productCode() + R"(",)";
    out += R"("trial_allowed":)" + std::string(details->isTrialAllowed() ? "true" : "false") + R"(",)";
    out += R"("trial_period":)" + std::to_string(details->trialPeriod()) + R"(",)";
    out += R"("vm_allowed":)" + std::string(details->isVMAllowed() ? "true" : "false") + R"(",)";
    out += R"("floating_timeout":)" + std::to_string(details->floatingLicenseTimeout()) + R"(",)";
    out += R"("latest_version":")" + details->latestVersion() + R"(",)";
    std::string metadata  = details->metadata();
    std::string metadataFixed;
    metadataFixed.reserve(metadata.size());
    for (char c : metadata) {
        switch (c) {
        case '\"': metadataFixed += "\\\""; break;
        case '\\': metadataFixed += "\\\\"; break;
        default:
            metadataFixed += c;
        }
    }
    out += R"("metadata":")" + metadataFixed + R"(")";
    out += "}";

    return env->NewStringUTF(out.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_androidndksample_MainActivity_nativeSyncConsumption(
        JNIEnv* env, jobject)
{
    auto &lh = LicenseSpring::LicenseHandler::instance();
    if (!lh.isLicenseExists())
        return env->NewStringUTF("license not active");
    lh.syncConsumption();
    std::string out = (lh.wasError() ? "error: " + lh.getLastErrorMsg() : "consumptions synced");
    return env->NewStringUTF(out.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_androidndksample_MainActivity_nativeUpdateConsumption(
        JNIEnv* env, jobject /*thiz*/, jint amount)
{
    auto &lh = LicenseSpring::LicenseHandler::instance();
    if (!lh.isLicenseExists())
        return env->NewStringUTF("license not active");
    lh.updateConsumption(amount);
    std::string out = (lh.wasError() ? "error: " + lh.getLastErrorMsg() : "consumptions updated");
    return env->NewStringUTF(out.c_str());
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_example_androidndksample_MainActivity_nativeGetFeatures(
        JNIEnv* env, jobject /*thiz*/)
{
    auto &lh = LicenseSpring::LicenseHandler::instance();
    auto features = lh.getLicenseFeatures();
    if (lh.wasError())
        return nullptr;

    jclass strClass = env->FindClass("java/lang/String");
    jobjectArray arr = env->NewObjectArray(
            (jsize)features.size(),
            strClass,
            nullptr
    );

    for (jsize i = 0; i < (jsize)features.size(); ++i) {
        jstring s = env->NewStringUTF(features[i].code().c_str());
        env->SetObjectArrayElement(arr, i, s);
        env->DeleteLocalRef(s);
    }

    return arr;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_androidndksample_MainActivity_nativeCheckLicense(
        JNIEnv* env, jobject)
{
    auto &lh = LicenseSpring::LicenseHandler::instance();
    if (!lh.isLicenseExists())
        return env->NewStringUTF("license not active");
    lh.checkLicense();
    std::string out = (lh.wasError() ? "error: " + lh.getLastErrorMsg() : "license checked successfully");
    return env->NewStringUTF(out.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_androidndksample_MainActivity_nativeDeactivate(
        JNIEnv* env, jobject)
{
    auto &lh = LicenseSpring::LicenseHandler::instance();
    if (!lh.isLicenseExists())
        return env->NewStringUTF("license not active");
    lh.deactivateLicense();
    std::string out = (lh.wasError() ? "error: " +  lh.getLastErrorMsg() : "license deactivated successfully");
    return env->NewStringUTF(out.c_str());
}