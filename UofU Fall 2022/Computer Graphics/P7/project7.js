// This function takes the translation and two rotation angles (in radians) as input arguments.
// The two rotations are applied around x and y axes.
// It returns the combined 4x4 transformation matrix as an array in column-major order.
// You can use the MatrixMult function defined in project5.html to multiply two 4x4 matrices in the same format.
function GetModelViewMatrix( translationX, translationY, translationZ, rotationX, rotationY )
{
	// [TO-DO] Modify the code below to form the transformation matrix.
	var trans = [
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translationX, translationY, translationZ, 1
	];

	var rotY = [
		Math.cos(rotationY), 0, -Math.sin(rotationY), 0,
		0, 1, 0, 0,
		Math.sin(rotationY), 0, Math.cos(rotationY), 0,
		0, 0, 0, 1
	];

	var rotX = [
		1, 0, 0, 0,
		0, Math.cos(rotationX), Math.sin(rotationX), 0,
		0, -Math.sin(rotationX), Math.cos(rotationX), 0,
		0, 0, 0, 1
	];

	var combRot = MatrixMult(rotY, rotX);
	var trans2 = MatrixMult(trans, combRot);

	var mv = trans2;
	return mv;
}


// [TO-DO] Complete the implementation of the following class.

class MeshDrawer
{
	// The constructor is a good place for taking care of the necessary initializations.
	constructor()
	{
		// [TO-DO] initializations
		this.prog = InitShaderProgram(objVS, objFS);

		// Get the ids of the uniform variables in the shaders
		this.mvp = gl.getUniformLocation(this.prog, 'mvp');

		this.mv = gl.getUniformLocation(this.prog, 'mv');

		this.norm = gl.getUniformLocation(this.prog, 'norm');

		this.sampler = gl.getUniformLocation(this.prog, 'samp');

		this.light = gl.getUniformLocation(this.prog, 'light');

		this.shininess = gl.getUniformLocation(this.prog, 'shiny');

		// Get the ids of the vertex attributes in the shaders
		this.vertPos = gl.getAttribLocation(this.prog, 'pos');

		this.texPos = gl.getAttribLocation(this.prog, 'tPos');

		this.vertNorm = gl.getAttribLocation(this.prog, 'vertNorms');


		this.vertBuffer = gl.createBuffer();

		this.texBuffer = gl.createBuffer();

		this.normBuffer = gl.createBuffer();

		this.swapAxes = 0.0;
	}
	
	// This method is called every time the user opens an OBJ file.
	// The arguments of this function is an array of 3D vertex positions,
	// an array of 2D texture coordinates, and an array of vertex normals.
	// Every item in these arrays is a floating point value, representing one
	// coordinate of the vertex position or texture coordinate.
	// Every three consecutive elements in the vertPos array forms one vertex
	// position and every three consecutive vertex positions form a triangle.
	// Similarly, every two consecutive elements in the texCoords array
	// form the texture coordinate of a vertex and every three consecutive 
	// elements in the normals array form a vertex normal.
	// Note that this method can be called multiple times.
	setMesh( vertPos, texCoords, normals )
	{
		// [TO-DO] Update the contents of the vertex buffer objects.
		gl.useProgram(this.prog);

		gl.bindBuffer(gl.ARRAY_BUFFER, this.vertBuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertPos), gl.STATIC_DRAW);

		gl.bindBuffer(gl.ARRAY_BUFFER, this.texBuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(texCoords), gl.STATIC_DRAW);

		gl.bindBuffer(gl.ARRAY_BUFFER, this.normBuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(normals), gl.STATIC_DRAW);

