while true; do adb shell dumpsys media.audio_flinger | grep -i "Head position"; sleep 0.1; done
