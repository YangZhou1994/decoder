//TODO get path
#include "XX"
#include "XX/vpe_decoder.h"


JNIEXPORT jobjectArray JNICALL Java_org_XX(JNIEnv *env,jobject obj,jint len,jbyteArray buffer)
{
	unit8_t *contentdata=new uchar[len];
	env->GetByteArrayRegion(buffer,0,len,(signed char*)contentdata);
	VideoData data;
	//VideoData have been defined in vpe_decoder.h as a struct
	int buffer_len=len;
	vpeDecoder(buffer_len,contentdata,&data);
	
	//TODO get path
	jclass framesdata=env->FindClass("XX/VideoData");
	if(framesdata==NULL) return NULL;
	
	//TODO find what's mean of the jmethodID&&GetMethodID 
	jmethodID constructor=env->GetMethodID((framesdata."<init>","()V");
	if(constructor==NULL) return NULL;
	
	jfieldID widthid=env->GetFieldID(framesdata,"width","I");
	jfieldID heightid=env->GetFieldID(framesdata,"height","I");
	jfieldID channelsid=env->GetFieldID(framesdata,"channels","I");
	
	//TODO find the way to get List<byte[]> frames

	jobject return_obj=env->NewObject(framesdata,constructor);

	env->SetIntField(return_obj,widthid,data.width);
	env->SetIntField(return_obj,heightid,data.height);
	env->SetIntFiled(return_obj,channelsid,data.channels);
	//TODO find the way to set List<byte[]> frames
	//env->Set....

	return return_obj;
}

