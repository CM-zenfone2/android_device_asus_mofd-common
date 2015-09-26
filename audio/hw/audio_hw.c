/*
 * Copyright (C) 2011 The Android Open Source Project
 * Copyright (C) 2012 Wolfson Microelectronics plc
 * Copyright (C) 2013 The CyanogenMod Project
 *               Daniel Hillenbrand <codeworkx@cyanogenmod.com>
 *               Guillaume "XpLoDWilD" Lesniak <xplodgui@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "audio_hw_primary_wrapper"
#define LOG_NDEBUG 0

#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <dlfcn.h>

#include <cutils/log.h>
#include <cutils/properties.h>
#include <cutils/str_parms.h>

#include <hardware/audio.h>
#include <hardware/hardware.h>

#include <system/audio.h>

struct audio_device {
    struct audio_hw_device base;
    struct audio_hw_device *vendor;
};

struct stream_out {
    struct audio_stream_out base;
    struct audio_stream_out *vendor;
};

struct stream_in {
    struct audio_stream_in base;
    struct audio_stream_in *vendor;
};

#define STR(s) #s

#define DEVICE_COMMON_CALL(device, func, ...) ({ \
    struct audio_device *__wrapper_dev = (struct audio_device *) device; \
    ALOGI("crpalmer: device_common: " STR(func) "\n"); \
    __wrapper_dev->vendor->common.func((struct hw_device_t *) __wrapper_dev->vendor, ##__VA_ARGS__); \
})

#define DEVICE_CALL(device, func, ...) ({ \
    struct audio_device *__wrapper_dev = (struct audio_device *) device; \
    ALOGI("crpalmer: device: " STR(func) "\n"); \
    __wrapper_dev->vendor->func(__wrapper_dev->vendor, ##__VA_ARGS__); \
})

#define OUT_COMMON_CALL(device, func, ...) ({ \
    struct stream_out *__wrapper_dev = (struct stream_out *) device; \
    ALOGI("crpalmer: out_common: " STR(func) "\n"); \
    __wrapper_dev->vendor->common.func((struct audio_stream *) __wrapper_dev->vendor, ##__VA_ARGS__); \
})

#define OUT_CALL(device, func, ...) ({ \
    struct stream_out *__wrapper_dev = (struct stream_out *) device; \
    ALOGI("crpalmer: out: " STR(func) "\n"); \
    __wrapper_dev->vendor->func(__wrapper_dev->vendor, ##__VA_ARGS__); \
})

#define IN_COMMON_CALL(device, func, ...) ({ \
    struct stream_in *__wrapper_dev = (struct stream_in *) device; \
    ALOGI("crpalmer: in_common: " STR(func) "\n"); \
    __wrapper_dev->vendor->common.func((struct audio_stream *) __wrapper_dev->vendor, ##__VA_ARGS__); \
})

#define IN_CALL(device, func, ...) ({ \
    struct stream_in *__wrapper_dev = (struct stream_in *) device; \
    ALOGI("crpalmer: in: " STR(func) "\n"); \
    __wrapper_dev->vendor->func(__wrapper_dev->vendor, ##__VA_ARGS__); \
})

static struct hw_device_t *gVendorModule;

static int check_vendor_module()
{
    int rv = 0;

    if(gVendorModule)
        return 0;

    rv = hw_get_module_by_class("audio", "primary.vendor",
            (const hw_module_t **)&gVendorModule);

    if (rv)
        ALOGE("failed to open vendor audio primary module");
    return rv;
}

/* API functions */

static uint32_t out_get_sample_rate(const struct audio_stream *stream)
{
    return OUT_COMMON_CALL(stream, get_sample_rate);
}

static int out_set_sample_rate(struct audio_stream *stream, uint32_t rate)
{
    return OUT_COMMON_CALL(stream, set_sample_rate, rate);
}

static size_t out_get_buffer_size(const struct audio_stream *stream)
{
    return OUT_COMMON_CALL(stream, get_buffer_size);
}

