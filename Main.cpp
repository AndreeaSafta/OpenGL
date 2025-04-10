#define NOMINMAX
#include <windows.h>
#include <algorithm>
#include <glew.h>
#include <freeglut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#include <vector>    
using namespace std;
#endif


const int WIDTH = 800;
const int HEIGHT = 800;


float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 3.0f;
float yaw = -90.0f, pitch = 0.0f;
float cameraSpeed = 0.1f;
bool mouseLeftPressed = false;
float sensitivity = 0.2f;
int lastMouseX, lastMouseY;

GLuint textureFloor, textureWall, textureWallS, textureCeiling, textureGlass, textureDoor, textureSphere, textureSphereCircuit,
textureSphere2, textureSphere3, textureCub, textureBear, textureEye, textureInnerEar, textureNose, textureBearInner, textureCarpet, textureCorner, textureEdge, textureCornerTopLeft,
textureCornerTopRight, textureCornerBottomLeft, textureCornerBottomRight, textureEdgeTop, textureEdgeBottom, textureEdgeLeft, textureEdgeRight;



void loadTexture(const char* filename, GLuint& textureID) {
    int width, height, nrChannels;

    
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

      
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       

        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format == GL_RGBA ? GL_RGBA8 : GL_RGB8, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        std::cout << "Textura a fost incarcata cu succes: " << width << "x" << height << std::endl;
    }
    else {
        std::cerr << "Eroare la incarcarea texturii: " << filename << std::endl;
    }
}

void drawSkybox() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    // podeaua
    glBindTexture(GL_TEXTURE_2D, textureFloor);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-5, -5, -5);
    glTexCoord2f(1, 0); glVertex3f(5, -5, -5);
    glTexCoord2f(1, 1); glVertex3f(5, -5, 5);
    glTexCoord2f(0, 1); glVertex3f(-5, -5, 5);
    glEnd();

    // peretele cu peisaj
    glBindTexture(GL_TEXTURE_2D, textureWall);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(5, -5, -5);
    glTexCoord2f(1, 0); glVertex3f(-5, -5, -5);
    glTexCoord2f(1, 1); glVertex3f(-5, 5, -5);
    glTexCoord2f(0, 1); glVertex3f(5, 5, -5);
    glEnd();

    // Geamul transparent
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, textureGlass);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(5, -5, -5);
    glTexCoord2f(1, 0); glVertex3f(-5, -5, -5);
    glTexCoord2f(1, 1); glVertex3f(-5, 5, -4.9);
    glTexCoord2f(0, 1); glVertex3f(5, 5, -4.9);
    glEnd();
    glDisable(GL_BLEND);

    // peretele din spate
    glBindTexture(GL_TEXTURE_2D, textureWallS);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-5, -5, 5);
    glTexCoord2f(1, 0); glVertex3f(5, -5, 5);
    glTexCoord2f(1, 1); glVertex3f(5, 5, 5);
    glTexCoord2f(0, 1); glVertex3f(-5, 5, 5);
    glEnd();

    // peretele stanga
    glBindTexture(GL_TEXTURE_2D, textureWallS);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-5, -5, -5);
    glTexCoord2f(1, 0); glVertex3f(-5, -5, 5);
    glTexCoord2f(1, 1); glVertex3f(-5, 5, 5);
    glTexCoord2f(0, 1); glVertex3f(-5, 5, -5);
    glEnd();

    // peretele dreapta
   
    glBindTexture(GL_TEXTURE_2D, textureWallS);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex3f(5, -5, -5);
    glTexCoord2f(0, 0); glVertex3f(5, 5, -5);
    glTexCoord2f(1, 0); glVertex3f(5, 5, 5);
    glTexCoord2f(1, 1); glVertex3f(5, -5, 5);
    glEnd();

    //Usa
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, textureDoor);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(4.9, -5, 1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(4.9, -5, -2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(4.9, 1, -2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(4.9, 1, 1);
    glEnd();
    glDisable(GL_BLEND);

    // tavan
    glBindTexture(GL_TEXTURE_2D, textureCeiling);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-5, 5, 5);
    glTexCoord2f(1, 0); glVertex3f(5, 5, 5);
    glTexCoord2f(1, 1); glVertex3f(5, 5, -5);
    glTexCoord2f(0, 1); glVertex3f(-5, 5, -5);
    glEnd();

    glDisable(GL_TEXTURE_2D);

 
}

void drawTexturedSphere(float x, float y, float z, float radius, GLuint texture) {
    GLUquadric* quad = gluNewQuadric();
    glBindTexture(GL_TEXTURE_2D, texture);

    glEnable(GL_TEXTURE_2D);
    gluQuadricTexture(quad, GL_TRUE);

    glPushMatrix();
    glTranslatef(x, y, z);
    gluSphere(quad, radius, 30, 30);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    gluDeleteQuadric(quad);
}

void drawTexturedCube(float x, float y, float z, float size, GLuint texture, bool isSmallCube) {
    if (isSmallCube) {
        
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);  
    }
    else {
        glDisable(GL_CULL_FACE);  
    }

    glBindTexture(GL_TEXTURE_2D, texture);

    glEnable(GL_TEXTURE_2D);

    

   
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size / 2, y - size / 2, z + size / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + size / 2, y - size / 2, z + size / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size / 2, y + size / 2, z + size / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x - size / 2, y + size / 2, z + size / 2);
    glEnd();

   
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size / 2, y - size / 2, z - size / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x - size / 2, y + size / 2, z - size / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size / 2, y + size / 2, z - size / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + size / 2, y - size / 2, z - size / 2);
    glEnd();

   
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size / 2, y - size / 2, z - size / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x - size / 2, y - size / 2, z + size / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x - size / 2, y + size / 2, z + size / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x - size / 2, y + size / 2, z - size / 2);
    glEnd();

   
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + size / 2, y - size / 2, z - size / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + size / 2, y + size / 2, z - size / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size / 2, y + size / 2, z + size / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + size / 2, y - size / 2, z + size / 2);
    glEnd();

   
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size / 2, y - size / 2, z - size / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + size / 2, y - size / 2, z - size / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size / 2, y - size / 2, z + size / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x - size / 2, y - size / 2, z + size / 2);
    glEnd();

  
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size / 2, y + size / 2, z - size / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x - size / 2, y + size / 2, z + size / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size / 2, y + size / 2, z + size / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + size / 2, y + size / 2, z - size / 2);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    if (isSmallCube) {
        glDisable(GL_CULL_FACE); 
    }
}

