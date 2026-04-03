#ifndef _H_Camera_H_
#define _H_Camera_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Viewport.h"

class Camera
{
public:
	explicit Camera(Viewport* vp);
	~Camera() = default;

	// viewPort
	Viewport const& viewPort() const { return *mViewPort; };

	// view matrix
	glm::mat4 const& viewMat() const { return mViewMat; };

	void set2D();
	void set3D();
	void setCenital();

	void pitch(GLfloat a); // rotates a degrees on the X axis
	void yaw(GLfloat a);   // rotates a degrees on the Y axis
	void roll(GLfloat a);  // rotates a degrees on the Z axis

	void moveLR(GLfloat d); // mueve d unidades a la derecha (d>0) o a la izquierda (d<0)
	void moveFB(GLfloat d); // mueve d unidades hacia adelante (d>0) o hacia atrás (d<0)
	void moveUD(GLfloat d); // mueve d unidades hacia arriba (d>0) o hacia abajo (d<0)

	void pitchReal(GLfloat cs); //Rotacion en x (u)
	void yawReal(GLfloat cs); //Rotacion en y (v)
	void rollReal(GLfloat cs); //Rotacion en z (n)

	void orbit(GLdouble incAng, GLdouble incY); 

	// projection matrix
	glm::mat4 const& projMat() const { return mProjMat; };

	void changePrj() { bOrto = !bOrto; setPM(); }; // cambia de proyección ortogonal a perspectiva o viceversa

	// sets scene visible area size
	void setSize(GLdouble xw, GLdouble yh);
	// updates the scale factor
	void setScale(GLdouble s);

	// transfers its viewport, the view matrix and projection matrix to the GPU
	void upload() const;

protected:
	glm::vec3 mEye = {0.0, 0.0, 500.0}; // camera's position
	glm::vec3 mLook = {0.0, 0.0, 0.0};  // target's position
	glm::vec3 mUp = {0.0, 1.0, 0.0};    // the up vector

	glm::vec3 mRight;
	glm::vec3 mFront;
	glm::vec3 mUpward;

	// para el orbit()
	GLdouble mRadio = 1000.0; // Esfera virtual de radio 1000 
	GLdouble mAng = -45; // Longitud 45 grados oeste.

	GLfloat mFov = 50.0f; // field of view inicial

	void SetAxes(); // updates the right, front and up vectors

	glm::mat4 mViewMat;   // view matrix = inverse of modeling matrix
	void uploadVM() const; // transfers viewMat to the GPU

	glm::mat4 mProjMat;   // projection matrix
	void uploadPM() const; // transfers projMat to the GPU

	GLfloat xRight, xLeft, yTop, yBot;     // size of scene visible area
	GLfloat mNearVal = 1, mFarVal = 10000; // view volume
	GLfloat mScaleFact = 1;                // scale factor
	bool bOrto = true;                      // orthogonal or perspective projection

	Viewport* mViewPort; // the viewport

	void setVM();
	void setPM();
};

#endif //_H_Camera_H_
