#pragma once
#include "stdafx.h"
#include "SceneObject.h"
#include "SceneManager.h"

using namespace scene_obj;

// Reference to current scene
manager::SceneManager const* sceneManager = manager::SceneManager::getInstance();

SceneObject::SceneObject(res::Model* model, res::Shader* shader, std::vector<res::Texture*>* textures) :
	model(model), shader(shader), textures(textures)
{
	// Get a reference
	mainCamera = manager::SceneManager::getInstance()->mainCamera;

	// Initial set the follow state inactive
	followCamera = nullptr;
	followAxes = 0;

	// Initial light values
	material_kd = material_ks = material_shininess = 0;
}

SceneObject::SceneObject() : SceneObject(nullptr, nullptr, nullptr) {}

/*
* Set the core variables for this object. Not using this function after
* after instantiation will break the program.
*/
void SceneObject::Init(int id, std::string name, ObjectType type, Vector3 position, Vector3 rotation, Vector3 scale, bool DEPTH_TEST) {
	this->id = id;
	this->name = name;
	this->type = type;
	this->position = position;
	this->rotation = toRadians(rotation);
	this->scale = scale;
	this->DEPTH_TEST = DEPTH_TEST;
}

/*
* Function used to draw on the screen / esContext this object.
* We need to have a valid shader and model reference.
*/
void SceneObject::Draw(ESContext* esContext) {
	if (shader == nullptr || model == nullptr) {
		std::cerr << "Error in Draw function! " << name << " doesn't have a valid Shader or Model\n";
		return;
	}

	// Bind model
	glBindBuffer(GL_ARRAY_BUFFER, model->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ibo);

	// Load shader
	glUseProgram(shader->program);

	// Send to shader object data
	DrawCommon();
	DrawLights();
	DrawSpecific();

	// Load textures. Basic object can load up to 5 textures at the same time
	int size = textures->size();
	if (size > 0) {
		switch (size) {
		case 5: sendTexture(GL_TEXTURE4, 4);
			glUniform1i(shader->textureUniform_4, 4);
		case 4: sendTexture(GL_TEXTURE3, 3);
			glUniform1i(shader->textureUniform_3, 3);
		case 3: sendTexture(GL_TEXTURE2, 2);
			glUniform1i(shader->textureUniform_2, 2);
		case 2: sendTexture(GL_TEXTURE1, 1);
			glUniform1i(shader->textureUniform_1, 1);
		default: sendTexture(GL_TEXTURE0, 0);
			glUniform1i(shader->textureUniform_0, 0);
		}
	}

	// Draw model on screen
	glDrawElements(GL_TRIANGLES, model->nr_index, GL_UNSIGNED_INT, 0);


	// Unbind model
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/*
* Function used to load specific elements used by a child class.
* Called after DrawCommon function.
*/
void SceneObject::DrawSpecific() {}

/*
* Update core function. Implements core functions for an object.
*/
void SceneObject::Update(ESContext* esContext, float deltaTime) {
	if (followCamera != nullptr) {
		FollowCamera();
	}
}

/*
* Load(activate + binding) a texture in the current used shader program
*/
void SceneObject::sendTexture(int TEXTURE, int id) {
	glActiveTexture(TEXTURE);
	glBindTexture(textures->at(id)->getTarget(), textures->at(id)->textureId);
}

/*
* Send common data information to current shader program used:
* vertex attributes, MVP matrix, fog information, main camera position.
*/
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

	if (shader->normalAttribute != -1)
	{
		glEnableVertexAttribArray(shader->normalAttribute);
		glVertexAttribPointer(shader->normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(2 * sizeof(Vector3)));
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
		glUniformMatrix4fv(shader->viewMatrix, 1, GL_FALSE, (GLfloat*)(mainCamera->getViewMatrix().m));
	}

	if (shader->perspectiveMatrix != -1)
	{
		glUniformMatrix4fv(shader->perspectiveMatrix, 1, GL_FALSE, (GLfloat*)(mainCamera->getPerspectiveMatrix().m));
	}

	if (shader->cameraPos != -1)
	{
		glUniform3fv(shader->cameraPos, 1, (GLfloat*)(&mainCamera->getPosition()));
	}

	if (shader->fogClear_r != -1) {
		glUniform1f(shader->fogClear_r, sceneManager->fogClear_r);
	}

	if (shader->fogTrans_R != -1) {
		glUniform1f(shader->fogTrans_R, sceneManager->fogTrans_R);
	}

	if (shader->fogColor != -1) {
		glUniform3fv(shader->fogColor, 1, (GLfloat*)(&sceneManager->fogColor));
	}
}