void drawTeddyBear(float x, float y, float z) {
    drawTexturedSphere(x, y, z, 1.0f, textureBear);

    float bellyButtonRadius = 0.7f;
    drawTexturedSphere(x, y , z + 0.4f, bellyButtonRadius, textureBearInner);
   
    drawTexturedSphere(x, y + 1.5f, z, 0.8f, textureBear);

    drawTexturedSphere(x - 0.7f, y + 2.2f, z, 0.3f, textureBear);
    drawTexturedSphere(x + 0.7f, y + 2.2f, z, 0.3f, textureBear);

    float innerEarRadius = 0.15f;  
    drawTexturedSphere(x - 0.7f, y + 2.25f, z + 0.2f, innerEarRadius, textureInnerEar);  
    drawTexturedSphere(x + 0.7f, y + 2.25f, z + 0.2f, innerEarRadius, textureInnerEar);  

    drawTexturedSphere(x - 0.7f, y + 2.15f, z - 0.1f, innerEarRadius, textureInnerEar);  
    drawTexturedSphere(x + 0.7f, y + 2.15f, z - 0.1f, innerEarRadius, textureInnerEar);  

    drawTexturedSphere(x - 0.5f, y - 0.5f, z, 0.5f, textureBear);
    drawTexturedSphere(x + 0.5f, y - 0.5f, z, 0.5f, textureBear);

    drawTexturedSphere(x - 1.0f, y + 0.5f, z, 0.3f, textureBear);
    drawTexturedSphere(x + 1.0f, y + 0.5f, z, 0.3f, textureBear);

    float eyeRadius = 0.10f; 
    float eyeOffsetX = 0.25f;
    float eyeOffsetY = 0.3f;  

    drawTexturedSphere(x - eyeOffsetX, y + 1.8f, z + 0.6f, eyeRadius, textureEye); 
    drawTexturedSphere(x + eyeOffsetX, y + 1.8f, z + 0.6f, eyeRadius, textureEye); 

    float noseRadius = 0.10f;  
    drawTexturedSphere(x, y + 1.5f, z + 0.8f, noseRadius, textureNose); 
}


