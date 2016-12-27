#version 150
// ^ Change this to version 130 if you have compatibility issues

// Refer to the lambert shader files for useful comments

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

in vec4 vs_Pos;
in vec4 vs_Col;

out vec4 fs_Col;

void main()
{
    fs_Col = vs_Col;
    vec4 modelposition = u_Model * vs_Pos;

    //built-in things to pass down the pipeline
    gl_Position = u_ViewProj * modelposition;

}
