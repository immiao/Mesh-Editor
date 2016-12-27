#version 150
// ^ Change this to version 130 if you have compatibility issues

//This is a vertex shader. While it is called a "shader" due to outdated conventions, this file
//is used to apply matrix transformations to the arrays of vertex data passed to it.
//Since this code is run on your GPU, each vertex is transformed simultaneously.
//If it were run on your CPU, each vertex would have to be processed in a FOR loop, one at a time.
//This simultaneous transformation allows your program to run much faster, especially when rendering
//geometry with millions of vertices.

uniform mat4 u_Model;       // The matrix that defines the transformation of the
                            // object we're rendering. In this assignment,
                            // this will be the result of traversing your scene graph.

uniform mat4 u_ModelInvTr;  // The inverse transpose of the model matrix.
                            // This allows us to transform the object's normals properly
                            // if the object has been non-uniformly scaled.

uniform mat4 u_ViewProj;    // The matrix that defines the camera's transformation.
                            // We've written a static matrix for you to use for HW2,
                            // but in HW3 you'll have to generate one yourself

in vec4 vs_Pos;             // The array of vertex positions passed to the shader

in vec4 vs_Nor;             // The array of vertex normals passed to the shader

in vec4 vs_Col;             // The array of vertex colors passed to the shader.

out vec4 fs_Nor;            // The array of normals that has been transformed by u_ModelInvTr. This is implicitly passed to the fragment shader.
out vec4 fs_LightVec;       // The direction in which our virtual light lies, relative to each vertex. This is implicitly passed to the fragment shader.
out vec4 fs_Col;            // The color of each vertex. This is implicitly passed to the fragment shader.
out vec4 fs_Pos;
out vec4 testPos;
const vec4 lightPos = vec4(5, 5, 3, 1); //The position of our virtual light, which is used to compute the shading of
                                        //the geometry in the fragment shader.

uniform mat4 u_JointTransforms[20];
uniform mat4 u_BindMatrices[20];
in vec2 vs_Weights;
in uvec2 vs_JointIndices;

void main()
{
    fs_Col = vs_Col;                         // Pass the vertex colors to the fragment shader for interpolation
    //fs_Col = u_BindMatrices[vs_JointIndices[1]][3];
    //fs_Col = vec4(vs_Weights.y, 0.0, 0.0, 1.0);
    fs_Pos = vs_Pos;
    mat3 invTranspose = mat3(u_ModelInvTr);
    fs_Nor = vec4(invTranspose * vec3(vs_Nor), 0);          // Pass the vertex normals to the fragment shader for interpolation.
                                                            // Transform the geometry's normals by the inverse transpose of the
                                                            // model matrix. This is necessary to ensure the normals remain
                                                            // perpendicular to the surface after the surface is transformed by
                                                            // the model matrix.
    vec4 temp = vs_Weights[0] * u_JointTransforms[vs_JointIndices[0]] * u_BindMatrices[vs_JointIndices[0]] * vs_Pos
            + vs_Weights[1] * u_JointTransforms[vs_JointIndices[1]] * u_BindMatrices[vs_JointIndices[1]] * vs_Pos;
    //temp = u_JointTransforms[vs_JointIndices[0]] * u_BindMatrices[vs_JointIndices[0]] * vs_Pos;
    //vec4 temp = (u_JointTransforms[1] * u_BindMatrices[1]) * vs_Pos;
    //vec4 temp = (vs_Weights.x + vs_Weights.y) * vs_Pos;
    //fs_Col = vec4(1.0, 0.0, 0.0, 1.0);
    //vec4 temp = (u_JointTransforms[0]) * vs_Pos;
    //vec4 temp = u_BindMatrices[0] * vs_Pos;
    //vec4 temp = vec4(vs_Weights[0], vs_Weights[0], vs_Weights[0], 1.0);
    //fs_Col = temp;
    vec4 modelposition = u_Model * temp;   // Temporarily store the transformed vertex positions for use below
    //vec4 modelposition = u_Model * vs_Pos;

    fs_LightVec = lightPos - modelposition;  // Compute the direction in which the light source lies

    //testPos = u_ViewProj * modelposition;
    gl_Position = u_ViewProj * modelposition;// gl_Position is a built-in variable of OpenGL which is
                                             // used to render the final positions of the geometry's vertices
}
