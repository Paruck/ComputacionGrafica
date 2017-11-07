#version 430 core

layout (location = 0) in vec4 vPosition;

uniform mat4 mTransform;
uniform float fTime;
uniform vec3 myLightPosition;
uniform vec3 vEye;

out vec4 color;

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

//Derivada parcial de "x"
float Dx(float x, float z) 
{
	return	2*sin(fTime*x)*-2*sin(fTime*z)+2*fTime*cos(fTime*x)*2*cos(fTime*z);
}

//Derivada parcial de "z"
float Dz(float x, float z) 
{
	return	2*sin(fTime*x)*-2*fTime*sin(fTime*z)+2*cos(fTime*x)*2*cos(fTime*z);
}

float F(float x, float z) 
{
	return 2*sin(fTime*vPosition.x)*2*cos(fTime*vPosition.z);
}

//calculamos la normal antes de aplicar transformaciones
vec3 calculateNormal (vec4 v)
{
	vec3 vt1 = vec3(1.0, Dx(v.x, v.z), 0.0);
	vec3 vt2 = vec3(0.0, Dz(v.z, v.x), 1.0);

	return cross (vt1, vt2);
}

void main()
{
	//COLORES DE LUZ Y MATERIAL
	const vec3 lA = vec3( 0.2, 0.2, 0.2 );
	const vec3 lD = vec3( 0.4, 0.4, 0.4 );
	const vec3 lS = vec3( 1.0, 1.0, 1.0 );
	const vec3 mA = vec3( 1.0, 0.5, 0.0 );
	const vec3 mD = vec3( 1.0, 0.5, 0.0 );
	const vec3 mS = vec3( 1.0, 1.0, 1.0 );
	const float fShininess = 10.0;

	vec4 vNewVec = vPosition;
	//calculamos la altura de este vertice
	vNewVec.y = F(vNewVec.x, vNewVec.z);

	//CALCULO DE NORMALES
	vec3 normal = normalize(calculateNormal (vNewVec));
	mat4 matForNormals = transpose(inverse(mTransform));
	vec3 newNormal = normalize(matForNormals * vec4(normal, 1.0)).xyz;

	//calculo de posicion de luz
	vec3 lightv = normalize( myLightPosition - vNewVec.xyz);

	color = clamp(
			  vec4(getAmbient(lA, mA), 1.0) 
			+ vec4(getDifusse(lD, mD, lightv, newNormal), 1.0)
			+ vec4(getSpecular(lS, mS, lightv, newNormal, fShininess, vNewVec.xyz), 1.0)
			,0.0, 1.0);
	
   gl_Position = mTransform * vNewVec;
}