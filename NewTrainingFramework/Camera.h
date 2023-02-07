#pragma once
#include "../Utilities/utilities.h"

namespace camera {
	class Camera {

	public:
		// class structures
		enum class CameraType {
			PERSPECTIVE,
			PARALLEL
		};

		enum class CameraView {
			FIRST_PERSON,
			THIRD_PERSON
		};

	public:
		// Constructors
		Camera(CameraView camView, Vector3 target, Vector3 position, Vector3 up,
			GLfloat nearPlane, GLfloat farPlane, GLfloat fov,
			GLfloat moveSpeed, GLfloat rotateSpeed);

		Camera(Vector3 target, Vector3 position, Vector3 up,
			GLfloat nearPlane, GLfloat farPlane, GLfloat fov);

		Camera(Vector3 target, Vector3 position, Vector3 up);

		Camera();

		// Function for manipulating camera's position and rotation
		void MoveOx(GLint sens);
		void MoveOy(GLint sens);
		void MoveOz(GLint);

		void rotateOx(GLint sens);
		void rotateOy(GLint sens);
		void rotateOz(GLint sens);

		// Getter functions
		Matrix getViewMatrix() const;
		Matrix getWorldMatrix() const;
		Matrix getPerspectiveMatrix() const;

		// Setter functions
		void setDeltaTime(GLfloat deltaTime);
		void setMoveSpeed(GLfloat moveSpeed);
		void setRotateSpeed(GLfloat rotateSpeed);
		void setPerspective(GLfloat nearPlane, GLfloat farPlane, GLfloat fov);

		// Debug functions
		friend std::ostream& operator<<(std::ostream& os, const Camera& camera) {
			os  << " Camera View = " << camera.camView << " Position =  " << camera.position
				<< " Target = " << camera.target << " Up = " << camera.up << " Move Speed " << camera.moveSpeed
				<< " Rotate Speed = " << camera.rotateSpeed
				<< " fov = " << camera.fov << " Near Plane = " << camera.nearPlane << " Far Plane = " << camera.farPlane;
			return os;
		}
		friend std::ostream& operator<<(std::ostream& os, const CameraView& camView) {
			switch (camView) {
			case CameraView::FIRST_PERSON: os << "First Person"; break;
			case CameraView::THIRD_PERSON: os << "Third Person"; break;
			}
			return os;
		}
		friend std::ostream& operator<<(std::ostream& os, const CameraType& camType) {
			switch (camType) {
			case CameraType::PERSPECTIVE: os << "Perspective"; break;
			case CameraType::PARALLEL: os << "Parallel"; break;
			}
			return os;
		}

	private:
		// Internal camera functions:
		// Recalculate axes after rotation
		inline void calculateAxis();

		// Recalculate worldMatrix and viewMatrix after moving or rotating
		void updateWorldView();

	private:
		// Camera's intern variables
		CameraView camView;

		Vector3 position;
		Vector3 target;
		Vector3 up;

		Vector3 xAxis;
		Vector3 yAxis;
		Vector3 zAxis;

		GLfloat moveSpeed;
		GLfloat rotateSpeed;

		GLfloat nearPlane;
		GLfloat farPlane;
		GLfloat fov;
		GLfloat aspect;

		GLfloat deltaTime;

		Matrix viewMatrix;
		Matrix worldMatrix;
		Matrix perspectiveMatrix;
	};
}