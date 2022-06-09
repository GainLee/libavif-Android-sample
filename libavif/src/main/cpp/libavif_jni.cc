#include <android/bitmap.h>
#include <android/log.h>
#include <jni.h>
#include <string.h>

#include "avif/avif.h"

#define LOG_TAG "avif_jni"
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))

#define FUNC(RETURN_TYPE, NAME, ...)                                      \
  extern "C" {                                                            \
  JNIEXPORT RETURN_TYPE Java_com_gain_libavif_AvifCodec_##NAME( \
      JNIEnv* env, jobject /*thiz*/, ##__VA_ARGS__);                      \
  }                                                                       \
  JNIEXPORT RETURN_TYPE Java_com_gain_libavif_AvifCodec_##NAME( \
      JNIEnv* env, jobject /*thiz*/, ##__VA_ARGS__)

namespace {

    jfieldID global_info_width;
    jfieldID global_info_height;
    jfieldID global_info_depth;

// RAII wrapper class that properly frees the decoder related objects on
// destruction.
    struct AvifDecoderWrapper {
    public:
        AvifDecoderWrapper() = default;

        // Not copyable or movable.
        AvifDecoderWrapper(const AvifDecoderWrapper &) = delete;

        AvifDecoderWrapper &operator=(const AvifDecoderWrapper &) = delete;

        ~AvifDecoderWrapper() {
            if (decoder != nullptr) {
                avifDecoderDestroy(decoder);
            }
        }

        avifDecoder *decoder = nullptr;
    };

    struct AvifEncoderWrapper {
    public:
        AvifEncoderWrapper() = default;

        // Not copyable or movable.
        AvifEncoderWrapper(const AvifEncoderWrapper &) = delete;

        AvifEncoderWrapper &operator=(const AvifEncoderWrapper &) = delete;

        ~AvifEncoderWrapper() {
            if (encoder != nullptr) {
                avifEncoderDestroy(encoder);
            }
        }

        avifEncoder *encoder = nullptr;
    };

    bool CreateDecoderAndParse(AvifDecoderWrapper *const decoder,
                               const uint8_t *const buffer, int length) {
        decoder->decoder = avifDecoderCreate();
        if (decoder->decoder == nullptr) {
            LOGE("Failed to create AVIF Decoder.");
            return false;
        }
        decoder->decoder->ignoreXMP = AVIF_TRUE;
        decoder->decoder->ignoreExif = AVIF_TRUE;
        avifResult res = avifDecoderSetIOMemory(decoder->decoder, buffer, length);
        if (res != AVIF_RESULT_OK) {
            LOGE("Failed to set AVIF IO to a memory reader.");
            return false;
        }
        res = avifDecoderParse(decoder->decoder);
        if (res != AVIF_RESULT_OK) {
            LOGE("Failed to parse AVIF image: %s.", avifResultToString(res));
            return false;
        }
        return true;
    }

}  // namespace

jint JNI_OnLoad(JavaVM *vm, void * /*reserved*/) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    const jclass info_class =
            env->FindClass("com/gain/libavif/AvifCodec$Info");
    global_info_width = env->GetFieldID(info_class, "width", "I");
    global_info_height = env->GetFieldID(info_class, "height", "I");
    global_info_depth = env->GetFieldID(info_class, "depth", "I");
    return JNI_VERSION_1_6;
}

FUNC(jboolean, isAvifImage, jobject encoded, int length) {
    const uint8_t *const buffer =
            static_cast<const uint8_t *>(env->GetDirectBufferAddress(encoded));
    const avifROData avif = {buffer, static_cast<size_t>(length)};
    return avifPeekCompatibleFileType(&avif);
}

FUNC(jboolean, getInfo, jobject encoded, int length, jobject info) {
    const uint8_t *const buffer =
            static_cast<const uint8_t *>(env->GetDirectBufferAddress(encoded));
    AvifDecoderWrapper decoder;
    if (!CreateDecoderAndParse(&decoder, buffer, length)) {
        return false;
    }
    env->SetIntField(info, global_info_width, decoder.decoder->image->width);
    env->SetIntField(info, global_info_height, decoder.decoder->image->height);
    env->SetIntField(info, global_info_depth, decoder.decoder->image->depth);
    return true;
}

