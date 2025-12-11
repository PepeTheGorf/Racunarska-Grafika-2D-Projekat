#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Spill {
    float x, y;
    unsigned texture;
};

class Ingredient {
public:
    float x, y;
    float width, height;
    bool placed = false;
    bool isKetchup = false;
    bool isMustard = false;
    unsigned texture;

    std::vector<Spill> spills; 

    Ingredient(float startX, float startY, float width, float height, unsigned texture, bool isKetchup = false, bool isMustard = false);
    ~Ingredient();

    void update(float deltaTime, float stackY, GLFWwindow* window);
    void draw(unsigned shader);
    void drawSpills(unsigned shader); 

private:
    unsigned VAO, VBO;

    void handleMovement(float deltaTime, GLFWwindow* window);
    void handleAction(float stackY, GLFWwindow* window);
};

