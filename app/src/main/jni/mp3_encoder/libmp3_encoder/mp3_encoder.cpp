#include "mp3_encoder.h"
#include <android/log.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

Mp3Encoder::Mp3Encoder() {
    pcmFile= NULL;
    mp3File= NULL;
    lameClient= NULL;
}

Mp3Encoder::~Mp3Encoder() {
}

int Mp3Encoder::Init(const char* pcmFilePath, const char *mp3FilePath, int sampleRate, int channels, int bitRate) {
	//pcmFile = fopen(pcmFilePath, "rb");
    pcmFile = fopen("/mnt/sdcard/a_songstudio/vocal.pcm", "rb");
    __android_log_print(ANDROID_LOG_INFO, "liuyi", "vocal.pcmis %s\n", strerror(errno));

	if(pcmFile) {
		//mp3File = fopen(mp3FilePath, "wb");
        mp3File = fopen("/mnt/sdcard/a_songstudio/vocal.mp3", "wb+");
		__android_log_print(ANDROID_LOG_INFO, "liuyi", "vocal.mp3 %s\n", strerror(errno));
		if(mp3File) {
			lameClient = lame_init();
			lame_set_in_samplerate(lameClient, sampleRate);
			lame_set_out_samplerate(lameClient, sampleRate);
			lame_set_num_channels(lameClient, channels);
			lame_set_brate(lameClient, bitRate / 1000);
			lame_init_params(lameClient);
			return 0;
		}
	}
	return -1;
}

void Mp3Encoder::Encode() {
	int bufferSize = 1024 * 256;
	short* buffer = new short[bufferSize / 2];
	short* leftBuffer = new short[bufferSize / 4];
	short* rightBuffer = new short[bufferSize / 4];
	uint8_t* mp3_buffer = new uint8_t[bufferSize];
	int readBufferSize = 0;
	while ((readBufferSize = fread(buffer, 2, bufferSize / 2, pcmFile)) > 0) {
		for (int i = 0; i < readBufferSize; i++) {
			if (i % 2 == 0) {
				leftBuffer[i / 2] = buffer[i];
			} else {
				rightBuffer[i / 2] = buffer[i];
			}
		}
		int wroteSize = lame_encode_buffer(lameClient, (short int *) leftBuffer, (short int *) rightBuffer, readBufferSize / 2, mp3_buffer, bufferSize);
		fwrite(mp3_buffer, 1, wroteSize, mp3File);
	}
	delete[] buffer;
	delete[] leftBuffer;
	delete[] rightBuffer;
	delete[] mp3_buffer;
}

/**
 * liuyi add 单声道的pcm转换
 */
void Mp3Encoder::Convert()
{
	int read, write;

	const int PCM_SIZE = 8192;
	const int MP3_SIZE = 129600;

	short int pcm_buffer[PCM_SIZE];
	unsigned char mp3_buffer[MP3_SIZE];
	do {
		//读出来的pcm数据放在 pcm_buffer里面,从pcm文件里面读PCM_SIZE个 2*sizeof(short int)个数据,放倒pcm_buffer
		read = fread(pcm_buffer, 1, PCM_SIZE, pcmFile);
		if (read == 0) {
			write = lame_encode_flush(lameClient, mp3_buffer, MP3_SIZE);
		} else {
			write = lame_encode_buffer(lameClient, pcm_buffer,pcm_buffer, read, mp3_buffer, MP3_SIZE);
			//write = lame_encode_buffer_interleaved(lameClient, pcm_buffer, read, mp3_buffer, MP3_SIZE);
		}
		fwrite(mp3_buffer, write,1, mp3File);
	}while (read != 0);
}

void Mp3Encoder::Destory() {
	if(pcmFile) {
		fclose(pcmFile);
	}
	if(mp3File) {
		fclose(mp3File);
		lame_close(lameClient);
	}
}
