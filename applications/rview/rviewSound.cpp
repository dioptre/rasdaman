/*
* This file is part of rasdaman community.
*
* Rasdaman community is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Rasdaman community is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
*
* Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
rasdaman GmbH.
*
* For more information please see <http://www.rasdaman.org>
* or contact Peter Baumann via <baumann@rasdaman.com>.
/

/**
 *  PURPOSE:
 *
 *  rView sound ``viewer'' for playing back MDD as audio samples.
 *  This file consists of two parts: the highly platform dependent
 *  SoundPlayer class providing the actual low-level sound playback
 *  and the general purpose wxWindows class rviewSoundPlayer that
 *  implements a typical rView display mode window and playback
 *  controls.
 *  The SoundPlayer class currently has implementations for Solaris
 *  and WindowsNT. Supporting other Unix brands only requires
 *  changes to SoundPlayer::configureDevice, supporting other
 *  platforms is considerably more complicated (see Unix vs. NT
 *  implementations).
 *
 *  COMMENTS:
 *      None
 */



// Standard wxWindows preamble.
#ifdef __GNUG__
#pragma implementation
#endif

#ifndef __HAL_ONLY__
// changed in wxWindows 2.4.2:
//#include "wx_prec.h"
#include <wx/wxprec.h>


#ifdef __BORLANDC__
#pragma hdrstop
#endif


#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#endif // HAL


/* All platforms */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <math.h>

#include <iostream.h>

/* Platform class (e.g. Unix) */
#ifndef __VISUALC__

#include <unistd.h>
#include <fcntl.h>

/* Platform specific */
#ifdef __sun
#include <multimedia/libaudio.h>
#endif

#endif


#ifndef __HAL_ONLY__
#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif

#include "raslib/rmdebug.hh"
#include "raslib/primitivetype.hh"
#include "raslib/minterval.hh"
//#include "rasodmg/odmgtypes.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/ref.hh"

#endif // HAL

#include "rviewSound.hh"
#ifndef __HAL_ONLY__
#include "rviewTypes.hh"
#include "rviewPrefs.hh"
#include "labelManager.hh"
#endif // HAL




/* Minimum size of ulaw table */
const int ULAW_LD_TABLE_MIN=10;
/* log2 of the length of a linear segment in ulaw coding */
const int ULAW_LD_LINEAR_SIZE=7;

// general debug messages (should only enable on Unix)
//#define SOUND_PLAYER_DEBUG
// Debug output to file?
//#define SOUND_DEBUG_FILE "Z:\\dehmel\\rview\\slog"



const int rviewSoundPlayer::sound_bwidth = 50;
const int rviewSoundPlayer::sound_bheight = 30;
const int rviewSoundPlayer::sound_sheight = 50;
const int rviewSoundPlayer::sound_twidth = 120;
const int rviewSoundPlayer::sound_theight = 50;
const int rviewSoundPlayer::sound_cwidth = 120;
const int rviewSoundPlayer::sound_cheight = 30;
const int rviewSoundPlayer::sound_ctrly = rviewSoundPlayer::sound_bheight + rviewSoundPlayer::sound_theight + rviewSoundPlayer::sound_sheight + 5*rviewDisplay::display_border;
const int rviewSoundPlayer::sound_width = rviewDisplay::display_width;
const int rviewSoundPlayer::sound_height = rviewDisplay::display_cheight + rviewSoundPlayer::sound_ctrly + 2*rviewDisplay::display_border;
const int rviewSoundPlayer::sound_latencies = 11;




/* Global variables and functions */
static soundPlayer *activePlayer = NULL;
static int inCallback = 0;

#ifdef SOUND_DEBUG_FILE
static FILE *debugfd=NULL;
#endif


#ifdef __VISUALC__
VOID CALLBACK handle_sound_timer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
#ifdef SOUND_PLAYER_DEBUG
    cout << "timer called, time " << dwTime << endl;
#endif

    if ((activePlayer != NULL) && (inCallback == 0))
    {
        inCallback = 1;
        activePlayer->writeSamples(dwTime);
        inCallback = 0;
    }
}

#else

static void handle_sound_timer(int signal)
{
#ifdef SOUND_PLAYER_DEBUG
    cout << "timer called " << signal << ", player " << (void*)activePlayer << ", inCallback " << inCallback << endl;
#endif

    if ((activePlayer != NULL) && (inCallback == 0))
    {
        inCallback = 1;
        activePlayer->writeSamples();
        inCallback = 0;
    }
}
#endif




/*
 *  soundPlayer: low-level sound driver, hardware abstraction layer
 */

soundPlayer::soundPlayer(void)
{
    setupVariables();
}

soundPlayer::soundPlayer(int frq, int ch, FILE *fp, rviewSoundFormat fmt, int lat)
{
    setupVariables();
    newSample(frq, ch, fp, fmt, lat);
}

soundPlayer::soundPlayer(int frq, int ch, const signed char *data, int len, rviewSoundFormat fmt, int lat)
{
    setupVariables();
    newSample(frq, ch, data, len, fmt, lat);
}

soundPlayer::soundPlayer(int frq, int ch, const unsigned char *data, int len, rviewSoundFormat fmt, int lat)
{
    setupVariables();
    newSample(frq, ch, data, len, fmt, lat);
}

soundPlayer::soundPlayer(int frq, int ch, const short *data, int len, rviewSoundFormat fmt, int lat)
{
    setupVariables();
    newSample(frq, ch, data, len, fmt, lat);
}

soundPlayer::~soundPlayer(void)
{
    playbackStop();
    if (buffer != NULL) delete [] buffer;
    if (convBuff != NULL) delete [] convBuff;
    if (LinToUlaw != NULL) delete [] LinToUlaw;
    if (UlawToLin != NULL) delete [] UlawToLin;
}

void soundPlayer::setupVariables(void)
{
    buffer = NULL;
    convBuff = NULL;
    inData = NULL;
    sampleFile = NULL;
    suspendedPlayer = NULL;
    LinToUlaw = NULL;
    UlawToLin = NULL;
    timerActive = FALSE;
    loopMode = 0;
#ifdef __VISUALC__
    waveOut = (HWAVEOUT)0;
    timerID = 0;
    for (int i=0; i<RVIEW_SND_BUFFERS; i++) waveHdrs[i].lpData = NULL;
#else
    audioDevice = -1;
#endif

#ifdef SOUND_DEBUG_FILE
    if (debugfd == NULL) debugfd = fopen(SOUND_DEBUG_FILE, "w");
#endif
}

