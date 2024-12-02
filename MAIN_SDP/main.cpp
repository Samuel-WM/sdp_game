#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHImages.h>
#include <string>
#include <vector>
#include <map>

// Updated Game States
#define MAIN_MENU 0
#define INSTRUCTIONS 1
#define STATS 2
#define CREDITS 3
#define BIOME_SELECT 4
#define DESERT_BIOME 5
#define TUNDRA_BIOME 6
#define FOREST_BIOME 7
#define SAFARI_BIOME 8
#define QUESTION_STATE 9

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Menu Button Constants
#define MENU_BUTTON_WIDTH 150
#define MENU_BUTTON_HEIGHT 30
#define MENU_BUTTON_SPACING 10

// Question UI constants
#define QUESTION_BOX_X 0
#define QUESTION_BOX_Y 20
#define QUESTION_BOX_WIDTH 320  
#define QUESTION_BOX_HEIGHT 200 
#define ANSWER_BUTTON_HEIGHT 30 
#define ANSWER_SPACING 35  

struct ClickableRegion {
    int x, y, width, height;
    std::string name;
    std::string question;
    std::string correct_answer;
    std::vector<std::string> answers;
    bool visited;  
};

// Helper function to check if a touch is within a rectangular region
bool isTouchInRegion(float touchX, float touchY, float x, float y, float width, float height) {
    return (touchX >= x && touchX <= x + width && touchY >= y && touchY <= y + height);
}


struct MenuButton {
    int x;
    int y;
    int width;
    int height;
    std::string text;
    
    MenuButton(int _x, int _y, int _w, int _h, const std::string& _text) 
        : x(_x), y(_y), width(_w), height(_h), text(_text) {}
};

class GameState {
public:
    int currentState;
    int previousState;
    int totalCoins;
    int totalLives;
    ClickableRegion* currentQuestion;
    
    GameState() {
        currentState = MAIN_MENU;  
        previousState = MAIN_MENU;
        totalCoins = 0;
        totalLives = 3;
        currentQuestion = nullptr;
    }
};

/*
Try removing this function. I feel like
its easier manually doing this as the functionality is 
iffy
*/
void drawCenteredText(const char* text, int y, int fontSize = 1) {
    int textWidth = strlen(text) * 12 * fontSize;  
    int x = (SCREEN_WIDTH - textWidth) / 2;
    if(fontSize > 1) {
        LCD.SetFontColor(WHITE);
    }
    LCD.WriteAt(text, x, y);
}

std::vector<MenuButton> createMainMenuButtons() {
    int startY = 80;
    std::vector<MenuButton> buttons = {
        {(SCREEN_WIDTH - MENU_BUTTON_WIDTH) / 2, 
         startY, 
         MENU_BUTTON_WIDTH, 
         MENU_BUTTON_HEIGHT, 
         "Play Game"},

        {(SCREEN_WIDTH - MENU_BUTTON_WIDTH) / 2, 
         startY + (MENU_BUTTON_HEIGHT + MENU_BUTTON_SPACING), 
         MENU_BUTTON_WIDTH, 
         MENU_BUTTON_HEIGHT, 
         "Stats"},
        
        {(SCREEN_WIDTH - MENU_BUTTON_WIDTH) / 2, 
         startY + 2* (MENU_BUTTON_HEIGHT + MENU_BUTTON_SPACING), 
         MENU_BUTTON_WIDTH, 
         MENU_BUTTON_HEIGHT, 
         "Instructions"},
        
        {(SCREEN_WIDTH - MENU_BUTTON_WIDTH) / 2, 
         startY + 3 * (MENU_BUTTON_HEIGHT + MENU_BUTTON_SPACING), 
         MENU_BUTTON_WIDTH, 
         MENU_BUTTON_HEIGHT, 
         "Credits"},

  
    };
    return buttons;
}

void drawMenuButton(const MenuButton& button) {
    // Draw button background
    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(button.x, button.y, button.width, button.height);
    LCD.SetFontColor(BLACK);
    LCD.FillRectangle(button.x + 2, button.y + 2, button.width - 4, button.height - 4);
    
    // Draw button text
    LCD.SetFontColor(WHITE);
    int textX = button.x + (button.width - button.text.length() * 12) / 2;
    int textY = button.y + (button.height - 12) / 2;
    LCD.WriteAt(button.text.c_str(), textX, textY);
}


