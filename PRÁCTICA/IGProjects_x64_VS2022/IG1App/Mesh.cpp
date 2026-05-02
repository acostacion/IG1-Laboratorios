#include "Mesh.h"

using namespace std;
using namespace glm;

// Placeholder for the pending index of a GPU object
constexpr GLuint NONE = numeric_limits<GLuint>::max();

Mesh::Mesh()
 : mVAO(NONE)
 , mVBO(NONE)
 , mCBO(NONE)
 , mTBO(NONE)	
 , mNBO(NONE)
{
}

Mesh::~Mesh() {
	unload();
}

void
Mesh::draw() const {
	glDrawArrays(
	  mPrimitive,
	  0,
	  size()); // primitive graphic, first index and number of elements to be rendered
}

void
Mesh::load() {
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
			glBufferData(GL_ARRAY_BUFFER, vColors.size() * sizeof(vec4), 
				vColors.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), nullptr);
			glEnableVertexAttribArray(1);
		}

		if (vTexCoords.size() > 0) {
			glGenBuffers(1, &mTBO);
			glBindBuffer(GL_ARRAY_BUFFER, mTBO);
			glBufferData(GL_ARRAY_BUFFER, vTexCoords.size() * sizeof(vec2),
				vTexCoords.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), nullptr);
			glEnableVertexAttribArray(2);
		}

		if (vNormals.size() > 0) { 
			glGenBuffers(1, &mNBO);
			glBindBuffer(GL_ARRAY_BUFFER, mNBO);
			glBufferData(GL_ARRAY_BUFFER, vNormals.size() * sizeof(vec3),
				vNormals.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
			glEnableVertexAttribArray(3);
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

		// eliminar los vectores de normales de la GPU
		if (mNBO != NONE) {
			glDeleteBuffers(1, &mNBO);
			mNBO = NONE;
		}
	}
}

void
Mesh::render() const {
	assert(mVAO != NONE);

	glBindVertexArray(mVAO);
	draw();
}

Mesh* Mesh::createRGBAxes(GLdouble l)
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

Mesh* Mesh::generateBoxOutlineTexCor(GLdouble length) {
	Mesh* mesh = generateBoxOutline(length);
	mesh->vTexCoords.reserve(mesh->mNumVertices);

	/*
	0---------2
	|    /    |
	1---------3
	*/

	// Cara 1.
	mesh->vTexCoords.emplace_back(0, 1); // 0.
	mesh->vTexCoords.emplace_back(0, 0); // 1.
	mesh->vTexCoords.emplace_back(1, 1); // 2.
	mesh->vTexCoords.emplace_back(1, 0); // 3.

	// Cara 2.
	mesh->vTexCoords.emplace_back(0, 1); // 4.
	mesh->vTexCoords.emplace_back(0, 0); // 5.

	// Cara 3.
	mesh->vTexCoords.emplace_back(1, 1); // 6.
	mesh->vTexCoords.emplace_back(1, 0); // 7.

	// Cara 4.
	mesh->vTexCoords.emplace_back(0, 1); // 8.
	mesh->vTexCoords.emplace_back(0, 0); // 9.

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

Mesh* Mesh::generateStar3D(GLdouble re, GLuint np, GLdouble h) {
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_FAN;

	mesh->mNumVertices = 2 * np + 2;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// Centro del fan
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);

	GLdouble ri = re / 2.0;
	GLdouble angIncr = 360.0 / np; // ángulo entre puntas

	for (GLuint i = 0; i < np; i++) {
		GLdouble angExt = glm::radians(90.0 + i * angIncr);       // punta exterior
		GLdouble angInt = glm::radians(90.0 + (i + 0.5) * angIncr); // valle interior

		// Vértice exterior (punta)
		mesh->vVertices.emplace_back(
			re * cos(angExt), re * sin(angExt), h);
		// Vértice interior (valle)
		mesh->vVertices.emplace_back(
			ri * cos(angInt), ri * sin(angInt), h);
	}

	// Cerrar el fan repitiendo la primera punta
	mesh->vVertices.push_back(mesh->vVertices[1]);

	return mesh;
}