void soundPlayer::playbackStop(void)
{
#ifdef SOUND_PLAYER_DEBUG
    cout << "soundPlayer::playbackStop" << endl;
#endif

    stopTimer();

#ifdef __VISUALC__
    if (waveOut != (HWAVEOUT)0)
    {
        // Reset is CRITICALLY important here! Otherwise playback won't work again
        // until you quit and restart your app.
        waveOutReset(waveOut);
        waveOutClose(waveOut);
        waveOut = (HWAVEOUT)0;
    }
    freeWaveHeaders();
#else
    if (audioDevice != -1)
    {
        close(audioDevice);
        audioDevice = -1;
    }
#endif
    inData = NULL;
    sampleFile = NULL;
}

int soundPlayer::newSample(int frq, int ch, FILE *fp, rviewSoundFormat fmt, int lat)
{
    if (configureDevice(frq, ch, -1, fmt, lat) != 0) return -1;
    sampleFile = fp;
    return 0;
}

int soundPlayer::newSample(int frq, int ch, const signed char *data, int len, rviewSoundFormat fmt, int lat)
{
    if (configureDevice(frq, ch, len, fmt, lat) != 0) return -1;
    inData = (char*)data;
    return 0;
}

int soundPlayer::newSample(int frq, int ch, const unsigned char *data, int len, rviewSoundFormat fmt, int lat)
{
    if (configureDevice(frq, ch, len, fmt, lat) != 0) return -1;
    inData = (char*)data;
    return 0;
}

int soundPlayer::newSample(int frq, int ch, const short *data, int len, rviewSoundFormat fmt, int lat)
{
    if (configureDevice(frq, ch, len, fmt, lat) != 0) return -1;
    inData = (char*)data;
    return 0;
}

int soundPlayer::playbackActive(void)
{
    if ((sampleFile != NULL) || (inData != NULL)) return dataOffset;

    return -1;
}

int soundPlayer::playbackGetOffset(void)
{
    if ((sampleFile == NULL) && (inData == NULL)) return -1;
    return dataOffset;
}

void soundPlayer::ensureUlawTable(int ulawsize)
{
    int i, j, shift, range, sign, base, vsh;
    unsigned char val;
    unsigned char *u;

    if (LinToUlaw != NULL)
    {
        if (ldUlawSize >= ulawsize) return;
        delete [] LinToUlaw;
    }
    if (ulawsize < ULAW_LD_TABLE_MIN) ldUlawSize = ULAW_LD_TABLE_MIN;
    else ldUlawSize = ulawsize;
    LinToUlaw = new unsigned char[(1<<ldUlawSize)];

    range = 255 << (ULAW_LD_LINEAR_SIZE + ldUlawSize - 16);
    u = LinToUlaw + (1<<(ldUlawSize-1)) - range;

    for (i=-8; i<8; i++)
    {
        if (i < 0)
        {
            shift = -1-i;
            sign = -1;
            base = 127 - 16*shift - 15;
        }
        else
        {
            shift = i;
            sign = 1;
            base = 255 - 16*shift;
        }
        vsh = ULAW_LD_LINEAR_SIZE + ldUlawSize + shift - 20;
        if (vsh < 0)
        {
            vsh = -vsh;
            for (j=0; j<(1<<(ULAW_LD_LINEAR_SIZE + ldUlawSize + shift - 16)); j++)
            {
                val = j << vsh;
                val = base - sign * val;
                *u++ = val;
            }
        }
        else
        {
            for (j=0; j<(1<<(ULAW_LD_LINEAR_SIZE + ldUlawSize + shift - 16)); j++)
            {
                val = j >> vsh;
                val = base - sign * val;
                *u++ = val;
            }
        }
    }
    i = (1<<(ldUlawSize-1)) - range;
    memset(LinToUlaw, LinToUlaw[i], i-1);
    memset(LinToUlaw + (1<<ldUlawSize) - i, LinToUlaw[(1<<ldUlawSize) - i - 1], i);

    /*for (i=0; i<(1<<(ldUlawSize-4)); i++)
    {
      printf("%4x: ", i*16);
      for (j=0; j<16; j++)
      {
        printf("%2x ", LinToUlaw[16*i + j]);
      }
      printf("\n");
    }*/
}

void soundPlayer::ensureLinearTable(void)
{
    int i, j;
    short *data;

    if (UlawToLin != NULL) return;

    UlawToLin = new short[256];
    data = UlawToLin;

    for (i=0; i<128; i++)
    {
        j = ((127 - i) >> 4);
        *data++ = -((((1 << j) - 1) << ULAW_LD_LINEAR_SIZE) + ((16-(i&15)) << (j + ULAW_LD_LINEAR_SIZE - 4)));
    }
    for (i=128; i<256; i++)
    {
        j = ((255 - i) >> 4);
        *data++ = ((((1 << j) -1) << ULAW_LD_LINEAR_SIZE) + ((15-(i&15)) << (j + ULAW_LD_LINEAR_SIZE - 4)));
    }

    /*for (i=0; i<32; i++)
    {
      printf("%4x: ", i*8);
      for (j=0; j<8; j++)
      {
        printf("%6d ", UlawToLin[8*i + j]);
      }
      printf("\n");
    }*/
}

char *soundPlayer::ensureConvBuff(int size)
{
    if (convBuff == NULL)
    {
        convBuff = new char [size];
        cbuffSize = size;
    }
    else
    {
        if (cbuffSize < size)
        {
            delete [] convBuff;
            convBuff = new char [size];
            cbuffSize = size;
        }
    }
    return convBuff;
}

char *soundPlayer::ensureSampleBuff(int size)
{
    if (buffer == NULL)
    {
        buffer = new char[size];
        buffSize = size;
    }
    else
    {
        if (buffSize < size)
        {
            delete [] buffer;
            buffer = new char[size];
            buffSize = size;
        }
    }
    return buffer;
}

