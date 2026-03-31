#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_operation.hpp>
#include "Shader.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/matrix_query.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_major_storage.hpp>
#include <glm/gtc/matrix_access.hpp>  // este es el que tiene funcion row

using namespace glm;

Camera::Camera(Viewport* vp)
  : mViewMat(1.0)
  , mProjMat(1.0)
  , xRight(vp->width() / 2.0)
  , xLeft(-xRight)
  , yTop(vp->height() / 2.0)
  , yBot(-yTop)
  , mViewPort(vp)
{
	setPM();
}

void 
Camera::SetAxes()
{
	mRight = glm::row(mViewMat, 0);
	mUpward = glm::row(mViewMat, 1);
	mFront = -glm::row(mViewMat, 2); // negado porque la cámara mira hacia -Z
}

void
Camera::uploadVM() const
{
	Shader::setUniform4All("modelView", mViewMat);
}

void
Camera::setVM()
{
	mViewMat = lookAt(mEye, mLook, mUp); // glm::lookAt defines the view matrix
	SetAxes();
}

void
Camera::set2D()
{
	mEye = {0, 0, 500};
	mLook = {0, 0, 0};
	mUp = {0, 1, 0};
	setVM();
}

void
Camera::set3D()
{
	mEye = {500, 500, 500};
	mLook = {0, 10, 0};
	mUp = {0, 1, 0};
	setVM();
}

void
Camera::pitch(GLfloat a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::vec3(1.0, 0, 0));
	// glm::rotate returns mViewMat * rotationMatrix
	SetAxes();
}

void
Camera::yaw(GLfloat a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::vec3(0, 1.0, 0));
	// glm::rotate returns mViewMat * rotationMatrix
	SetAxes();
}

void
Camera::roll(GLfloat a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::vec3(0, 0, 1.0));
	// glm::rotate returns mViewMat * rotationMatrix
	SetAxes();
}

void 
Camera::moveLR(GLfloat d) {
	mEye += mRight * d;
	mLook += mRight * d;
	mViewMat = glm::translate(mViewMat, glm::vec3(-d, 0, 0)); // en espacio de cámara
	SetAxes();
}

void 
Camera::moveUD(GLfloat d) {
	mEye += mUpward * d;
	mLook += mUpward * d;
	mViewMat = glm::translate(mViewMat, glm::vec3(0, -d, 0));
	SetAxes();
}

void 
Camera::moveFB(GLfloat d) {
	mEye += mFront * d;
	mLook += mFront * d;
	mViewMat = glm::translate(mViewMat, glm::vec3(0, 0, d));
	SetAxes();
}

void
Camera::setSize(GLdouble xw, GLdouble yh)
{
	xRight = xw / 2.0;
	xLeft = -xRight;
	yTop = yh / 2.0;
	yBot = -yTop;
	setPM();
}

void
Camera::setScale(GLdouble s)
{
	mScaleFact -= s;
	if (mScaleFact < 0)
		mScaleFact = 0.01;
	setPM();
}

void
Camera::setPM()
{
	if (bOrto) { //  if orthogonal projection
		mProjMat = ortho(xLeft * mScaleFact,
		                 xRight * mScaleFact,
		                 yBot * mScaleFact,
		                 yTop * mScaleFact,
		                 mNearVal,
		                 mFarVal);
		// glm::ortho defines the orthogonal projection matrix
	}
}

void
Camera::uploadPM() const
{
	Shader::setUniform4All("projection", mProjMat);
}

void
Camera::upload() const
{
	mViewPort->upload();
	uploadVM();
	uploadPM();
}
