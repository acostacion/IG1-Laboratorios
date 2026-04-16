#ifndef _H_Mesh_H_
#define _H_Mesh_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <numbers>
#include <vector>

class Mesh
{
public:
	static Mesh* createRGBAxes(GLdouble l); // creates a new 3D-RGB axes mesh
	static Mesh* generateRegularPolygon(GLuint num, GLdouble r);
	static Mesh* generateCube(GLdouble length);
	static Mesh* generateRGBCubeTriangles(GLdouble length);
	static Mesh* generateRectangle(GLdouble w, GLdouble h);
	static Mesh* generateRGBRectangle(GLdouble w, GLdouble h);
	static Mesh* generateBoxOutline(GLdouble length);
	static Mesh* generateBoxOutlineTexCor(GLdouble length);
	static Mesh* generateRectangleTexCor(GLdouble w, GLdouble h, GLuint rw = 1, GLuint rh = 1);
	static Mesh* generateStar3D(GLdouble re, GLuint np, GLdouble h);
	static Mesh* generateStar3DTexCor(GLdouble re, GLuint np, GLdouble h);

	Mesh();
	virtual ~Mesh();

	Mesh(const Mesh& m) = delete;            // no copy constructor
	Mesh& operator=(const Mesh& m) = delete; // no copy assignment

	virtual void render() const;

	GLuint size() const { return mNumVertices; }; // number of elements
	std::vector<glm::vec3> const& vertices() const { return vVertices; };
	std::vector<glm::vec4> const& colors() const { return vColors; };

	void setColorBuffer(std::vector<glm::vec4> const& colors) {
		vColors = colors;
	}

	void setPrimitive(GLuint primitive) { mPrimitive = primitive; }

	virtual void load();
	virtual void unload();

protected:
	GLuint mPrimitive =
	  GL_TRIANGLES;						// graphic primitive: GL_POINTS, GL_LINES, GL_TRIANGLES, ...
	GLuint mNumVertices = 0;			// number of elements ( = vVertices.size())
	std::vector<glm::vec3> vVertices;	// vertex array
	std::vector<glm::vec4> vColors;		// color array
	std::vector<glm::vec2> vTexCoords;  // texture coordinates array
	std::vector<glm::vec3> vNormals;	//vector normales
	
	virtual void draw() const;

	GLuint mVAO;  // vertex array object
	GLuint mNBO;  // vertex array object con normales

private:
	GLuint mVBO;  // vertex buffer object
	GLuint mCBO;  // color buffer object
	GLuint mTBO = 0; // texture coordinates buffer object
};

class IndexMesh : public Mesh {
public:
	IndexMesh();
	virtual ~IndexMesh();
	void load() override;
	void unload() override;

	static IndexMesh* generateByRevolution(const std::vector<glm::vec2>& profile, GLuint nSamples, GLfloat angleMax = 2 * std::numbers::pi);
	void buildNormalVectors();
	static IndexMesh* generateSphere(GLdouble radius, GLuint nParallel, GLuint nMeridians);
	static IndexMesh* generateIndexedBox8(GLdouble l);
	static IndexMesh* generateIndexedBox(GLdouble l);

protected:
	void draw() const override;
	std::vector<GLuint> vIndexes;

private:
	GLuint mIBO;  // vertex object buffer
};

#endif //_H_Scene_H_
