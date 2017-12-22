#version 430 core

uniform float fTime;
uniform vec3 myLightPosition;
uniform vec3 vEye;
uniform mat4 mTransform;

uniform  sampler2D texture0;
uniform  sampler2D texture1;

in vec4 newPos;
out vec4 fColor;
in vec2 textureCoords;


vec3 getAmbient(in vec3 light, in vec3 mat)
{
	return light * mat;
}

vec3 getDifusse(in vec3 light, in vec3 mat, in vec3 lightv, in vec3 Normal)
{
	return max(0.0, dot(lightv, Normal)) * mat * light;
}


vec3 getSpecular( in vec3 light, in vec3 mat, in vec3 lightv, in vec3 Normal, in float shininess, in vec3 myPos)
{
	vec3 viewv = normalize( vEye - myPos);
	vec3 refl = normalize(reflect(vEye - lightv, Normal ));

	//specular light computation
	vec3 specular = vec3( 0.0, 0.0, 0.0 );
	if( dot(lightv, viewv) > 0.0)
	{
		specular = pow(max(0.0, dot(viewv, refl)), shininess) * light * mat;
	}
	return specular;
}

float F(float x, float z) 
{
	//return   2.0 * sin (.005 * (x*x   +  z*z) - fTime);

	return 2.0 * sin (.005 * (x*x + z*z) - fTime);
}

//Derivada parcial de "a"
float Dp(float a, float b) 
{
	return 2 * cos(0.005 * (a*a + b*b) - fTime) * 0.005 * 2 * a;

	//return   2 * cos (0.005 * (a*a + b*b) - fTime) * 0.005 * 2.0*a;
}

vec3 calculateNormal (vec4 v)
{
	vec3 vt1 = vec3(1.0, (v.x, v.z), 0.0);
	vec3 vt2 = vec3(0.0, (v.z, v.x), 1.0);

	return normalize(cross (vt2, vt1));
}



void main()
{
	const vec3 lA = vec3( 0.2, 0.2, 0.2 );
	const vec3 lD = vec3( 0.4, 0.4, 0.4 );
	const vec3 lS = vec3( 1.0, 1.0, 1.0 );
	const vec3 mA = vec3( 1.0, 0.5, 0.0 );
	const vec3 mD = vec3( 1.0, 0.5, 0.0 );
	const vec3 mS = vec3( 1.0, 1.0, 1.0 );
	const float fShininess = 10.0;

	vec4 vNewPos = newPos;

	//calculamos la altura de este vertice

	//CALCULO DE NORMALES
	vec3 normal = calculateNormal (vNewPos);
	mat4 matForNormals = transpose(inverse(mTransform));
	vec3 newNormal = normalize(matForNormals * vec4(normal, 1.0)).xyz;

	//calculo de posicion de luz
	vec3 lightv = normalize( myLightPosition - vNewPos.xyz);

	vec4 color = clamp(
			  vec4(getAmbient(lA, mA), 1.0) 
			+ vec4(getDifusse(lD, mD, lightv, newNormal), 1.0)
			+ vec4(getSpecular(lS, mS, lightv, newNormal, fShininess, vNewPos.xyz), 1.0)
			,0.0, 1.0);

	vec4 textura0 = texture(texture0, textureCoords);
    vec4 textura1 = texture(texture1, textureCoords);
	vec4 textures = mix(textura0, textura1, 0.5);
	fColor = color* textures; 
	//Final color = glColor*texture*lighting;
}