void drawBackButton() {
    // Draw back button
    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(10, 10, 60, 30);
    // Inner border
    LCD.DrawRectangle(12, 12, 56, 26);
    LCD.SetFontColor(BLACK);
    LCD.FillRectangle(13, 13, 54, 24);
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Back", 16, 15);
}

void handleMainMenu(GameState& gameState, float& touchX, float& touchY) {
    // Clear the screen and set up the main menu UI
    LCD.Clear(BLACK);
    
    // Load and draw the main menu background
    FEHImage main;
    main.Open("home.png");
    main.Draw(0, 0);
    main.Close(); 
    
    // Display menu title and subtitle
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("EcoQuest", 105, 20);
    LCD.WriteAt("Go on an Adventure", 50, 50);
    

    std::vector<MenuButton> buttons = createMainMenuButtons();
    for (const auto& button : buttons) {
        drawMenuButton(button);
    }
    
    while(!LCD.Touch(&touchX,&touchY)) {};

       
    if (touchX >= 0 && touchY >= 0) {
        for (size_t i = 0; i < buttons.size(); i++) {
            const auto& button = buttons[i];
            if (isTouchInRegion(touchX, touchY, button.x, button.y, button.width, button.height)) {
                gameState.previousState = gameState.currentState;  
                switch (i) {
                    
                    case 0:  // Play Game
                        gameState.currentState = BIOME_SELECT;
                        break;
                    case 1:  // Instructions
                        gameState.currentState = STATS;
                        break;
                    case 2:  // Credits
                        gameState.currentState = INSTRUCTIONS;
                        break;
                    case 3: 
                        gameState.currentState = CREDITS;
                        break;

                }
                touchX = -1;
                touchY = -1;
                Sleep(0.2);
                break;
            }
        }
    }
    
   
    
}

void handleStats(GameState& gameState, float touchX, float touchY) {
   

    
    
        FEHImage stats;
        stats.Open("stats.png");
        stats.Draw(0, 0);
        stats.Close();
    
        LCD.SetFontColor(WHITE);
        LCD.WriteAt("Stats: 0", 50, 65);
        LCD.WriteAt("Coins: 0", 70, 90);
        LCD.WriteAt("Best Play:", 70, 120);


        drawBackButton();
    
    
    // Check for back button click
    if (touchX >= 10 && touchX <= 70 && touchY >= 10 && touchY <= 40) {
        gameState.currentState = MAIN_MENU;
    }
}



// function to display instructions
void handleInstructions(GameState& gameState, float touchX, float touchY) {
    static bool needsRedraw = true;
    
    if(needsRedraw) {
        LCD.Clear(BLACK);
        FEHImage instruct;
        instruct.Open("instruct.png");
        instruct.Draw(0, 0);
        
        LCD.SetFontColor(BLACK);
        
        
        LCD.WriteAt("How To Play:", 10, 45);
        LCD.WriteAt("1. Choose a biome ", 20, 70);
        LCD.WriteAt("2. Click on animals", 20, 100);
        LCD.WriteAt("3. Answer questions  ", 20, 130);
        LCD.WriteAt("4. Watch your lives", 20, 160);
        LCD.WriteAt("5. Visit all animals ", 20, 190);

        
        
        // Add back button
        drawBackButton();
        needsRedraw = false;
    }
        
    if(touchX >= 10 && touchX <= 70 && touchY >= 10 && touchY <= 40) {
        gameState.currentState = MAIN_MENU;
        needsRedraw = true;
    }
    
}

