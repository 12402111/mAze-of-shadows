
#include <windows.h>
#include <mmsystem.h>

int main() {
    PlaySoundA("assets/sounds/bgm1.wav", NULL, SND_ASYNC | SND_LOOP);
    MessageBoxA(0, "Sound should be playing...", "Sound Test", 0);
    Sleep(5000); // wait 5 seconds before ending
    return 0;
}
