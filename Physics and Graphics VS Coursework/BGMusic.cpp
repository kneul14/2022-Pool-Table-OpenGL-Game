#include "BGMusic.h"

int BGMusic::main()
{    
    std::cout << "Music is playing \n";
    PlaySound(TEXT("for-elevator-jazz-music-WAV.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    std::cout << "Music is not playing \n";

    return 0;
}