FUNC(jboolean, decode, jobject encoded, int length, jobject bitmap) {
    const uint8_t *const buffer =
            static_cast<const uint8_t *>(env->GetDirectBufferAddress(encoded));
    AvifDecoderWrapper decoder;
    if (!CreateDecoderAndParse(&decoder, buffer, length)) {
        return false;
    }
    avifResult res = avifDecoderNextImage(decoder.decoder);
    if (res != AVIF_RESULT_OK) {
        LOGE("Failed to decode AVIF image. Status: %d", res);
        return false;
    }
    AndroidBitmapInfo bitmap_info;
    if (AndroidBitmap_getInfo(env, bitmap, &bitmap_info) < 0) {
        LOGE("AndroidBitmap_getInfo failed.");
        return false;
    }
    // Ensure that the bitmap is large enough to store the decoded image.
    if (bitmap_info.width < decoder.decoder->image->width ||
        bitmap_info.height < decoder.decoder->image->height) {
        LOGE(
                "Bitmap is not large enough to fit the image. Bitmap %dx%d Image "
                "%dx%d.",
                bitmap_info.width, bitmap_info.height, decoder.decoder->image->width,
                decoder.decoder->image->height);
        return false;
    }
    // Ensure that the bitmap format is either RGBA_8888 or RGBA_F16.
    if (bitmap_info.format != ANDROID_BITMAP_FORMAT_RGBA_8888 &&
        bitmap_info.format != ANDROID_BITMAP_FORMAT_RGBA_F16) {
        LOGE("Bitmap format (%d) is not supported.", bitmap_info.format);
        return false;
    }
    void *bitmap_pixels = nullptr;
    if (AndroidBitmap_lockPixels(env, bitmap, &bitmap_pixels) !=
        ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("Failed to lock Bitmap.");
        return false;
    }
    avifRGBImage rgb_image;
    avifRGBImageSetDefaults(&rgb_image, decoder.decoder->image);
    if (bitmap_info.format == ANDROID_BITMAP_FORMAT_RGBA_F16) {
        rgb_image.depth = 16;
        rgb_image.isFloat = AVIF_TRUE;
    } else {
        rgb_image.depth = 8;
    }
    rgb_image.pixels = static_cast<uint8_t *>(bitmap_pixels);
    rgb_image.rowBytes = bitmap_info.stride;
    res = avifImageYUVToRGB(decoder.decoder->image, &rgb_image);
    AndroidBitmap_unlockPixels(env, bitmap);
    if (res != AVIF_RESULT_OK) {
        LOGE("Failed to convert YUV Pixels to RGB. Status: %d", res);
        return false;
    }
    return true;
}

FUNC(jbyteArray, encodeRGBA8888, jobject pixels, int length, int width, int height) {
    AvifEncoderWrapper encode;
    avifRWData avifOutput = AVIF_DATA_EMPTY;
    avifRGBImage rgb;
    memset(&rgb, 0, sizeof(rgb));

    avifImage *image = avifImageCreate(width, height, 8,
                                       AVIF_PIXEL_FORMAT_YUV444); // these values dictate what goes into the final AVIF
    avifRGBImageSetDefaults(&rgb, image);
    // Override RGB(A)->YUV(A) defaults here: depth, format, chromaUpsampling, ignoreAlpha, alphaPremultiplied, libYUVUsage, etc

    // Alternative: set rgb.pixels and rgb.rowBytes yourself, which should match your chosen rgb.format
    // Be sure to use uint16_t* instead of uint8_t* for rgb.pixels/rgb.rowBytes if (rgb.depth > 8)
    avifRGBImageAllocatePixels(&rgb);

    // Fill your RGB(A) data here
    const uint8_t *const pixelBuffer =
            static_cast<const uint8_t *>(env->GetDirectBufferAddress(pixels));
    memcpy(rgb.pixels, pixelBuffer, rgb.rowBytes * image->height);

    avifResult convertResult = avifImageRGBToYUV(image, &rgb);

    if (convertResult != AVIF_RESULT_OK) {
        LOGE("Failed to convert to YUV(A): %s\n", avifResultToString(convertResult));
        return NULL;
    }

    encode.encoder = avifEncoderCreate();
    encode.encoder->maxThreads = 64;
    encode.encoder->speed = AVIF_SPEED_FASTEST;
    encode.encoder->maxQuantizer = 24;
    encode.encoder->minQuantizer = 22;

    // Call avifEncoderAddImage() for each image in your sequence
    // Only set AVIF_ADD_IMAGE_FLAG_SINGLE if you're not encoding a sequence
    // Use avifEncoderAddImageGrid() instead with an array of avifImage* to make a grid image
    avifResult addImageResult = avifEncoderAddImage(encode.encoder, image, 1,
                                                    AVIF_ADD_IMAGE_FLAG_SINGLE);
    if (addImageResult != AVIF_RESULT_OK) {
        LOGE("Failed to add image to encoder: %s\n", avifResultToString(addImageResult));
        return NULL;
    }

    avifResult finishResult = avifEncoderFinish(encode.encoder, &avifOutput);
    if (finishResult != AVIF_RESULT_OK) {
        LOGE("Failed to finish encode: %s\n", avifResultToString(finishResult));
        return NULL;
    }

    jbyteArray jarr = env->NewByteArray(avifOutput.size);
    env->SetByteArrayRegion(jarr, 0, avifOutput.size,
                            reinterpret_cast<const jbyte *>(avifOutput.data));

    if (image) {
        avifImageDestroy(image);
    }

    avifRWDataFree(&avifOutput);
    avifRGBImageFreePixels(&rgb); // Only use in conjunction with avifRGBImageAllocatePixels()

    return jarr;
}