const char *soundPlayer::ensureSamplesForDevice(const char *source, int len)
{
    int needSize;
    int needSamples;
    int i;

#ifdef SOUND_PLAYER_DEBUG
    cout << "soundPlayer::ensureSamplesForDevice" << endl;
#endif

    if (format == devFormat) return source;

    needSamples = len * channels;
    needSize = len * devSampSize;

    ensureConvBuff(needSize);

    // Device is 8 bit signed linear (mean at 0)
    if (devFormat == rsf_lin8)
    {
        signed char *dest = (signed char*)convBuff;

        if (format == rsf_ulin8)
        {
            const unsigned char *src = (const unsigned char*)source;

            for (i=0; i<needSamples; i++) dest[i] = (signed char)(src[i] - 0x80);
        }
        else if (format == rsf_ulaw8)
        {
            const unsigned char *src = (const unsigned char*)source;

            ensureLinearTable();
            for (i=0; i<needSamples; i++) dest[i] = (signed char)(UlawToLin[src[i]] >> 8);
        }
        else if (format == rsf_lin16)
        {
            const short *src = (const short *)source;

            for (i=0; i<needSamples; i++) dest[i] = (signed char)(src[i] >> 8);
        }
    }
    // Device is 8 bit unsigned linear (mean at 0x80)
    else if (devFormat == rsf_ulin8)
    {
        unsigned char *dest = (unsigned char*)convBuff;

        if (format == rsf_lin8)
        {
            const signed char *src = (const signed char*)source;

            for (i=0; i<needSamples; i++) dest[i] = (unsigned char)(src[i]) + 0x80;
        }
        else if (format == rsf_ulaw8)
        {
            const unsigned char *src = (const unsigned char*)source;

            ensureLinearTable();
            for (i=0; i<needSamples; i++) dest[i] = (unsigned char)((UlawToLin[src[i]] >> 8) + 0x80);
        }
        else if (format == rsf_lin16)
        {
            const short *src = (const short*)source;

            for (i=0; i<needSamples; i++) dest[i] = (short)((src[i] >> 8) + 0x80);
        }
    }
    // Device is 8 bit ulaw
    else if (devFormat == rsf_ulaw8)
    {
        unsigned char *dest = (unsigned char*)convBuff;

        if (format == rsf_lin8)
        {
            const signed char *src = (const signed char*)source;
            short val;

            ensureUlawTable(8);

            for (i=0; i<needSamples; i++)
            {
                val = (src[i] << 8);
                dest[i] = LinToUlaw[(1<<(ldUlawSize-1)) + (val >> (16 - ldUlawSize))];
            }
        }
        else if (format == rsf_ulin8)
        {
            const unsigned char *src = (const unsigned char*)source;
            short val;

            ensureUlawTable(8);

            for (i=0; i<needSamples; i++)
            {
                val = (src[i] << 8) - 0x8000;
                dest[i] = LinToUlaw[(1<<(ldUlawSize-1)) + (val >> (16 - ldUlawSize))];
            }
        }
        else if (format == rsf_lin16)
        {
            const short *src = (const short*)source;
            short val;

            ensureUlawTable(14);

            for (i=0; i<needSamples; i++)
            {
                val = src[i];
                dest[i] = LinToUlaw[(1<<(ldUlawSize-1)) + (val >> (16 - ldUlawSize))];
            }
        }
    }
    // Device is 16 bit linear
    else if (devFormat == rsf_lin16)
    {
        short *dest = (short*)convBuff;

        if (format == rsf_lin8)
        {
            const signed char *src = (const signed char*)source;

            for (i=0; i<needSamples; i++) dest[i] = (short)(src[i] << 8);
        }
        else if (format == rsf_ulin8)
        {
            const unsigned char *src = (const unsigned char*)source;

            for (i=0; i<needSamples; i++) dest[i] = (short)((src[i] << 8) - 0x8000);
        }
        else if (format == rsf_ulaw8)
        {
            const unsigned char *src = (const unsigned char *)source;

            ensureLinearTable();
            for (i=0; i<needSamples; i++) dest[i] = (short)(UlawToLin[src[i]]);
        }
    }

    return convBuff;
}

const char *soundPlayer::ensureSamples(int &num)
{
    const char *result;
    char *b;

    if (sampleFile != NULL)
    {
        ensureSampleBuff(num * sampleSize);
#ifdef SOUND_PLAYER_DEBUG
        cout << "read " << num << " samples from " << (void*)sampleFile << endl;
#endif
        if (loopMode == 0)
        {
            num = fread(buffer, sampleSize, num, sampleFile);
            dataOffset += num;
        }
        else
        {
            int total, dsize;

            b = buffer;
            total = num;
            while (total > 0)
            {
                dsize = fread(b, sampleSize, total, sampleFile);
                dataOffset += dsize;
                if (dsize < total)
                {
                    fseek(sampleFile, 0, SEEK_SET);
                    dataOffset = 0;
                }
                b += dsize * sampleSize;
                total -= dsize;
            }
        }
        if (handleOutOfData(num) != 0)
        {
            sampleFile = NULL;
            return NULL;
        }
        result = ensureSamplesForDevice(buffer, num);
    }
    else if (inData != NULL)
    {
        if (loopMode == 0)
        {
            if (num + dataOffset > inLength) num = inLength - dataOffset;
            result = (char*)inData + dataOffset * sampleSize;
            dataOffset += num;
        }
        else
        {
            int total, dsize;

            ensureSampleBuff(num * sampleSize);
            b = buffer;
            total = num;
            while (total > 0)
            {
                dsize = inLength - dataOffset;
                if (dsize > total) dsize = total;
                memcpy(b, inData + dataOffset * sampleSize, dsize * sampleSize);
                dataOffset += dsize;
                if (dataOffset >= inLength) dataOffset = 0;
                b += dsize * sampleSize;
                total -= dsize;
            }
            result = buffer;
        }
        if (handleOutOfData(num) != 0)
        {
            inData = NULL;
            return NULL;
        }
        result = ensureSamplesForDevice(result, num);
    }
    else
    {
        playbackStop();
        result = NULL;
        num = 0;
    }

    return result;
}

int soundPlayer::handleOutOfData(int dataSize)
{
    // If there is data, do nothing.
    if (dataSize > 0) return 0;

#ifdef __VISUALC__
    // On Windows we have to make sure both buffers are played to the end.
    // Since the new one is all empty that leaves only the current one,
    // therefore delay by 1 buffer.
    if (emptyBuffers < 1)
    {
        emptyBuffers++;
        return 0;
    }
#endif
    // On Unix we can stop immediately
    playbackStop();
    return 1;
}

