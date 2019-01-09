#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "log.h"
#include "property_service.h"
#include "util.h"
#include "vendor_init.h"

#include "init_sec.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/strings.h>

using android::base::Trim;
using android::base::GetProperty;
using android::base::ReadFileToString;
using android::init::property_set;

static std::string bootloader;

static void property_override(char const prop[], char const value[]) {
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

static device_variant parse_variant(std::string bl) {
    device_variant ret = VARIANT_MAX;

    if (bl.find("G950F") != std::string::npos)
        ret = VARIANT_G950F;
    else if (bl.find("G950S") != std::string::npos)
        ret = VARIANT_G950S;
    else if (bl.find("G950K") != std::string::npos)
        ret = VARIANT_G950K;
    else if (bl.find("G950L") != std::string::npos)
        ret = VARIANT_G950L;
    else if (bl.find("G950W8") != std::string::npos)
        ret = VARIANT_G950W8;
    else if (bl.find("G955F") != std::string::npos)
        ret = VARIANT_G955F;
    else if (bl.find("G955S") != std::string::npos)
        ret = VARIANT_G955S;
    else if (bl.find("G955K") != std::string::npos)
        ret = VARIANT_G955K;
    else if (bl.find("G955L") != std::string::npos)
        ret = VARIANT_G955L;
    else if (bl.find("G955W8") != std::string::npos)
        ret = VARIANT_G955W8;

    return ret;
}

static device_variant get_variant_from_cmdline()
{
    bootloader = android::base::GetProperty("ro.bootloader", "");
    device_variant ret = parse_variant(bootloader);

    if (ret >= VARIANT_MAX) {
        //INFO("Unknown bootloader id: %s, forcing international (F) variant\n", bootloader.c_str());
	LOG(INFO) << "Unknown bootloader id: %s " << "forcing international (F) variant\n" << bootloader.c_str();
        if (bootloader.find("G950") != std::string::npos)
            ret = VARIANT_G950F;
        else
            ret = VARIANT_G955F;
    }

    return ret;
}

void vendor_load_properties()
{
    const device_variant variant = get_variant_from_cmdline();

    std::string fingerprint, description, model, device;

    switch (variant) {
    case VARIANT_G950F:
        /* dreamltexx */
        fingerprint = "samsung/dreamltexx/dreamlte:7.0/NRD90M/G950FXXU1AQH3:user/release-keys";
        description = "dreamltexx-user 7.0 NRD90M G950FXXU1AQH3 release-keys";
        model = "SM-G950F";
        device = "dreamltexx";
        break;
    case VARIANT_G950S:
        /* dreamlteskt */
        fingerprint = "samsung/dreamlteskt/dreamlte:7.0/NRD90M/G950SXXU1AQH3:user/release-keys";
        description = "dreamlteskt-user 7.0 NRD90M G950SXXU1AQH3 release-keys";
        model = "SM-G950S";
        device = "dreamlteskt";
        break;
    case VARIANT_G950K:
        /* dreamltektt */
        fingerprint = "samsung/dreamltektt/dreamlte:7.0/NRD90M/G950KXXU1AQH3:user/release-keys";
        description = "dreamltektt-user 7.0 NRD90M G950KXXU1AQH3 release-keys";
        model = "SM-G950K";
        device = "dreamltektt";
        break;
    case VARIANT_G950L:
        /* dreamltelgt */
        fingerprint = "samsung/dreamltelgt/dreamlte:7.0/NRD90M/G950LXXU1AQH3:user/release-keys";
        description = "dreamltelgt-user 7.0 NRD90M G950LXXU1AQH3 release-keys";
        model = "SM-G950L";
        device = "dreamltelgt";
        break;
    case VARIANT_G950W8:
        /* dreamltebmc */
        fingerprint = "samsung/dreamltebmc/dreamlte:7.0/NRD90M/G950W8XXU1AQH3:user/release-keys";
        description = "dreamltebmc-user 7.0 NRD90M G950W8XXU1AQH3 release-keys";
        model = "SM-G950W8";
        device = "dreamltebmc";
        break;
    case VARIANT_G955F:
        /* dream2ltexx */
        fingerprint = "samsung/dream2ltexx/dream2lte:7.0/NRD90M/G955FXXU1AQH3:user/release-keys";
        description = "dream2ltexx-user 7.0 NRD90M G955FXXU1AQH3 release-keys";
        model = "SM-G955F";
        device = "dream2ltexx";
        break;
    case VARIANT_G955S:
        /* dream2lteskt */
        fingerprint = "samsung/dream2lteskt/dream2lte:7.0/NRD90M/G955SXXU1AQH3:user/release-keys";
        description = "dream2lteskt-user 7.0 NRD90M G955SXXU1AQH3 release-keys";
        model = "SM-G955S";
        device = "dream2lteskt";
        break;
    case VARIANT_G955K:
        /* dream2ltektt */
        fingerprint = "samsung/dream2ltektt/dream2lte:7.0/NRD90M/G955KXXU1AQH3:user/release-keys";
        description = "dream2ltektt-user 7.0 NRD90M G955KXXU1AQH3 release-keys";
        model = "SM-G955K";
        device = "dream2ltektt";
        break;
    case VARIANT_G955L:
        /* dream2ltelgt */
        fingerprint = "samsung/dream2ltelgt/dream2lte:7.0/NRD90M/G955LXXU1AQH3:user/release-keys";
        description = "dream2ltelgt-user 7.0 NRD90M G955LXXU1AQH3 release-keys";
        model = "SM-G955L";
        device = "dream2ltelgt";
        break;
    case VARIANT_G955W8:
        /* dream2ltebmc */
        fingerprint = "samsung/dream2ltebmc/dream2lte:7.0/NRD90M/G955W8XXU1AQH3:user/release-keys";
        description = "dream2ltebmc-user 7.0 NRD90M G955W8XXU1AQH3 release-keys";
        model = "SM-G955W8";
        device = "dream2ltebmc";
        break;
    default:
        break;
    }

    //INFO("Found bootloader id: %s setting build properties for: %s device\n", bootloader.c_str(), device.c_str());
    LOG(INFO) << "Found bootloader id: %s " << "setting build properties for: %s device\n" << bootloader.c_str() << device.c_str();

    property_override("ro.build.fingerprint", fingerprint.c_str());
    property_override("ro.build.description", description.c_str());
    property_override("ro.build.product", device.c_str());
    property_override("ro.product.model", model.c_str());
    property_override("ro.product.device", device.c_str());
}
