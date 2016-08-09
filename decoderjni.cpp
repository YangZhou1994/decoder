//TODO get final plath
#include "XX"
#include "XX/vpe_decoder.h"


JNIEXPORT jobjectArray JNICALL Java_org_XX(JNIEnv *env,jobject obj,jint len,jbyteArray buffer)
{
	unit8_t *contentdata=new uchar[len];
	env->GetByteArrayRegion(buffer,0,len,(signed char*)contentdata);
	VideoData data;
	//VideoData have been defined in vpe_decoder.h as a struct
	/*
		typedef struct _video_data_t{
			int width;
			int height;
			int channels;
			vector<char*> frames;
		}Videodata
	*/
	int buffer_len=len;
	vpeDecoder(buffer_len,contentdata,&data);
	if((data.frames).empty) return NULL;
	//TODO get final  path
	jclass framesdata=env->FindClass("XX/VideoData");
	if(framesdata==NULL) return NULL;
	
	//TODO find what's mean of the jmethodID&&GetMethodID
	//Q: (Is this step  necessary?)
	/*
		the class is like follow:
			public class VideoData{
				public int width;
				public int height;
				public int channels;
				public byte[][] frames;
			}
	*/
	jmethodID constructor=env->GetMethodID((framesdata,"<init>","()V");
	
	jfieldID widthid=env->GetFieldID(framesdata,"width","I");
	if(widthid==NULL) return NULL;
	jfieldID heightid=env->GetFieldID(framesdata,"height","I");
 	if(heightid==NULL) return NULL;
	jfieldID channelsid=env->GetFieldID(framesdata,"channels","I");
 	if(channelsisd==NULL) return NULL;
	jfieldID framesid=env->GetFieldID(framesdata,"frames","[[B");
	if(framesid==NULL) return NULL;	
	
	jobject return_obj=env->NewObject(framesdata,constructor);
	
	//set width,height,channel
	env->SetIntField(return_obj,widthid,data.width);
	env->SetIntField(return_obj,heightid,data.height);
	env->SetIntFiled(return_obj,channelsid,data.channels);
	env->setIntFiled
	//set byte[][] frames 
	for(int i=0;i<(data.frames).size;++i)
	{
		jbyteArray getbytes=(env)->NewByteArray((jsize)strlen(data.frames[i]));
		(env)->SetByteArrayRegion(getbytes,0,(jsize)strlen(data.frames[i]),(jbyte*)data.frames[i]);
		env->SetObjectField(return_obj,framesid,getbytes);
	}

	return return_obj;
}