int soundPlayer::playbackSetPosition(int position)
{
    int oldPosition = -1;

    oldPosition = dataOffset;

    if (position >= 0)
    {
        if (sampleFile != NULL)
        {
            fseek(sampleFile, position, SEEK_SET);
            dataOffset = (int)ftell(sampleFile);
        }
        else
        {
            dataOffset = (position < inLength) ? position : inLength-1;
        }
    }
    return oldPosition;
}

int soundPlayer::playbackLoopMode(int lpMode)
{
    int oldLoop = loopMode;

    loopMode = lpMode;

    return oldLoop;
}


#ifdef __VISUALC__
void soundPlayer::freeWaveHeaders(void)
{
    int i;

    for (i=0; i<RVIEW_SND_BUFFERS; i++)
    {
        if (waveHdrs[i].lpData != NULL)
        {
            waveOutUnprepareHeader(waveOut, waveHdrs + i, sizeof(WAVEHDR));
            delete [] waveHdrs[i].lpData;
            waveHdrs[i].lpData = NULL;
        }
    }
}
#endif

int soundPlayer::configureDevice(int frq, int ch, int len, rviewSoundFormat fmt, int lat)
{
    // only allow 1 active player at a time.
    if ((activePlayer != NULL) && (activePlayer != this))
    {
        cerr << "Another player is already active" << endl;
        return -1;
    }

    playbackStop();

    frequency = frq;
    latency = lat;
    channels = ch;
    format = fmt;
    inLength = len;
    samplesWritten = 0;
    dataOffset = 0;
    samplesWriteahead = (frequency * latency) / 1000;

#ifdef __VISUALC__
    devFormat = rsf_lin16;
#else
#ifdef __sun
    if ((audioDevice = open("/dev/audio", O_WRONLY)) != -1)
    {
        Audio_hdr head;

        if (fmt == rsf_ulaw8)
        {
            head.encoding = AUDIO_ENCODING_ULAW;
            devFormat = rsf_ulaw8;
            head.bytes_per_unit = 1;
        }
        else
        {
            head.encoding = AUDIO_ENCODING_LINEAR;
            devFormat = rsf_lin16;
            head.bytes_per_unit = 2;
        }
        head.sample_rate = frequency;
        head.channels = channels;
        head.samples_per_unit = 1;
        head.data_size = AUDIO_UNKNOWN_SIZE;
        head.endian = AUDIO_ENDIAN_BIG;
        audio_set_play_config(audioDevice, &head);
    }
#endif
    if (audioDevice == -1)
    {
        cerr << "Unable to initialize audio device!" << endl;
        return -1;
    }
#endif

    switch (format)
    {
    case rsf_lin8:
    case rsf_ulin8:
    case rsf_ulaw8:
        sampleSize = 1;
        break;
    case rsf_lin16:
        sampleSize = 2;
        break;
    default:
        sampleSize = 0;
        break;
    }
    sampleSize *= channels;
    switch (devFormat)
    {
    case rsf_lin8:
    case rsf_ulin8:
    case rsf_ulaw8:
        devSampSize = 1;
        break;
    case rsf_lin16:
        devSampSize = 2;
        break;
    default:
        devSampSize = 0;
    }
    devSampSize *= channels;

#ifdef SOUND_PLAYER_DEBUG
    cout << "sampleSize " << sampleSize << ", devSampSize " << devSampSize << ", writeahead " << samplesWriteahead << endl;
#endif

#ifdef __VISUALC__
    waveFmt.wFormatTag = WAVE_FORMAT_PCM;
    waveFmt.nChannels = channels;
    waveFmt.nSamplesPerSec = frequency;
    waveFmt.nAvgBytesPerSec = 2*channels*frequency;
    waveFmt.nBlockAlign = 2*channels;
    waveFmt.wBitsPerSample = 16*channels;
    waveFmt.cbSize = 0;

    int i;

    if ((i = waveOutOpen(&waveOut, WAVE_MAPPER, &waveFmt, 0, 0, WAVE_FORMAT_QUERY)) != MMSYSERR_NOERROR)
    {
        cerr << "Unable to initialize audio device! (" << i << ")" << endl;
        return -1;
    }
    waveOutOpen(&waveOut, WAVE_MAPPER, &waveFmt, 0, 0, CALLBACK_NULL | WAVE_FORMAT_DIRECT);

    freeWaveHeaders();

    for (i=0; i<RVIEW_SND_BUFFERS; i++)
    {
        waveHdrs[i].lpData = (LPSTR)(new char[samplesWriteahead * devSampSize]);
        waveHdrs[i].dwBufferLength = (DWORD)(samplesWriteahead * devSampSize);
        //waveOutPrepareHeader(waveOut, waveHdrs + i, sizeof(WAVEHDR));
        // Init flags to "done" for writeSamples
        waveHdrs[i].dwFlags = WHDR_DONE;
    }
    currentHeader = 0;
    emptyBuffers = 0;

#endif

    return startTimer();
}


void soundPlayer::playbackSuspend(void)
{
    stopTimer(0);
#ifdef __VISUALC__
    // NT!!! If you issue these calls playback gets _seriously_ screwed up later on.
    //waveOutPause(waveOut);
#endif
}


void soundPlayer::playbackResume(void)
{
    samplesWritten = 0;
#ifdef __VISUALC__
    //waveOutRestart(waveOut);
#endif
    startTimer(0);
}


#ifdef __VISUALC__
// VISUALC
int soundPlayer::setTimerInterval(unsigned int ti)
{
    if (ti == 0)
    {
        if (timerID != 0)
        {
            if (!KillTimer(NULL, timerID)) return 1;
            timerID = 0;
        }
    }
    else
    {
        // ti is in us!
        if (timerID == 0)
        {
            timerID = SetTimer(NULL, 1, (UINT)ti/1000, (TIMERPROC)handle_sound_timer);
            if (timerID == 0) return 1;
        }
    }

    timerActive = (ti != 0);

    return 0;
}

#else
// UNIX only
int soundPlayer::setTimerInterval(unsigned int ti)
{
    struct itimerval value;
    struct sigaction act;
    int res;

    act.sa_handler = handle_sound_timer;
    act.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &act, &oact);
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_usec = ti;
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = ti;
    res = setitimer(ITIMER_REAL, &value, &ovalue);

    timerActive = (ti != 0);

    return res;
}
#endif


