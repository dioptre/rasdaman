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

/**
*   @file rviewSound.hh
*
*   @ingroup Applications
*/

#ifndef _RVIEW_SOUND_H_
#define _RVIEW_SOUND_H_


#include <stdio.h>
#include <signal.h>

#ifdef __VISUALC__
#include <windows.h>
#endif


#ifdef __HAL_ONLY__
#ifndef __VISUALC__
#include "bool.h"
#endif
typedef bool bool;
#ifndef TRUE
#define TRUE true
#define FALSE false
#endif
#else
#include "rviewUtils.hh"
#include "rviewDisplay.hh"
#endif // HAL



/* Default latency of sound playback */
#define RVIEW_SND_LATENCY   100
/* Timer interval relative to sample buffer runtime */
#define RVIEW_SND_RELPERIOD 0.5

/* Number of sample buffers (NT only) */
#define RVIEW_SND_BUFFERS   3


enum rviewSoundFormat
{
    rsf_none,
    rsf_lin8,
    rsf_ulin8,
    rsf_ulaw8,
    rsf_lin16
};



/*
 *  soundPlayer: hardware abstraction layer
 */

class soundPlayer
{
public:

    // Base constructor
    soundPlayer(void);
    // Constructor for input from file
    soundPlayer(int frq, int ch, FILE *fp, rviewSoundFormat fmt, int lat=RVIEW_SND_LATENCY);
    // Constructor for 8bit linear (default)
    soundPlayer(int frq, int ch, const signed char *data, int len, rviewSoundFormat fmt=rsf_lin8, int lat=RVIEW_SND_LATENCY);
    // Constructor for 8bit ulaw (default)
    soundPlayer(int frq, int ch, const unsigned char *data, int len, rviewSoundFormat fmt=rsf_ulaw8, int lat=RVIEW_SND_LATENCY);
    // Constructor for 16bit linear (default)
    soundPlayer(int frq, int ch, const short *data, int len, rviewSoundFormat fmt=rsf_lin16, int lat=RVIEW_SND_LATENCY);

    ~soundPlayer(void);

    // For changing during playback
    int newSample(int frq, int ch, FILE *fp, rviewSoundFormat fmt, int lat=RVIEW_SND_LATENCY);
    int newSample(int frq, int ch, const signed char *data, int len, rviewSoundFormat=rsf_lin8, int lat=RVIEW_SND_LATENCY);
    int newSample(int frq, int ch, const unsigned char *data, int len, rviewSoundFormat=rsf_ulaw8, int lat=RVIEW_SND_LATENCY);
    int newSample(int frq, int ch, const short *data, int len, rviewSoundFormat=rsf_lin16, int lat=RVIEW_SND_LATENCY);

    int playbackGetOffset(void);
    int playbackActive(void);
    void playbackSuspend(void);
    void playbackResume(void);
    void playbackStop(void);
    int  playbackSetPosition(int position);
    int  playbackLoopMode(int lpMode);

#ifdef __VISUALC__
    void writeSamples(DWORD systime);
#else
    void writeSamples(void);
#endif


protected:

    void setupVariables(void);
    const char *ensureSamplesForDevice(const char *source, int len);
    const char *ensureSamples(int &num);
    int configureDevice(int frq, int ch, int len, rviewSoundFormat fmt, int lat);
    void ensureUlawTable(int ulawsize);
    void ensureLinearTable(void);
    char *ensureConvBuff(int size);
    char *ensureSampleBuff(int size);

    int setTimerInterval(unsigned int ti);
    int startTimer(int ap=1);
    int stopTimer(int ap=1);
    int handleOutOfData(int dataSize);

    rviewSoundFormat format;
    rviewSoundFormat devFormat;
    int sampleSize, devSampSize;
    FILE *sampleFile;
    int dataOffset, inLength;
    int frequency, channels, latency, samplesWriteahead;
    int buffSize, cbuffSize;
    char *buffer, *convBuff;
    const char *inData;
    unsigned char *LinToUlaw;
    short *UlawToLin;
    int ldUlawSize;
    int samplesWritten;
    soundPlayer *suspendedPlayer;
    int loopMode;
    bool timerActive;

    // Unix specifics
#ifdef __VISUALC__
    void freeWaveHeaders(void);

    HWAVEOUT waveOut;
    WAVEFORMATEX waveFmt;
    WAVEHDR waveHdrs[RVIEW_SND_BUFFERS];
    UINT timerID;
    DWORD lastSyncTime;
    int currentHeader;
    int emptyBuffers;
#else
    int audioDevice;
    struct timeval lastSyncTime;
    struct sigaction oact;
    struct itimerval ovalue;
#endif
};



#ifndef __HAL_ONLY__

/*
 *  rviewSoundPlayer: sound player widget
 */

class rviewSoundPlayer: public rviewDisplay
{
public:

    rviewSoundPlayer(mdd_frame *mf, unsigned int flags=0);
    ~rviewSoundPlayer(void);

    void label(void);
    int process(wxObject &obj, wxEvent &evt);
    virtual int openViewer(void);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;
    virtual int getViewerType(void) const;

    void OnSize(int w, int h);

    int newProjection(void);
    void prepareToDie(void);

    typedef struct format_desc
    {
        const char *labelName;
        rviewSoundFormat fmt;
        int sampleSize;
    } format_desc;

    // constants
    // Sound player window button size
    static const int sound_bwidth;
    static const int sound_bheight;
    // Sound player window slider height
    static const int sound_sheight;
    // Sound player window text size
    static const int sound_twidth;
    static const int sound_theight;
    // Sount player choice size
    static const int sound_cwidth;
    static const int sound_cheight;
    // Sound player window ctrl area size
    static const int sound_ctrly;
    // Sound player frame size
    static const int sound_width;
    static const int sound_height;
    // Number of latency entries
    static const int sound_latencies;


protected:

    int buildSample(void);
    int newSample(void);
    int startPlayback(void);
    int stopPlayback(void);
    void setSlider(int offset);
    bool setLoopMode(bool lm);

    soundPlayer player;
    rviewButton *toStart, *toEnd;
    rviewButton *pbPause, *pbStart, *pbStop, *pbLoop;
    rviewSlider *slider;
    rviewText *frqWidget;
    rviewChoice *fmtWidget, *latWidget;
    int frequency, channels, latency;
    int lastOffset;
    int *latencies;
    void *sampleBuffer;
    int sampleLength;
    bool paused;
    bool playbackOn;
    bool loopMode;
    int dim1, dim2;
    int typeLength;
    int currentFormat;
    unsigned int freeDims;

    static const format_desc soundFormatDesc[];
};

#endif // HAL

#endif