FUNC(jbyteArray, encodeY420, jobject yBuf, jobject uBuf, jobject vBuf, int strideY, int strideU, int strideV, int width, int height) {
    AvifEncoderWrapper encode;
    avifRWData avifOutput = AVIF_DATA_EMPTY;
    avifRGBImage rgb;
    memset(&rgb, 0, sizeof(rgb));

    avifImage *image = avifImageCreate(width, height, 8,
                                       AVIF_PIXEL_FORMAT_YUV420); // these values dictate what goes into the final AVIF

    avifImageAllocatePlanes(image, AVIF_PLANES_YUV | AVIF_PLANES_A);

    // Fill your YUV(A) data here
    const uint8_t *const pYBuf =
            static_cast<const uint8_t *>(env->GetDirectBufferAddress(yBuf));
    const uint8_t *const pUBuf =
            static_cast<const uint8_t *>(env->GetDirectBufferAddress(uBuf));
    const uint8_t *const pVBuf =
            static_cast<const uint8_t *>(env->GetDirectBufferAddress(vBuf));
    memcpy(image->yuvPlanes[AVIF_CHAN_Y], pYBuf, image->yuvRowBytes[AVIF_CHAN_Y] * image->height);
    memcpy(image->yuvPlanes[AVIF_CHAN_U], pUBuf, image->yuvRowBytes[AVIF_CHAN_U] * image->height/2);
    memcpy(image->yuvPlanes[AVIF_CHAN_V], pVBuf, image->yuvRowBytes[AVIF_CHAN_V] * image->height/2);
    memset(image->alphaPlane, 255, image->alphaRowBytes * image->height);

    encode.encoder = avifEncoderCreate();
    encode.encoder->maxThreads = 64;
    encode.encoder->speed = AVIF_SPEED_FASTEST;
    encode.encoder->maxQuantizer = 24;
    encode.encoder->minQuantizer = 22;

    // Call avifEncoderAddImage() for each image in your sequence
    // Only set AVIF_ADD_IMAGE_FLAG_SINGLE if you're not encoding a sequence
    // Use avifEncoderAddImageGrid() instead with an array of avifImage* to make a grid image
    avifResult addImageResult = avifEncoderAddImage(encode.encoder, image, 1,
                                                    AVIF_ADD_IMAGE_FLAG_SINGLE);
    if (addImageResult != AVIF_RESULT_OK) {
        LOGE("Failed to add image to encoder: %s\n", avifResultToString(addImageResult));
        return NULL;
    }

    avifResult finishResult = avifEncoderFinish(encode.encoder, &avifOutput);
    if (finishResult != AVIF_RESULT_OK) {
        LOGE("Failed to finish encode: %s\n", avifResultToString(finishResult));
        return NULL;
    }

    jbyteArray jarr = env->NewByteArray(avifOutput.size);
    env->SetByteArrayRegion(jarr, 0, avifOutput.size,
                            reinterpret_cast<const jbyte *>(avifOutput.data));

    if (image) {
        avifImageDestroy(image);
    }
    avifRWDataFree(&avifOutput);
    avifRGBImageFreePixels(&rgb); // Only use in conjunction with avifRGBImageAllocatePixels()

    return jarr;
}