int soundPlayer::startTimer(int ap)
{
    unsigned int period;

#ifdef SOUND_PLAYER_DEBUG
    cout << "soundPlayer::startTimer" << endl;
#endif

    if (timerActive) return -1;

    if (ap != 0)
    {
        if ((activePlayer != this) && (activePlayer != NULL))
        {
            activePlayer->playbackSuspend();
            suspendedPlayer = activePlayer;
        }
    }
    activePlayer = this;
#ifdef __VISUALC__
    lastSyncTime = GetTickCount();
#else
    struct timezone tzp;
    gettimeofday(&lastSyncTime, &tzp);
#endif

    // period in us
    period = (unsigned int)(1000 * latency * RVIEW_SND_RELPERIOD);
    if (setTimerInterval(period) != 0)
    {
        cerr << "Unable to set up timer!" << endl;
        return -1;
    }

#ifdef SOUND_PLAYER_DEBUG
    cout << "started timer with period " << period << endl;
#endif

    return 0;
}

int soundPlayer::stopTimer(int ap)
{
    if (!timerActive) return -1;

    setTimerInterval(0);
    timerActive = FALSE;

    activePlayer = NULL;
    if (ap != 0)
    {
        if (suspendedPlayer != NULL)
        {
            activePlayer = suspendedPlayer;
            suspendedPlayer = NULL;
            activePlayer->playbackResume();
        }
    }
    return 0;
}

#ifdef __VISUALC__
void soundPlayer::writeSamples(DWORD systime)
#else
void soundPlayer::writeSamples(void)
#endif
{
    int samplesNeeded;
#ifdef __VISUALC__
    samplesNeeded = ((systime - lastSyncTime) * frequency) / 1000;
#else
    struct timeval tp;
    struct timezone tzp;
    float usecs;

    gettimeofday(&tp, &tzp);
    usecs = (float)(tp.tv_usec - lastSyncTime.tv_usec);
    samplesNeeded = (tp.tv_sec - lastSyncTime.tv_sec) * frequency + (int)((usecs * frequency) / 1e6);
#endif

#ifdef SOUND_PLAYER_DEBUG
    cout << "soundPlayer::writeSamples" << endl;
#endif

#ifdef SOUND_DEBUG_FILE
    fprintf(debugfd, "%d: written: %d, needed: %d, deltawa: %d\n", systime, samplesWritten, samplesNeeded, samplesWritten - samplesNeeded - samplesWriteahead);
#endif

    if ((samplesWritten - samplesNeeded) >= samplesWriteahead)
    {
#ifdef SOUND_PLAYER_DEBUG
        cout << "soundPlayer: stalled" << endl;
#endif
        samplesWritten = (samplesWritten - samplesNeeded);
#ifdef __VISUALC__
        lastSyncTime = systime;
#else
        lastSyncTime.tv_sec = tp.tv_sec;
        lastSyncTime.tv_usec = tp.tv_usec;
#endif

#ifdef SOUND_DEBUG_FILE
        fprintf(debugfd, "stalled\n");
#endif
    }
    else
    {
        int num;

#ifdef SOUND_PLAYER_DEBUG
        cout << "written " << samplesWritten << ", needed " << samplesNeeded << endl;
#endif

        // Underflow?
        if ((samplesWritten - samplesNeeded) < 0)
        {
            samplesWritten = samplesNeeded;
#ifdef SOUND_DEBUG_FILE
            fprintf(debugfd, "underflow\n");
#endif
        }
        //num = samplesWritten - samplesNeeded;
        num = samplesWriteahead;
        if (num != 0)
        {
            const char *newSamples;

#ifdef SOUND_PLAYER_DEBUG
            cout << "samples " << num << ", device " << endl;
#endif
            if ((newSamples = ensureSamples(num)) != NULL)
            {
#ifdef __VISUALC__
                int bsize = waveHdrs[currentHeader].dwBufferLength;

#ifdef SOUND_DEBUG_FILE
                fprintf(debugfd, "need %d, buff %d, flags %x\n", num, bsize, waveHdrs[currentHeader].dwFlags);
#endif
                // If the buffer is still being used we do nothing.
                if ((waveHdrs[currentHeader].dwFlags & (WHDR_INQUEUE | WHDR_DONE)) != WHDR_DONE) return;
#ifdef SOUND_DEBUG_FILE
                fprintf(debugfd, "fill Buffer %d\n", currentHeader);
#endif
                //waveOutUnprepareHeader(waveOut, waveHdrs + currentHeader, sizeof(WAVEHDR));
                // must prepare the header here or no playback.
                //waveHdrs[currentHeader].dwFlags = 0;
                waveOutPrepareHeader(waveOut, waveHdrs + currentHeader, sizeof(WAVEHDR));
                memcpy(waveHdrs[currentHeader].lpData, newSamples, num * devSampSize);
                // Fill rest of buffer with the correct 0 value
                if (num * devSampSize != bsize)
                {
                    int fillValue = 0;

                    memset(waveHdrs[currentHeader].lpData + num * devSampSize, fillValue, bsize - num * devSampSize);
                }
                waveOutWrite(waveOut, waveHdrs + currentHeader, sizeof(WAVEHDR));
                currentHeader++;
                if (currentHeader >= RVIEW_SND_BUFFERS) currentHeader = 0;
                samplesWritten += samplesWriteahead;
#else
                write(audioDevice, newSamples, num * devSampSize);
                samplesWritten += num;
#endif
            }
#ifdef SOUND_PLAYER_DEBUG
            cout << "soundPlayer: wrote " << num << " samples" << endl;
#endif
        }
    }
}





#ifndef __HAL_ONLY__

// Descriptor for sound formats
const rviewSoundPlayer::format_desc rviewSoundPlayer::soundFormatDesc[] =
{
    {"soundFmtLin8", rsf_lin8, 1},
    {"soundFmtUlin8", rsf_ulin8, 1},
    {"soundFmtUlaw8", rsf_ulaw8, 1},
    {"soundFmtLin16", rsf_lin16, 2},
    {NULL, rsf_none, 0}
};


/*
 *  rviewSoundPlayer: sound player widget, platform independent.
 */