Mesh* Mesh::generateStar3DTexCor(GLdouble re, GLuint np, GLdouble h) {
	Mesh* mesh = generateStar3D(re, np, h); // reutilizamos la geometría
	mesh->vTexCoords.reserve(mesh->mNumVertices);

	GLdouble ri = re / 2.0;
	GLdouble angIncr = 360.0 / np;

	// Centro -> centro de la textura
	mesh->vTexCoords.emplace_back(0.5, 0.5);

	for (GLuint i = 0; i < np; i++) {
		GLdouble angExt = glm::radians(90.0 + i * angIncr);
		GLdouble angInt = glm::radians(90.0 + (i + 0.5) * angIncr);

		// Exterior: borde de la textura (normalizado a [0,1])
		mesh->vTexCoords.emplace_back(
			0.5 + 0.5 * cos(angExt),
			0.5 + 0.5 * sin(angExt));
		// Interior: mitad del radio en textura
		mesh->vTexCoords.emplace_back(
			0.5 + 0.25 * cos(angInt),
			0.5 + 0.25 * sin(angInt));
	}

	// Cerrar
	mesh->vTexCoords.push_back(mesh->vTexCoords[1]);

	return mesh;
}

IndexMesh::IndexMesh() : mIBO(NONE) {
}

IndexMesh::~IndexMesh() {
	unload();
}

