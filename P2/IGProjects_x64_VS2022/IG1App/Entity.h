#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

class Abs_Entity // abstract class
{
public:
	Abs_Entity()
	  : mModelMat(1.0)  // 4x4 identity matrix
	  , mShader(nullptr) {};
	virtual ~Abs_Entity();

	Abs_Entity(const Abs_Entity& e) = delete;            // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete; // no copy assignment

	virtual void render(const glm::mat4& modelViewMat) const = 0; // abstract method
	virtual void update();

	// modeling matrix
	glm::mat4 const& modelMat() const { return mModelMat; };
	void setModelMat(glm::mat4 const& aMat) { mModelMat = aMat; };

	// load or unload entity data into the GPU
	void load();
	void unload();

protected:
	Mesh* mMesh = nullptr; // the mesh
	glm::mat4 mModelMat;  // modeling matrix
	Shader* mShader; // shader

	// transfers modelViewMat to the GPU
	virtual void upload(const glm::mat4& mModelViewMat) const;
};


class EntityWithColors : public Abs_Entity
{
public:
	explicit EntityWithColors();
	virtual void render(const glm::mat4& modelViewMat) const override;
};

class SingleColorEntity : public Abs_Entity {
public:
	explicit SingleColorEntity(glm::vec4 color = glm::vec4(1.0f));
	void render(const glm::mat4& modelViewMat) const override;

	glm::vec4 getColor() const { return mColor; }
	void setColor(glm::vec4 c) { mColor = c; }
protected:
	glm::vec4 mColor;
};

class RGBAxes : public EntityWithColors
{
public:
	explicit RGBAxes(GLdouble l);
};

class RegularPolygon : public SingleColorEntity {
public:
	explicit RegularPolygon(GLuint num, GLdouble r);
};

class Cube : public SingleColorEntity {
public:
	Cube(GLdouble l);
	void render(const glm::mat4& modelViewMat) const override;
};

class RGBCube: public EntityWithColors
{
public:
	RGBCube(GLdouble l);
	void update() override;

private:
	int _angle = 0.0;
	GLdouble _l; // length
	int _axisState = 0; // comienza en x.

	// 0 -> eje X.
	// 1 -> eje Z.
	// 2 -> eje Y.
	void rotateOnAxis(GLint n);
};

class RGBTriangle : public EntityWithColors {
public:
	explicit RGBTriangle(GLdouble l, GLdouble r);
	void update() override;
private:
	GLdouble _l; // length
	GLdouble _r; // radius
	float _localAngle;
	float _orbitAngle;
};

class RGBRectangle : public EntityWithColors {
public:
	explicit RGBRectangle(GLdouble w, GLdouble h);
	void render(const glm::mat4& modelViewMat) const override;
};

class BoxOutline : public SingleColorEntity {
public:
	explicit BoxOutline(GLdouble length);
	void render(const glm::mat4& modelViewMat) const override;
};

class EntityWithTexture : public Abs_Entity {
public:
	explicit EntityWithTexture();
	void render(const glm::mat4& modelViewMat) const override;
	void setTexture(Texture* t) { mTexture = t; }
protected:
	Texture* mTexture = nullptr;
	bool mModulate = false;
};

class Ground : public EntityWithTexture {
public:
	explicit Ground(GLdouble w, GLdouble h);
};

class Star3D : public EntityWithTexture {
public:
	explicit Star3D(GLdouble re, GLuint np, GLdouble h);
	void render(const glm::mat4& modelViewMat) const override;
	void update() override;
private:
	GLdouble _h;
	float _zAngle = 0.0f;
	float _yAngle = 0.0f;
};

#endif //_H_Entities_H_
