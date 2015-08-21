#
# Copyright 2013 The Android Open-Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#


# call dalvik heap config
$(call inherit-product-if-exists, frameworks/native/build/phone-xxhdpi-2048-dalvik-heap.mk)

# call hwui memory config
$(call inherit-product-if-exists, frameworks/native/build/phone-xxhdpi-2048-hwui-memory.mk)

DEVICE_PACKAGE_OVERLAYS := \
    device/asus/mofd-common/overlay

# Ramdisk
PRODUCT_PACKAGES += \
    config_init.sh \
    fstab \
    fstab.mofd_v1 \
    init.avc.rc \
    init.bt.rc \
    init.bt.vendor.rc \
    init.class_main.sh \
    init.common.rc \
    init.config_init.rc \
    init.debug.rc \
    init.diag.rc \
    init.firmware.rc \
    init.gps.rc \
    init.logtool.rc \
    init.modem.rc \
    init.mofd_v1.rc \
    init.nfc.rc \
    init.platform.usb.rc \
    init.readahead.rc \
    init.recovery.mofd_v1.rc \
    init.watchdog.rc \
    init.wifi.rc \
    init.wifi.vendor.rc \
    intel_prop \
    intel_prop.cfg \
    rfkill_bt.sh \
    thermald \
    ueventd.mofd_v1.rc

# Audio
PRODUCT_PACKAGES += \
    libtinycompress \
    libtinyalsa \
    audio.a2dp.default \
    audio.r_submix.default \
    audio.usb.default

# Charger
PRODUCT_PACKAGES += \
    charger \
    charger_res_images

# GPS
PRODUCT_COPY_FILES += \
    device/asus/mofd-common/configs/gps.conf:system/etc/gps.conf \
    device/asus/mofd-common/configs/gps.xml:system/etc/gps.xml

PRODUCT_PROPERTY_OVERRIDES += \
    ro.spid.gps.tty=ttyMFD2

# Lights
PRODUCT_PACKAGES += \
    lights.mofd_v1

# Radio
PRODUCT_PROPERTY_OVERRIDES += \
    ro.ril.status.polling.enable=0 \
    rild.libpath=/system/lib/librapid-ril-core.so \
    ro.telephony.default_network=9 \
    ro.telephony.ril_class=Zenfone2RIL

# specific management of audio_policy.conf
PRODUCT_COPY_FILES += \
    device/asus/mofd-common/configs/audio_policy.conf:system/etc/audio_policy.conf

# Add props used in stock
PRODUCT_PROPERTY_OVERRIDES +=  \
    ro.dalvik.vm.isa.arm=x86 \
    ro.enable.native.bridge.exec=1 \
    drm.service.enabled=true \
    ro.com.widevine.cachesize=16777216 \
    media.stagefright.cache-params=10240/20480/15 \
    media.aac_51_output_enabled=true \
    dalvik.vm.implicit_checks=none

# Set the prop to enable arm native bridge
ADDITIONAL_DEFAULT_PROPERTIES += ro.dalvik.vm.native.bridge=libhoudini.so

# set USB OTG enabled to add support for USB storage type
PRODUCT_PROPERTY_OVERRIDES += persist.sys.isUsbOtgEnabled=1

# Set default USB interface
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += persist.sys.usb.config=mtp

PRODUCT_COPY_FILES += \
    device/asus/mofd-common/bluetooth/bt_vendor.conf:system/etc/bluetooth/bt_vendor.conf \
    device/asus/mofd-common/wifi/bcmdhd.cal:system/etc/wifi/bcmdhd.cal 

PRODUCT_PROPERTY_OVERRIDES += \
    bt.hfp.WideBandSpeechEnabled=true

# IMG graphics
PRODUCT_PACKAGES += \
    hwcomposer.moorefield

#Video
PRODUCT_COPY_FILES += \
    device/asus/mofd-common/media/media_profiles.xml:system/etc/media_profiles.xml \
    device/asus/mofd-common/media/wrs_omxil_components.list:system/etc/wrs_omxil_components.list \
    frameworks/av/media/libstagefright/data/media_codecs_google_audio.xml:system/etc/media_codecs_google_audio.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_video.xml:system/etc/media_codecs_google_video.xml \
    device/asus/mofd-common/media/media_codecs.xml:system/etc/media_codecs.xml \
    device/asus/mofd-common/media/mfx_omxil_core.conf:system/etc/mfx_omxil_core.conf \
    device/asus/mofd-common/media/video_isv_profile.xml:system/etc/video_isv_profile.xml

# psb video
PRODUCT_PACKAGES += \
    pvr_drv_video

# Media SDK and OMX IL components
PRODUCT_PACKAGES += \
    msvdx_bin \
    topaz_bin

# libva
PRODUCT_PACKAGES += \
    libva \
    libva-android \
    libva-tpi \
    vainfo

#libstagefrighthw
PRODUCT_PACKAGES += \
    libstagefrighthw