static audio_channel_mask_t out_get_channels(const struct audio_stream *stream)
{
    return OUT_COMMON_CALL(stream, get_channels);
}

static audio_format_t out_get_format(const struct audio_stream *stream)
{
    return OUT_COMMON_CALL(stream, get_format);
}

static int out_set_format(struct audio_stream *stream, audio_format_t format)
{
    return OUT_COMMON_CALL(stream, set_format, format);
}

static int out_standby(struct audio_stream *stream)
{
    return OUT_COMMON_CALL(stream, standby);
}

static int out_dump(const struct audio_stream *stream, int fd)
{
    return OUT_COMMON_CALL(stream, dump, fd);
}

static int out_set_parameters(struct audio_stream *stream, const char *kvpairs)
{
    return OUT_COMMON_CALL(stream, set_parameters, kvpairs);
}

static char * out_get_parameters(const struct audio_stream *stream, const char *keys)
{
    return OUT_COMMON_CALL(stream, get_parameters, keys);
}

static uint32_t out_get_latency(const struct audio_stream_out *stream)
{
    return OUT_CALL(stream, get_latency);
}

static int out_set_volume(struct audio_stream_out *stream, float left,
                          float right)
{
    return OUT_CALL(stream, set_volume, left, right);
}

static ssize_t out_write(struct audio_stream_out *stream, const void* buffer,
                         size_t bytes)
{
    return OUT_CALL(stream, write, buffer, bytes);
}

static int out_get_render_position(const struct audio_stream_out *stream,
                                   uint32_t *dsp_frames)
{
    return OUT_CALL(stream, get_render_position, dsp_frames);
}

static int out_add_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    return OUT_COMMON_CALL(stream, add_audio_effect, effect);
}

static int out_remove_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    return OUT_COMMON_CALL(stream, remove_audio_effect, effect);
}

static int out_get_next_write_timestamp(const struct audio_stream_out *stream,
                                        int64_t *timestamp)
{
    return OUT_CALL(stream, get_next_write_timestamp, timestamp);
}

static uint32_t in_get_sample_rate(const struct audio_stream *stream)
{
    return IN_COMMON_CALL(stream, get_sample_rate);
}

static int in_set_sample_rate(struct audio_stream *stream, uint32_t rate)
{
    return IN_COMMON_CALL(stream, set_sample_rate, rate);
}

static size_t in_get_buffer_size(const struct audio_stream *stream)
{
    return IN_COMMON_CALL(stream, get_buffer_size);
}

static audio_channel_mask_t in_get_channels(const struct audio_stream *stream)
{
    return IN_COMMON_CALL(stream, get_channels);
}

static audio_format_t in_get_format(const struct audio_stream *stream)
{
    return IN_COMMON_CALL(stream, get_format);
}

static int in_set_format(struct audio_stream *stream, audio_format_t format)
{
    return IN_COMMON_CALL(stream, set_format, format);
}

static int in_standby(struct audio_stream *stream)
{
    return IN_COMMON_CALL(stream, standby);
}

static int in_dump(const struct audio_stream *stream, int fd)
{
    return IN_COMMON_CALL(stream, dump, fd);
}

static int in_set_parameters(struct audio_stream *stream, const char *kvpairs)
{
    ALOGI("crpalmer %s kvpairs %s\n", __func__, kvpairs);
    return IN_COMMON_CALL(stream, set_parameters, kvpairs);
}

static char * in_get_parameters(const struct audio_stream *stream, const char *keys)
{
    return IN_COMMON_CALL(stream, get_parameters, keys);
}

static int in_add_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    return IN_COMMON_CALL(stream, add_audio_effect, effect);
}

static int in_remove_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    return IN_COMMON_CALL(stream, remove_audio_effect, effect);
}

static int in_set_gain(struct audio_stream_in *stream, float gain)
{
    return IN_CALL(stream, set_gain, gain);
}

static ssize_t in_read(struct audio_stream_in *stream, void* buffer,
                       size_t bytes)
{
    return IN_CALL(stream, read, buffer, bytes);
}

