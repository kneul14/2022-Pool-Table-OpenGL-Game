#include "BGMusic.h"
#define MISCFILEPATH "C:/UniversityOrebro/2022-Pool-Table-OpenGL-Game/Physics and Graphics VS Coursework/Music"

int BGMusic::main()
{
    std::cout << "Music is playing \n";
    PlaySound(TEXT("for-elevator-jazz-music-WAV.wav"), NULL, SND_FILENAME | SND_SYNC);
    std::cout << "Music is not playing \n";

    return 0;
}