//  handleCredits to include a back button
void handleCredits(GameState& gameState, float touchX, float touchY) {
    static bool needsRedraw = true;
    
    if(needsRedraw) {
        LCD.Clear(BLACK);
        LCD.SetFontColor(BLACK);
        
        FEHImage credit;
        credit.Open("credits.png");
        credit.Draw(0, 0);        

        LCD.WriteAt("Development Team:", 20, 55);
        LCD.WriteAt("Samuel Wales-McGrath ", 20, 80);
        LCD.WriteAt("Vamshi Somapoli ", 20, 110);
        LCD.WriteAt("", 20, 70);
        LCD.WriteAt("Special Thanks To:", 20, 150);
        LCD.WriteAt("FEH, Ethan Joll, and TAs", 20, 175);
        LCD.WriteAt("As well as ClassMates", 20, 200);
        
        //  back button
        drawBackButton();
        needsRedraw = false;
    }
    
    
    //Check for back button click
    if(touchX >= 10 && touchX <= 70 && touchY >= 10 && touchY <= 40) {
        gameState.currentState = MAIN_MENU;
        needsRedraw = true;
    }
    
}


// Image loading helper
bool loadAndDrawImage(const char* filename) {
    FEHImage image;
    image.Open(filename);
    image.Draw(0, 0);
    return true;
}

std::vector<ClickableRegion> getBiomeRegions() {
    return {
        {0, 0, 160, 120, "Desert", "", "", {}, false},
        {160, 0, 320, 120, "Tundra", "", "", {}, false},
        {0, 120, 160, 240, "Safari", "", "", {}, false},
        {160, 120, 320, 240, "Forest", "", "", {}, false}
    };
}

// Animal information for each biome
std::map<int, std::vector<ClickableRegion>> getBiomeAnimals() {
    std::map<int, std::vector<ClickableRegion>> biomeAnimals;
    
    // Desert animals
    biomeAnimals[DESERT_BIOME] = {
        {80, 104, 66, 87, "Camel", 
         "How do camels survive in the desert?",
         "Store water in humps",
         {"Store water in humps", "Drink cactus juice", "Sleep during day", "Eat sand"},
         false},
        {162, 158, 100, 43, "Lizard", 
         "What defense mechanism does a horned lizard have?",
         "Shoots blood from eyes",
         {"Shoots blood from eyes", "Changes color", "Grows larger", "Becomes invisible"},
         false},
        {157, 71, 77, 96, "Snake",
         "How does a sidewinder snake move in hot sand?",
         "Sideways motion",
         {"Sideways motion", "Jumping", "Rolling", "Swimming"},
         false}
    };
    
    // Tundra animals
    biomeAnimals[TUNDRA_BIOME] = {
        {125, 110, 48, 25, "Polar Bear",
         "How do polar bears stay warm ?",
         "Thick blubber layer",
         {"Thick blubber layer", "Hot springs", "Underground caves", "Constant movement"},
         false},
        
        {193, 190, 89, 27, "Orca",
         "Primary diet of orca?",
         "Fish",
         {"Plankton", "Fish", "Seaweed", "Insects"},
         false},
        
        {103, 103, 11, 23, "Penguin 1",
         "What helps penguin swim efficiently?",
         "Flipper-like wings",
         {"Webbed feet", "Flipper-like wings", "Claws", "Long tails"},
         false},
        
        {83, 160, 27, 30, "Penguin 2",
         "What species of penguin is largest",
         "Emperor",
         {"King", "Adelie", "Chinstrap", "Emperor"},
         false}
    };
    
    // Forest animals (Temperate)
    biomeAnimals[FOREST_BIOME] = {
        {68, 105, 44, 25, "Black Bear",
         "What do black bears eat for hibernation?",
         "Berries and nuts",
         {"Insects", "Fish", "Berries and nuts", "Small mammals"},
         false},
        
        {182, 131, 31, 25, "Red Fox",
         "What is a  hunting strategy used by a fox?",
         "Ambushing prey with a pounce",
         {"Digging for food", "Ambushing prey with a pounce", "Waiting in trees for prey", "Hunting in large packs"},
         false},
        
        {153, 159, 13, 14, "Bunny",
         "What is a bunny's source of nutrition?",
         "Grass and leafy plants",
         {"Grass and leafy plants", "Nuts and seeds", "Insects", "Fish"},
         false}
    };
    
    // Safari animals
    biomeAnimals[SAFARI_BIOME] = {
        {193, 75, 53, 94, "Giraffe",
         "Why do giraffes have long necks?",
         "Reach tall trees",
         {"Reach tall trees", "See predators", "Stay cool", "Look pretty"},
         false},
        
        {147, 127, 39, 40, "Zebra",
         "What is the  purpose of a zebra's stripes?",
         "Confusing predators ",
         {"Camouflage ", "Attracting mates", "Confusing predators ", "Regulate body temperature"},
         false},
        
        {80, 123, 41, 45, "Lion",
         "Which lions do most of the hunting?",
         "Female lions",
         {"Female lions", "Male lions", "Cubs", "All hunt equally"},
         false}
    };
    
    return biomeAnimals;
}