# libmix
PRODUCT_PACKAGES += \
    libmixvbp_mpeg4 \
    libmixvbp_h264 \
    libmixvbp_h264secure \
    libmixvbp_vc1 \
    libmixvbp_vp8 \
    libva_videodecoder \
    libva_videoencoder

PRODUCT_PACKAGES += \
    libwrs_omxil_common \
    libwrs_omxil_core_pvwrapped \
    libOMXVideoDecoderAVC \
    libOMXVideoDecoderH263 \
    libOMXVideoDecoderMPEG4 \
    libOMXVideoDecoderWMV \
    libOMXVideoDecoderVP8 \
    libOMXVideoDecoderVP9HWR \
    libOMXVideoDecoderVP9Hybrid \
    libOMXVideoEncoderAVC \
    libOMXVideoEncoderH263 \
    libOMXVideoEncoderMPEG4 \
    libOMXVideoEncoderVP8

#libISV
PRODUCT_PACKAGES += libisv_omx_core

# pvr
PRODUCT_PACKAGES += \
    libpvr2d

# libdrm
PRODUCT_PACKAGES += \
    libdrm \
    dristat \
    drmstat

# Wifi
PRODUCT_PACKAGES += \
    libwpa_client \
    lib_driver_cmd_bcmdhd \
    hostapd \
    dhcpcd.conf \
    wpa_supplicant \
    bcmdhd.cal

PRODUCT_COPY_FILES += \
    device/asus/mofd-common/wifi/wpa_supplicant.conf:/system/etc/wifi/wpa_supplicant.conf

PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
    frameworks/native/data/etc/android.hardware.camera.autofocus.xml:system/etc/permissions/android.hardware.camera.autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.camera.xml:system/etc/permissions/android.hardware.camera.xml \
    frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml \
    frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
    frameworks/native/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml \
    frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
    frameworks/native/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/native/data/etc/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
    frameworks/native/data/etc/android.hardware.sensor.barometer.xml:system/etc/permissions/android.hardware.sensor.barometer.xml \
    frameworks/native/data/etc/android.hardware.sensor.stepcounter.xml:system/etc/permissions/android.hardware.sensor.stepcounter.xml \
    frameworks/native/data/etc/android.hardware.sensor.stepdetector.xml:system/etc/permissions/android.hardware.sensor.stepdetector.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
    frameworks/native/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml \
    frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
    frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml \
    frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/native/data/etc/android.hardware.audio.low_latency.xml:system/etc/permissions/android.hardware.audio.low_latency.xml \
    frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/etc/permissions/android.hardware.bluetooth_le.xml \
    frameworks/native/data/etc/android.hardware.ethernet.xml:system/etc/permissions/android.hardware.ethernet.xml

# Key layout files
PRODUCT_COPY_FILES += \
    device/asus/mofd-common/keylayout/ASUS_TransKeyboard.kl:system/usr/keylayout/ASUS_TransKeyboard.kl \
    device/asus/mofd-common/keylayout/ftxxxx_ts.kl:system/usr/keylayout/ftxxxx_ts.kl \
    device/asus/mofd-common/keylayout/ftxxxx_ts.kcm:system/usr/keychars/ftxxxx_ts.kcm \
    device/asus/mofd-common/keylayout/gpio-keys.kl:system/usr/keylayout/gpio-keys.kl \
    device/asus/mofd-common/keylayout/lm49453-audio_Intel_MID_Audio_Jack.kl:system/usr/keylayout/lm49453-audio_Intel_MID_Audio_Jack.kl \
    device/asus/mofd-common/keylayout/Vendor_0b05_Product_17fc.kl:system/usr/keylayout/Vendor_0b05_Product_17fc.kl \
    device/asus/mofd-common/keylayout/Vendor_0b05_Product_1803.kl:system/usr/keylayout/Vendor_0b05_Product_1803.kl

# Keyhandler
PRODUCT_PACKAGES += \
    com.cyanogenmod.keyhandler

#GFX Config
PRODUCT_COPY_FILES += \
    device/asus/mofd-common/powervr.ini:system/etc/powervr.ini

# Thermal itux
ENABLE_ITUXD := true
PRODUCT_PACKAGES += \
    ituxd

$(call inherit-product-if-exists, vendor/asus/mofd-common/mofd-common-vendor.mk)

# Add WiFi Firmware
$(call inherit-product-if-exists, hardware/broadcom/wlan/bcmdhd/firmware/bcm4339/device-bcm.mk)

# specific management of sep_policy.conf
PRODUCT_COPY_FILES += \
    device/asus/mofd-common/sep_policy.conf:system/etc/security/sep_policy.conf

# hardware optimizations
#PRODUCT_PROPERTY_OVERRIDES += \
#    dalvik.vm.isa.x86.features=sse4_2,aes_in,popcnt,movbe
