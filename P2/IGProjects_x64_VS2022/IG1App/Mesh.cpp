#include "Mesh.h"

using namespace std;
using namespace glm;

// Placeholder for the pending index of a GPU object
constexpr GLuint NONE = numeric_limits<GLuint>::max();

Mesh::Mesh()
 : mVAO(NONE)
 , mVBO(NONE)
 , mCBO(NONE)
{
}

Mesh::~Mesh()
{
	unload();
}

void
Mesh::draw() const
{
	glDrawArrays(
	  mPrimitive,
	  0,
	  size()); // primitive graphic, first index and number of elements to be rendered
}

void
Mesh::load()
{
	assert(mVBO == NONE); // not already loaded

	if (vVertices.size() > 0) { // transfer data
		glGenBuffers(1, &mVBO);
		glGenVertexArrays(1, &mVAO);

		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, vVertices.size() * sizeof(vec3), vVertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
		glEnableVertexAttribArray(0);

		if (vColors.size() > 0) {             // upload colors
			glGenBuffers(1, &mCBO);

			glBindBuffer(GL_ARRAY_BUFFER, mCBO);
			glBufferData(GL_ARRAY_BUFFER, vColors.size() * sizeof(vec4), vColors.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), nullptr);
			glEnableVertexAttribArray(1);
		}

		if (vTexCoords.size() > 0) {
			glGenBuffers(1, &mTBO);
			glBindBuffer(GL_ARRAY_BUFFER, mTBO);
			glBufferData(GL_ARRAY_BUFFER, vTexCoords.size() * sizeof(glm::vec2),
				vTexCoords.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
			glEnableVertexAttribArray(2);
		}
	}
}

void
Mesh::unload()
{
	if (mVAO != NONE) {
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mVBO);
		mVAO = NONE;
		mVBO = NONE;

		if (mCBO != NONE) {
			glDeleteBuffers(1, &mCBO);
			mCBO = NONE;
		}

		if (mTBO != NONE) {
			glDeleteBuffers(1, &mTBO);
			mTBO = NONE;
		}
	}
}

void
Mesh::render() const
{
	assert(mVAO != NONE);

	glBindVertexArray(mVAO);
	draw();
}

Mesh*
Mesh::createRGBAxes(GLdouble l)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINES;

	mesh->mNumVertices = 6;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// X axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(l, 0.0, 0.0);
	// Y axis vertices
	mesh->vVertices.emplace_back(0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, l, 0.0);
	// Z axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, 0.0, l);

	mesh->vColors.reserve(mesh->mNumVertices);
	// X axis color: red  (Alpha = 1 : fully opaque)
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	// Y axis color: green
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// Z axis color: blue
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}

Mesh* Mesh::generateRegularPolygon(GLuint num, GLdouble r)
{
	// creamos malla y le ponemos la primitiva
	Mesh* mesh = new Mesh(); 
	mesh->mPrimitive = GL_LINE_LOOP;

	// establecemos num de vertices y al array de vertices le reservamos ese tamanio.
	mesh->mNumVertices = num;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// se van guardando en sentido antihorario
	GLdouble alpha = 90.0;
	GLdouble incremento = 360.0 / num;
	for (GLuint i = 0; i < num + 2; i++) {
		GLdouble x = r * glm::cos(glm::radians(alpha));
		GLdouble y = r * glm::sin(glm::radians(alpha));
		alpha += incremento;

		// (Cx + R * cos(alpha), Cy + R * sin(alpha), 0).
		mesh->vVertices.emplace_back(x, y, 0.0);
	}

	return mesh;
}

Mesh* Mesh::generateCube(GLdouble length) {
	Mesh* mesh = new Mesh();

	/*
	0---------2
	|    /    |
	1---------3
	*/

	// Establecemos primitiva GL_TRIANGLES
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 14;
	mesh->vVertices.reserve(mesh->mNumVertices);

	GLdouble r = length / 2;

	// TODO FIXEAR
	vector<glm::vec3> v = {

		// -Y
		{-r, -r, -r},
		{r, -r, -r},
		{-r, -r, r},
		{r, -r, r},

		// +X
		{r,r,r},
		{r, -r, -r},
		{r, r, -r},

		// -Z
		// el v[0]
		{-r, r, -r},

		// -X
		// el v[2]
		{-r, r, r},

		// +Y
		// el v[4]
		// el v[8]
		// el v[6]
	};

	for (int i = 0; i < 7; ++i) {
		mesh->vVertices.emplace_back(v[i]);
	}

	mesh->vVertices.push_back(mesh->vVertices[0]);
	mesh->vVertices.emplace_back(v[7]);
	mesh->vVertices.push_back(mesh->vVertices[2]);
	mesh->vVertices.emplace_back(v[8]);
	mesh->vVertices.push_back(mesh->vVertices[4]);
	mesh->vVertices.push_back(mesh->vVertices[8]);
	mesh->vVertices.push_back(mesh->vVertices[6]);

	return mesh;
}

