
#include <stdlib.h>
#include <stdio.h>
#include "vpe_decoder.h"
#include <iostream>
#include <vector>

using namespace std;
#ifdef _WIN32  
//Windows  
extern "C"
{
#include "libavcodec/avcodec.h"  
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#define __STDC_CONSTANT_MACROS  

 
};
#else
//Linux...  
#ifdef __cplusplus  
extern "C"
{
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include <iostream>

#ifdef __cplusplus
};
#endif
#endif

struct VideoContent
{
	uint8_t *data;
	int len;
};

//Callback  
int read_buffer(void *opaque, uint8_t *buf, int buf_size) {
        VideoContent* content =(VideoContent *)opaque;
	assert(content->len<=buf_size);
	buf_size=content->len;
	memcpy(buf,content->data,content->len);
	return content->len;
}


bool vpeDecoder(int buffer_len, char* buffer, VideoData& data) {
    if (buffer_len <= 0) {
    	fprintf(stderr, "Error: Buffer length must ...\n");
	    return false;
	}
	if (buffer == NULL) {
		return false;
	}
	
	VideoContent* content;
	memcpy(content->data,buffer,buffer_len);
	content->len=buffer_len;
	AVFormatContext *pFormatCtx;
        int             i, videoindex;
        AVCodecContext  *pCodecCtx;
        AVCodec         *pCodec;
        int     numBytes, frameFinished;
        uint8_t *buffer;
        AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));
        SwsContext *pSWSCtx;
        av_register_all();
        avformat_network_init();
        pFormatCtx = avformat_alloc_context();

        //Init AVIOContext  
        unsigned char *aviobuffer = (unsigned char *)av_malloc(content->len);
        AVIOContext *avio = avio_alloc_context(aviobuffer, content->len, 0, content, read_buffer, NULL, NULL);	
	pFormatCtx->pb = avio;	
	if (avformat_open_input(&pFormatCtx, NULL, NULL, NULL) != 0) {
                printf("Couldn't open input stream.\n");
                return -1;
        }
        if (avformat_find_stream_info(pFormatCtx, NULL)<0) {
                printf("Couldn't find stream information.\n");
                return -1;
        }


        videoindex = -1;
        for (i = 0; i<pFormatCtx->nb_streams; i++)
                if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
                        videoindex = i;
                        break;
                }
        if (videoindex == -1) {
                printf("Didn't find a video stream.\n");
                return -1;
        }
	 pCodecCtx = pFormatCtx->streams[videoindex]->codec;
        pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
        if (pCodec == NULL) {
                printf("Codec not found.\n");
                return -1;
        }
        if (avcodec_open2(pCodecCtx, pCodec, NULL)<0) {
                printf("Could not open codec.\n");
                return -1;
        }

        AVFrame *pFrame,*pFrameRGB;
                pFrame= av_frame_alloc();
        pFrameRGB = av_frame_alloc();
        numBytes = avpicture_get_size(AV_PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height);
	 buffer = new uint8_t[numBytes];
        avpicture_fill((AVPicture *)pFrameRGB, buffer, AV_PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height);
        pSWSCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);

        i = 0;
        while (av_read_frame(pFormatCtx, packet) >= 0)
        {
                if (packet->stream_index == videoindex)
                {
                        avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, packet);
                        if (frameFinished)
                        {
                                {
                                        sws_scale(pSWSCtx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);

					//TODU set framedata to struct
					(data.frames).push_back((char*)pFrameRGB->data[0]);
					data.width=pFrame->width;
					data.height=pFrame->height;
                                }
                        }
                }
                av_free_packet(packet);

        }
	data.channels=3;
        avcodec_close(pCodecCtx);
        avformat_close_input(&pFormatCtx);




	return true;
}
