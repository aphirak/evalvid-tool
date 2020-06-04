/*
 * Utils for libavcodec
 * Copyright (c) 2002 Fabrice Bellard.
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file allcodecs.c
 * Utils for libavcodec.
 */

#include "avcodec.h"

#if defined(CONFIG_MSVC) || defined(__SYMBIAN32__) 
#    include "../config_msvc.h"
#endif

#if defined(CONFIG_MSVC) ||  defined(__SYMBIAN32__)

void avcodec_get_chroma_sub_sample(int pix_fmt, int *h_shift, int *v_shift)
{
	switch (pix_fmt) {
	case PIX_FMT_YUV420P: 
	case PIX_FMT_YUVJ420P:
		*h_shift = 1; *v_shift = 1; break;
	case PIX_FMT_YUV422P: 
	case PIX_FMT_YUV422:
	case PIX_FMT_UYVY422:
	case PIX_FMT_YUVJ422P:
		*h_shift = 1; *v_shift = 0; break;
	case PIX_FMT_YUV410P:
		*h_shift = 2; *v_shift = 2; break;
	case PIX_FMT_YUV411P:
	case PIX_FMT_UYVY411:
		*h_shift = 2; *v_shift = 0; break;
	default:
	    *h_shift = *v_shift = 0;
		break;
	}
}


/* Picture field are filled with 'ptr' addresses. Also return size */
int avpicture_fill(AVPicture *picture, uint8_t *ptr,
		   int pix_fmt, int width, int height)
{
    int size, w2, h2, size2, vs, hs;
   
    if(avcodec_check_dimensions(NULL, width, height))
        goto fail;

	avcodec_get_chroma_sub_sample(pix_fmt, &hs, &vs);
    size = width * height;
    switch(pix_fmt) {
    case PIX_FMT_YUV420P:
    case PIX_FMT_YUV422P:
    case PIX_FMT_YUV444P:
    case PIX_FMT_YUV410P:
    case PIX_FMT_YUV411P:
    case PIX_FMT_YUVJ420P:
    case PIX_FMT_YUVJ422P:
    case PIX_FMT_YUVJ444P:
        w2 = (width + (1 << hs) - 1) >> hs;
        h2 = (height + (1 << vs) - 1) >> vs;
        size2 = w2 * h2;
        picture->data[0] = ptr;
        picture->data[1] = picture->data[0] + size;
        picture->data[2] = picture->data[1] + size2;
        picture->linesize[0] = width;
        picture->linesize[1] = w2;
        picture->linesize[2] = w2;
        return size + 2 * size2;
    case PIX_FMT_RGB24:
    case PIX_FMT_BGR24:
        picture->data[0] = ptr;
        picture->data[1] = NULL;
        picture->data[2] = NULL;
        picture->linesize[0] = width * 3;
        return size * 3;
    case PIX_FMT_RGBA32:
        picture->data[0] = ptr;
        picture->data[1] = NULL;
        picture->data[2] = NULL;
        picture->linesize[0] = width * 4;
        return size * 4;
    case PIX_FMT_RGB555:
    case PIX_FMT_RGB565:
    case PIX_FMT_YUV422:
        picture->data[0] = ptr;
        picture->data[1] = NULL;
        picture->data[2] = NULL;
        picture->linesize[0] = width * 2;
        return size * 2;
    case PIX_FMT_UYVY422:
        picture->data[0] = ptr;
        picture->data[1] = NULL;
        picture->data[2] = NULL;
        picture->linesize[0] = width * 2;
        return size * 2;
    case PIX_FMT_UYVY411:
        picture->data[0] = ptr;
        picture->data[1] = NULL;
        picture->data[2] = NULL;
        picture->linesize[0] = width + width/2;
        return size + size/2;
    case PIX_FMT_GRAY8:
        picture->data[0] = ptr;
        picture->data[1] = NULL;
        picture->data[2] = NULL;
        picture->linesize[0] = width;
        return size;
    case PIX_FMT_MONOWHITE:
    case PIX_FMT_MONOBLACK:
        picture->data[0] = ptr;
        picture->data[1] = NULL;
        picture->data[2] = NULL;
        picture->linesize[0] = (width + 7) >> 3;
        return picture->linesize[0] * height;
    case PIX_FMT_PAL8:
        size2 = (size + 3) & ~3;
        picture->data[0] = ptr;
        picture->data[1] = ptr + size2; /* palette is stored here as 256 32 bit words */
        picture->data[2] = NULL;
        picture->linesize[0] = width;
        picture->linesize[1] = 4;
        return size2 + 256 * 4;
    default:
fail:
        picture->data[0] = NULL;
        picture->data[1] = NULL;
        picture->data[2] = NULL;
        picture->data[3] = NULL;
        return -1;
    }
}

