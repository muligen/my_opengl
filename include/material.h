#pragma once
#include <common.h>
#include <shaders/shader_m.h>
#include <texture.h>

//����һ��Ĭ�ϵ�shader������,global������
Shader* get_default_shader() {
    Shader* s = new Shader("shader/vs.vs", "shader/fs.fs");
    zTexture* texs = new zTexture();
    texs->gen_Texture("resources/textures/blue_king.png");
    texs->gen_Texture("resources/textures/red_king.png");
    // bind textures on corresponding texture units
    s->use();
    s->setInt("texture1", 0);
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, texs->tex_flags[1]);    
    s->setInt("texture2", 1);
    s->setTextures(texs);
    return s;
}

class zMaterial
{
private:
    Shader* mShader = nullptr;
public:
    zMaterial(Shader* s);
    ~zMaterial();
    void use();
    void update_mvp(glm::mat4& m, glm::mat4& v, glm::mat4& p);
};

zMaterial::zMaterial(Shader* s = nullptr)
{
    if (s != nullptr) {
        mShader = s;
    }
    else {
        mShader = get_default_shader();
    }
}

zMaterial::~zMaterial()
{

}

void zMaterial::use() {
    mShader->use();
    mShader->texs->loop();
}

void zMaterial::update_mvp(glm::mat4& m, glm::mat4& v, glm::mat4& p) {
    mShader->use();
    mShader->setMat4("model", m);
    mShader->setMat4("view", v);
    mShader->setMat4("projection", p);
}