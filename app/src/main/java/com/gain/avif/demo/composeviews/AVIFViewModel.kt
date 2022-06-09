package com.gain.avif.demo.composeviews

import android.app.Application
import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import androidx.lifecycle.*
import com.gain.libavif.AvifCodec
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.io.File
import java.io.FileOutputStream
import java.nio.ByteBuffer
import java.text.SimpleDateFormat
import java.util.*

class AVIFViewModel(var app:Application): AndroidViewModel(app) {
    private val _decodedBitmap: MutableLiveData<Bitmap> = MutableLiveData()
    val decodedBitmap: LiveData<Bitmap> = _decodedBitmap

    private val _encodedFile: MutableLiveData<File> = MutableLiveData()
    val encodedFile: LiveData<File> = _encodedFile

    fun decode(filePath: String, bitmap:MutableLiveData<Bitmap> = _decodedBitmap) {
        viewModelScope.launch(Dispatchers.Main) {
            bitmap.postValue(decodeAvif(filePath))
        }
    }

    fun encodeRGBA(filePath: String) {
        viewModelScope.launch(Dispatchers.Main) {
            _encodedFile.postValue(encodeRGBToAvif(filePath))
        }
    }

    fun encodeYUV(filePath: String, width: Int, height: Int) {
        viewModelScope.launch(Dispatchers.Main) {
            _encodedFile.postValue(encodeYUVToAvif(filePath, width, height))
        }
    }

    private suspend fun encodeYUVToAvif(yuvFilePath: String, width: Int, height: Int): File = withContext(Dispatchers.IO) {
        var inputStream = app.assets.open(yuvFilePath)

        var yBuffer = ByteBuffer.allocateDirect(width * height)
        var uBuffer = ByteBuffer.allocateDirect(width * height/4)
        var vBuffer = ByteBuffer.allocateDirect(width * height/4)

        inputStream.use {
            var byteArray = ByteArray(width * height)
            it.read(byteArray)
            yBuffer.put(byteArray)

            byteArray = ByteArray(width * height/4)
            it.read(byteArray)
            uBuffer.put(byteArray)

            it.read(byteArray)
            vBuffer.put(byteArray)
        }
        var avifBytes = AvifCodec.encodeY420(
            yBuffer,
            uBuffer,
            vBuffer,
            width,
            width/2,
            width/2,
            width,
            height
        )

        var avifFile = createFile(app, "avif")
        var fos = FileOutputStream(avifFile)
        fos.use {
            it.write(avifBytes)
            it.flush()
        }

        avifFile
    }

    private suspend fun encodeRGBToAvif(bmpPath: String): File = withContext(Dispatchers.IO) {
        var inputStream = app.assets.open(bmpPath)
        var bitmap = BitmapFactory.decodeStream(inputStream)

        var pixelBuffer = ByteBuffer.allocateDirect(bitmap.width * bitmap.height * 4)
        bitmap.copyPixelsToBuffer(pixelBuffer)
        var avifBytes = AvifCodec.encodeRGBA8888(
            pixelBuffer,
            bitmap.width * bitmap.height * 4,
            bitmap.width,
            bitmap.height
        )

        var avifFile = createFile(app, "avif")
        var fos = FileOutputStream(avifFile)
        fos.use {
            it.write(avifBytes)
            it.flush()
        }

        avifFile
    }

    private suspend fun decodeAvif(filePath: String): Bitmap = withContext(Dispatchers.IO) {
        var inputStream =  app.assets.open(filePath)

        var bufArray = inputStream.use {
            it.readBytes()
        }

        var byteBuf = ByteBuffer.allocateDirect(bufArray.size)
        byteBuf.put(bufArray)
        byteBuf.position(0)

        var isAvifImage = AvifCodec.isAvifImage(byteBuf)
        var imgInfo = AvifCodec.Info()
        AvifCodec.getInfo(byteBuf, bufArray.size, imgInfo)
        if (imgInfo.depth > 1) {
            //TODO
        }
        byteBuf.position(0)
        var resultBmp = Bitmap.createBitmap(imgInfo.width, imgInfo.height, Bitmap.Config.ARGB_8888)
        AvifCodec.decode(byteBuf, byteBuf.capacity(), resultBmp)

        resultBmp
    }

    private fun createFile(context: Context, extension: String): File {
        val sdf = SimpleDateFormat("yyyy_MM_dd_HH_mm_ss_SSS", Locale.US)
        return File(context.getExternalFilesDir("avif"), "IMG_${sdf.format(Date())}.$extension")
    }
}