		this.numTriangles = vertPos.length / 3;
	}
	
	// This method is called when the user changes the state of the
	// "Swap Y-Z Axes" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	swapYZ( swap )
	{
		// [TO-DO] Set the uniform parameter(s) of the vertex shader
		gl.useProgram(this.prog);
		this.swapAxes = swap;
		this.Shader_swapYZ = gl.getUniformLocation(this.prog, 'swapYZ');
		gl.uniform1f(this.Shader_swapYZ, this.swapAxes);
	}
	
	// This method is called to draw the triangular mesh.
	// The arguments are the model-view-projection transformation matrixMVP,
	// the model-view transformation matrixMV, the same matrix returned
	// by the GetModelViewProjection function above, and the normal
	// transformation matrix, which is the inverse-transpose of matrixMV.
	draw( matrixMVP, matrixMV, matrixNormal )
	{
		// [TO-DO] Complete the WebGL initializations before drawing
		gl.useProgram(this.prog);

		gl.uniformMatrix4fv(this.mvp, false, matrixMVP);
		gl.uniformMatrix4fv(this.mv, false, matrixMV);
		gl.uniformMatrix3fv(this.norm, false, matrixNormal);

		this.Shader_showTex = gl.getUniformLocation(this.prog, 'showTex');
		gl.uniform1f(this.Shader_showTex, this.showTex);

		this.Shader_swapYZ = gl.getUniformLocation(this.prog, 'swapYZ');
		gl.uniform1f(this.Shader_swapYZ, this.swapAxes);

		gl.uniform1f(this.sampler, 0);



		gl.bindBuffer(gl.ARRAY_BUFFER, this.normBuffer);
		gl.vertexAttribPointer(this.vertNorm, 3, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray(this.vertNorm);

		gl.bindBuffer(gl.ARRAY_BUFFER, this.texBuffer);
		gl.vertexAttribPointer(this.texPos, 2, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray(this.texPos);

		gl.bindBuffer(gl.ARRAY_BUFFER, this.vertBuffer);
		gl.vertexAttribPointer(this.vertPos, 3, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray(this.vertPos);


		gl.drawArrays(gl.TRIANGLES, 0, this.numTriangles);
	}
	
	// This method is called to set the texture of the mesh.
	// The argument is an HTML IMG element containing the texture data.
	setTexture( img )
	{
		// [TO-DO] Bind the texture
		const mytex = gl.createTexture();

		gl.bindTexture(gl.TEXTURE_2D, mytex);
		gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, img);
		gl.generateMipmap(gl.TEXTURE_2D);

		gl.activeTexture(gl.TEXTURE0);
		gl.bindTexture(gl.TEXTURE_2D, mytex);

		// You can set the texture image data using the following command.
		gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, img);

		// [TO-DO] Now that we have a texture, it might be a good idea to set
		// some uniform parameter(s) of the fragment shader, so that it uses the texture.
		this.showTex = 1.0;
	}
	
	// This method is called when the user changes the state of the
	// "Show Texture" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	showTexture( show )
	{
		// [TO-DO] set the uniform parameter(s) of the fragment shader to specify if it should use the texture.
		if (show == 0) {
			this.showTex = 0.0;
		}

		else {
			this.showTex = 1.0;
		}
	}
	
	// This method is called to set the incoming light direction
	setLightDir( x, y, z )
	{
		// [TO-DO] set the uniform parameter(s) of the fragment shader to specify the light direction.
		gl.useProgram(this.prog);
		gl.uniform3f(this.light, x, y, z);
	}
	
	// This method is called to set the shininess of the material
	setShininess( shininess )
	{
		// [TO-DO] set the uniform parameter(s) of the fragment shader to specify the shininess.
		gl.useProgram(this.prog);
		gl.uniform1f(this.shininess, shininess);
	}
}


