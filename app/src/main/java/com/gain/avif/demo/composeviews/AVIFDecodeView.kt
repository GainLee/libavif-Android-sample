package com.gain.avif.demo.composeviews

import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.*
import androidx.compose.material.Button
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.livedata.observeAsState
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.asImageBitmap
import androidx.compose.ui.unit.dp
import androidx.navigation.NavController
import com.gain.avif.demo.ui.navigation.NavigationScreen

@Composable
fun AVIFDecodeView(navController: NavController, avifViewModel: AVIFViewModel) {
    Column(
        verticalArrangement = Arrangement.Bottom,
        modifier = Modifier
            .fillMaxWidth()
            .fillMaxHeight()
            .padding(horizontal = 8.dp)
    ) {
        val resBitmap = avifViewModel.decodedBitmap.observeAsState()
        if (resBitmap.value != null) {
            Image(
                bitmap = resBitmap.value!!.asImageBitmap(),
                contentDescription = "Decoded bitmap",
            )
        }

        Button(
            onClick = { avifViewModel.decode("images/android.avif") },
            modifier = Modifier.fillMaxWidth()
        ) {
            Text(text = "Decode AVIF")
        }

        Button(
            onClick = { navController.navigate(NavigationScreen.AVIF_ENCODE.name) },
            modifier = Modifier.fillMaxWidth()
        ) {
            Text(text = "Encode AVIF")
        }
    }
}