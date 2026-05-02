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
#include <glm/gtx/rotate_vector.hpp> // para que no de problemas al rotar

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
Camera::setVM()
{
	mViewMat = lookAt(mEye, mLook, mUp); // glm::lookAt defines the view matrix
	SetAxes();
}

void 
Camera::set2D() {
	mEye = { 0, 0, 500 };
	mLook = { 0, 0, 0 };
	mUp = { 0, 1, 0 };
	mRadio = 500.0;
	mAng = 0.0;
	setVM();
}

void 
Camera::set3D() {
	mEye = { 500, 500, 500 };
	mLook = { 0, 10, 0 };
	mUp = { 0, 1, 0 };

	// inicializar mRadio y mAng coherentemente con mEye y mLook
	GLdouble dx = mEye.x - mLook.x;
	GLdouble dz = mEye.z - mLook.z;
	mRadio = sqrt(dx * dx + dz * dz);   // distancia horizontal al look
	mAng = glm::degrees(atan2(-dz, dx)); // ángulo coherente con orbit()

	setVM();
}

void 
Camera::setCenital() {
	mEye = { mLook.x, mLook.y + 1000, mLook.z }; // directamente encima del look
	mUp = { 0, 0, -1 }; // el up no puede ser (0,1,0) porque la cámara mira hacia -Y
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
	setVM();
}

void 
Camera::moveUD(GLfloat d) {
	mEye += mUpward * d;
	mLook += mUpward * d;
	setVM();
}

void Camera::pitchReal(GLfloat cs) {
	//Dice si
	mLook = mEye + rotate(mLook - mEye, (glm::radians(cs)), mRight);
	mUp = rotate(mUp, (glm::radians(cs)), mRight);

	setVM();
}

void Camera::yawReal(GLfloat cs) {
	//Dice no
	mLook = mEye + rotate(mLook - mEye, (glm::radians(cs)), mUpward);
	mUp = rotate(mUp, (glm::radians(cs)), mUpward);
	setVM();
}

void Camera::rollReal(GLfloat cs) {
	mUp = rotate(mUp, (glm::radians(cs)), mFront);
	setVM();
}

void Camera::orbit(GLdouble incAng, GLdouble incY) {
	mAng += incAng;
	mEye.x = mLook.x + cos(radians(mAng)) * mRadio;
	mEye.z = mLook.z - sin(radians(mAng)) * mRadio;
	mEye.y += incY;
	mUp = vec3(0, 1, 0);
	setVM();
}

void 
Camera::moveFB(GLfloat d) {
	mEye += mFront * d;
	mLook += mFront * d;
	setVM();
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
	if (bOrto) {
		mScaleFact -= s;
		if (mScaleFact < 0) mScaleFact = 0.01;
	}
	// si no es ortogonal, ajustamos el campo de visión (FOV) para simular el zoom
	else {
		mFov -= s * 10;
		if (mFov < 1.0f) mFov = 1.0f;
		if (mFov > 170.0f) mFov = 170.0f;
	}
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
	else {
		mProjMat = glm::perspective(glm::radians(mFov),
			xRight / yTop,
			mNearVal * 200, // para evitar el efecto "zoom"
			mFarVal);
	}
}

void Camera::upload() const {
	mViewPort->upload();
	uploadPM();
	uploadVM();
}

void Camera::uploadVM() const {
	Shader::setUniform4All("view", mViewMat);
}

void Camera::uploadPM() const {
	// Forzar que el shader "light" exista antes de setUniform4All
	Shader::get("light")->use();
	Shader::get("light")->setUniform("projection", mProjMat);

	Shader::setUniform4All("projection", mProjMat);
}