Mesh* Mesh::generateRGBCubeTriangles(GLdouble length) {
	Mesh* mesh = new Mesh();

	// Establecemos primitiva GL_TRIANGLES
	mesh->mPrimitive = GL_TRIANGLES;
	mesh->mNumVertices = 36;
	mesh->vVertices.reserve(mesh->mNumVertices);

	GLdouble r = length / 2;

	vector<glm::vec3> v = {

		// Cara -Z
	   {-r,-r,-r}, { r,-r,-r}, { r, r,-r},
	   {-r,-r,-r}, { r, r,-r}, {-r, r,-r},

	   // Cara +Z
	   {-r,-r, r}, { r, r, r}, { r,-r, r},
	   {-r,-r, r}, {-r, r, r}, { r, r, r},

	   // Cara -X
	   {-r,-r,-r}, {-r, r,-r}, {-r, r, r},
	   {-r,-r,-r}, {-r, r, r}, {-r,-r, r},

	   // Cara +X
	   { r,-r,-r}, { r, r, r}, { r, r,-r},
	   { r,-r,-r}, { r,-r, r}, { r, r, r},

	   // Cara -Y
	   {-r,-r,-r}, {-r,-r, r}, { r,-r, r},
	   {-r,-r,-r}, { r,-r, r}, { r,-r,-r},

	   // Cara +Y
	   {-r, r,-r}, { r, r,-r}, { r, r, r},
	   {-r, r,-r}, { r, r, r}, {-r, r, r}
	};

	for (int i = 0; i < mesh->mNumVertices; ++i) {
		mesh->vVertices.emplace_back(v[i]);
	}


	// ----COLORES----.
	/* 0: red (1.0, 0.0, 0.0, 1.0);
	   1: green (0.0, 1.0, 0.0, 1.0);
	   2: blue (0.0, 0.0, 1.0, 1.0) */

	mesh->vColors.reserve(mesh->mNumVertices);

	// Z y -Z
	for (int i = 0; i < 12; ++i)
		mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);

	// X y -X
	for (int i = 0; i < 12; ++i)
		mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);

	// Y y -Y
	for (int i = 0; i < 12; ++i)
		mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	

	return mesh;
}

Mesh* Mesh::generateRectangle(GLdouble w, GLdouble h) {
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 4;
	mesh->vVertices.reserve(mesh->mNumVertices);
	GLdouble hw = w / 2;
	GLdouble hh = h / 2;
	mesh->vVertices.emplace_back(-hw, -hh, 0.0);
	mesh->vVertices.emplace_back(hw, -hh, 0.0);
	mesh->vVertices.emplace_back(-hw, hh, 0.0);
	mesh->vVertices.emplace_back(hw, hh, 0.0);
	return mesh;
}

Mesh* Mesh::generateRGBRectangle(GLdouble w, GLdouble h) {
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 4;
	mesh->vVertices.reserve(mesh->mNumVertices);
	GLdouble hw = w / 2;
	GLdouble hh = h / 2;
	mesh->vVertices.emplace_back(-hw, -hh, 0.0);
	mesh->vVertices.emplace_back(hw, -hh, 0.0);
	mesh->vVertices.emplace_back(-hw, hh, 0.0);
	mesh->vVertices.emplace_back(hw, hh, 0.0);
	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0); // Green
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0); // Blue
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0); // Red
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0); // Green
	return mesh;
}

Mesh* Mesh::generateBoxOutline(GLdouble length)
{
	Mesh* mesh = new Mesh();

	/*
	0---------2
	|    /    |
	1---------3
	*/

	// Establecemos primitiva GL_TRIANGLE_STRIP
	mesh->mPrimitive = GL_TRIANGLE_STRIP;

	mesh->mNumVertices = 10; // !!!! PONER LOS VERTICES JUSTOS
	mesh->vVertices.reserve(mesh->mNumVertices);

	GLdouble r = length / 2;

	// CARA 1 (PLANO XY).
	mesh->vVertices.emplace_back(r, r, -r); // 0.
	mesh->vVertices.emplace_back(r, -r, -r); // 1.
	mesh->vVertices.emplace_back(-r, r, -r); // 2.
	mesh->vVertices.emplace_back(-r, -r, -r); // 3.

	// CARA 2 (PLANO YZ).
	mesh->vVertices.emplace_back(-r, r, r); // 4.
	mesh->vVertices.emplace_back(-r, -r, r); // 5.

	// CARA 3 (PARALELO A XY).
	mesh->vVertices.emplace_back(r, r, r); // 6.
	mesh->vVertices.emplace_back(r, -r, r); // 7.

	// CARA 4 (PARALELO A YZ). (vertices de cierre)
	mesh->vVertices.push_back(mesh->vVertices[0]); // 8. = 0 (misma pos que el 0).
	mesh->vVertices.push_back(mesh->vVertices[1]); // 9. = 1 (misma pos que el 1).

	return mesh;
}

Mesh* Mesh::generateRectangleTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh) {
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 4;
	mesh->vVertices.reserve(4);

	GLdouble hw = w / 2, hh = h / 2;
	mesh->vVertices.emplace_back(-hw, -hh, 0.0);
	mesh->vVertices.emplace_back(hw, -hh, 0.0);
	mesh->vVertices.emplace_back(-hw, hh, 0.0);
	mesh->vVertices.emplace_back(hw, hh, 0.0);

	mesh->vTexCoords.reserve(4);
	mesh->vTexCoords.emplace_back(0.0, 0.0);
	mesh->vTexCoords.emplace_back(rw, 0.0);
	mesh->vTexCoords.emplace_back(0.0, rh);
	mesh->vTexCoords.emplace_back(rw, rh);

	return mesh;
}
