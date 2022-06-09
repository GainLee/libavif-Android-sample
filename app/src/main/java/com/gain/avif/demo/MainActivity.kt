package com.gain.avif.demo

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.viewModels
import androidx.compose.animation.ExperimentalAnimationApi
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import com.gain.avif.demo.composeviews.AVIFEncodeView
import com.gain.avif.demo.composeviews.AVIFDecodeView
import com.gain.avif.demo.composeviews.AVIFViewModel
import com.gain.avif.demo.ui.navigation.EnterAnimation
import com.gain.avif.demo.ui.navigation.NavigationScreen
import com.gain.avif.demo.ui.theme.Libavif_androidTheme

class MainActivity : ComponentActivity() {
    private val mAVIFViewModel: AVIFViewModel by viewModels()

    @OptIn(ExperimentalAnimationApi::class)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContent {
            Libavif_androidTheme{
                val navController = rememberNavController()

                NavHost(navController, startDestination = NavigationScreen.AVIF_DECODE.name) {
                    composable(NavigationScreen.AVIF_DECODE.name) {
                        EnterAnimation {
                            AVIFDecodeView(navController, mAVIFViewModel)
                        }
                    }

                    composable(NavigationScreen.AVIF_ENCODE.name) {
                        EnterAnimation {
                            AVIFEncodeView(navController, mAVIFViewModel)
                        }
                    }
                }
            }
        }
    }
}