int avpicture_get_size(int pix_fmt, int width, int height)
{
    AVPicture dummy_pict;
    return avpicture_fill(&dummy_pict, NULL, pix_fmt, width, height);
}

/* 2x2 -> 1x1 */
void ff_shrink22(uint8_t *dst, int dst_wrap,
                     const uint8_t *src, int src_wrap,
                     int width, int height)
{
    int w;
    const uint8_t *s1, *s2;
    uint8_t *d;

    for(;height > 0; height--) {
        s1 = src;
        s2 = s1 + src_wrap;
        d = dst;
        for(w = width;w >= 4; w-=4) {
            d[0] = (s1[0] + s1[1] + s2[0] + s2[1] + 2) >> 2;
            d[1] = (s1[2] + s1[3] + s2[2] + s2[3] + 2) >> 2;
            d[2] = (s1[4] + s1[5] + s2[4] + s2[5] + 2) >> 2;
            d[3] = (s1[6] + s1[7] + s2[6] + s2[7] + 2) >> 2;
            s1 += 8;
            s2 += 8;
            d += 4;
        }
        for(;w > 0; w--) {
            d[0] = (s1[0] + s1[1] + s2[0] + s2[1] + 2) >> 2;
            s1 += 2;
            s2 += 2;
            d++;
        }
        src += 2 * src_wrap;
        dst += dst_wrap;
    }
}

/* 4x4 -> 1x1 */
void ff_shrink44(uint8_t *dst, int dst_wrap,
                     const uint8_t *src, int src_wrap,
                     int width, int height)
{
    int w;
    const uint8_t *s1, *s2, *s3, *s4;
    uint8_t *d;

    for(;height > 0; height--) {
        s1 = src;
        s2 = s1 + src_wrap;
        s3 = s2 + src_wrap;
        s4 = s3 + src_wrap;
        d = dst;
        for(w = width;w > 0; w--) {
            d[0] = (s1[0] + s1[1] + s1[2] + s1[3] +
                    s2[0] + s2[1] + s2[2] + s2[3] +
                    s3[0] + s3[1] + s3[2] + s3[3] +
                    s4[0] + s4[1] + s4[2] + s4[3] + 8) >> 4;
            s1 += 4;
            s2 += 4;
            s3 += 4;
            s4 += 4;
            d++;
        }
        src += 4 * src_wrap;
        dst += dst_wrap;
    }
}

/* 8x8 -> 1x1 */
void ff_shrink88(uint8_t *dst, int dst_wrap,
                     const uint8_t *src, int src_wrap,
                     int width, int height)
{
    int w, i;

    for(;height > 0; height--) {
        for(w = width;w > 0; w--) {
            int tmp=0;
            for(i=0; i<8; i++){
                tmp += src[0] + src[1] + src[2] + src[3] + src[4] + src[5] + src[6] + src[7];
                src += src_wrap;
            }
            *(dst++) = (tmp + 32)>>6;
            src += 8 - 8*src_wrap;
        }
        src += 8*src_wrap - 8*width;
        dst += dst_wrap - width;
    }
}

void ff_img_copy_plane(uint8_t *dst, int dst_wrap,
                           const uint8_t *src, int src_wrap,
                           int width, int height)
{
    if((!dst) || (!src))
        return;
    for(;height > 0; height--) {
        memcpy(dst, src, width);
        dst += dst_wrap;
        src += src_wrap;
    }
}
#endif