bool checkRegionClick(float x, float y, const ClickableRegion& region) {
    return (x >= region.x && x <= region.x + region.width &&
            y >= region.y && y <= region.y + region.height);
}



void drawQuestion(const ClickableRegion& animal) {
    LCD.Clear();
    
    // Draw background overlay
    LCD.SetFontColor(BLACK);
    LCD.FillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // Draw question box
    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(QUESTION_BOX_X - 2, QUESTION_BOX_Y - 2, 
                     QUESTION_BOX_WIDTH + 4, QUESTION_BOX_HEIGHT + 4);
    LCD.SetFontColor(BLACK);
    LCD.FillRectangle(QUESTION_BOX_X, QUESTION_BOX_Y, 
                     QUESTION_BOX_WIDTH, QUESTION_BOX_HEIGHT);
    
    LCD.SetFontColor(WHITE);

    std::string question = animal.question;
    int maxCharsPerLine = 25;
    int yOffset = 0;
    
    while (question.length() > maxCharsPerLine ) {
        int splitPos = question.substr(0, maxCharsPerLine).find_last_of(" ");
        if (splitPos == std::string::npos) splitPos = maxCharsPerLine;
        
        LCD.WriteAt(question.substr(0, splitPos).c_str(), 
                   QUESTION_BOX_X + 10, 
                   QUESTION_BOX_Y + 15 + yOffset);
        
        question = question.substr(splitPos + 1);
        yOffset += 15;
    }
    LCD.WriteAt(question.c_str(), 
               QUESTION_BOX_X + 10, 
               QUESTION_BOX_Y + 15 + yOffset);
    
    // Draw answer buttons with new dimensions
    for(int i = 0; i < animal.answers.size(); i++) {
        int buttonY = QUESTION_BOX_Y + 50 + (i * 30);
        
        LCD.SetFontColor(WHITE);
        LCD.DrawRectangle(QUESTION_BOX_X + 10, buttonY, 
                         QUESTION_BOX_WIDTH - 20, ANSWER_BUTTON_HEIGHT);
        
        LCD.SetFontColor(BLACK);
        LCD.FillRectangle(QUESTION_BOX_X + 11, buttonY + 1, 
                         QUESTION_BOX_WIDTH - 22, ANSWER_BUTTON_HEIGHT - 2);
        
        LCD.SetFontColor(WHITE);
        LCD.WriteAt(animal.answers[i].c_str(), 
                   QUESTION_BOX_X + 15, buttonY + 3);
    }
}




void drawStatusBar(int coins, int lives) {
    // Create status bar background
    LCD.SetFontColor(BLACK);
    LCD.FillRectangle(0, SCREEN_HEIGHT - 30, SCREEN_WIDTH, 30);
    
    // Draw coin icon and count
    FEHImage coinIcon;
    coinIcon.Open("coin.png");
    coinIcon.Draw(14, SCREEN_HEIGHT - 30);
    
    LCD.SetFontColor(WHITE);
    LCD.WriteAt(coins, 45, SCREEN_HEIGHT - 22);
    
    // Draw hearts for lives
    FEHImage heartIcon;
    heartIcon.Open("heart.png");
    for(int i = 0; i < lives; i++) {
        heartIcon.Draw(185 + (i * 35), SCREEN_HEIGHT - 30);
    }
}