rviewSoundPlayer::rviewSoundPlayer(mdd_frame *mf, unsigned int flags) : rviewDisplay(mf, sound_ctrly, flags)
{
    int w, h;
    char buffer[STRINGSIZE];
    char **latStr, **fmtStr;
    int fnumLin8, fnumUlaw8, fnumLin16;
    char *b;
    int i;

    RMDBGENTER(3, RMDebug::module_applications, "rviewSoundPlayer", "rviewSoundPlayer()");

#if 0
    {
        dimMDD = 1;
        interv = r_Minterval(1);
        //interv << r_Sinterval(r_Range(0), r_Range(41744));
        //#define SAMPLE_NAME "death.sam"
        //interv << r_Sinterval(r_Range(0), r_Range(79842));
        //#define SAMPLE_NAME "cyber.sam"
        interv << r_Sinterval(r_Range(0), r_Range(77071));
#define SAMPLE_NAME "willy.sam"
        r_Ref<r_Marray<r_Octet> > mddPtr = new r_Marray<r_Octet>(interv);
        mddPtr->set_type_by_name("OctetString");
        sprintf(buffer, "marray <octet, [0:%d]>", interv[0].high());
        mddPtr->set_type_structure(buffer);
        char *data = mddPtr->get_array();
#ifdef __VISUALC__
        FILE *fp = fopen("Z:\\dehmel\\rview\\samples\\" SAMPLE_NAME, "rb");
#else
        FILE *fp = fopen("samples" DIR_SEPARATOR SAMPLE_NAME, "rb");
#endif
        if (fp != NULL)
        {
            fread(data, 1, interv[0].high() - interv[0].low() + 1, fp);
            fclose(fp);
        }
        else
        {
            memset(data, 0x80, interv[0].high() - interv[0].low() + 1);
        }
        mddObj = (r_Ref<r_GMarray>)mddPtr;
        baseType = rbt_uchar;
    }
#endif

    sampleBuffer = NULL;
    lastOffset = 0;
    paused = FALSE;
    typeLength = mddObj->get_type_length();
    playbackOn = FALSE;

    // Defaults
    frequency = prefs->soundFreq;
    latency = prefs->soundLatency;
    loopMode = prefs->soundLoop;

    GetClientSize(&w, &h);

    toStart = new rviewButton(ctrlPanel, "<<");
    toEnd = new rviewButton(ctrlPanel, ">>");
    pbPause = new rviewButton(ctrlPanel, "||");
    pbStart = new rviewButton(ctrlPanel, ">");
    pbStop = new rviewButton(ctrlPanel, "[]");
    pbLoop = new rviewButton(ctrlPanel);

    slider = new rviewSlider(ctrlPanel, 0, 0, 1000, 10, lman->lookup("soundPlayTime"));
    frqWidget = new rviewText(ctrlPanel, frequency);

    fnumLin8 = 0;
    fnumUlaw8 = 0;
    fnumLin16 = 0;
    for (i=0; soundFormatDesc[i].labelName != NULL; i++)
    {
        switch (soundFormatDesc[i].fmt)
        {
        case rsf_lin8:
            fnumLin8 = i;
            break;
        case rsf_ulaw8:
            fnumUlaw8 = i;
            break;
        case rsf_lin16:
            fnumLin16 = i;
            break;
        default:
            break;
        }
    }
    fmtStr = new char *[i];
    for (i=0; soundFormatDesc[i].labelName != NULL; i++)
        fmtStr[i] = lman->lookup(soundFormatDesc[i].labelName);
    fmtWidget = new rviewChoice(ctrlPanel, i, fmtStr, lman->lookup("soundFormat"));
    delete [] fmtStr;

    latencies = new int[sound_latencies];
    for (i=0; i<sound_latencies; i++) latencies[i] = 50 * (i+1);
    latStr = new char *[sound_latencies];
    b = buffer;
    for (i=0; i<sound_latencies; i++)
    {
        latStr[i] = b;
        b += 1 + sprintf(b, "%dms", latencies[i]);
    }
    latWidget = new rviewChoice(ctrlPanel, i, latStr, lman->lookup("soundLatency"));
    delete [] latStr;
    for (i=0; i<sound_latencies-1; i++)
    {
        if ((latencies[i] <= latency) && (latencies[i+1] > latency)) break;
    }
    latWidget->SetSelection(i);

    b = projString;
    b += sprintf(b, "*:*");
    for (i=1; i<dimMDD; i++)
    {
        b += sprintf(b, ", %ld", interv[i].low());
    }
    project->SetValue(projString);

    // Interpret 2D data as multiple channels
    setModeDimension((dimMDD == 1) ? 1 : 2);

    setLoopMode(loopMode);

    // Mustn't create errorbox before all widgets are created.
    switch (baseType)
    {
    case rbt_char:
        currentFormat = fnumUlaw8;
        break;
    case rbt_uchar:
        currentFormat = fnumLin8;
        break;
    case rbt_short:
        currentFormat = fnumLin16;
        break;
    default:
    {
        rviewErrorbox::reportError(lman->lookup("errorSoundFormat"), rviewSoundPlayer::getFrameName(), "rviewSoundFormat");
        currentFormat = -1;
        objectInitializedOK = FALSE;
    }
    break;
    }
    if (currentFormat >= 0) fmtWidget->SetSelection(currentFormat);

    RMDBGEXIT(3, RMDebug::module_applications, "rviewSoundPlayer", "rviewSoundPlayer()");
}


int rviewSoundPlayer::openViewer(void)
{

    RMDBGONCE(3, RMDebug::module_applications, "rviewSoundPlayer", "openViewer()");

    if (currentFormat < 0)
    {
        rviewErrorbox::reportError(lman->lookup("errorSoundFormat"), rviewSoundPlayer::getFrameName(), "openViewer");
        objectInitializedOK = FALSE;
        return -1;
    }

    if (rviewDisplay::openViewer() == 0)
    {
        int w, h;

        GetClientSize(&w, &h);

        label();

        frameWidth=-1;
        frameHeight=-1;

        OnSize(w, h);
        OnSize(w, h);

        Show(TRUE);

        return 0;
    }
    return -1;
}


rviewSoundPlayer::~rviewSoundPlayer(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewSoundPlayer", "~rviewSoundPlayer()");
    closeViewer();
    if (latencies)
    {
        delete [] latencies;
        latencies=0;
    }
    if (sampleBuffer)
    {
        delete [] sampleBuffer;
        sampleBuffer=0;
    }
}