#define REGISTER_ENCODER(X,x) \
		 if(ENABLE_##X##_ENCODER)  register_avcodec(&x##_encoder)
#define REGISTER_DECODER(X,x) \
          if(ENABLE_##X##_DECODER)  register_avcodec(&x##_decoder)
#define REGISTER_ENCDEC(X,x)  REGISTER_ENCODER(X,x); REGISTER_DECODER(X,x)

#define REGISTER_PARSER(X,x) if(ENABLE_##X##_PARSER)  av_register_codec_parser(&x##_parser)

/* If you do not call this function, then you can select exactly which
   formats you want to support */

/**
 * simple call to register all the codecs.
 */
void avcodec_register_all(void)
{
    static int inited = 0;

    if (inited != 0)
        return;
    inited = 1;

    /* video codecs */
    REGISTER_DECODER(AASC, aasc);
    REGISTER_ENCDEC (ASV1, asv1);
    REGISTER_ENCDEC (ASV2, asv2);
    REGISTER_DECODER(AVS, avs);
    REGISTER_DECODER(BMP, bmp);
    REGISTER_DECODER(CAVS, cavs);
    REGISTER_DECODER(CINEPAK, cinepak);
    REGISTER_DECODER(CLJR, cljr);
    REGISTER_DECODER(CSCD, cscd);
    REGISTER_DECODER(CYUV, cyuv);
    REGISTER_DECODER(DSICINVIDEO, dsicinvideo);
    REGISTER_ENCDEC (DVVIDEO, dvvideo);
    REGISTER_DECODER(EIGHTBPS, eightbps);
    REGISTER_ENCDEC (FFV1, ffv1);
    REGISTER_ENCDEC (FFVHUFF, ffvhuff);
    REGISTER_ENCDEC (FLASHSV, flashsv);
    REGISTER_DECODER(FLIC, flic);
    REGISTER_ENCDEC (FLV, flv);
    REGISTER_DECODER(FOURXM, fourxm);
    REGISTER_DECODER(FRAPS, fraps);
    REGISTER_ENCDEC (GIF, gif);
    REGISTER_ENCDEC (H261, h261);
    REGISTER_ENCDEC (H263, h263);
    REGISTER_DECODER(H263I, h263i);
    REGISTER_ENCODER(H263P, h263p);
    REGISTER_DECODER(H264, h264);
    REGISTER_ENCDEC (HUFFYUV, huffyuv);
    REGISTER_DECODER(IDCIN, idcin);
    REGISTER_DECODER(INDEO2, indeo2);
    REGISTER_DECODER(INDEO3, indeo3);
    REGISTER_DECODER(INTERPLAY_VIDEO, interplay_video);
    REGISTER_ENCODER(JPEGLS, jpegls);
    REGISTER_DECODER(KMVC, kmvc);
    REGISTER_ENCODER(LJPEG, ljpeg);
    REGISTER_DECODER(LOCO, loco);
    REGISTER_DECODER(MDEC, mdec);
    REGISTER_ENCDEC (MJPEG, mjpeg);
    REGISTER_DECODER(MJPEGB, mjpegb);
    REGISTER_DECODER(MMVIDEO, mmvideo);
    REGISTER_DECODER(MPEG_XVMC, mpeg_xvmc);
    REGISTER_ENCDEC (MPEG1VIDEO, mpeg1video);
    REGISTER_ENCDEC (MPEG2VIDEO, mpeg2video);
    REGISTER_ENCDEC (MPEG4, mpeg4);
    REGISTER_DECODER(MPEGVIDEO, mpegvideo);
    REGISTER_ENCDEC (MSMPEG4V1, msmpeg4v1);
    REGISTER_ENCDEC (MSMPEG4V2, msmpeg4v2);
    REGISTER_ENCDEC (MSMPEG4V3, msmpeg4v3);
    REGISTER_DECODER(MSRLE, msrle);
    REGISTER_DECODER(MSVIDEO1, msvideo1);
    REGISTER_DECODER(MSZH, mszh);
    REGISTER_DECODER(NUV, nuv);
    REGISTER_ENCODER(PAM, pam);
    REGISTER_ENCODER(PBM, pbm);
    REGISTER_ENCODER(PGM, pgm);
    REGISTER_ENCODER(PGMYUV, pgmyuv);
    REGISTER_ENCDEC (PNG, png);
    REGISTER_ENCODER(PPM, ppm);
    REGISTER_DECODER(QDRAW, qdraw);
    REGISTER_DECODER(QPEG, qpeg);
    REGISTER_DECODER(QTRLE, qtrle);
    REGISTER_ENCDEC (RAWVIDEO, rawvideo);
    REGISTER_DECODER(ROQ, roq);
    REGISTER_DECODER(RPZA, rpza);
    REGISTER_ENCDEC (RV10, rv10);
    REGISTER_ENCDEC (RV20, rv20);
    REGISTER_DECODER(SMACKER, smacker);
    REGISTER_DECODER(SMC, smc);
    REGISTER_ENCDEC (SNOW, snow);
    REGISTER_DECODER(SP5X, sp5x);
    REGISTER_ENCDEC (SVQ1, svq1);
    REGISTER_DECODER(SVQ3, svq3);
    REGISTER_DECODER(TARGA, targa);
    REGISTER_DECODER(THEORA, theora);
    REGISTER_DECODER(TIERTEXSEQVIDEO, tiertexseqvideo);
    REGISTER_DECODER(TIFF, tiff);
    REGISTER_DECODER(TRUEMOTION1, truemotion1);
    REGISTER_DECODER(TRUEMOTION2, truemotion2);
    REGISTER_DECODER(TSCC, tscc);
    REGISTER_DECODER(ULTI, ulti);
    REGISTER_DECODER(VC1, vc1);
    REGISTER_DECODER(VCR1, vcr1);
    REGISTER_DECODER(VMDVIDEO, vmdvideo);
    REGISTER_DECODER(VMNC, vmnc);
    REGISTER_DECODER(VP3, vp3);
    REGISTER_DECODER(VP5, vp5);
    REGISTER_DECODER(VP6, vp6);
    REGISTER_DECODER(VP6F, vp6f);
    REGISTER_DECODER(VQA, vqa);
    REGISTER_ENCDEC (WMV1, wmv1);
    REGISTER_ENCDEC (WMV2, wmv2);
    REGISTER_DECODER(WMV3, wmv3);
    REGISTER_DECODER(WNV1, wnv1);
    REGISTER_ENCODER(X264, x264);
    REGISTER_DECODER(XAN_WC3, xan_wc3);
    REGISTER_DECODER(XL, xl);
    REGISTER_ENCODER(XVID, xvid);
    REGISTER_ENCDEC (ZLIB, zlib);
    REGISTER_ENCDEC (ZMBV, zmbv);

    /* audio codecs */
    REGISTER_DECODER(AAC, aac);
    REGISTER_DECODER(MPEG4AAC, mpeg4aac);
    REGISTER_DECODER(AC3, ac3);
    REGISTER_ENCODER(AC3, ac3);
    REGISTER_DECODER(ALAC, alac);
    REGISTER_ENCDEC (AMR_NB, amr_nb);
    REGISTER_ENCDEC (AMR_WB, amr_wb);
    REGISTER_DECODER(COOK, cook);
    REGISTER_DECODER(DSICINAUDIO, dsicinaudio);
    REGISTER_DECODER(DTS, dts);
    REGISTER_ENCODER(FAAC, faac);
    REGISTER_ENCDEC (FLAC, flac);
    REGISTER_DECODER(IMC, imc);
    REGISTER_ENCDEC (LIBGSM, libgsm);
    REGISTER_ENCODER(LIBTHEORA, libtheora);
    REGISTER_DECODER(MACE3, mace3);
    REGISTER_DECODER(MACE6, mace6);
    REGISTER_ENCDEC (MP2, mp2);
    REGISTER_DECODER(MP3, mp3);
    REGISTER_DECODER(MP3ADU, mp3adu);
    REGISTER_ENCODER(MP3LAME, mp3lame);
    REGISTER_DECODER(MP3ON4, mp3on4);
    REGISTER_DECODER(MPC7, mpc7);
    if (!ENABLE_VORBIS_ENCODER)  REGISTER_ENCODER(OGGVORBIS, oggvorbis);
    if (!ENABLE_VORBIS_DECODER)  REGISTER_DECODER(OGGVORBIS, oggvorbis);
    REGISTER_DECODER(QDM2, qdm2);
    REGISTER_DECODER(RA_144, ra_144);
    REGISTER_DECODER(RA_288, ra_288);
    REGISTER_DECODER(SHORTEN, shorten);
    REGISTER_DECODER(SMACKAUD, smackaud);
    REGISTER_ENCDEC (SONIC, sonic);
    REGISTER_ENCODER(SONIC_LS, sonic_ls);
    REGISTER_DECODER(TRUESPEECH, truespeech);
    REGISTER_DECODER(TTA, tta);
    REGISTER_DECODER(VMDAUDIO, vmdaudio);
    REGISTER_ENCDEC (VORBIS, vorbis);
    REGISTER_DECODER(WAVPACK, wavpack);
    REGISTER_DECODER(WMAV1, wmav1);
    REGISTER_DECODER(WMAV2, wmav2);
    REGISTER_DECODER(WS_SND1, ws_snd1);

    /* pcm codecs */
    REGISTER_ENCDEC (PCM_ALAW, pcm_alaw);
    REGISTER_ENCDEC (PCM_MULAW, pcm_mulaw);
    REGISTER_ENCDEC (PCM_S8, pcm_s8);
    REGISTER_ENCDEC (PCM_S16BE, pcm_s16be);
    REGISTER_ENCDEC (PCM_S16LE, pcm_s16le);
    REGISTER_ENCDEC (PCM_S24BE, pcm_s24be);
    REGISTER_ENCDEC (PCM_S24DAUD, pcm_s24daud);
    REGISTER_ENCDEC (PCM_S24LE, pcm_s24le);
    REGISTER_ENCDEC (PCM_S32BE, pcm_s32be);
    REGISTER_ENCDEC (PCM_S32LE, pcm_s32le);
    REGISTER_ENCDEC (PCM_U8, pcm_u8);
    REGISTER_ENCDEC (PCM_U16BE, pcm_u16be);
    REGISTER_ENCDEC (PCM_U16LE, pcm_u16le);
    REGISTER_ENCDEC (PCM_U24BE, pcm_u24be);
    REGISTER_ENCDEC (PCM_U24LE, pcm_u24le);
    REGISTER_ENCDEC (PCM_U32BE, pcm_u32be);
    REGISTER_ENCDEC (PCM_U32LE, pcm_u32le);

    /* dpcm codecs */
    REGISTER_DECODER(INTERPLAY_DPCM, interplay_dpcm);
    REGISTER_DECODER(ROQ_DPCM, roq_dpcm);
    REGISTER_DECODER(SOL_DPCM, sol_dpcm);
    REGISTER_DECODER(XAN_DPCM, xan_dpcm);

    /* adpcm codecs */
    REGISTER_ENCDEC (ADPCM_4XM, adpcm_4xm);
    REGISTER_ENCDEC (ADPCM_ADX, adpcm_adx);
    REGISTER_ENCDEC (ADPCM_CT, adpcm_ct);
    REGISTER_ENCDEC (ADPCM_EA, adpcm_ea);
    REGISTER_ENCDEC (ADPCM_G726, adpcm_g726);
    REGISTER_ENCDEC (ADPCM_IMA_DK3, adpcm_ima_dk3);
    REGISTER_ENCDEC (ADPCM_IMA_DK4, adpcm_ima_dk4);
    REGISTER_ENCDEC (ADPCM_IMA_QT, adpcm_ima_qt);
    REGISTER_ENCDEC (ADPCM_IMA_SMJPEG, adpcm_ima_smjpeg);
    REGISTER_ENCDEC (ADPCM_IMA_WAV, adpcm_ima_wav);
    REGISTER_ENCDEC (ADPCM_IMA_WS, adpcm_ima_ws);
    REGISTER_ENCDEC (ADPCM_MS, adpcm_ms);
    REGISTER_ENCDEC (ADPCM_SBPRO_2, adpcm_sbpro_2);
    REGISTER_ENCDEC (ADPCM_SBPRO_3, adpcm_sbpro_3);
    REGISTER_ENCDEC (ADPCM_SBPRO_4, adpcm_sbpro_4);
    REGISTER_ENCDEC (ADPCM_SWF, adpcm_swf);
    REGISTER_ENCDEC (ADPCM_XA, adpcm_xa);
    REGISTER_ENCDEC (ADPCM_YAMAHA, adpcm_yamaha);

    /* subtitles */
    REGISTER_ENCDEC (DVBSUB, dvbsub);
    REGISTER_ENCDEC (DVDSUB, dvdsub);

    /* parsers */
#if 0
    REGISTER_PARSER (AAC, aac);
    REGISTER_PARSER (AC3, ac3);
    REGISTER_PARSER (CAVSVIDEO, cavsvideo);
    REGISTER_PARSER (DVBSUB, dvbsub);
    REGISTER_PARSER (DVDSUB, dvdsub);
    REGISTER_PARSER (H261, h261);
    REGISTER_PARSER (H263, h263);
    REGISTER_PARSER (H264, h264);
    REGISTER_PARSER (MJPEG, mjpeg);
    REGISTER_PARSER (MPEG4VIDEO, mpeg4video);
    REGISTER_PARSER (MPEGAUDIO, mpegaudio);
    REGISTER_PARSER (MPEGVIDEO, mpegvideo);
    REGISTER_PARSER (PNM, pnm);
#else
    REGISTER_PARSER (MPEG4VIDEO, mpeg4video);
    REGISTER_PARSER (MPEGAUDIO, mpegaudio);
    REGISTER_PARSER (MPEGVIDEO, mpegvideo);
#endif

    av_register_bitstream_filter(&dump_extradata_bsf);
    av_register_bitstream_filter(&remove_extradata_bsf);
    av_register_bitstream_filter(&noise_bsf);
    av_register_bitstream_filter(&mp3_header_compress_bsf);
    av_register_bitstream_filter(&mp3_header_decompress_bsf);
    av_register_bitstream_filter(&mjpega_dump_header_bsf);
}

