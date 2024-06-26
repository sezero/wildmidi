/*
 * out_wave.c -- WAVE output
 *
 * Copyright (C) WildMidi Developers 2020
 *
 * This file is part of WildMIDI.
 *
 * WildMIDI is free software: you can redistribute and/or modify the player
 * under the terms of the GNU General Public License and you can redistribute
 * and/or modify the library under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either version 3 of
 * the licenses, or(at your option) any later version.
 *
 * WildMIDI is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License and
 * the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License and the
 * GNU Lesser General Public License along with WildMIDI.  If not,  see
 * <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "wildplay.h"

static uint32_t wav_size;
static FILE *out_wav;

static int open_wav_output(const char *output, unsigned int *r) {
    const unsigned int rate = *r;
    uint8_t wav_hdr[] = {
        0x52, 0x49,
        0x46, 0x46, /* "RIFF"  */
        0x00, 0x00,
        0x00, 0x00, /* riffsize: pcm size + 36 (filled when closing.) */
        0x57, 0x41,
        0x56, 0x45, /* "WAVE"  */
        0x66, 0x6D,
        0x74, 0x20, /* "fmt "  */
        0x10, 0x00,
        0x00, 0x00, /* length of this RIFF block: 16  */
        0x01, 0x00, /* wave format == 1 (WAVE_FORMAT_PCM)  */
        0x02, 0x00, /* channels == 2  */
        0x00, 0x00,
        0x00, 0x00, /* sample rate (filled below)  */
        0x00, 0x00,
        0x00, 0x00, /* bytes_per_sec: rate * channels * format bytes  */
        0x04, 0x00, /* block alignment: channels * format bytes == 4  */
        0x10, 0x00, /* format bits == 16  */
        0x64, 0x61,
        0x74, 0x61, /* "data"  */
        0x00, 0x00,
        0x00, 0x00 /* datasize: the pcm size (filled when closing.)  */
    };

    if (output[0] == '\0') {
        fprintf(stderr, "Output file is not defined\r\n");
        return (-1);
    }

    out_wav = fopen(output, "wb");
    if (!out_wav) {
        fprintf(stderr, "Error: unable to open file for writing (%s)\r\n",
                strerror(errno));
        return (-1);
    } else {
        uint32_t bytes_per_sec;

        wav_hdr[24] = (rate) & 0xFF;
        wav_hdr[25] = (rate >> 8) & 0xFF;

        bytes_per_sec = rate * 4;
        wav_hdr[28] = (bytes_per_sec) & 0xFF;
        wav_hdr[29] = (bytes_per_sec >> 8) & 0xFF;
        wav_hdr[30] = (bytes_per_sec >> 16) & 0xFF;
        wav_hdr[31] = (bytes_per_sec >> 24) & 0xFF;
    }

    if (fwrite(wav_hdr, 1, 44, out_wav) != 44) {
        fprintf(stderr, "ERROR: failed writing wav header (%s)\r\n",
                strerror(errno));
        fclose(out_wav);
        out_wav = NULL;
        return (-1);
    }

    wav_size = 0;

    return (0);
}

static int write_wav_output(void *output_data, int output_size) {
#ifdef WORDS_BIGENDIAN
    /* libWildMidi outputs host-endian, *.wav must have little-endian. */
    uint16_t *swp = (uint16_t *)output_data;
    int i = (output_size / 2) - 1;
    for (; i >= 0; --i) {
        swp[i] = (swp[i] << 8) | (swp[i] >> 8);
    }
#endif
    if (output_size < 0) return (-1);
    if (fwrite(output_data, 1, output_size, out_wav) != (size_t)output_size) {
        fprintf(stderr, "\nERROR: failed writing wav (%s)\r\n",
              strerror(errno));
        fclose(out_wav);
        out_wav = NULL;
        return (-1);
    }

    wav_size += output_size;
    return (0);
}

static void close_wav_output(void) {
    uint8_t wav_count[4];
    if (!out_wav)
        return;

    printf("Finishing and closing wav output\r");
    wav_count[0] = (wav_size) & 0xFF;
    wav_count[1] = (wav_size >> 8) & 0xFF;
    wav_count[2] = (wav_size >> 16) & 0xFF;
    wav_count[3] = (wav_size >> 24) & 0xFF;
    fseek(out_wav, 40, SEEK_SET);
    if (fwrite(wav_count, 1, 4, out_wav) != 4) {
        fprintf(stderr, "\nERROR: failed writing wav (%s)\r\n",
                strerror(errno));
        goto end;
    }

    wav_size += 36;
    wav_count[0] = (wav_size) & 0xFF;
    wav_count[1] = (wav_size >> 8) & 0xFF;
    wav_count[2] = (wav_size >> 16) & 0xFF;
    wav_count[3] = (wav_size >> 24) & 0xFF;
    fseek(out_wav, 4, SEEK_SET);
    if (fwrite(wav_count, 1, 4, out_wav) != 4) {
        fprintf(stderr, "\nERROR: failed writing wav (%s)\r\n",
                strerror(errno));
        goto end;
    }

end:
    printf("\n");
    if (!out_wav) {
        fclose(out_wav);
    }
    out_wav = NULL;
}

static void pause_wav_output(void) {}
static void resume_wav_output(void) {}

audiodrv_info audiodrv_wave = {
    "wave",
    "Save stream to WAVE file",
    open_wav_output,
    write_wav_output,
    close_wav_output,
    pause_wav_output,
    resume_wav_output
};