void handleBiomeSelect(GameState& gameState, float& touchX, float& touchY, 
                       const std::vector<ClickableRegion>& biomeRegions) {
    // Clear and draw the biome selection screen
    LCD.Clear(BLACK);
    loadAndDrawImage("biomes1.png");
    
    
    // Display the title and status bar
    LCD.SetFontColor(WHITE);
    drawCenteredText("Pick Your Biome", 111); // Title
    drawStatusBar(gameState.totalCoins, gameState.totalLives); // Coins and lives
    drawBackButton(); // Draw back button in a consistent location

    // Handle touch input
    if (touchX >= 0 && touchY >= 0) {
        // Check the back button first
        if (isTouchInRegion(touchX, touchY, 10, 10, 60, 30)) { // Back button coordinates
            gameState.currentState = MAIN_MENU;
            touchX = -1; // Reset touch to avoid accidental multiple clicks
            touchY = -1;
            return;
        }

        // Check biome regions
        for (const auto& region : biomeRegions) {
            if (checkRegionClick(touchX, touchY, region)) {
                gameState.previousState = gameState.currentState;
                
                // Set the game state based on the clicked region
                if (region.name == "Desert") {
                    gameState.currentState = DESERT_BIOME;
                } else if (region.name == "Tundra") {
                    gameState.currentState = TUNDRA_BIOME;
                } else if (region.name == "Forest") {
                    gameState.currentState = FOREST_BIOME;
                } else if (region.name == "Safari") {
                    gameState.currentState = SAFARI_BIOME;
                }
                
                Sleep(0.2);

                touchX = -1; // Reset touch
                touchY = -1;
                return;
            }
        }
    }
}


bool handleQuestionInput(GameState& gameState, float touchX, float touchY) {
    if (!gameState.currentQuestion) return false;
    
    for(int i = 0; i < gameState.currentQuestion->answers.size(); i++) {
        int buttonY = QUESTION_BOX_Y + 50 + (i * 30); 
        
        if (touchX >= QUESTION_BOX_X + 10 && 
            touchX <= QUESTION_BOX_X + QUESTION_BOX_WIDTH - 20 &&
            touchY >= buttonY && 
            touchY <= buttonY + ANSWER_BUTTON_HEIGHT) {
            Sleep(1.0);
            LCD.Clear();
            
            if (gameState.currentQuestion->answers[i] == 
                gameState.currentQuestion->correct_answer) {
                LCD.SetFontColor(GREEN);
                drawCenteredText("Correct!", SCREEN_HEIGHT/2 - 10);
                if (!gameState.currentQuestion->visited) {
                    gameState.totalCoins += 10;
                    gameState.currentQuestion->visited = true;
                }
            } else {
                
                LCD.SetFontColor(RED);
                drawCenteredText("Wrong!", SCREEN_HEIGHT/2 - 10);
                gameState.totalLives--;
            }
            
            Sleep(1.0);
            gameState.currentState = gameState.previousState; // This will return to the biome page
            gameState.currentQuestion = nullptr;
            return true;
        }
    }
    return false;
}


void handleQuestionState(GameState& gameState, float touchX, float touchY) {
    static bool questionDrawn = false;
    
    if (!questionDrawn) {
        drawQuestion(*gameState.currentQuestion);
        questionDrawn = true;
        return;
    }
    
    if (LCD.Touch(&touchX, &touchY)) {
        if (handleQuestionInput(gameState, touchX, touchY)) {
            questionDrawn = false;
        }
    }
}
  
void handleBiome(GameState& gameState, int biomeState, const char* imageFile, 
                 std::map<int, std::vector<ClickableRegion>>& biomeAnimals,
                 float touchX, float touchY) {

    
    while (true){
        loadAndDrawImage(imageFile);
        drawBackButton();
        drawStatusBar(gameState.totalCoins, gameState.totalLives);
        
        while(!LCD.Touch(&touchX,&touchY)) {};
        /* Wait until the touch releases */
        while(LCD.Touch(&touchX,&touchY)) {};
        
        // Handle touch input
        if (touchX >= 0 && touchY >= 0) {
            // Check for back button 
            if (isTouchInRegion(touchX, touchY, 10, 10, 60, 30)) {
                gameState.currentState = BIOME_SELECT;
                Sleep(0.2);
                return;
            }

            // Check for animal clicks 
            auto& animals = biomeAnimals[biomeState];
            for (auto& animal : animals) {
                if (isTouchInRegion(touchX, touchY, animal.x, animal.y, animal.width, animal.height)) {
                    gameState.currentQuestion = &animal;
                    gameState.previousState = biomeState;
                    gameState.currentState = QUESTION_STATE;
                    return;
                }
            }
        }
        LCD.Clear();
    }   
}


