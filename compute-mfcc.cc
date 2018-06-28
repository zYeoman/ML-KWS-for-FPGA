// -----------------------------------------------------------------------------
// Wrapper for MFCC feature extractor
// -----------------------------------------------------------------------------
//
//  Copyright (C) 2016 D S Pavan Kumar
//  dspavankumar [at] gmail [dot] com
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "mfcc-official.h"

typedef struct     // WAV stores wave file header
{
        int rId;
        int rLen;
        int wId;
        int fId;

        int fLen;

        short wFormatTag;
        short nChannels;
        int nSamplesPerSec;   // Sampling rate. This returns to FS variable
        int nAvgBytesPerSec;  // All other parameters are not used in processing
        short nBlockAlign;
        short wBitsPerSample;
        int dId;
        int wSampleLength;
}WAV;

// Main
int main(int argc, char* argv[]) {

    // Assign variables
    int numCepstra = 10;
    int samplingRate = 16000;
    int winLength = 40;
    int frameShift = 20;
    int frame_len = winLength*samplingRate/1000;
    int frame_shift = frameShift*samplingRate/1000;

    MFCC mfccComputer (numCepstra, frame_len, 8);
    int16_t* buffer = new int16_t[frame_len];
    float out[100];

    WAV header;
    FILE *sourcefile;
    sourcefile=fopen(argv[1],"rb");
    fread(&header,sizeof(WAV),1,sourcefile);
    while(1) {
        if(fread(buffer,sizeof(int16_t),frame_len,sourcefile) == (size_t)frame_len)
            mfccComputer.mfcc_compute(buffer, out);
        else
            break;
        for(int i = 0; i < numCepstra; i++) {
            printf("%f\n", out[i]);
        }
        fseek(sourcefile, (frame_shift-frame_len)*sizeof(int16_t), SEEK_CUR);
    }
    return 0;
}
