
uniform sampler2D diffuse;
varying vec2 texCoordVar;

void main() {
    gl_FragColor = vec4(1,0,0,1);//texture2D(diffuse, texCoordVar);
}

//void main() {
//    gl_FragColor = texture2D(diffuse, texCoordVar);
//}