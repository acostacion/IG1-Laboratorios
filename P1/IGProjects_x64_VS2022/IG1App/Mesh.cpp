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
	mesh->mPrimitive = GL_TRIANGLES;
	mesh->mNumVertices = 5;
	mesh->vVertices.reserve(mesh->mNumVertices);

	GLdouble r = length / 2;

	// TODO hacer formal
	vector<glm::vec3> v = {
		{-r, r, r},
		{-r, -r, r},
		{r, r, r},
		{r, -r, r},
		{r, r, -r}

		// TODO hacer con triangle strip luego
	};

	vector<GLint> indexes = { 0, 1, 2, 3, 4};
	

	for (int i = 0; i < mesh->mNumVertices; ++i) {
		mesh->vVertices.emplace_back(v[indexes[i]]);
	}

	return mesh;
}