void drawRoadTile(float x, float z, GLuint texture) {
    float roadHeight = -4.99f;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(x, roadHeight, z);
    glTexCoord2f(1, 0); glVertex3f(x + 1, roadHeight, z);
    glTexCoord2f(1, 1); glVertex3f(x + 1, roadHeight, z + 1);
    glTexCoord2f(0, 1); glVertex3f(x, roadHeight, z + 1);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}
void drawCircuit() {
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    int size = 6;
    float offset = -size / 2.0f;

    for (int i = 0; i < size; ++i) {
        if (i == 0) {
           
            drawRoadTile(i + offset, offset, textureCornerBottomLeft);   
            drawRoadTile(i + offset, size - 1 + offset, textureCornerTopLeft);    
            drawRoadTile(size - 1 + offset, i + offset, textureCornerBottomRight); 
            drawRoadTile(size - 1 + offset, size - 1 + offset, textureCornerTopRight); 
        }
        else {
            if (i < size - 1) {
                drawRoadTile(i + offset, offset, textureEdgeBottom);  
                drawRoadTile(i + offset, size - 1 + offset, textureEdgeTop); 
                drawRoadTile(offset, i + offset, textureEdgeLeft);   
                drawRoadTile(size - 1 + offset, i + offset, textureEdgeRight);  
            }
        }
    }
   
    glDisable(GL_BLEND); 
}


float objectPosX = -3.0f;  
float objectPosZ = -3.0f;  
float objectSpeed = 0.002f; 
int direction = 0;         



void drawObject() {
    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT);
    glTranslatef(objectPosX + 0.5f, -4.68f, objectPosZ + 0.5f);
    glColor3f(0.5f, 0.0f, 0.0f);
    glutSolidSphere(0.3, 20, 20);  
    glPopAttrib();
    glPopMatrix();

}

