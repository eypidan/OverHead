#include "pch.h"

//setting
float dudv_move = 0;
//Model OrganodronCity("./models/OrganodronCity/OrganodronCity.obj");
int iii = 1;

unsigned int cubemapTexture = 0;
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

void renderCube(Shader modelShader,Camera currentCamera,vaoSet inputVAO,Model *magicCube){
	//==== data init ====
	// building position
	glm::vec3 buildingPos[] = {
		glm::vec3(15.0f,  0.0f, 15.0f),
		glm::vec3(-10.0f,  0.0f,  -10.0f),
		glm::vec3(-15.5f, 0.0f, 25.0f)
	};
    modelShader.use();
    modelShader.setVec3("lightPos", lightPos);
    modelShader.setVec3("lightColor", lightColor);
    modelShader.setVec3("viewPos", currentCamera.Position);

    glm::mat4 view = currentCamera.GetViewMatrix();
    modelShader.setMat4("view", view);
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(currentCamera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    modelShader.setMat4("projection", projection);
    
    glm::mat4 model = glm::mat4(1.0f);

	glBindVertexArray(inputVAO.planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
    //draw the building
    //1st draw as normal
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);//enable write stencil buffer
    
    for (int i = 0;i < 3;i++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, buildingPos[i]);
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
        modelShader.setMat4("model", model);
        magicCube->Draw(modelShader); //rendering while writing into stencil buffer
        //OrganodronCity.Draw(modelShader);
    }
    //2nd draw scaled verision
    // inputShaderSet.singleColorShader->use();
    // inputShaderSet.singleColorShader->setMat4("view", view);
    // inputShaderSet.singleColorShader->setMat4("projection", projection);
    // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    // glStencilMask(0x00);//close write stencil buffer

    // GLfloat scale = 1.05;
    // for (int i = 0;i < 3;i++) {
    //     model = glm::mat4(1.0f);
    //     model = glm::translate(model, buildingPos[i]);
    //     model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
    //     model = glm::scale(model, glm::vec3(scale, scale, scale));
    //     inputShaderSet.singleColorShader->setMat4("model", model);
    //     magicCube->Draw(*inputShaderSet.singleColorShader);
    //     //OrganodronCity.Draw(modelShader);
    // }
    // glStencilFunc(GL_ALWAYS, 1, 0xFF);
    // glStencilMask(0xFF);

}

void renderSide(Shader singleColorShader, Camera currentCamera, Model *magicCube){
    // building position
	glm::vec3 buildingPos[] = {
		glm::vec3(15.0f,  0.0f, 15.0f),
		glm::vec3(-10.0f,  0.0f,  -10.0f),
		glm::vec3(-15.5f, 0.0f, 25.0f)
	};

    //2nd draw scaled verision
    singleColorShader.use();

    glm::mat4 view = currentCamera.GetViewMatrix();
    singleColorShader.setMat4("view", view);
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(currentCamera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    singleColorShader.setMat4("projection", projection);

    singleColorShader.setMat4("view", view);
    singleColorShader.setMat4("projection", projection);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);//close write stencil buffer

    GLfloat scale = 1.05;
    for (int i = 0;i < 3;i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, buildingPos[i]);
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        singleColorShader.setMat4("model", model);
        magicCube->Draw(singleColorShader);
        //OrganodronCity.Draw(modelShader);
    }
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
}



void renderSun(Shader sunShader,unsigned int sunVAO,Camera currentCamera){
    //draw light source
    sunShader.use();
    
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(glm::mat3(currentCamera.GetViewMatrix())); // remove translation from the view matrix, but keep rotation	
	glm::mat4 projection = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube

    sunShader.setMat4("model", model);
    sunShader.setMat4("projection", projection);
    sunShader.setMat4("view", view);
    sunShader.setVec3("lightColor", lightColor);

    glBindVertexArray(sunVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void renderSkybox(Shader skyboxShader,unsigned int skyboxVAO,Camera currentCamera){
    if (cubemapTexture == 0)  cubemapTexture = loadCubemap(faces);
    //draw skybox last
    glDepthFunc(GL_LEQUAL);
    skyboxShader.use();
    glm::mat4 view = glm::mat4(glm::mat3(currentCamera.GetViewMatrix())); // remove translation from the view matrix, but keep rotation	
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(currentCamera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	skyboxShader.setMat4("view", view);
    skyboxShader.setMat4("projection", projection);
    // skybox cubeGL_LESS
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}



    ////draw water plane
    //inputShaderSet.waterShader->use();
    //inputShaderSet.waterShader->setMat4("projection", projection);
    //inputShaderSet.waterShader->setMat4("view", view);
    //model = glm::mat4(1.0f);
    ////model = glm::scale(model, glm::vec3(5.0f));
    //inputShaderSet.waterShader->setMat4("model", model);
    //dudv_move += 0.0005f; // speed
    //dudv_move = fmod(dudv_move, 1.0f);
    //inputShaderSet.waterShader->setFloat("dudv_move", dudv_move);

    //glBindVertexArray(inputVAO.waterVAO);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    //glBindVertexArray(0);