void SceneObject::DrawLights() {
	if (shader->ratio != -1) {
		glUniform1f(shader->ratio, sceneManager->al_ratio);
	}

	if (shader->colorAmbientalGlobal != -1) {
		glUniform3fv(shader->colorAmbientalGlobal, 1, (GLfloat*)(&sceneManager->ambientalLight->light_color));
	}

	if (shader->material_kd != -1) {
		glUniform1f(shader->material_kd, material_kd);
	}

	if (shader->material_ks != -1) {
		glUniform1f(shader->material_ks, material_ks);
	}

	if (shader->material_shininess != -1) {
		glUniform1f(shader->material_shininess, material_shininess);
	}

	if (shader->light_number != -1) {
		glUniform1i(shader->light_number, light_number);
	}

	for (int i = 0; i < light_number; i++) {
		std::string name;

		// Light type
		name = std::string("u_lights[") + std::to_string(i) + std::string("].type");
		GLint loc_type = glGetUniformLocation(shader->program, name.c_str());
		if (loc_type != -1)
		glUniform1i(loc_type, ((*obj_lights)[lights_ids[i]])->type);

		// Light position
		name = std::string("u_lights[") + std::to_string(i) + std::string("].light_position");
		GLint loc_position = glGetUniformLocation(shader->program, name.c_str());
		if (loc_position != -1)
		glUniform3fv(loc_position, 1, (GLfloat*)(&((*obj_lights)[lights_ids[i]]->light_position)));

		// Light direction
		name = std::string("u_lights[") + std::to_string(i) + std::string("].light_direction");
		GLint loc_direction = glGetUniformLocation(shader->program, name.c_str());
		if (loc_direction != -1)
		glUniform3fv(loc_direction, 1, (GLfloat*)(&((*obj_lights)[lights_ids[i]]->light_direction)));

		// Light color
		name = std::string("u_lights[") + std::to_string(i) + std::string("].light_color");
		GLint loc_color = glGetUniformLocation(shader->program, name.c_str());
		if (loc_color != -1)
		glUniform3fv(loc_color, 1, (GLfloat*)(&((*obj_lights)[lights_ids[i]]->light_color)));

		// Spec light color
		name = std::string("u_lights[") + std::to_string(i) + std::string("].spec_light_color");
		GLint spec_loc_color = glGetUniformLocation(shader->program, name.c_str());
		if (spec_loc_color != -1)
		glUniform3fv(spec_loc_color, 1, (GLfloat*)(&((*obj_lights)[lights_ids[i]]->spec_light_color)));

		// Diff Light color
		name = std::string("u_lights[") + std::to_string(i) + std::string("].diff_light_color");
		GLint diff_loc_color = glGetUniformLocation(shader->program, name.c_str());
		if (diff_loc_color != -1)
		glUniform3fv(diff_loc_color, 1, (GLfloat*)(&((*obj_lights)[lights_ids[i]]->difuze_light_color)));

		// Cut off angle
		name = std::string("u_lights[") + std::to_string(i) + std::string("].cut_off");
		GLint loc_cut_off = glGetUniformLocation(shader->program, name.c_str());
		if (loc_cut_off != -1)
		glUniform1f(loc_cut_off, (*obj_lights)[lights_ids[i]]->cut_off);
	}
}

/*
* Execute follow state on the bound axis. All axis can have a follow
* state, or none. The variable verify if we have this state on an axis,
* using bit operation : 001 Ox, 010 Oy, 100 Oz, 011 Ox & Oy etc.
* Final position is offset axis + follow camera position on the same axis
*/
void SceneObject::FollowCamera() {
	Vector3 cameraPos = followCamera->getPosition();
	if (followAxes & 1 == 1) {
		//ox
		position.x = followOffset.x + cameraPos.x;
	}
	if (followAxes >> 1 == 1) {
		//oy
		position.y = followOffset.y + cameraPos.y;
	}
	if (followAxes >> 2 == 1) {
		//oz
		position.z = followOffset.z + cameraPos.z;
	}
}


/*
* Set follow camera state for this object. This function need to be called
* after the Init function. The initial camera position is used as an offset.
*/
void SceneObject::SetFollowCamera(char followAxes, camera::Camera* followCamera) {
	this->followAxes = followAxes;
	this->followCamera = followCamera;
	followOffset = position;
}

void SceneObject::SetMaterialProp(Vector3 material) {
	material_kd = material.x;
	material_ks = material.y;
	material_shininess = material.z;
}

void SceneObject::SetLights(std::map<int, light::LightSource*>* obj_lights) {
	this->obj_lights = obj_lights;
}

std::vector<int>* SceneObject::GetObjLights() {
	return &lights_ids;
}

void SceneObject::SetLightsIds(std::vector<int> ids) {
	lights_ids = ids;
	light_number = ids.size();
}

SceneObject::~SceneObject() {
}