void IndexMesh::load() {
	Mesh::load(); 
	glBindVertexArray(mVAO);
	glGenBuffers(1, &mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		vIndexes.size() * sizeof(GLuint),
		vIndexes.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void IndexMesh::unload() {
	if (mIBO != NONE) {
		glDeleteBuffers(1, &mIBO);
		mIBO = NONE;
	}
	Mesh::unload(); // esto ya borra mVAO, mVBO, etc.
}

IndexMesh* IndexMesh::generateByRevolution(const std::vector<glm::vec2>& profile, GLuint nSamples, GLfloat angleMax) {
	IndexMesh* mesh = new IndexMesh();
	mesh->mPrimitive = GL_TRIANGLES;

	int tamPerfil = profile.size();
	
	mesh->vVertices.reserve((nSamples + 1) * tamPerfil);
	mesh->vTexCoords.reserve((nSamples + 1) * tamPerfil);
	
	GLdouble theta1 = angleMax / nSamples; 
	//Crea los vertices
	for (int i = 0; i <= nSamples; ++i) { // muestra i-esima
		GLdouble c = cos(i * theta1), s = sin(i * theta1);

		// recorrido horizontal textura
		float u = float(i) / float(nSamples);

		for (int j = 0; j < tamPerfil; ++j) {// rota el perfil
			auto p = profile[j]; 
			mesh->vVertices.emplace_back(p.x * c, p.y, -p.x * s);

			// recorrido vertical textura
			float v = float(j) / float(tamPerfil - 1);
			mesh->vTexCoords.emplace_back(u, v); // coordenadas textura (si no se hace con float() se ralla)

		}
	}

	//Despues une los vertices para formar caras
	for (int i = 0; i < nSamples; ++i) // caras i a i + 1 (todas las repeticiones del perfil)
		for (int j = 0; j < tamPerfil - 1; ++j) { // una cara (puntos dentro del perfil)
			if (profile[j].x != 0.0) // triangulo inferior
				for (auto [s, t] : { std::pair{i, j}, std::pair{i, j + 1}, std::pair{i + 1, j} }) {
					mesh->vIndexes.push_back(s * tamPerfil + t);
				}


			if (profile[j + 1].x != 0.0) // triangulo superior
				for (auto [s, t] : { std::pair{i, j + 1}, std::pair{i + 1, j + 1}, std::pair{i + 1, j} }) {
					mesh->vIndexes.push_back(s * tamPerfil + t);
				}
		}

	//Reserva vertices
	mesh->mNumVertices = mesh->vVertices.size();

	mesh->buildNormalVectors();
	//Devuelve la malla correspondiente
	return mesh;
}

void IndexMesh::buildNormalVectors() {
	vNormals.clear();
	// inicializar una normal por VERTICE (no por índice)
	vNormals.resize(vVertices.size(), glm::vec3(0.0f, 0.0f, 0.0f));

	// acumular normales de cada triángulo en sus vértices
	for (int i = 0; i + 2 < (int)vIndexes.size(); i += 3) {
		GLuint i0 = vIndexes[i], i1 = vIndexes[i + 1], i2 = vIndexes[i + 2];
		glm::vec3 normal = glm::cross(
			vVertices[i1] - vVertices[i0],
			vVertices[i2] - vVertices[i0]
		);
		// no normalizar aquí: la contribución es proporcional al área del triángulo
		vNormals[i0] += normal;
		vNormals[i1] += normal;
		vNormals[i2] += normal;
	}

	// normalizar al final
	for (auto& n : vNormals)
		if (glm::length(n) > 0.0001f)
			n = glm::normalize(n);
}

IndexMesh* IndexMesh::generateSphere(GLdouble radius, GLuint nParallel, GLuint nMeridians) {
	std::vector<glm::vec2> profile;

	// Se van guardando en sentido antihorario desde x = 0
	GLdouble alpha = 90.0;
	GLdouble incremento = 180.0 / (nParallel + 1); //Solo media esfera, por eso 180
	//Conseguimos los puntos del perfil
	for (GLuint i = 0; i < nParallel + 1; i++) {
		GLdouble x = radius * glm::cos(glm::radians(alpha));
		GLdouble y = radius * glm::sin(glm::radians(alpha));
		alpha -= incremento;

		profile.emplace_back(x, y);
	}

	return generateByRevolution(profile, nMeridians, 2 * std::numbers::pi);
}

IndexMesh* IndexMesh::generateIndexedBox8(GLdouble l) {
	IndexMesh* mesh = new IndexMesh();
	mesh->mPrimitive = GL_TRIANGLES;

	GLdouble h = l / 2.0;

	// 8 vértices del cubo (según las diapositivas)
	mesh->vVertices = {
		{ h,  h, -h}, // 0
		{ h, -h, -h}, // 1
		{ h,  h,  h}, // 2
		{ h, -h,  h}, // 3
		{-h,  h,  h}, // 4
		{-h, -h,  h}, // 5
		{-h,  h, -h}, // 6
		{-h, -h, -h}  // 7
	};

	// 36 índices (12 triángulos, 2 por cara) según las diapositivas
	mesh->vIndexes = {
		0, 1, 2,  2, 1, 3,  // cara derecha
		2, 3, 4,  4, 3, 5,  // cara frontal
		4, 5, 6,  6, 5, 7,  // cara izquierda
		6, 7, 0,  0, 7, 1,  // cara trasera
		4, 6, 2,  2, 6, 0,  // cara superior
		1, 7, 3,  3, 7, 5   // cara inferior
	};

	mesh->mNumVertices = mesh->vVertices.size();
	mesh->buildNormalVectors();

	return mesh;
}

IndexMesh* IndexMesh::generateIndexedBox(GLdouble l) {
	IndexMesh* mesh = new IndexMesh();
	mesh->mPrimitive = GL_TRIANGLES;
	GLdouble h = l / 2.0;

	// Cada cara define sus 4 vértices propios, sin compartir con otras caras.
	// Orden de los vértices en cada cara: esquina superior-izquierda en sentido
	// antihorario visto desde el exterior (para que la normal salga hacia fuera).
	mesh->vVertices = {
		// Cara derecha (x = +h), normal (1,0,0)
		{ h,  h, -h},  //  0
		{ h, -h, -h},  //  1
		{ h, -h,  h},  //  2
		{ h,  h,  h},  //  3

		// Cara izquierda (x = -h), normal (-1,0,0)
		{-h,  h,  h},  //  4
		{-h, -h,  h},  //  5
		{-h, -h, -h},  //  6
		{-h,  h, -h},  //  7

		// Cara superior (y = +h), normal (0,1,0)
		{-h,  h, -h},  //  8
		{ h,  h, -h},  //  9
		{ h,  h,  h},  // 10
		{-h,  h,  h},  // 11

		// Cara inferior (y = -h), normal (0,-1,0)
		{-h, -h,  h},  // 12
		{ h, -h,  h},  // 13
		{ h, -h, -h},  // 14
		{-h, -h, -h},  // 15

		// Cara frontal (z = +h), normal (0,0,1)
		{ h,  h,  h},  // 16
		{ h, -h,  h},  // 17
		{-h, -h,  h},  // 18
		{-h,  h,  h},  // 19

		// Cara trasera (z = -h), normal (0,0,-1)
		{-h,  h, -h},  // 20
		{-h, -h, -h},  // 21
		{ h, -h, -h},  // 22
		{ h,  h, -h},  // 23
	};

	// Cada cara: 2 triángulos -> 6 índices
	// Triángulo 1: 0,1,2  Triángulo 2: 0,2,3
	mesh->vIndexes = {
		 0,  1,  2,   0,  2,  3,  // cara derecha
		 4,  5,  6,   4,  6,  7,  // cara izquierda
		 8,  9, 10,   8, 10, 11,  // cara superior
		12, 13, 14,  12, 14, 15,  // cara inferior
		16, 17, 18,  16, 18, 19,  // cara frontal
		20, 21, 22,  20, 22, 23,  // cara trasera
	};

	mesh->mNumVertices = mesh->vVertices.size();
	mesh->buildNormalVectors();
	return mesh;
}

void IndexMesh::draw() const {
	glDrawElements(
		mPrimitive, // primitiva ( GL_TRIANGLES , etc.)
		vIndexes.size(), // número de índices
		GL_UNSIGNED_INT, // tipo de los índices
		nullptr // offset en el VBO de índices
	);
}