/*player start*/
    struct Vec3 {
        float x, y, z;
    };

    struct AABB {
        Vec3 min;
        Vec3 max;
    };

    
    Vec3 playerPosition = { 0.0f, -4.4f, 0.0f };
    const float playerSize = 1.0f;

    
    Vec3 teddyPosition = { 0.0f, -4.4f, 3.0f };


    float computeOverlapVolume(const AABB& a, const AABB& b) {
        float overlapX = std::max(0.0f, std::min(a.max.x, b.max.x) - std::max(a.min.x, b.min.x));
        float overlapY = std::max(0.0f, std::min(a.max.y, b.max.y) - std::max(a.min.y, b.min.y));
        float overlapZ = std::max(0.0f, std::min(a.max.z, b.max.z) - std::max(a.min.z, b.min.z));
        return overlapX * overlapY * overlapZ;
    }
   
    void drawCube(Vec3 pos, float size) {
        glPushMatrix();
        glPushAttrib(GL_CURRENT_BIT);      
        glDisable(GL_TEXTURE_2D);        
        glColor3f(0.0f, 0.3f, 0.0f);       
        glTranslatef(pos.x, pos.y, pos.z);
        glutSolidCube(size);
        glPopAttrib();                     
        glPopMatrix();
    }
    
    AABB getPlayerAABB(Vec3 pos, float size) {
        return {
            {pos.x - size / 2, pos.y - size / 2, pos.z - size / 2},
            {pos.x + size / 2, pos.y + size / 2, pos.z + size / 2}
        };
    }

    std::vector<AABB> getWalls() {
        return {
            {{-5.0f, -5.0f, -5.1f}, {5.0f, 5.0f, -5.0f}},
            {{-5.0f, -5.0f, 5.0f}, {5.0f, 5.0f, 5.1f}},   
            {{-5.1f, -5.0f, -5.0f}, {-5.0f, 5.0f, 5.0f}}, 
            {{5.0f, -5.0f, -5.0f}, {5.1f, 5.0f, 5.0f}},   
            {{-5.0f, 5.0f, -5.0f}, {5.0f, 5.1f, 5.0f}},   
            {{-5.0f, -5.1f, -5.0f}, {5.0f, -5.0f, 5.0f}}  
        };
    }

    AABB getSphereAABB(float x, float y, float z, float radius) {
        return {
            {x - radius, y - radius, z - radius},
            {x + radius, y + radius, z + radius}
        };
    }

    std::vector<AABB> getTeddyBearFeet(Vec3 pos) {
        std::vector<AABB> feet;

       
        feet.push_back(getSphereAABB(pos.x - 0.5f, pos.y - 1.2f, pos.z, 0.5f)); 
        feet.push_back(getSphereAABB(pos.x + 0.5f, pos.y - 1.2f, pos.z, 0.5f)); 

        return feet;
    }

    AABB getCubeAABB(float centerX, float centerY, float centerZ, float size) {
        float halfSize = size / 2.0f;
        return {
            {centerX - halfSize, centerY - halfSize, centerZ - halfSize},
            {centerX + halfSize, centerY + halfSize, centerZ + halfSize}
        };
    }

    bool AABBOverlap(const AABB& a, const AABB& b) {
        return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
            (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
            (a.min.z <= b.max.z && a.max.z >= b.min.z);
    }

    Vec3 getDirectionVector() {
        switch (direction) {
        case 0: return { 1.0f, 0.0f, 0.0f }; 
        case 1: return { 0.0f, 0.0f, 1.0f }; 
        case 2: return { -1.0f, 0.0f, 0.0f }; 
        case 3: return { 0.0f, 0.0f, -1.0f }; 
        }
        return { 0, 0, 0 };
    }

    float dotProduct(const Vec3& a, const Vec3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    Vec3 subtract(const Vec3& a, const Vec3& b) {
        return { a.x - b.x, a.y - b.y, a.z - b.z };
    }

    Vec3 sphere1Pos = { 4.5f, -4.6f, -3.5f };
    float sphere1Radius = 0.3f;

    Vec3 sphere2Pos = { 1.0f, -4.7f, -4.5f };
    float sphere2Radius = 0.3f;

    Vec3 sphere3Pos = { 3.5f, -4.6f, -3.0f };
    float sphere3Radius = 0.3f;

   

    void moveObject() {
        AABB sphereBox = getSphereAABB(objectPosX + 0.5f, -4.68f, objectPosZ + 0.5f, 0.3f);
        AABB playerBox = getPlayerAABB(playerPosition, playerSize);
        AABB cubeBox = getCubeAABB(4.0f, -4.7f, -4.3f, 0.8f);

        Vec3 sphereCenter = { objectPosX + 0.5f, -4.68f, objectPosZ + 0.5f };
        Vec3 dir = getDirectionVector();

        Vec3 toPlayer = subtract(playerPosition, sphereCenter);
        float playerDot = dotProduct(dir, toPlayer);

        if (AABBOverlap(sphereBox, playerBox)) {
            if (playerDot > 0) {
                return;
            }
        }
        Vec3 cubePos = { 4.0f, -4.7f, -4.3f };
        Vec3 toCube = subtract(cubePos, sphereCenter);
        float cubeDot = dotProduct(dir, toCube);
        if (AABBOverlap(sphereBox, cubeBox) && cubeDot > 0) {
            return;
        }

        if (direction == 0 && objectPosX < 2.0f) {
            objectPosX += objectSpeed;
        }
        else if (direction == 1 && objectPosZ < 2.0f) {
            objectPosZ += objectSpeed;
        }
        else if (direction == 2 && objectPosX > -3.0f) {
            objectPosX -= objectSpeed;
        }
        else if (direction == 3 && objectPosZ > -3.0f) {
            objectPosZ -= objectSpeed;
        }

        if (objectPosX >= 2.0f && direction == 0) direction = 1;
        if (objectPosZ >= 2.0f && direction == 1) direction = 2;
        if (objectPosX <= -3.0f && direction == 2) direction = 3;
        if (objectPosZ <= -3.0f && direction == 3) direction = 0;

    }

    bool checkCollision(const AABB& playerBox, const std::vector<AABB>& walls, const std::vector<AABB>& teddyBearFeet) {
        for (const auto& wall : walls) {
            if (playerBox.min.x <= wall.max.x && playerBox.max.x >= wall.min.x &&
                playerBox.min.y <= wall.max.y && playerBox.max.y >= wall.min.y &&
                playerBox.min.z <= wall.max.z && playerBox.max.z >= wall.min.z) {
                return true;
            }
        }

        for (const auto& part : teddyBearFeet) {
            if (playerBox.min.x <= part.max.x && playerBox.max.x >= part.min.x &&
                playerBox.min.y <= part.max.y && playerBox.max.y >= part.min.y &&
                playerBox.min.z <= part.max.z && playerBox.max.z >= part.min.z) {
                return true;
            }
        }

        return false;
    }

  
   
    void handleKeys(unsigned char key, int x, int y) {
        Vec3 nextPos = playerPosition;

        if (key == 'w') nextPos.z -= 0.1f;
        if (key == 's') nextPos.z += 0.1f;
        if (key == 'a') nextPos.x -= 0.1f;
        if (key == 'd') nextPos.x += 0.1f;

        AABB nextBox = getPlayerAABB(nextPos, playerSize);

        std::vector<AABB> walls = getWalls();
        std::vector<AABB> teddyParts = getTeddyBearFeet(teddyPosition);
        AABB teddyBody = getSphereAABB(teddyPosition.x, teddyPosition.y, teddyPosition.z, 1.0f);
        teddyParts.push_back(teddyBody);
        AABB teddyBelly = getSphereAABB(teddyPosition.x, teddyPosition.y, teddyPosition.z + 0.4f, 0.7f);
        teddyParts.push_back(teddyBelly);

  
        std::vector<AABB> obstacles;
        obstacles.push_back(getSphereAABB(sphere1Pos.x, sphere1Pos.y, sphere1Pos.z, sphere1Radius));
        obstacles.push_back(getSphereAABB(sphere2Pos.x, sphere2Pos.y, sphere2Pos.z, sphere2Radius));
        obstacles.push_back(getSphereAABB(sphere3Pos.x, sphere3Pos.y, sphere3Pos.z, sphere3Radius));
        obstacles.push_back(getCubeAABB(4.0f, -4.7f, -4.3f, 0.8f));

        if (checkCollision(nextBox, walls, teddyParts) || checkCollision(nextBox, obstacles, {})) {
            return;
        }

        AABB movingSphereBox = getSphereAABB(objectPosX + 0.5f, -4.68f, objectPosZ + 0.5f, 0.3f);
        float currentOverlap = computeOverlapVolume(getPlayerAABB(playerPosition, playerSize), movingSphereBox);
        float nextOverlap = computeOverlapVolume(nextBox, movingSphereBox);

        if (nextOverlap > currentOverlap) {
            return;
        }

        playerPosition = nextPos;
        glutPostRedisplay();
    }
    /*player end*/

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float dirX = cos(yaw * M_PI / 180.0) * cos(pitch * M_PI / 180.0);
    float dirY = sin(pitch * M_PI / 180.0);
    float dirZ = sin(yaw * M_PI / 180.0) * cos(pitch * M_PI / 180.0);

    gluLookAt(cameraX, cameraY, cameraZ,
        cameraX + dirX, cameraY + dirY, cameraZ + dirZ, 0, 1, 0);

    // Bec tavan
    glPushMatrix();
    glTranslatef(0.0f, 4.5f, 0.0f);
    //glColor3f(1.0f, 1.0f, 0.8f); // galben deschis
    glColor3f(0.6f, 0.8f, 1.0f);//albastru 
    glutSolidSphere(0.3f, 20, 20);
    glPopMatrix();

    //// Bec perete
    //glPushMatrix();
    //glTranslatef(-4.9f, 0.0f, 0.0f);
    //glColor3f(0.5f, 1.0f, 0.5f); // verde deschis
    //glutSolidSphere(0.1f, 10, 10);
    //glPopMatrix();

    // Lumina 1 - Bec pe tavan
    GLfloat light1_position[] = { 0.0f, 4.8f, 0.0f, 1.0f }; // aproape de centrul camerei sus
    GLfloat light1_diffuse[] = { 1.0f, 1.0f, 0.9f, 1.0f };
    GLfloat light1_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light1_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);

    // Lumina 2 - Pe un perete (de exemplu, peretele din dreapta)
    GLfloat light2_position[] = { 4.9f, 0.0f, 0.0f, 1.0f }; // pe peretele din dreapta
    GLfloat light2_diffuse[] = { 0.8f, 0.8f, 1.0f, 1.0f }; // ușor albastruie
    GLfloat light2_specular[] = { 0.9f, 0.9f, 1.0f, 1.0f };
    GLfloat light2_ambient[] = { 0.1f, 0.1f, 0.2f, 1.0f };

    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);

                     //x            y     dim
    drawTexturedSphere(4.5f, -4.6f, -3.5f, 0.3f,textureSphere);
    drawTexturedSphere(1.0f, -4.7f, -4.5f, 0.3f,textureSphere2);
    drawTexturedSphere(3.5f, -4.6f, -3.0f, 0.3f, textureSphere3);
    drawTexturedCube(4.0f, -4.7f, -4.3f, 0.8f, textureCub, false);

    
    drawCircuit();
    teddyPosition = { -3.0f, -4.0f, -4.0f };
    drawTeddyBear(teddyPosition.x, teddyPosition.y, teddyPosition.z);
    drawSkybox();
    drawTeddyBear(-3.0f, -4.0f, -4.0f);

    drawCube(playerPosition, playerSize);
   
    moveObject();
    drawCircuit();
    drawObject();



    glutSwapBuffers();
    glutPostRedisplay();
}