// This function is called for every step of the simulation.
// Its job is to advance the simulation for the given time step duration dt.
// It updates the given positions and velocities.
function SimTimeStep( dt, positions, velocities, springs, stiffness, damping, particleMass, gravity, restitution )
{
	var forces = Array( positions.length ); // The total for per particle

	// [TO-DO] Compute the total force of each particle
	// force[i] force computation force is vec3 mig = gravity
	// spring is a list
	for (var j = 0; j < forces.length; j++)
	{
		forces[j] = gravity.mul(particleMass);
	}

	for (var k = 0; k < springs.length; k++)
	{
		var len = ((positions[springs[k].p1].sub(positions[springs[k].p0])).len());
		var dir = (positions[springs[k].p1].sub(positions[springs[k].p0])).div(len);
		var springForce = dir.mul(stiffness * (len - springs[k].rest));

		var l = (velocities[springs[k].p1].sub(velocities[springs[k].p0])).dot(dir);

		var dampingForce = dir.mul(damping * l);

		forces[springs[k].p0] = forces[springs[k].p0].add(springForce.add(dampingForce));
		forces[springs[k].p1] = forces[springs[k].p1].sub(springForce.add(dampingForce));

    }
	
	// [TO-DO] Update positions and velocities
	for (var i = 0; i < positions.length; i++)
	{
		var accel = forces[i].div(particleMass);

		velocities[i].inc(accel.mul(dt));

		positions[i].inc((velocities[i].mul(dt)));

		// [TO-DO] Handle collisions

		//Restitution is bounding box
		//if im out of bounds, don't expect 6 if for +-x, +-y, +-z
		if (positions[i].x >= 1) {
			var h = positions[i].x - 1;
			var hprime = restitution * h;

			positions[i].x = 1 - hprime;
			velocities[i].x = (-restitution) * velocities[i].x;
		}

		if (positions[i].x < -1) {
			var h = positions[i].x + 1;
			var hprime = restitution * h;

			positions[i].x = -1 - hprime;
			velocities[i].x = (-restitution) * velocities[i].x;
		}

		if (positions[i].y >= 1) {
			var h = positions[i].y - 1;
			var hprime = restitution * h;

			positions[i].y = 1 - hprime;
			velocities[i].y = (-restitution) * velocities[i].y;
		}

		if (positions[i].y < -1) {
			var h = positions[i].y + 1;
			var hprime = restitution * h;

			positions[i].y = -1 - hprime;
			velocities[i].y = (-restitution) * velocities[i].y;
		}

		if (positions[i].z >= 1) {
			var h = positions[i].z - 1;
			var hprime = restitution * h;

			positions[i].z = 1 - hprime;
			velocities[i].z = (-restitution) * velocities[i].z;
		}

		if (positions[i].z < -1) {
			var h = positions[i].z + 1;
			var hprime = restitution * h;

			positions[i].z = -1 - hprime;
			velocities[i].z = (-restitution) * velocities[i].z;
		}
    }
	
	
}

var objVS = `
	attribute vec3 pos;
	attribute vec2 tPos;
	attribute vec3 vertNorms;
	uniform float showTex;
	uniform float swapYZ;
	uniform mat4 mvp;
	uniform mat4 mv;
	uniform mat3 norm;
	

	varying highp vec2 texturePos;
	varying highp float texShown;
	varying vec3 norm_n;
	varying vec3 pos_mv;

	void main()
	{
		if (swapYZ == 0.0)
		{
			gl_Position = mvp * vec4(pos,1);
		}
		else {
			gl_Position = mvp * vec4(pos.xzy,1);
		}
		norm_n = norm * vertNorms;
		pos_mv = (mv * vec4(pos, 1)).xyz;
		texturePos = tPos;
		texShown = showTex;
	}
`;
// Fragment shader source code
var objFS = `
	precision mediump float;
	varying highp vec2 texturePos;
	varying highp float texShown;
	varying vec3 norm_n;
	varying vec3 pos_mv;
	uniform sampler2D samp;
	uniform vec3 light;
	uniform float shiny;
	void main()
	{
		vec3 normalized_n = normalize(norm_n);
		vec3 normalized_L = normalize(light);
		
		float diffuse = max(dot(normalized_n, normalized_L), 0.0);
		vec3 camView = normalize(-pos_mv);
		float spec = pow(max(dot(normalize((light + camView)), normalized_n), 0.0), shiny);

		if (texShown == 1.0)
		{
			gl_FragColor = (diffuse * texture2D(samp, texturePos)) + vec4((vec3(1, 1, 1) * spec), 1);
		}
		else {
			gl_FragColor = (diffuse * vec4(1,gl_FragCoord.z*gl_FragCoord.z,0,1)) + vec4((vec3(1, 1, 1) * spec), 1);
		}
	}
`;