static uint32_t in_get_input_frames_lost(struct audio_stream_in *stream)
{
    return IN_CALL(stream, get_input_frames_lost);
}

static int adev_open_output_stream(struct audio_hw_device *dev,
                                   audio_io_handle_t handle,
                                   audio_devices_t devices,
                                   audio_output_flags_t flags,
                                   struct audio_config *config,
                                   struct audio_stream_out **stream_out,
                                   const char *address)
{
    struct audio_device *adev = (struct audio_device *)dev;
    struct stream_out *out;
    int ret;

    *stream_out = NULL;

    out = (struct stream_out *)calloc(1, sizeof(struct stream_out));
    if (!out)
        return -ENOMEM;

    ret = DEVICE_CALL(dev, open_output_stream, handle, devices, flags, config, &out->vendor, address);
    if (ret) {
        free(out);
        return ret;
    }

    out->base.common.get_sample_rate = out_get_sample_rate;
    out->base.common.set_sample_rate = out_set_sample_rate;
    out->base.common.get_buffer_size = out_get_buffer_size;
    out->base.common.get_channels = out_get_channels;
    out->base.common.get_format = out_get_format;
    out->base.common.set_format = out_set_format;
    out->base.common.standby = out_standby;
    out->base.common.dump = out_dump;
    out->base.common.set_parameters = out_set_parameters;
    out->base.common.get_parameters = out_get_parameters;
    out->base.common.add_audio_effect = out_add_audio_effect;
    out->base.common.remove_audio_effect = out_remove_audio_effect;
    out->base.get_latency = out_get_latency;
    out->base.set_volume = out_set_volume;
    out->base.write = out_write;
    out->base.get_render_position = out_get_render_position;
    out->base.get_next_write_timestamp = out_get_next_write_timestamp;

    *stream_out = &out->base;

    return 0;
}

static void adev_close_output_stream(struct audio_hw_device *dev,
                                     struct audio_stream_out *stream)
{
    struct stream_out *astream = (struct stream_out *) stream;
    DEVICE_CALL(dev, close_output_stream, astream->vendor);
    free(stream);
}

static int adev_set_parameters(struct audio_hw_device *dev, const char *kvpairs)
{
    ALOGI("crpalmer %s kvpairs %s\n", __func__, kvpairs);
    return DEVICE_CALL(dev, set_parameters, kvpairs);
}

static char * adev_get_parameters(const struct audio_hw_device *dev,
                                  const char *keys)
{
    return DEVICE_CALL(dev, get_parameters, keys);
}

static int adev_init_check(const struct audio_hw_device *dev)
{
    return DEVICE_CALL(dev, init_check);
}

static int adev_set_voice_volume(struct audio_hw_device *dev, float volume)
{
    return DEVICE_CALL(dev, set_voice_volume, volume);
}

static int adev_set_master_volume(struct audio_hw_device *dev, float volume)
{
    return DEVICE_CALL(dev, set_master_volume, volume);
}

static int adev_set_mode(struct audio_hw_device *dev, audio_mode_t mode)
{
    return DEVICE_CALL(dev, set_mode, mode);
}

static int adev_set_mic_mute(struct audio_hw_device *dev, bool state)
{
    return DEVICE_CALL(dev, set_mic_mute, state);
}

static int adev_get_mic_mute(const struct audio_hw_device *dev, bool *state)
{
    return DEVICE_CALL(dev, get_mic_mute, state);
}

static size_t adev_get_input_buffer_size(const struct audio_hw_device *dev,
                                         const struct audio_config *config)
{
    return DEVICE_CALL(dev, get_input_buffer_size, config);
}

