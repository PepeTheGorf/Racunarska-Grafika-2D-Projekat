#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Header/Util.h"
#include "../Header/Button.h"
#include "../Header/Patty.h"
#include "../Header/Stove.h"
#include "../Header/Table.h"
#include "../Header/LoadingBar.h"
#include "../Header/Ingredient.h"

#include <iostream>
#include <thread>

// Main fajl funkcija sa osnovnim komponentama OpenGL programa

// Projekat je dozvoljeno pisati počevši od ovog kostura
// Toplo se preporučuje razdvajanje koda po fajlovima (i eventualno potfolderima) !!!
// Srećan rad!

bool orderPlaced = false;
bool burgerDone = false;
Button* orderButton = nullptr;
Patty* patty = nullptr;
Button* student = nullptr;
Button* endMessage = nullptr;
Stove* stove = nullptr;
Table* table = nullptr;
LoadingBar* loadingBar = nullptr;

GLFWcursor* cursor;

float lastTime = 0.0f;

float previousStackY = -0.7f; 

unsigned ketchuoSpillTex, mustardSpillTex;


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !orderPlaced) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        if (orderButton && orderButton->checkClick(xpos, ypos, width, height)) {
            orderPlaced = true;

            patty = new Patty(0.0f, 0.8f, 0.22f, 0.1f);
            patty->startOrder();
        }
    }
}

