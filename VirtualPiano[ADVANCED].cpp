1 #include < iostream >
    2 #include < windows.h >
    3 #include < conio.h >
    4 #include < map >
    5 #include < string >
    6 7 // Function to play a note using Windows Beep function
    8 void playNote(int frequency, int duration)
{
    9 Beep(frequency, duration);
    10
}
11 12  // Map keyboard keys to frequencies (in Hz)
    13 // Using C4 (middle C) = 261.63 Hz as base
    14 std::map<char, int>
        keyToFrequency = {
            15             // White keys (C D E F G A B)
            16 {'a', 262}, // C4
            17 {'s', 294}, // D4
            18 {'d', 330}, // E4
            19 {'f', 349}, // F4
            20 {'g', 392}, // G4
            21 {'h', 440}, // A4
            22 {'j', 494}, // B4
            23 {'k', 523}, // C5
            24 {'l', 587}, // D5
            25 {';', 659}, // E5
            26 27          // Black keys (C#/Db, D#/Eb, F#/Gb, G#/Ab, A#/Bb)
            28 {'w', 277}, // C#4/Db4
            29 {'e', 311}, // D#4/Eb4
            30 {'t', 370}, // F#4/Gb4
            31 {'y', 415}, // G#4/Ab4
            32 {'u', 466}, // A#4/Bb4
            33 {'o', 554}, // C#5/Db5
            34 {'p', 622}, // D#5/Eb5
            35             // Note: We're missing some black keys, but this gives a basic range
            36};
37 38 int main()
{
    39 std::cout << "Virtual Piano\n";
    40 std::cout << "=============\n";
    41 std::cout << "White keys: A S D F G H J K L ;\n";
    42 std::cout << "Black keys: W E T Y U O P\n";
    41 std::cout << "White keys: A S D F G H J K L ;\n";
    42 std::cout << "Black keys: W E T Y U O P\n";
    43 std::cout << "Press ESC to exit\n\n";
    44 45 const int noteDuration = 200; // milliseconds
    46 47 while (true)
    {
        48 if (_kbhit())
        {
            49 char key = _getch();
            50 51 // Exit on ESC
                52 if (key == 27)
            {
                53 break;
                54
            }
            55 56 // Convert to lowercase for consistency
                57 key = tolower(key);
            58 59 // Check if key is mapped
                60 if (keyToFrequency.find(key) != keyToFrequency.end())
            {
                61 int frequency = keyToFrequency[key];
                62 std::cout << "Playing note: " << key << " (" << frequency << " Hz)\n";
                63 playNote(frequency, noteDuration);
                64
            }
            65 // Optional: handle other keys
                66
        }
        67 68 // Small delay to prevent high CPU usage
            69 Sleep(10);
        70
    }
    71 72 std::cout << "Exiting virtual piano.\n";
    73 return 0;
    74
}