int main() {
    // Initialize game state and touch variables
    GameState gameState;
    float touchX = -1, touchY = -1;
    bool touched = false;
    
    // Initialize  components
    std::vector<ClickableRegion> biomeRegions = getBiomeRegions();
    std::map<int, std::vector<ClickableRegion>> biomeAnimals = getBiomeAnimals();
    
    // Initialize display
    LCD.Clear(BLACK);
    
    // Track previous state
    int lastState = -1;
    
    // Main game loop
    while(1) {
        // Handle touch 
        touched = LCD.Touch(&touchX, &touchY);
        if (!touched) {
            touchX = -1;
            touchY = -1;
        }
        
        // Clear screen  when state changes
        if (lastState != gameState.currentState) {
            LCD.Clear(BLACK);
            lastState = gameState.currentState;
        }
        
        // Handle current state
        switch(gameState.currentState) {
            case MAIN_MENU:
                handleMainMenu(gameState, touchX, touchY);
                break;
            
            case STATS:
                handleStats(gameState, touchX, touchY);
                break;
                
            case INSTRUCTIONS:
                handleInstructions(gameState, touchX, touchY);
                break;
                
            case CREDITS:
                handleCredits(gameState, touchX, touchY);
                break;
                
            case BIOME_SELECT:
                handleBiomeSelect(gameState, touchX, touchY, biomeRegions);
                break;
                
            case DESERT_BIOME:
                handleBiome(gameState, DESERT_BIOME, "desert.png", 
                          biomeAnimals, touchX, touchY);
                break;
                
            case TUNDRA_BIOME:
                handleBiome(gameState, TUNDRA_BIOME, "tundra.png", 
                          biomeAnimals, touchX, touchY);
                break;
                
            case FOREST_BIOME:
                handleBiome(gameState, FOREST_BIOME, "temperate.png", 
                          biomeAnimals, touchX, touchY);
                break;
                
            case SAFARI_BIOME:
                handleBiome(gameState, SAFARI_BIOME, "safari.png", 
                          biomeAnimals, touchX, touchY);
                break;
                
            case QUESTION_STATE:
                if(gameState.currentQuestion) {
                    handleQuestionState(gameState, touchX, touchY);
                } else {
                    // Safely handle invalid question state
                    gameState.currentState = gameState.previousState;
                    LCD.Clear(BLACK);
                }
                break;
            

                
            default:
                //Handle invalid state by returning to main menu
                gameState.currentState = MAIN_MENU;
                LCD.Clear(BLACK);
                break;
        }
        
        // Check for game over condition
        if(gameState.totalLives <= 0) {
            // Clear and show game over screen
            LCD.Clear(BLACK);
            
            // Draw game over text
            LCD.SetFontColor(RED);
            drawCenteredText("Game Over!", 100);
            LCD.SetFontColor(WHITE);
            drawCenteredText("Final Score:", 120);
            
            // Display final score
            char scoreStr[20];
            sprintf(scoreStr, "%d", gameState.totalCoins);
            drawCenteredText(scoreStr, 140);
            
            Sleep(3.0);
            
            gameState = GameState();
            LCD.Clear(BLACK);
            lastState = -1;  
            continue;
        }
        
        if(gameState.currentState < 0 || gameState.currentState > QUESTION_STATE) {
            gameState.currentState = MAIN_MENU;
            LCD.Clear(BLACK);
            lastState = -1;
        }
        
        // Update  display
        LCD.Update();
        
        // Small delay to prevent screen flickerhhh
        Sleep(0.016);  
    }
    
    return 0;
}