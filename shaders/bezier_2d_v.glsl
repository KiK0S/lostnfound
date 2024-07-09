#version 300 es

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexturePosition;

uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uProjectionMatrix;
uniform vec2 controlPoints[4];

out vec2 bezierPos;

void main()
{
	// Map the vertex position to the parameter t (range 0 to 1)
	float t = (aPos.x + 1.0) / 2.0; // Assumes quad vertices range from -1 to 1

	// Calculate the Bézier curve position
	float u = 1.0 - t;
	vec2 P0 = controlPoints[0];
	vec2 P1 = controlPoints[1];
	vec2 P2 = controlPoints[2];
	vec2 P3 = controlPoints[3];
	bezierPos = u*u*u * P0 +
							3.0 * u*u * t * P1 +
							3.0 * u * t*t * P2 +
							t*t*t * P3;

	// Calculate the tangent vector of the Bézier curve
	vec2 tangent = -3.0 * u*u * P0 +
									3.0 * (u*u - 2.0 * u * t) * P1 +
									3.0 * (2.0 * t * u - t*t) * P2 +
									3.0 * t*t * P3;

	// Normalize the tangent vector to get the direction
	tangent = normalize(tangent);

	// Calculate the normal vector (perpendicular to the tangent)
	vec2 normal = normalize(vec2(-tangent.y, tangent.x));

	// Adjust the Bézier position along the normal
	bezierPos += 0.01 * aPos.y * normal;

	gl_Position = vec4(bezierPos, 0.0, 1.0);
}