static int adev_open_input_stream(struct audio_hw_device *dev,
                                  audio_io_handle_t handle,
                                  audio_devices_t devices,
                                  struct audio_config *config,
                                  struct audio_stream_in **stream_in,
                                  audio_input_flags_t flags,
                                  const char *address,
                                  audio_source_t source)
{
    struct audio_device *adev = (struct audio_device *)dev;
    struct stream_in *in;
    int ret;

    *stream_in = NULL;

    in = (struct stream_in *)calloc(1, sizeof(struct stream_in));
    if (!in)
        return -ENOMEM;

    if (devices == AUDIO_DEVICE_IN_BACK_MIC) {
        ALOGI("crpalmer: hacking back mic => builtin mic\n");
        devices = AUDIO_DEVICE_IN_BUILTIN_MIC;
    }

    ret = DEVICE_CALL(dev, open_input_stream, handle, devices, config, &in->vendor, flags, address, source);
    if (ret) {
        free(in);
        return ret;
    }

    in->base.common.get_sample_rate = in_get_sample_rate;
    in->base.common.set_sample_rate = in_set_sample_rate;
    in->base.common.get_buffer_size = in_get_buffer_size;
    in->base.common.get_channels = in_get_channels;
    in->base.common.get_format = in_get_format;
    in->base.common.set_format = in_set_format;
    in->base.common.standby = in_standby;
    in->base.common.dump = in_dump;
    in->base.common.set_parameters = in_set_parameters;
    in->base.common.get_parameters = in_get_parameters;
    in->base.common.add_audio_effect = in_add_audio_effect;
    in->base.common.remove_audio_effect = in_remove_audio_effect;
    in->base.set_gain = in_set_gain;
    in->base.read = in_read;
    in->base.get_input_frames_lost = in_get_input_frames_lost;

    *stream_in = &in->base;
    return 0;
}

static void adev_close_input_stream(struct audio_hw_device *dev,
                                   struct audio_stream_in *stream)
{
    struct stream_in *astream = (struct stream_in *) stream;
    DEVICE_CALL(dev, close_input_stream, astream->vendor);
    free(stream);
}

static int adev_dump(const audio_hw_device_t *device, int fd)
{
    return DEVICE_CALL(device, dump, fd);
}

static int adev_close(hw_device_t *device)
{
    int ret = DEVICE_COMMON_CALL(device, close);
    free(device);
    return ret;
}

static int adev_open(const hw_module_t* module, const char* name,
                     hw_device_t** device)
{
    struct audio_device *adev;
    int ret;

    if (strcmp(name, AUDIO_HARDWARE_INTERFACE) != 0)
        return -EINVAL;

    if ((ret = check_vendor_module()) != 0)
        return ret;

    adev = calloc(1, sizeof(struct audio_device));
    if (!adev)
        return -ENOMEM;

    ret = audio_hw_device_open(gVendorModule, &adev->vendor);
    if (ret) {
        free(adev);
        return ret;
    }

    adev->base.common.tag = HARDWARE_DEVICE_TAG;
    adev->base.common.version = AUDIO_DEVICE_API_VERSION_2_0;
    adev->base.common.module = (struct hw_module_t *) module;
    adev->base.common.close = adev_close;

    adev->base.init_check = adev_init_check;
    adev->base.set_voice_volume = adev_set_voice_volume;
    adev->base.set_master_volume = adev_set_master_volume;
    adev->base.set_mode = adev_set_mode;
    adev->base.set_mic_mute = adev_set_mic_mute;
    adev->base.get_mic_mute = adev_get_mic_mute;
    adev->base.set_parameters = adev_set_parameters;
    adev->base.get_parameters = adev_get_parameters;
    adev->base.get_input_buffer_size = adev_get_input_buffer_size;
    adev->base.open_output_stream = adev_open_output_stream;
    adev->base.close_output_stream = adev_close_output_stream;
    adev->base.open_input_stream = adev_open_input_stream;
    adev->base.close_input_stream = adev_close_input_stream;
    adev->base.dump = adev_dump;
    adev->base.set_master_mute = NULL;
    adev->base.get_master_mute = NULL;

    *device = &adev->base.common;

    return 0;
}

static struct hw_module_methods_t hal_module_methods = {
    .open = adev_open,
};

struct audio_module HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = AUDIO_MODULE_API_VERSION_0_1,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = AUDIO_HARDWARE_MODULE_ID,
        .name = "JA audio HW HAL",
        .author = "The CyanogenMod Project",
        .methods = &hal_module_methods,
    },
};
