#pragma once
#include "stdafx.h"
#include "SceneObject.h"
#include "SceneManager.h"

using namespace scene_obj;

SceneObject::SceneObject(res::Model* model, res::Shader* shader, std::vector<res::Texture*> * textures) :
	model(model), shader(shader), textures(textures)
{

}

SceneObject::SceneObject() : SceneObject(nullptr, nullptr, nullptr) {}

void SceneObject::Init(int id, std::string name, ObjectType type, Vector3 position, Vector3 rotation, Vector3 scale, bool DEPTH_TEST) {
	this->id = id;
	this->name = name;
	this->type = type;
	this->position = position;
	this->rotation = toRadians(rotation);
	this->scale = scale;
	this->DEPTH_TEST = DEPTH_TEST;
}

void SceneObject::Draw(ESContext* esContext) {
	if (shader == nullptr || model == nullptr) {
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, model->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ibo);

	glUseProgram(shader->program);
	DrawCommon();

	int size = textures->size();
	if (size > 0) {
		switch (size) {
		case 3: sendTexture(GL_TEXTURE2, 2);
			glUniform1i(shader->textureUniform_2, 2);
		case 2: sendTexture(GL_TEXTURE1, 1);
			glUniform1i(shader->textureUniform_1, 1);
		default: sendTexture(GL_TEXTURE0, 0);
			glUniform1i(shader->textureUniform_0, 0);
		}
	}
	
	glDrawElements(GL_TRIANGLES, model->nr_index, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SceneObject::Update(ESContext* esContext, float deltaTime) {

}

void SceneObject::sendTexture(int TEXTURE, int id) {
	glActiveTexture(TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures->at(id)->textureId);
}

void SceneObject::DrawCommon() {
	if (shader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(shader->positionAttribute);
		glVertexAttribPointer(shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (shader->colorAttribute != -1)
	{
		glEnableVertexAttribArray(shader->colorAttribute);
		glVertexAttribPointer(shader->colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vector3));
	}

	if (shader->uvAttribute != -1)
	{
		glEnableVertexAttribArray(shader->uvAttribute);
		glVertexAttribPointer(shader->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(5 * sizeof(Vector3)));
	}

	if (shader->modelMatrix != -1)
	{
		Matrix modelMatrix;
		modelMatrix.SetIdentity();
		modelMatrix = Transform3D::Scale(scale)
			* Transform3D::RotateX(rotation.x)
			* Transform3D::RotateY(rotation.y)
			* Transform3D::RotateZ(rotation.z)
			* Transform3D::Translate(position);
		glUniformMatrix4fv(shader->modelMatrix, 1, GL_FALSE, (GLfloat*)modelMatrix.m);
	}

	if (shader->viewMatrix != -1)
	{
		
		glUniformMatrix4fv(shader->viewMatrix, 1, GL_FALSE, (GLfloat*)(manager::SceneManager::getInstance()->mainCamera->getViewMatrix().m));
	}

	if (shader->perspectiveMatrix != -1)
	{
		glUniformMatrix4fv(shader->perspectiveMatrix, 1, GL_FALSE, (GLfloat*)(manager::SceneManager::getInstance()->mainCamera->getPerspectiveMatrix().m));
	}
}


SceneObject::~SceneObject() {

}