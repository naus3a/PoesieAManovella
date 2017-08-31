#version 120

uniform sampler2DRect tex;
uniform float near;
uniform float far;

void main(){
    vec2 tc = gl_TexCoord[0].st;
    vec4 texel = texture2DRect(tex, tc);
    if(texel.r > near || texel.r < far) texel = vec4(0.0,0.0,0.0,1.0);
    
    gl_FragColor = texel;
}