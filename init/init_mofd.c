/*
   Copyright (c) 2013, The Linux Foundation. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <private/android_filesystem_config.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

#define SOC_MAX_FREQ_PATH  "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"
#define MAX1DOT3    1333000
#define MAX1DOT8    1833000
#define MAX2DOT3    2333000

#define BUF_SIZE         64
static char tmp[BUF_SIZE];

static unsigned long max_freq;

int read_file2(const char *fname, char *data, int max_size)
{
    int fd, rc;

    if (max_size < 1)
        return 0;

    fd = open(fname, O_RDONLY);
    if (fd < 0) {
        ERROR("failed to open '%s'\n", fname);
        return 0;
    }

    rc = read(fd, data, max_size - 1);
    if ((rc > 0) && (rc < max_size))
        data[rc] = '\0';
    else
        data[0] = '\0';
    close(fd);

    return 1;
}

void vendor_load_properties()
{
    int rc;

    /* Collect cpu info */
    rc = read_file2(SOC_MAX_FREQ_PATH, tmp, sizeof(tmp));
    if (rc) {
        max_freq = strtoul(tmp, NULL, 0);
    }
    if (!max_freq) {
        /* abort */
        ERROR("Max Freq detection failed, skipping interactive initialization\n");
        return;
    }

    ERROR("Detected max_cpu_freq=%lu\n",max_freq);

    switch (max_freq){
        case MAX1DOT3:
            property_set("ro.sys.perf.device.powersave","1083000");
            property_set("ro.sys.perf.device.touchboost","1250000");
            property_set("ro.sys.perf.device.full","1333000");
            break;
        case MAX1DOT8:
            property_set("ro.sys.perf.device.powersave","1250000");
            property_set("ro.sys.perf.device.touchboost","1500000");
            property_set("ro.sys.perf.device.full","1833000");
            break;
        case MAX2DOT3:
        default:
            property_set("ro.sys.perf.device.powersave","1500000");
            property_set("ro.sys.perf.device.touchboost","1833000");
            property_set("ro.sys.perf.device.full","2333000");
            break;
    }
}