const char *rviewSoundPlayer::getFrameName(void) const
{
    return "rviewSoundPlayer";
}

rviewFrameType rviewSoundPlayer::getFrameType(void) const
{
    return rviewFrameTypeSound;
}

int rviewSoundPlayer::getViewerType(void) const
{
    return RVIEW_RESDISP_SOUND;
}


bool rviewSoundPlayer::setLoopMode(bool lm)
{
    bool oldMode = loopMode;

    loopMode = lm;
    if (loopMode)
    {
        pbLoop->SetLabel("<-->");
    }
    else
    {
        pbLoop->SetLabel("--->");
    }
    player.playbackLoopMode((loopMode) ? 1 : 0);

    return oldMode;
}


void rviewSoundPlayer::setSlider(int offset)
{
    slider->SetValue((offset + frequency - 1) / frequency);
}

void rviewSoundPlayer::label(void)
{
    SetTitle(lman->lookup("titleSound"));
    slider->SetLabel(lman->lookup("soundPlayTime"));
    frqWidget->SetLabel(lman->lookup("soundFrequency"));
    latWidget->SetLabel(lman->lookup("soundLatency"));

    rviewDisplay::label();
}

int rviewSoundPlayer::process(wxObject &obj, wxEvent &evt)
{
    int type = evt.GetEventType();

    RMDBGONCE(3, RMDebug::module_applications, "rviewSoundPlayer", "process(...)");

    if (rviewDisplay::process(obj, evt) != 0)
    {
        return 1;
    }

    if (type == wxEVENT_TYPE_BUTTON_COMMAND)
    {
        if (&obj == (wxObject*)pbStart)
        {
            startPlayback();
            return 1;
        }
        else if (&obj == (wxObject*)pbStop)
        {
            stopPlayback();
            return 1;
        }
        else if (&obj == (wxObject*)toStart)
        {
            player.playbackSetPosition(0);
            setSlider(0);
            return 1;
        }
        else if (&obj == (wxObject*)toEnd)
        {
            player.playbackSetPosition(sampleLength);
            setSlider(sampleLength);
            return 1;
        }
        else if (&obj == (wxObject*)pbPause)
        {
            if (paused)
            {
                player.playbackResume();
                paused = FALSE;
            }
            else
            {
                player.playbackSuspend();
                paused = TRUE;
            }
            return 1;
        }
        else if (&obj == (wxObject*)pbLoop)
        {
            setLoopMode(!loopMode);
            return 1;
        }
    }

    if (type == wxEVENT_TYPE_SLIDER_COMMAND)
    {
        if (&obj == (wxObject*)slider)
        {
            int soff, poff;

            if ((poff = player.playbackGetOffset()) >= 0)
            {
                soff = slider->GetValue();
                if (soff != poff/frequency)
                {
                    player.playbackSetPosition(soff * frequency);
                }
            }
            return 1;
        }
    }

    if (type == wxEVENT_TYPE_CHOICE_COMMAND)
    {
        if (&obj == (wxObject*)fmtWidget)
        {
            int newFmt;

            newFmt = fmtWidget->GetSelection();
            if ((soundFormatDesc[newFmt].sampleSize != baseSize) && (currentFormat >= 0))
            {
                fmtWidget->SetSelection(currentFormat);
            }
            else
            {
                currentFormat = newFmt;
            }
            return 1;
        }
    }

    return 0;
}


void rviewSoundPlayer::OnSize(int w, int h)
{
    int x, y, posx, posy, d;

    RMDBGONCE(3, RMDebug::module_applications, "rviewSoundPlayer", "OnSize( " << w << ", " << h << " )");

    rviewDisplay::OnSize(w, h);

    GetClientSize(&x, &y);

    //need to resize?
    if (( sound_width != x) || ( sound_height != y))
    {
        frameWidth =  sound_width;
        frameHeight =  sound_height;
        x =  sound_width;
        y =  sound_height;
        SetClientSize(x, y);
        return;
    }


    d = (x - 7*display_border) / 6;
    posx = display_border;
    posy = display_border + display_cheight;
    toStart->SetSize(posx, posy, d, sound_bheight);
    posx += d + display_border;
    pbPause->SetSize(posx, posy, d, sound_bheight);
    posx += d + display_border;
    pbStart->SetSize(posx, posy, d, sound_bheight);
    posx += d + display_border;
    pbStop->SetSize(posx, posy, d, sound_bheight);
    posx += d + display_border;
    toEnd->SetSize(posx, posy, d, sound_bheight);
    posx += d + display_border;
    pbLoop->SetSize(posx, posy, d, sound_bheight);
    posx = display_border;
    posy += sound_bheight + display_border;
    d = (x - 4*display_border) / 3;
    frqWidget->SetSize(posx, posy, d, sound_theight);
    fmtWidget->SetSize(posx + d + display_border, posy, 6*d/9, sound_cheight);
    latWidget->SetSize(posx + 2*(d + display_border), posy, 5*d/9, sound_cheight);
    posy += sound_theight + 2*display_border;
    slider->SetSize(display_border, posy, x - 2*display_border, sound_theight);
}



int rviewSoundPlayer::newProjection(void)
{
    RMDBGONCE(3, RMDebug::module_applications, "rviewSoundPlayer", "newProjection()");

    if (playbackOn)
    {
        player.playbackStop();
        if (buildSample() != 0) return -1;
        if (newSample() != 0) return -1;    // implicitly does playbackResume()
    }
    else
    {
        return startPlayback();
    }
    return 0;
}


void rviewSoundPlayer::prepareToDie(void)
{
    //cout << "rviewSoundPlayer::prepareToDie" << endl;
    stopPlayback();
    ::wxYield();
}


