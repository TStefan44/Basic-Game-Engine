#pragma once
#include "stdafx.h"
#include "Camera.h"

using namespace camera;

Camera::Camera(CameraView camView, Vector3 target, Vector3 position, Vector3 up,
	GLfloat nearPlane, GLfloat farPlane, GLfloat fov,
	GLfloat moveSpeed, GLfloat rotateSpeed) :
	camView(camView), position(position), target(target), up(up),
	moveSpeed(moveSpeed), rotateSpeed(rotateSpeed), nearPlane(nearPlane),
	farPlane(farPlane), fov(fov)
{
	// Init deltaTime
	deltaTime = 0;

	// Init dir axis
	calculateAxis();

	// Init worldMatrix and viewMatrix
	updateWorldView();

	// Init perspective matrix with a perspective projection
	GLfloat screenWidth = (GLfloat)Globals::getScreenWidth();
	GLfloat screenHeight = (GLfloat)Globals::getScreenHeight();

	perspectiveMatrix = perspectiveMatrix.SetIdentity();
	aspect = screenWidth / screenHeight;
	perspectiveMatrix = perspectiveMatrix.SetPerspective(fov, aspect, nearPlane, farPlane);
}

Camera::Camera(Vector3 target, Vector3 position, Vector3 up,
	GLfloat nearPlane, GLfloat farPlane, GLfloat fov) :
	Camera(CameraView::FIRST_PERSON, target, position, up, nearPlane, farPlane, fov, 0.5f, 10.f) {}

Camera::Camera(Vector3 target, Vector3 position, Vector3 up) :
	Camera(target, position, up, 0.001f, 100.f, toRadians(45)) {}

Camera::Camera() : Camera(Vector3(0, 0, 2), Vector3(0, 0, -1), Vector3(0, 1, 0), 0.1f, 100.f, toRadians(45)) {}

/*
* Move camera on OZ axis with given sens
* 1 - forward and 0 - back
*/
void Camera::MoveOz(GLint sens) {
	Vector3 dirVect = zAxis * moveSpeed * (float)sens * deltaTime;
	position += dirVect;
	target += dirVect;

	updateWorldView();
}

/*
* Move camera on OX axis with given sens
* 1 - right and 0 - left
*/
void Camera::MoveOx(GLint sens) {
	Vector3 dirVect = xAxis * moveSpeed * (float)sens * deltaTime;
	position += dirVect;
	target += dirVect;

	updateWorldView();
}

/*
* Move camera on OY axis with given sens
* 1 - up and 0 - down
*/
void Camera::MoveOy(GLint sens) {
	Vector3 dirVect = yAxis * moveSpeed * (float)sens * deltaTime;
	position += dirVect;
	target += dirVect;

	updateWorldView();
}

/*
* Rotate camera around OX axis with given sens
* 1 - trigonometric and 0 - clockwise
*/
void Camera::rotateOx(GLint sens) {
	Matrix mRotateOx;
	mRotateOx.SetRotationX(toRadians(rotateSpeed * (float)sens * deltaTime));

	Vector4 rotatedLocalUp = mRotateOx * Vector4(0, 1, 0, 0);
	Vector4 localTarget = Vector4(0.f, 0.f, -(target - position).Length(), 1.f);
	Vector4 rotatedTarget = mRotateOx * localTarget;

	up = (rotatedLocalUp * worldMatrix).Normalize().toVector3();
	target = (rotatedTarget * worldMatrix).toVector3();

	calculateAxis();
	updateWorldView();
}

/*
* Rotate camera around OY axis with given sens
* 1 - trigonometric and 0 - clockwise
*/
void Camera::rotateOy(GLint sens) {
	Matrix mRotateOY;
	mRotateOY.SetRotationY(toRadians(rotateSpeed * (float)sens * deltaTime));

	Vector4 localTarget = Vector4(0.f, 0.f, -(target - position).Length(), 1.f);
	Vector4 rotatedTarget = mRotateOY * localTarget;

	target = (rotatedTarget * worldMatrix).toVector3();

	zAxis = -(target - position).Normalize();
	xAxis = zAxis.Cross(yAxis).Normalize();

	updateWorldView();
}

/*
* Rotate camera around OZ axis with given sens
* 1 - trigonometric and 0 - clockwise
*/
void Camera::rotateOz(GLint sens) {
	Matrix mRotateOz;
	mRotateOz.SetRotationY(toRadians(rotateSpeed * (float)sens * deltaTime));

	Vector4 rotatedLocalUp = mRotateOz * Vector4(0, 1, 0, 0);

	up = (rotatedLocalUp * worldMatrix).Normalize().toVector3();

	yAxis = up.Normalize();
	xAxis = zAxis.Cross(yAxis).Normalize();

	updateWorldView();
}

void Camera::updateWorldView() {
	Matrix R;
	R.m[0][0] = xAxis.x; R.m[0][1] = xAxis.y; R.m[0][2] = xAxis.z; R.m[0][3] = 0;
	R.m[1][0] = yAxis.x; R.m[1][1] = yAxis.y; R.m[1][2] = yAxis.z; R.m[1][3] = 0;
	R.m[2][0] = zAxis.x; R.m[2][1] = zAxis.y; R.m[2][2] = zAxis.z; R.m[2][3] = 0;
	R.m[3][0] = 0; R.m[3][1] = 0; R.m[3][2] = 0; R.m[3][3] = 1;

	Matrix T;
	Matrix invT;
	T.SetTranslation(position.x, position.y, position.z);
	invT.SetTranslation(-position.x, -position.y, -position.z);

	worldMatrix = R * T;
	viewMatrix = invT * R.Transpose();
}

/*
* Recalculate camera's own axis after a rotation
*/
void inline Camera::calculateAxis() {
	zAxis = -(target - position).Normalize();
	yAxis = up.Normalize();
	xAxis = zAxis.Cross(yAxis).Normalize();
}

/*
* Setter and Getter functions
*/
void Camera::setPerspective(GLfloat nearPlane, GLfloat farPlane, GLfloat fov) {
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	this->fov = fov;
}

Matrix Camera::getViewMatrix() const {
	return viewMatrix;
}

Matrix Camera::getWorldMatrix() const {
	return worldMatrix;
}

Matrix Camera::getPerspectiveMatrix() const {
	return perspectiveMatrix;
}

Vector3 Camera::getPosition() const {
	return position;
}

void Camera::setDeltaTime(GLfloat deltaTime) {
	this->deltaTime = deltaTime;
}

void Camera::setMoveSpeed(GLfloat moveSpeed) {
	this->moveSpeed = moveSpeed;
}

void Camera::setRotateSpeed(GLfloat rotateSpeed) {
	this->rotateSpeed = rotateSpeed;
}