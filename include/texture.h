#pragma once

#include <common.h>
#include <stb_image.h>
#include "vector"
#include "string"
using namespace std;

class zTexture
{
private:
public:
    unsigned int texture;
    vector<unsigned int> tex_flags;
    zTexture(/* args */);
    ~zTexture();
    bool gen_Texture(const string path);
    void loop();
};

zTexture::zTexture(/* args */)
{
}

zTexture::~zTexture()
{
}

void zTexture::loop() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_flags[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex_flags[1]);
}

// ���룺 ��ͼƬ���غ����Դ棬��һ�������ĵ�ַ��Ϊflag��Ҫ������ͼƬ�ͼ�����������
bool zTexture::gen_Texture(const string path) {

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // ���ز���������
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        tex_flags.emplace_back(texture);
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);
    return data != nullptr;
}