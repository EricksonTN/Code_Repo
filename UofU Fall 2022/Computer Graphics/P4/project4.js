// This function takes the projection matrix, the translation, and two rotation angles (in radians) as input arguments.
// The two rotations are applied around x and y axes.
// It returns the combined 4x4 transformation matrix as an array in column-major order.
// The given projection matrix is also a 4x4 matrix stored as an array in column-major order.
// You can use the MatrixMult function defined in project4.html to multiply two 4x4 matrices in the same format.
function GetModelViewProjection( projectionMatrix, translationX, translationY, translationZ, rotationX, rotationY )
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
	var mvp = MatrixMult( projectionMatrix, trans2 );
	return mvp;
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

		// Get the ids of the vertex attributes in the shaders
		this.vertPos = gl.getAttribLocation(this.prog, 'pos');

		this.texPos = gl.getAttribLocation(this.prog, 'tPos');

		this.sampler = gl.getUniformLocation(this.prog, 'samp');

		this.vertBuffer = gl.createBuffer();

		this.texBuffer = gl.createBuffer();

		this.swapAxes = 0.0;

	}
	
	// This method is called every time the user opens an OBJ file.
	// The arguments of this function is an array of 3D vertex positions
	// and an array of 2D texture coordinates.
	// Every item in these arrays is a floating point value, representing one
	// coordinate of the vertex position or texture coordinate.
	// Every three consecutive elements in the vertPos array forms one vertex
	// position and every three consecutive vertex positions form a triangle.
	// Similarly, every two consecutive elements in the texCoords array
	// form the texture coordinate of a vertex.
	// Note that this method can be called multiple times.
	setMesh( vertPos, texCoords )
	{
		// [TO-DO] Update the contents of the vertex buffer objects.
		gl.useProgram(this.prog);
		gl.bindBuffer(gl.ARRAY_BUFFER, this.vertBuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertPos), gl.STATIC_DRAW);
		gl.bindBuffer(gl.ARRAY_BUFFER, this.texBuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(texCoords), gl.STATIC_DRAW);

		

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
	// The argument is the transformation matrix, the same matrix returned
	// by the GetModelViewProjection function above.
	draw( trans )
	{
		// [TO-DO] Complete the WebGL initializations before drawing
		gl.useProgram(this.prog);
		gl.uniformMatrix4fv(this.mvp, false, trans);

		this.Shader_showTex = gl.getUniformLocation(this.prog, 'showTex');
		gl.uniform1f(this.Shader_showTex, this.showTex);

		this.Shader_swapYZ = gl.getUniformLocation(this.prog, 'swapYZ');
		gl.uniform1f(this.Shader_swapYZ, this.swapAxes);

		gl.uniform1f(this.sampler, 0);

		gl.bindBuffer(gl.ARRAY_BUFFER, this.vertBuffer);
		gl.vertexAttribPointer(this.vertPos, 3, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray(this.vertPos);;

		gl.bindBuffer(gl.ARRAY_BUFFER, this.texBuffer);
		gl.vertexAttribPointer(this.texPos, 2, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray(this.texPos);;

		gl.drawArrays( gl.TRIANGLES, 0, this.numTriangles );
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
		gl.texImage2D( gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, img );

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
	
}

// Vertex shader source code
var objVS = `
	attribute vec3 pos;
	attribute vec2 tPos;
	uniform float showTex;
	uniform float swapYZ;
	uniform mat4 mvp;

	varying highp vec2 texturePos;
	varying highp float texShown;

	void main()
	{
		if (swapYZ == 0.0)
		{
			gl_Position = mvp * vec4(pos,1);
		}
		else {
			gl_Position = mvp * vec4(pos.xzy,1);
		}
		
		texturePos = tPos;
		texShown = showTex;
	}
`;
// Fragment shader source code
var objFS = `
	precision mediump float;
	varying highp vec2 texturePos;
	varying highp float texShown;
	uniform sampler2D samp;
	void main()
	{
		if (texShown == 1.0)
		{
			gl_FragColor = texture2D(samp, texturePos);
		}
		else {
			gl_FragColor = vec4(1,gl_FragCoord.z*gl_FragCoord.z,0,1);
		}
	}
`;
