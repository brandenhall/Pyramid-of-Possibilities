/* 
 * File:   Pyramid.cpp
 * Author: antoine
 * 
 * Created on June 14, 2014, 10:07 AM
 */

#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "ShaderLoader.hpp"
#include "model.h"
#include "leds.h"
#include "platforms.h"
#include "tier.h"

#include "Pyramid.hpp"

Pyramid::Pyramid() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glPointSize(2);
    
    programId = LoadShaders("TransformVertexShader.shader",
            "ColorFragmentShader.shader");
    matrixId = glGetUniformLocation(programId, "MVP");
    vertexPosition_modelspaceID = glGetAttribLocation(programId,
            "vertexPosition_modelspace");
    vertexColorID = glGetAttribLocation(programId, "vertexColor");

    this->loadBuffers();

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GL_FLOAT) * g_vertex_buffer_data_length,
            g_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GL_FLOAT) * g_vertex_buffer_data_length,
            g_color_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &ledsbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, ledsbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GL_FLOAT) * g_leds_buffer_data_length,
            g_leds_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &ledscolorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, ledscolorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GL_FLOAT) * g_leds_buffer_data_length,
            g_leds_color_buffer_data, GL_STATIC_DRAW);
}

void Pyramid::loadBuffers() {
    g_vertex_buffer_data_length = popNumVerts * 3;
    g_vertex_buffer_data = new GLfloat[g_vertex_buffer_data_length];
    float factor = 0.001f;
    for (int i = 0; i < g_vertex_buffer_data_length; i += 3) {
        float x = ((popVerts[i] - 8970) / 1.0775f + 20) * factor;
        float y = (popVerts[i + 1] / 1.0775f + 89.5f - 5000) * factor;
        float z = ((-9502 - popVerts[i + 2]) / 1.0775f - 40) * factor;
        g_vertex_buffer_data[i] = x;
        g_vertex_buffer_data[i + 1] = y;
        g_vertex_buffer_data[i + 2] = z;
    }

    g_color_buffer_data = new GLfloat[g_vertex_buffer_data_length];
    for (int i = 0; i < g_vertex_buffer_data_length; i += 3) {
        g_color_buffer_data[i] = 0.5f;
        g_color_buffer_data[i + 1] = 0.2f;
        g_color_buffer_data[i + 2] = 0.1f;
    }

    int nbPlatforms = 84;
    int nbLedsPerPlatform = 219;
    int ledsBufferSize = nbPlatforms * nbLedsPerPlatform * 3;
    g_leds_buffer_data_length = ledsBufferSize;
    g_leds_buffer_data = new GLfloat[ledsBufferSize];
    g_leds_color_buffer_data = new GLfloat[ledsBufferSize];
    int k = 0;
    for (int i = 0; i < 84; i++) {
        short* platformPosition = platformPositions[i];
        short x = platformPosition[0];
        short y = platformPosition[1];
        short z = (short) (platformPosition[2] + 50);
        short t = tier[i];
        short r = (short) (t & 1);
        short g = (short) ((t & 2) >> 1);
        short b = (short) ((t & 4) >> 2);
        for (int j = 0; j < nbLedsPerPlatform; j++) {
            short* ledPosition = ledPositions[j];
            g_leds_buffer_data[k] = (x + 1.05f * ledPosition[1]) * factor;
            g_leds_buffer_data[k + 1] = (z - 5000) * factor;
            g_leds_buffer_data[k + 2] = (y + 1.05f * ledPosition[0]) * factor;
            g_leds_color_buffer_data[k] = r;
            g_leds_color_buffer_data[k + 1] = g;
            g_leds_color_buffer_data[k + 2] = b;
            k += 3;
        }
    }
}

void Pyramid::render(glm::mat4 MVP) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programId);

    glUniformMatrix4fv(matrixId, 1, GL_FALSE, &MVP[0][0]);

    // Draw platforms
    glEnableVertexAttribArray(vertexPosition_modelspaceID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0,
            (void*) 0);

    glEnableVertexAttribArray(vertexColorID);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(vertexColorID, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glDrawArrays(GL_TRIANGLES, 0, g_vertex_buffer_data_length);

    // Draw leds
    glBindBuffer(GL_ARRAY_BUFFER, ledsbuffer);
    glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE,
            0, (void*) 0);

    glBindBuffer(GL_ARRAY_BUFFER, ledscolorbuffer);
    glVertexAttribPointer(vertexColorID, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glDrawArrays(GL_POINTS, 0, g_leds_buffer_data_length);

    glDisableVertexAttribArray(vertexPosition_modelspaceID);
    glDisableVertexAttribArray(vertexColorID);
}

Pyramid::~Pyramid() {
    delete g_vertex_buffer_data;
    delete g_color_buffer_data;
    delete g_leds_buffer_data;
    delete g_leds_color_buffer_data;
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteBuffers(1, &ledsbuffer);
    glDeleteBuffers(1, &ledscolorbuffer);
    glDeleteProgram(programId);
}

