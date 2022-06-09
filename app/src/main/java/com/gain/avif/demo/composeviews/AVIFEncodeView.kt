package com.gain.avif.demo.composeviews

import androidx.compose.foundation.layout.*
import androidx.compose.material.Button
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.livedata.observeAsState
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.navigation.NavController

@Composable
fun AVIFEncodeView(navController: NavController, avifViewModel: AVIFViewModel) {
    Column(
        verticalArrangement = Arrangement.Bottom,
        modifier = Modifier
            .fillMaxWidth()
            .fillMaxHeight()
            .padding(20.dp)
    ) {
        var hint = remember { mutableStateOf("AVIF file path will be here") }

        Box(contentAlignment = Alignment.Center,
            modifier = Modifier
                .weight(4f)
                .fillMaxWidth()) {
            val resFile = avifViewModel.encodedFile.observeAsState()
            if (resFile.value != null) {
                hint.value = "Encode done, AVIF file at " + resFile.value!!.absolutePath
            }

            Text(text = hint.value)
        }

        Button(
            onClick = {
                hint.value = "Encoding..."
                avifViewModel.encodeRGBA("images/dog.jpg")
                      },
            modifier = Modifier
                .fillMaxWidth()
                .height(40.dp)
                .weight(1f, fill = false)
        ) {
            Text(text = "Encode RGBA")
        }
        
        Spacer(modifier = Modifier.height(20.dp))

        Button(
            onClick = {
                hint.value = "Encoding..."
                avifViewModel.encodeYUV("images/4032x3024.y420", 4032, 3024)
            },
            modifier = Modifier
                .fillMaxWidth()
                .height(40.dp)
                .weight(1f, fill = false)
        ) {
            Text(text = "Encode YUV")
        }
    }
}