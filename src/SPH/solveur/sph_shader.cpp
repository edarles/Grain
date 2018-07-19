#define STRINGIFY(A) #A
#include <stdio.h>

// vertex shader for display particle
const char *sphereVertexShader = STRINGIFY(
//uniform float pointRadius;  // point size in world space
uniform float pointScale;   // scale to calculate size in pixels
void main()
{
    // calculate window-space point size
    vec3 posEye = vec3(gl_ModelViewMatrix * vec4(gl_Vertex.xyz, 1.0));
    float dist = length(posEye);
    gl_PointSize = gl_Vertex.w* (pointScale / dist);
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.xyz, 1.0);
    gl_FrontColor = gl_Color;
}
);

// pixel shader for display particle
const char *spherePixelShader = STRINGIFY(
void main()
{   
    const vec3 lightDir = vec3(0.577, 0.577, 0.577);
    // calculate normal from texture coordinates
    vec3 N;
    N.xy = gl_TexCoord[0].xy*vec2(2.0, -2.0) + vec2(-1.0, 1.0);
    float mag = dot(N.xy, N.xy);
    if (mag > 1.0) discard;   // kill pixels outside circle
    N.z = sqrt(1.0-mag);
    // calculate lighting
    float diffuse = max(0.0, dot(lightDir, N));
    gl_FragColor = gl_Color * diffuse;

}
);