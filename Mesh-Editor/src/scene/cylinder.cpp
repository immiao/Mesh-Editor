#include "cylinder.h"
#include<la.h>
#include <iostream>

static const int CYL_IDX_COUNT = 240;
static const int CYL_VERT_COUNT = 80;

Cylinder::Cylinder(GLWidget277 *context) : Drawable(context)
{}

//These are functions that are only defined in this cpp file. They're used for organizational purposes
//when filling the arrays used to hold the vertex and index data.
void createCylinderVertexPositions(glm::vec4 (&cyl_vert_pos)[CYL_VERT_COUNT]){
    //Create two rings of vertices
    //We'll be using 20-sided polygons to approximate circles
    //for the endcaps

    //Store top cap verts (IDX 0 - 19)
    for(int i = 0; i < 20; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f,0.5f,0,1);
        cyl_vert_pos[i] = v;
    }
    //Store bottom cap verts (IDX 20 - 39)
    for(int i = 20; i < 40; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians((i-20)*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f,-0.5f,0,1);
        cyl_vert_pos[i] = v;
    }

    //Create two more rings of vertices, this time for the barrel faces
    //We could use the same vertex rings as above, but GL only supports one normal per vertex
    //so we have to have two vertices per position in order to have sharp edges between the endcaps
    //and the barrel

    //Store top cap verts (IDX 40 - 59)
    for(int i = 0; i < 20; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f,0.5f,0,1);
        cyl_vert_pos[i + 40] = v;
    }
    //Store bottom cap verts (IDX 60 - 79)
    for(int i = 20; i < 40; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians((i-20)*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f,-0.5f,0,1);
        cyl_vert_pos[i + 40] = v;
    }
}


void createCylinderVertexNormals(glm::vec4 (&cyl_vert_nor)[CYL_VERT_COUNT]){
    //Store top cap normals (IDX 0 - 19)
    for(int i = 0; i < 20; i++){
        cyl_vert_nor[i] =  glm::vec4(0,1,0,0);
    }
    //Store bottom cap normals (IDX 20 - 39)
    for(int i = 20; i < 40; i++){
        cyl_vert_nor[i] =  glm::vec4(0,-1,0,0);
    }

    //Store top of barrel normals (IDX 40 - 59)
    for(int i = 0; i < 20; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(1,0,0,0);
        cyl_vert_nor[i + 40] = v;
    }
    //Store bottom of barrel normals (IDX 60 - 79)
    for(int i = 20; i < 40; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians((i-20)*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(1,0,0,0);
        cyl_vert_nor[i + 40] = v;
    }
}


void createCylinderIndices(GLuint (&cyl_idx)[CYL_IDX_COUNT]){
    //Build indices for the top cap (18 tris, indices 0 - 53, up to vertex 19)
    for(int i = 0; i < 18; i++){
        cyl_idx[i*3] = 0;
        cyl_idx[i*3 + 1] = i+1;
        cyl_idx[i*3 + 2] = i+2;
    }
    //Build indices for the top cap (18 tris, indices 54 - 107, up to vertex 39)
    for(int i = 18; i < 36; i++){
        cyl_idx[i*3] = 20;
        cyl_idx[i*3 + 1] = i+3;
        cyl_idx[i*3 + 2] = i+4;
    }
    //Build indices for the barrel of the cylinder
    for(int i = 0; i < 19; i++){
        cyl_idx[108 + i*6] = i + 40;
        cyl_idx[109 + i*6] = i + 41;
        cyl_idx[110 + i*6] = i + 60;
        cyl_idx[111 + i*6] = i + 41;
        cyl_idx[112 + i*6] = i + 61;
        cyl_idx[113 + i*6] = i + 60;
    }
    //Build the last quad of the barrel, which has looping indices
    cyl_idx[234] = 59;
    cyl_idx[235] = 40;
    cyl_idx[236] = 79;
    cyl_idx[237] = 40;
    cyl_idx[238] = 60;
    cyl_idx[239] = 79;
}

void Cylinder::create()
{
    GLuint cyl_idx[CYL_IDX_COUNT];
    glm::vec4 cyl_vert_pos[CYL_VERT_COUNT];
    glm::vec4 cyl_vert_nor[CYL_VERT_COUNT];

    createCylinderVertexPositions(cyl_vert_pos);
    createCylinderVertexNormals(cyl_vert_nor);
    createCylinderIndices(cyl_idx);

    count = CYL_IDX_COUNT;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // CYL_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, CYL_IDX_COUNT * sizeof(GLuint), cyl_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, CYL_VERT_COUNT * sizeof(glm::vec4), cyl_vert_pos, GL_STATIC_DRAW);

    generateNor();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    context->glBufferData(GL_ARRAY_BUFFER, CYL_VERT_COUNT * sizeof(glm::vec4), cyl_vert_nor, GL_STATIC_DRAW);
}
