1 #include < iostream >
    2 #include < windows.h >
    3 #include < conio.h >
    4 #include < fstream >
    5 #include < vector >
    6 #include < string >
    7 #include < algorithm >
    8 #include < chrono >
    9 #include < thread >
    10 11 // Constants
    12 const int WIDTH = 80;
13 const int HEIGHT = 25;
14 const int GROUND_Y = HEIGHT - 3;
15 const int DINO_X = 10;
16 const int DINO_WIDTH = 6;
17 const int DINO_HEIGHT = 4;
18 const int CACTUS_WIDTH = 3;
19 const int CACTUS_HEIGHT = 4;
20 21 // Dinosaur states
    22 enum DinoState { RUNNING,
                        JUMPING,
                        DUCKING };
23 24 // Game states
    25 enum GameState { PLAYING,
                        GAME_OVER,
                        WAITING_RESTART };
26 27 struct Dinosaur
{
    28 int x;
    29 int y;
    30 int width;
    31 int height;
    32 DinoState state;
    33 bool isJumping;
    34 int jumpVelocity;
    35 const int jumpForce = -8;
    36 const int gravity = 1;
    37 38 Dinosaur() : x(DINO_X), y(GROUND_Y - DINO_HEIGHT), width(DINO_WIDTH), height(DINO_HEIGHT),
                       39 state(RUNNING), isJumping(false), jumpVelocity(0) {}
    40
};
41 42 struct Cactus
{
    43 int x;
    44 int y;
    45 int width;
    46 int height;
    47 bool active;
    48 49 Cactus() : x(WIDTH), y(GROUND_Y - CACTUS_HEIGHT), width(CACTUS_WIDTH), height(CACTUS_HEIGHT), active(false) {}
    50
};
51 52 class DinoGame
{
    53 private : 54 Dinosaur dino;
    55 std::vector<Cactus> cacti;
    56 int score;
    57 int highScore;
    58 int gameSpeed;
    59 const int baseSpeed = 5;
    60 const int speedIncreaseInterval = 500; // Increase speed every 500 points
    61 GameState state;
    62 HANDLE console;
    63 COORD cursorPos;
    64 bool showWelcome;
    65 66 void initConsole()
    {
        67 console = GetStdHandle(STD_OUTPUT_HANDLE);
        68 CONSOLE_CURSOR_INFO cursorInfo;
        69 GetConsoleCursorInfo(console, &cursorInfo);
        70 cursorInfo.bVisible = FALSE; // Hide cursor
        71 SetConsoleCursorInfo(console, &cursorInfo);
        72
    }
    73 74 void clearScreen()
    {
        75 system("cls");
        76
    }
    77 78 void setCursorPosition(int x, int y)
    {
        79 cursorPos.X = x;
        80 cursorPos.Y = y;
        81 SetConsoleCursorPosition(console, cursorPos);
        82
    }
    83 84 void drawBorder()
    {
        85 // Draw top border
            86 for (int i = 0; i < WIDTH; i++)
        {
            87 setCursorPosition(i, 0);
            88 std::cout << "-";
            89
        }
        90 // Draw bottom border (ground)
            91 for (int i = 0; i < WIDTH; i++)
        {
            92 setCursorPosition(i, HEIGHT - 1);
            93 std::cout << "-";
            94
        }
        95 // Draw side borders
            96 for (int i = 0; i < HEIGHT; i++)
        {
            97 setCursorPosition(0, i);
            98 std::cout << "|";
            99 setCursorPosition(WIDTH - 1, i);
            100 std::cout << "|";
            101
        }
        102
    }
    103 104 void drawDinosaur()
    {
        105 // Clear previous dinosaur position
            106 for (int dy = 0; dy < dino.height; dy++)
        {
            107 for (int dx = 0; dx < dino.width; dx++)
            {
                108 setCursorPosition(dino.x + dx, dino.y + dy);
                109 std::cout << " ";
                110
            }
            111
        }
        112 113 // Draw dinosaur based on state
            114 if (dino.state == RUNNING)
        {
            115 // Running dinosaur
                116 setCursorPosition(dino.x, dino.y);
            117 std::cout << "  _____ ";
            118 setCursorPosition(dino.x, dino.y + 1);
            119 std::cout << " |_   _|";
            120 setCursorPosition(dino.x, dino.y + 2);
            121 std::cout << "   | |   ";
            122 setCursorPosition(dino.x, dino.y + 3);
            123 std::cout << "   |_|   ";
            124
        }
        else if (dino.state == JUMPING)
        {
            125 // Jumping dinosaur (simplified)
                126 setCursorPosition(dino.x, dino.y);
            127 std::cout << "  _____ ";
            128 setCursorPosition(dino.x, dino.y + 1);
            129 std::cout << " |_   _|";
            130 setCursorPosition(dino.x, dino.y + 2);
            131 std::cout << "   | |   ";
            132 setCursorPosition(dino.x, dino.y + 3);
            133 std::cout << "   |_|   ";
            134
        }
        135
    }
    136 137 void spawnCactus()
    {
        138 // Find first inactive cactus or add new one
            139 for (auto &cactus : cacti)
        {
            140 if (!cactus.active)
            {
                141 cactus.x = WIDTH;
                142 cactus.active = true;
                143 return;
                144
            }
            145
        }
        146 // If all are active, add a new one
            147 Cactus newCactus;
        148 newCactus.x = WIDTH;
        149 newCactus.active = true;
        150 cacti.push_back(newCactus);
        151
    }
    152 153 void updateCacti()
    {
        154 for (auto &cactus : cacti)
        {
            155 if (cactus.active)
            {
                156 cactus.x -= gameSpeed;
                157 158 // Deactivate if off screen
                    159 if (cactus.x < -cactus.width)
                {
                    160 cactus.active = false;
                    161
                }
                162
            }
            163
        }
        164 165 // Spawn new cactus periodically
            166 static int spawnTimer = 0;
        167 spawnTimer++;
        168 if (spawnTimer > (60 / gameSpeed) * 2)
        { // Adjust spawn rate based on speed
            169 spawnTimer = 0;
            170 spawnCactus();
            171
        }
        172
    }
    173 174 void drawCacti()
    {
        175 for (const auto &cactus : cacti)
        {
            176 if (cactus.active)
            {
                177 // Draw cactus (simple rectangle)
                    178 for (int dy = 0; dy < cactus.height; dy++)
                {
                    179 for (int dx = 0; dx < cactus.width; dx++)
                    {
                        180 setCursorPosition(cactus.x + dx, cactus.y + dy);
                        181 std::cout << "#";
                        182
                    }
                    183
                }
                184
            }
            185
        }
        186
    }
    187 188 bool checkCollision()
    {
        189 for (const auto &cactus : cacti)
        {
            190 if (cactus.active)
            {
                191 // Check if dinosaur overlaps with cactus
                    192 if (dino.x < cactus.x + cactus.width &&
                            193 dino.x + dino.width > cactus.x &&
                            194 dino.y < cactus.y + cactus.height &&
                            195 dino.y + dino.height > cactus.y)
                {
                    196 return true;
                    197
                }
                198
            }
            199
        }
        200 return false;
        201
    }
    202 203 void updateDinosaur()
    {
        204 // Handle jump
            205 if (dino.isJumping)
        {
            206 dino.y += dino.jumpVelocity;
            207 dino.jumpVelocity += dino.gravity;
            208 209 // Check if landed
                210 if (dino.y >= GROUND_Y - dino.height)
            {
                211 dino.y = GROUND_Y - dino.height;
                212 dino.isJumping = false;
                213 dino.state = RUNNING;
                214
            }
            215
        }
        216
    }
    217 218 void updateScore()
    {
        219 score++;
        220 221 // Increase difficulty
            222 if (score % speedIncreaseInterval == 0 && score > 0)
        {
            223 gameSpeed = baseSpeed + (score / speedIncreaseInterval);
            224
        }
        225
    }
    226 227 void loadHighScore()
    {
        228 std::ifstream file("dino_highscore.txt");
        229 if (file.is_open())
        {
            230 file >> highScore;
            231 file.close();
            232
        }
        else
        {
            233 highScore = 0;
            234
        }
        235
    }
    236 237 void saveHighScore()
    {
        238 if (score > highScore)
        {
            239 highScore = score;
            240 std::ofstream file("dino_highscore.txt");
            241 if (file.is_open())
            {
                242 file << highScore;
                243 file.close();
                244
            }
            245
        }
        246
    }
    247 248 void drawScore()
    {
        249 setCursorPosition(2, 2);
        250 std::cout << "Score: " << score;
        251 setCursorPosition(2, 3);
        252 std::cout << "High Score: " << highScore;
        253
    }
    254 255 void drawGameOver()
    {
        256 setCursorPosition(WIDTH / 2 - 5, HEIGHT / 2 - 2);
        257 std::cout << "GAME OVER";
        258 setCursorPosition(WIDTH / 2 - 8, HEIGHT / 2);
        259 std::cout << "Score: " << score;
        260 setCursorPosition(WIDTH / 2 - 9, HEIGHT / 2 + 1);
        261 std::cout << "High Score: " << highScore;
        262 setCursorPosition(WIDTH / 2 - 10, HEIGHT / 2 + 3);
        263 std::cout << "Press SPACE to Restart";
        264
    }
    265 266 void drawWelcome()
    {
        267 setCursorPosition(WIDTH / 2 - 8, HEIGHT / 2 - 2);
        268 std::cout << "DINO GAME";
        269 setCursorPosition(WIDTH / 2 - 10, HEIGHT / 2);
        270 std::cout << "Press SPACE to Start";
        271 setCursorPosition(WIDTH / 2 - 12, HEIGHT / 2 + 2);
        272 std::cout << "Use UP arrow or SPACE to jump";
        273
    }
    274 275 void processInput()
    {
        276 if (_kbhit())
        {
            277 char ch = _getch();
            278 if (ch == ' ' || ch == 224)
            { // Space or extended key (arrow keys)
                279 if (ch == 224)
                {                      // Arrow key prefix
                    280 ch = _getch(); // Get the actual arrow key
                    281 if (ch == 72)
                    { // UP arrow
                        282 if (!dino.isJumping && state == PLAYING)
                        {
                            283 dino.isJumping = true;
                            284 dino.state = JUMPING;
                            285 dino.jumpVelocity = dino.jumpForce;
                            286
                        }
                        287
                    }
                    288
                }
                else if (ch == ' ')
                { // Space bar
                    289 if (state == WAITING_RESTART)
                    {
                        290 resetGame();
                        291
                    }
                    else if (state == PLAYING && !dino.isJumping)
                    {
                        292 dino.isJumping = true;
                        293 dino.state = JUMPING;
                        294 dino.jumpVelocity = dino.jumpForce;
                        295
                    }
                    else if (state == GAME_OVER)
                    {
                        296 resetGame();
                        297
                    }
                    else if (!showWelcome)
                    {
                        298 // Start game from welcome screen
                            299 showWelcome = false;
                        300 state = PLAYING;
                        301
                    }
                    302
                }
                303
            }
            304
        }
        305
    }
    306 307 void resetGame()
    {
        308 dino = Dinosaur();
        309 cacti.clear();
        310 score = 0;
        311 gameSpeed = baseSpeed;
        312 state = PLAYING;
        313 loadHighScore();
        314
    }
    315 316 void render()
    {
        317 clearScreen();
        318 drawBorder();
        319 320 if (showWelcome)
        {
            321 drawWelcome();
            322
        }
        else if (state == GAME_OVER)
        {
            323 drawGameOver();
            324
        }
        else
        {
            325 drawDinosaur();
            326 drawCacti();
            327 drawScore();
            328
        }
        329 330 // Flush console output
            331 std::cout.flush();
        332
    }
    333 334 public : 335 DinoGame() : score(0), highScore(0), gameSpeed(baseSpeed), state(WAITING_RESTART), showWelcome(true)
    {
        336 initConsole();
        337 loadHighScore();
        338
    }
    339 340 void run()
    {
        341 bool running = true;
        342 auto lastTime = std::chrono::high_resolution_clock::now();
        343 const double targetFPS = 60.0;
        344 const double frameTime = 1.0 / targetFPS;
        345 346 while (running)
        {
            347 auto currentTime = std::chrono::high_resolution_clock::now();
            348 double elapsed = std::chrono::duration<double>(currentTime - lastTime).count();
            347 auto currentTime = std::chrono::high_resolution_clock::now();
            348 double elapsed = std::chrono::duration<double>(currentTime - lastTime).count();
            349 350 if (elapsed >= frameTime)
            {
                351 lastTime = currentTime;
                352 353 processInput();
                354 355 if (state == PLAYING)
                {
                    356 updateDinosaur();
                    357 updateCacti();
                    358 359 if (checkCollision())
                    {
                        360 state = GAME_OVER;
                        361 saveHighScore();
                        362
                    }
                    else
                    {
                        363 updateScore();
                        364
                    }
                    365
                }
                366 367 render();
                368
            }
            else
            {
                369 // Sleep to avoid consuming too much CPU
                    370 std::this_thread::sleep_for(std::chrono::milliseconds(1));
                371
            }
            372
        }
        373
    }
    374
};
375 376 int main()
{
    377 DinoGame game;
    378 game.run();
    379 return 0;
    380
}
╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌ Do you want to create dino_game.cpp ?