void limitFPS(float deltaTime, float targetFrameTime) {
    if (deltaTime < targetFrameTime) {
        float sleepTime = targetFrameTime - deltaTime;
        std::this_thread::sleep_for(std::chrono::duration<float>(sleepTime));
    }
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Brza Hrana", primaryMonitor, NULL);
    if (!window) return endProgram("Prozor nije uspeo da se kreira.");
    glfwMakeContextCurrent(window);

    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(w, true);
        }
    });

    if (glewInit() != GLEW_OK) return endProgram("GLEW nije uspeo da se inicijalizuje.");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.10f, 0.10f, 0.10f, 1.0f);

    unsigned int shader = createShader("Shaders/basic.vert", "Shaders/basic.frag");

    unsigned int orderTexture, studentTexture, stoveTexture, tableTexture;
    preprocessTexture(orderTexture, "Resources/order.png");
    preprocessTexture(studentTexture, "Resources/student.png");
    preprocessTexture(stoveTexture, "Resources/stove.png");
    preprocessTexture(tableTexture, "Resources/table.png");

    unsigned bunBottomTex, ketchupTex, mustardTex, picklesTex, onionTex, lettuceTex, cheeseTex, tomatoTex, bunTopTex, endMessageTex;
    preprocessTexture(bunBottomTex, "Resources/bun-bottom.png");
    //todo: add meat after!
    preprocessTexture(ketchupTex, "Resources/ketchup.png");
    preprocessTexture(mustardTex, "Resources/mustard.png");
    preprocessTexture(picklesTex, "Resources/pickles.png");
    preprocessTexture(onionTex, "Resources/onion.png");
    preprocessTexture(lettuceTex, "Resources/lettuce.png");
    preprocessTexture(cheeseTex, "Resources/cheese.png");
    preprocessTexture(tomatoTex, "Resources/tomato.png");
    preprocessTexture(bunTopTex, "Resources/bun-top.png");
    preprocessTexture(endMessageTex, "Resources/end.png");

    cursor = loadImageToCursor("Resources/cursor.png");
    glfwSetCursor(window, cursor);

    std::vector<Ingredient*> ingredients;
    int currentIngredientIndex = 0;
    float spawnX = 0.75f;  
    float spawnY = -0.75f;  

    ingredients.push_back(new Ingredient(spawnX, spawnY, 0.5f, 0.5f, bunBottomTex));
    ingredients.push_back(new Ingredient(spawnX, spawnY, 0.5f, 0.5f, bunBottomTex)); //placeholder for meat patty
    ingredients.push_back(new Ingredient(spawnX, spawnY, 0.5f, 0.5f, ketchupTex, true, false));
    ingredients.push_back(new Ingredient(spawnX, spawnY, 0.5f, 0.5f, mustardTex, false, true));
    ingredients.push_back(new Ingredient(spawnX, spawnY, 0.5f, 0.5f, picklesTex));
    ingredients.push_back(new Ingredient(spawnX, spawnY, 0.5f, 0.5f, onionTex));
    ingredients.push_back(new Ingredient(spawnX, spawnY, 0.5f, 0.5f, lettuceTex));
    ingredients.push_back(new Ingredient(spawnX, spawnY, 0.5f, 0.5f, cheeseTex));
    ingredients.push_back(new Ingredient(spawnX, spawnY, 0.5f, 0.5f, tomatoTex));
    ingredients.push_back(new Ingredient(spawnX, spawnY, 0.5f, 0.5f, bunTopTex));



    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "uTexture0"), 0);

    orderButton = new Button(0.0f, 0.0f, 0.2f, 0.2f);
    student = new Button(0.8f, 0.9f, 0.2f, 0.1f);

    stove = new Stove(0.0f, -0.7f, 0.8f, 0.5f, stoveTexture);
    table = new Table(0.0f, -0.7f, 2.0f, 0.8f, tableTexture);
    loadingBar = new LoadingBar(0.0f, 1.0f - 0.05f, 0.6f, 0.05f);
    endMessage = new Button(0.0f, 0.5f, 1.5f, 1.0f);

    lastTime = glfwGetTime();
    const float targetFPS = 75.0f;
    const float targetFrameTime = 1.0f / targetFPS;

    while (!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;

        limitFPS(deltaTime, targetFrameTime);
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT);

        if (!orderPlaced) {
            orderButton->draw(shader, orderTexture);
        }
        else {
            if (loadingBar->progress < 1.0f) 
                stove->draw(shader);
            else {
                table->draw(shader);
                burgerDone = true;
            }

            if (patty && !burgerDone) {
                patty->update(deltaTime, window);

                if (patty->isFrying(
                    stove->getY() + stove->getHeight() / 2,
                    stove->getX(),
                    stove->getWidth()
                )) {
                    loadingBar->progress += deltaTime * 0.25f;
                    if (loadingBar->progress > 1.0f) {
                        loadingBar->progress = 1.0f;
                    }
      
                    float rStart = 0.65f, gStart = 0.30f, bStart = 0.28f;
                    float rEnd = 0.48f, gEnd = 0.23f, bEnd = 0.18f;

                    float r = rStart + (rEnd - rStart) * loadingBar->progress;
                    float g = gStart + (gEnd - gStart) * loadingBar->progress;
                    float b = bStart + (bEnd - bStart) * loadingBar->progress;

                    patty->setColor(r, g, b, 1.0f);
                    
                }
                loadingBar->draw(shader);
                patty->draw(shader);
            }
            
            if (burgerDone) {
                if (currentIngredientIndex == 0) {
                    patty->setX(0.75f);
                    patty->setY(-0.75f);
                }
             

                for (int i = 0; i < ingredients.size(); i++) {
                    if (i == 1 && ingredients[0]->placed) {
                        patty->draw(shader);
                    }
                    else {
                        if (ingredients[i]->placed && !ingredients[i]->isKetchup && !ingredients[i]->isMustard) {
                            ingredients[i]->draw(shader);
                        }
                    }
                    ingredients[i]->drawSpills(shader);
                }

                if (currentIngredientIndex < ingredients.size()) {
                    if (currentIngredientIndex == 1) {
                        patty->update(deltaTime, window);
                        patty->draw(shader);

                        float yThreshold = 0.1f;
                        float xMin = -0.009f;
                        float xMax = 0.009f;

                        bool onCorrectY = fabs(patty->getY() - previousStackY) < yThreshold;
                        bool onCorrectX = (patty->getX() >= xMin && patty->getX() <= xMax);

                        if (onCorrectY && onCorrectX) {
                            patty->setPlaced(true);
                            previousStackY = patty->getY();
                        }


                        if (patty->getPlaced()) {
                            currentIngredientIndex++;
                        }
                    }
                    else {
                        Ingredient* active = ingredients[currentIngredientIndex];

                        active->update(deltaTime, previousStackY, window);
                        active->draw(shader);

                        if (active->placed) {
                            if (!active->isKetchup && !active->isMustard) {
                                if (currentIngredientIndex == 0)
                                    previousStackY = -0.8f + 0.18f;
                                else
                                    previousStackY = active->y;
                            }
                            currentIngredientIndex++;
                        }
                    }
                }

            }
        }
        student->draw(shader, studentTexture);
        if (currentIngredientIndex == 10) {
            endMessage->draw(shader, endMessageTex);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete orderButton;
    delete patty;
    delete student;
    delete stove;
    delete table;
    delete loadingBar;

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}