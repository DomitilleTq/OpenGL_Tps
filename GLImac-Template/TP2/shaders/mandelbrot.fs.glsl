#version 330

in vec3 vColor;
in vec2 vVertexPosition;

out vec3 fFragColor;
out vec2 fVertexPosition;

vec2 complexSqr(vec2 z){
	return vec2(z.x*z.x-z.y*z.y,2*z.x*z.y);
}

void main() {
	int N=100; int n=0;
	vec2 z = vec2(vVertexPosition.x,vVertexPosition.y);
	
	while (n<=N) {
		n++;
		if(length(z)<=2){
			fFragColor = vec3(0,0,0);
		}
		else{
		
			fFragColor = vec3(1,1,1);
			break;
		}
		z=complexSqr(z)+vVertexPosition;
	
	}
	
    //fFragColor = vColor;
    //fFragColor =vec3(1,0,0);
    fVertexPosition = vVertexPosition;
}
