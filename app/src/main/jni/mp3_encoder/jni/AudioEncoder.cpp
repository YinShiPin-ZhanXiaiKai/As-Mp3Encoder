#include "com_phuket_tour_studio_Mp3Encoder.h"
#include "../libmp3_encoder/mp3_encoder.h"
#include "../../common/CommonTools.h"

#define LOG_TAG "Mp3Encoder"

Mp3Encoder* encoder = NULL;

JNIEXPORT jint JNICALL Java_com_phuket_tour_studio_Mp3Encoder_init
  (JNIEnv * env, jobject obj, jstring pcmPathParam, jint channels, jint bitRate,
			jint sampleRate, jstring mp3PathParam) {
	const char* pcmPath = env->GetStringUTFChars(pcmPathParam, NULL);
	const char* mp3Path = env->GetStringUTFChars(mp3PathParam, NULL);
	LOGI("mp3Path= %s，pcmPath=%s", mp3Path,pcmPath);
	encoder = new Mp3Encoder();
	jint result =encoder->Init(pcmPath, mp3Path, sampleRate, channels, bitRate);
	env->ReleaseStringUTFChars(mp3PathParam, mp3Path);
	env->ReleaseStringUTFChars(pcmPathParam, pcmPath);
	return result;
}

JNIEXPORT void JNICALL Java_com_phuket_tour_studio_Mp3Encoder_encode(JNIEnv * env, jobject obj) {
	if(NULL != encoder) {
		//encoder->Encode();
		encoder->Convert();
	}
}

JNIEXPORT void JNICALL Java_com_phuket_tour_studio_Mp3Encoder_destroy(JNIEnv * env, jobject obj) {
	if(NULL != encoder) {
		encoder->Destory();
		delete encoder;
		encoder = NULL;
	}
}

