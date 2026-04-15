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
	virtual void load();
	virtual void unload();

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

class EntityWithTexture : public Abs_Entity {
public:
	explicit EntityWithTexture();
	void render(const glm::mat4& modelViewMat) const override;
	void setTexture(Texture* t) { mTexture = t; }
protected:
	Texture* mTexture = nullptr;
	bool mModulate = false;
};

class ColorMaterialEntity : public SingleColorEntity {
public:
	explicit ColorMaterialEntity();
	void render(const glm::mat4& modelViewMat) const override;
	static void toggleNormals() { mShowNormals = !mShowNormals; }

private:
	static bool mShowNormals;
};

class CompoundEntity : public Abs_Entity {
public:
	virtual ~CompoundEntity();

	void render(const glm::mat4& modelViewMat) const override;
	void update() override;
	void load() override;
	void unload() override;

	void addEntity(Abs_Entity* ae);
protected:
	std::vector<Abs_Entity*> gObjects;
};

class RGBAxes : public EntityWithColors {
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

class BoxOutline : public EntityWithTexture {
public:
	explicit BoxOutline(GLdouble length);
	void render(const glm::mat4& modelViewMat) const override;

	void setTextureInterior(Texture* tex) { mTextureInterior = tex; }

protected:
	Texture* mTextureInterior = nullptr; // texture
};

class Box : public EntityWithTexture
{
public:
	explicit Box(GLdouble length);
	~Box() override;

	void render(const glm::mat4& modelViewMat) const override;
	void renderBox(const glm::mat4& modelViewMat) const;
	void renderBoxUpper(const glm::mat4& modelViewMat) const;
	void renderBoxLower(const glm::mat4& modelViewMat) const;

	void update() override;

	void load() override;
	void unload() override;

	void setTextureInterior(Texture* tex) { mTextureInterior = tex; }
	void setModelMatAbj(glm::dmat4 const& bMat) { mModelMatAbj = bMat; }
	void setModelMatArr(glm::dmat4 const& cMat) { mModelMatArr = cMat; }
	void setBaseTranslation(glm::mat4 const& t) { mBaseTranslation = t; }

protected:
	Texture* mTextureInterior = nullptr; // texture

	GLdouble _length; // length de la box.

	// --- tapa abajo
	Mesh* mMeshTapaAbj = nullptr;
	glm::mat4 mModelMatAbj = glm::mat4(1.0);

	// --- tapa arriba
	Mesh* mMeshTapaArr = nullptr;
	glm::mat4 mModelMatArr = glm::mat4(1.0);
private:

	glm::dmat4 mBaseTranslation = glm::mat4(1.0); // traslación base de la caja

	// --- para el update.
	double angle = 180.0;  // empiza en 180 grados (empieza abierta)
	GLint openState = 1; // 0 -> abres 1 -> cierras
};

class Ground : public EntityWithTexture {
public:
	explicit Ground(GLdouble w, GLdouble h);
	~Ground() override;
};

class Star3D : public EntityWithTexture {
public:
	explicit Star3D(GLdouble re, GLuint np, GLdouble h);
	void render(const glm::mat4& modelViewMat) const override;
	void update() override;
	void setBaseTransform(glm::mat4 const& t) { mBaseTransform = t; }

private:
	GLdouble _h;
	float _zAngle = 0.0f;
	float _yAngle = 0.0f;
	glm::mat4 mBaseTransform = glm::mat4(1.0); // base: traslación + escala
};

class Photo : public EntityWithTexture {
public:
	explicit Photo(GLdouble w, GLdouble h);
	void render(const glm::mat4& modelViewMat) const override;
	void update() override;
};

class GlassParapet : public EntityWithTexture {
public:
	explicit GlassParapet(GLdouble length);
	void render(const glm::mat4& modelViewMat) const override;
};

class Grass : public EntityWithTexture {
public:
	explicit Grass(GLdouble l);
	void render(const glm::mat4& modelViewMat) const override;

private:
	void createFirstGrass(const glm::mat4& modelViewMat) const;
	void createSecondGrass(const glm::mat4& modelViewMat) const;
	void createThirdGrass(const glm::mat4& modelViewMat) const;

protected:
	GLdouble _length;
};

// TODO:
// Assertion failed: mVBO == NONE, file C:\Users\Usuario\Music\IG1-Laboratorios\PRÁCTICA\IGProjects_x64_VS2022\IG1App\Mesh.cpp, line 31
class Torus : public SingleColorEntity {
public: // NOTA: singlecolorentity?
	/*
	R - radio desde origen de coordenadas
	r - radio del perfil
	nPoints - vertices de la circunferencia del perfil
	nSamples - numero de repeticiones del perfil en la figura final
	*/
	explicit Torus(GLdouble R, GLdouble r, GLuint nPoints = 40, GLuint nSamples = 40);
	void render(const glm::mat4& modelViewMat) const override;
};

class Sphere : public ColorMaterialEntity {
public:
	explicit Sphere(GLdouble radius, GLuint nParallels, GLuint nMeridians);
};

class Disk : public ColorMaterialEntity {
public:
	explicit Disk(GLdouble R, GLdouble r, GLuint nRings, GLuint nSamples);
};

class Cone : public ColorMaterialEntity {
	/*
		· cono(r = 0 o R = 0)
		· cilindro(R = r)
		· tronco de cono de altura h, radio inferior r, radio superior R, rRings vértices en el perfil y nSamples muestras de revolución.
	*/
public:
	explicit Cone(GLdouble h, GLdouble r, GLdouble R, GLuint nRings, GLuint nSamples);
};

class SphereWithTexture : public EntityWithTexture {
public:
	explicit SphereWithTexture(GLdouble radius, GLuint nParallels, GLuint nMeridians);
};

class Droid : public CompoundEntity {
public:
	explicit Droid(GLdouble radius);
private:
	Cone* createHead(GLdouble r);
	Cone* createEye(GLdouble r);
};

class Robot : public CompoundEntity {
public:
	explicit Robot(GLdouble radius);
};

class SnowManHat : public CompoundEntity {
public: explicit SnowManHat(GLdouble radius);
};

class SnowManHead : public CompoundEntity {
public:
	explicit SnowManHead(GLdouble radius);
private:
	Cone* createEye(GLdouble r);
	Cone* createNose(GLdouble r);
};

class SnowMan : public CompoundEntity {
public:
	explicit SnowMan(GLdouble radius);
};

class IndexedBox : public ColorMaterialEntity {
public:
	explicit IndexedBox(GLdouble l);
};


#endif //_H_Entities_H_