void moveCamera(int key, int x, int y) {
    float dirX = cos(yaw * M_PI / 180.0);
    float dirZ = sin(yaw * M_PI / 180.0);

    switch (key) {
    case GLUT_KEY_UP:    cameraX += dirX * cameraSpeed; cameraZ += dirZ * cameraSpeed; break;
    case GLUT_KEY_DOWN:  cameraX -= dirX * cameraSpeed; cameraZ -= dirZ * cameraSpeed; break;
    case GLUT_KEY_LEFT:  cameraX -= dirZ * cameraSpeed; cameraZ += dirX * cameraSpeed; break;
    case GLUT_KEY_RIGHT: cameraX += dirZ * cameraSpeed; cameraZ -= dirX * cameraSpeed; break;
    }
    glutPostRedisplay();
}


void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        mouseLeftPressed = (state == GLUT_DOWN);
        lastMouseX = x;
        lastMouseY = y;
    }
}


void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}


void mouseMotion(int x, int y) {
    if (!mouseLeftPressed) return;

    float offsetX = (x - lastMouseX) * sensitivity;
    float offsetY = (lastMouseY - y) * sensitivity; 

    lastMouseX = x;
    lastMouseY = y;

    yaw += offsetX;
    pitch += offsetY;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glutPostRedisplay();
}

    void mouseWheel(int button, int dir, int x, int y) {
        if (dir > 0) {
            cameraZ -= 0.5f;  
        }
        else {
            cameraZ += 0.5f;  
        }
        glutPostRedisplay();
    }



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Camera Free Look with Texture");
    glewInit();
    if (!glewIsSupported("GL_VERSION_2_0")) {
        std::cerr << "OpenGL 2.0 nu este suportat!" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/wood2.jpg", textureFloor);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/spring1.jpg", textureWall);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/wall.jpg", textureWallS);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/ceiling.jpg", textureCeiling);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/door1.png", textureGlass);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/door3.jpg", textureDoor);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/fabric.jpg", textureSphere);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/red.jpg", textureSphere2);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/wool.jpg", textureSphere3);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/tile.jpg", textureCub);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/fur.jpg", textureBear);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/eye.jpg", textureEye);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/eye.jpg", textureNose);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/pink.jpg", textureInnerEar);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/pink.jpg", textureBearInner);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/leftt.png", textureCornerTopLeft);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/leftb.png", textureCornerBottomLeft);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/rightb.png", textureCornerBottomRight);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/train2.png", textureEdgeLeft);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/train2.png", textureEdgeRight);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/rightt.png", textureCornerTopRight);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/train.png", textureEdgeTop);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/train.png", textureEdgeBottom);
    loadTexture("C:/Users/Andreea/Desktop/AN4/sem2/SPG/images/wool.jpg", textureSphereCircuit);

    glutKeyboardFunc(handleKeys);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(moveCamera);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutMouseWheelFunc(mouseWheel);

    glutMainLoop();
    return 0;
}