int rviewSoundPlayer::newSample(void)
{
    int fmt;
    int status=0;

    RMDBGONCE(3, RMDebug::module_applications, "rviewSoundPlayer", "newSample()");

    frequency = atoi(frqWidget->GetValue());
    latency = latencies[latWidget->GetSelection()];

    fmt = fmtWidget->GetSelection();
    switch (fmt)
    {
    case 0:
        status = player.newSample(frequency, channels, (signed char*)sampleBuffer, sampleLength, rsf_lin8, latency);
        break;
    case 1:
        status = player.newSample(frequency, channels, (unsigned char *)sampleBuffer, sampleLength, rsf_ulin8, latency);
        break;
    case 2:
        status = player.newSample(frequency, channels, (unsigned char *)sampleBuffer, sampleLength, rsf_ulin8, latency);
        break;
    case 3:
        status = player.newSample(frequency, channels, (short *)sampleBuffer, sampleLength, rsf_lin16, latency);
        break;
    default:
    {
        rviewErrorbox::reportError(lman->lookup("errorSoundFormat"), rviewSoundPlayer::getFrameName(), "newSample");
    }
    return -1;
    }

    if (status != 0)
    {
        rviewErrorbox::reportError(lman->lookup("errorSoundDevice"), rviewSoundPlayer::getFrameName(), "newSample");
        return -1;
    }

    setSlider(0);
    slider->SetRange(0, (sampleLength + frequency - 1) / frequency);

    return 0;
}


int rviewSoundPlayer::buildSample(void)
{
    int stepx, stepy;
    union
    {
        char *c;
        short *s;
    } src, dest, srcBase;
    r_Ref<r_Marray<r_Char> > mddPtr = (r_Ref<r_Marray<r_Char> >)mddObj;
    char *base;
    int i;

    RMDBGONCE(3, RMDebug::module_applications, "rviewSoundPlayer", "buildSample()");

    strcpy(projString, project->GetValue());
    if (rviewParseProjection(getVirtualDomain(), pt1, pt2, projString, &freeDims) != dimMDD)
    {
        rviewErrorbox::reportError(lman->lookup("errorProjection"), rviewSoundPlayer::getFrameName(), "buildSample");
        return -1;
    }

    dim1 = dimMDD;
    dim2 = dimMDD;
    for (dim1=0; dim1<dimMDD; dim1++) if ((freeDims & (1<<dim1)) != 0) break;
    for (dim2=dim1+1; dim2<dimMDD; dim2++) if ((freeDims & (1<<dim2)) != 0) break;

    sampleLength = pt2[dim1] - pt1[dim1] + 1;

    if (sampleBuffer != NULL)
    {
        delete [] sampleBuffer;
        sampleBuffer = NULL;
    }

    base = mddObj->get_array();
    src.c = base + typeLength * ((char*)(&((*mddPtr)[pt1])) - base);

    r_Point paux(pt1);
    paux[dim1]++;
    stepx = (int)(&((*mddPtr)[paux]) - &((*mddPtr)[pt1]));

    // One channel or multiple ones?
    if (dim2 >= dimMDD)
    {
        channels = 1;
        sampleBuffer = (void*)(new char[sampleLength * typeLength]);
        dest.c = (char*)sampleBuffer;
        switch (typeLength)
        {
        case 1:
        {
            for (i=0; i<sampleLength; i++, src.c += stepx)
            {
                *dest.c++ = *src.c;
            }
        }
        break;
        case 2:
        {
            for (i=0; i<sampleLength; i++, src.s += stepx)
            {
                *dest.s++ = *src.s;
            }
        }
        break;
        default:
            break;
        }
    }
    else
    {
        int j;

        paux[dim1] = pt1[dim1];
        paux[dim2]++;
        stepy = (int)(&((*mddPtr)[paux]) - &((*mddPtr)[pt1]));

        channels = pt2[dim2] - pt1[dim2] + 1;
        sampleBuffer = (void*)(new char[sampleLength * typeLength * channels]);
        dest.c = (char*)sampleBuffer;
        srcBase.c = src.c;

        // Multiple channels ==> create channel-interleaved sample data.
        switch (typeLength)
        {
        case 1:
        {
            for (i=0; i<sampleLength; i++, srcBase.c += stepx)
            {
                for (j=0, src.c = srcBase.c; j<channels; j++, src.c += stepy)
                {
                    *dest.c++ = *src.c;
                }
            }
        }
        break;
        case 2:
        {
            for (i=0; i<sampleLength; i++, srcBase.c += stepx)
            {
                for (j=0, src.s = srcBase.s; j<channels; j++, src.s += stepy)
                {
                    *dest.s++ = *src.s;
                }
            }
        }
        break;
        default:
            break;
        }
    }
    return 0;
}


int rviewSoundPlayer::startPlayback(void)
{
    int offset;

    RMDBGENTER(3, RMDebug::module_applications, "rviewSoundPlayer", "startPlayback()");

    // no reentrancy
    if (playbackOn) return -1;

    if (sampleBuffer == NULL)
    {
        if (buildSample() != 0) return -1;
    }

    newSample();

    playbackOn = TRUE;
    paused = FALSE;

    lastOffset = 0;
    while ((offset = player.playbackActive()) >= 0)
    {
        if (abs(offset - lastOffset) >= frequency)
        {
            slider->SetValue(offset / frequency);
            lastOffset = offset;
        }
        ::wxYield();
        if (!playbackOn) break;
    }
    if (offset < 0)
    {
        setSlider(sampleLength);
    }

    playbackOn = FALSE;

    return 0;
}


int rviewSoundPlayer::stopPlayback(void)
{
    player.playbackStop();
    playbackOn = FALSE;

    return 0;
}





#else

// Testbed
int main(int argc, char *argv[])
{
    FILE *infile;
    soundPlayer *sp;
#ifdef __VISUALC__
    char *lumpname = "samples\\death.sam";
#else
    char *lumpname = "samples/death.sam";
#endif
    int offset, lastOffset;
    int frequency;

    if (argc > 1)
    {
        lumpname = argv[1];
    }

    if ((infile = fopen(lumpname, "rb")) == NULL)
    {
        cerr << "Couldn't open file " << lumpname << endl;
        exit(-1);
    }

    cout << "create new player" << endl;

    frequency = 11025;
    sp = new soundPlayer(frequency, 1, infile, rsf_ulin8, 500);

    lastOffset = -frequency;
    while ((offset = sp->playbackActive()) >= 0)
    {
        if (abs(offset - lastOffset) >= frequency)
        {
            printf("Time %ds\r", offset / frequency);
            fflush(stdout);
            lastOffset = offset;
        }
    }
    printf("\n", offset);

    cout << "delete player" << endl;

    delete sp;

    fclose(infile);

    return 0;
}

#endif // HAL
