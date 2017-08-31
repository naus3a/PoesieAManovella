#version 120

uniform sampler2DRect texWall;
uniform sampler2DRect texDpt;
uniform vec2 szWall;
uniform vec2 szDpt;
uniform vec2 posWall;

void main(){
    vec2 tc = gl_TexCoord[0].st;
    vec2 tcDpt;
    tcDpt.x = tc.x/szWall.x*szDpt.x;
    tcDpt.y = tc.y/szWall.y*szDpt.y;
    float dpt = texture2DRect(texDpt, tcDpt).r;
    vec4 texel = vec4(0.0,0.0,0.0,0.0);
    if(dpt>0.0){
        vec2 tcWall;
        tcWall.x = tc.x + posWall.x*szWall.x;
        tcWall.y = tc.y + posWall.y*szWall.y;
        tcWall.x = mod(tcWall.x, szWall.x);
        tcWall.y = mod(tcWall.y, szWall.y);
        texel = texture2DRect(texWall, tcWall)*dpt;
    }
    
    gl_FragColor = texel;
}