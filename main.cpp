#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <string>
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
#include "StaticMesh.h"
#include "Texture.h"
#include "Axis.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "glm/gtx/norm.hpp"


static void error_callback(int error, const char* description)
{
    std::cerr<<description<<"\n";
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    // Mac OS X is not allow to use >= 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // for Mac OS X
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1600, 1200, "Simple example", NULL, NULL);
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;	//vertex matrix needed
    glewInit();

	//------------- sun -------------------
    auto earth = StaticMesh::LoadMesh("../resource/sphere.obj");
    auto prog = Program::LoadFromFile("../resource/vs.txt", "../resource/fs.txt");
	auto text = Texture2D::LoadFromFile("../resource/earth.png");
	// Remove this line and see the difference
	text.setFilter(FilterMode::eNearestMipmapLinear, FilterMode::eLinear);

	if (!earth.hasUV()) {
		std::cerr<<"WARNING: The mesh has no UV data\n";
	}

    auto view = glm::lookAt(glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{-10.0f, -10.0f, -10.0f}, glm::vec3{0.0f, 1.0f, 0.0f});
    auto proj = glm::perspective(glm::pi<float>()/4, 1600.0f/1200.0f, 0.1f, 100.f);
    prog["vp"] = proj*view;	//[]查model id, = 上傳(glm::mat4 UniformVariable::operator=(const glm::mat4 &v))		//fs.txt	
    prog["text"] = 0;		//=(int32_t UniformVariable::operator=(int32_t val))	//貼圖綁在 0 號通道上
    auto uniform_model = prog["model"];
	//-------------- sun end ------------------------
	//------------- satel -------------------
	auto earth_satel = StaticMesh::LoadMesh("../resource/sphere.obj");
	auto prog_satel = Program::LoadFromFile("../resource/vs_BlinnPhong_Gouraud.txt", "../resource/fs_BlinnPhong_Gouraud.txt");	//Blinn-Phong Reflection Model + Gouraud shading
	auto text_satel = Texture2D::LoadFromFile("../resource/moon.png");
	// Remove this line and see the difference
	text_satel.setFilter(FilterMode::eNearestMipmapLinear, FilterMode::eLinear);

	if (!earth_satel.hasUV()) {
		std::cerr << "WARNING: The mesh has no UV data\n";
	}

	auto view_satel = glm::lookAt(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ -10.0f, -10.0f, -10.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	auto proj_satel = glm::perspective(glm::pi<float>() / 4, 1600.0f / 1200.0f, 0.1f, 100.f);
	prog_satel["vp"] = proj_satel*view_satel;
	prog_satel["text"] = 0;
	auto uniform_model_satel = prog_satel["model"];
	//-------------- satel end ------------------------

	//-------------- earth  -----------------------------
	auto sun = StaticMesh::LoadMesh("../resource/sphere.obj");
	//auto prog2 = Program::LoadFromFile("../resource/vs.txt", "../resource/fs.txt");
	//auto prog2 = Program::LoadFromFile("../resource/vs_Flat.txt", "../resource/fs_Flat.txt");	//Phong Reflection Model + Flat shading
	//auto prog2 = Program::LoadFromFile("../resource/vs_Phong_Gouraud.txt", "../resource/fs_Phong_Gouraud.txt");	//Phong Reflection Model + Gouraud shading
	//auto prog2 = Program::LoadFromFile("../resource/vs.txt", "../resource/fs.txt");	//Phong Reflection Model + Phong shading
	//auto prog2 = Program::LoadFromFile("../resource/vs_Flat.txt", "../resource/fs_Lambert_Flat.txt");	//Lambert Reflection Model + Flat shading
	//auto prog2 = Program::LoadFromFile("../resource/vs_Lambert_Gouraud.txt", "../resource/fs_Lambert_Gouraud.txt");	//Lambert Reflection Model + Gouraud shading
	//auto prog2 = Program::LoadFromFile("../resource/vs.txt", "../resource/fs_Lambert.txt");	//Lambert Reflection Model + Phong shading
	//auto prog2 = Program::LoadFromFile("../resource/vs_Flat.txt", "../resource/fs_BlinnPhong_Flat.txt");	//Blinn-Phone Reflection Model + Flat shading
	//auto prog2 = Program::LoadFromFile("../resource/vs_BlinnPhong_Gouraud.txt", "../resource/fs_BlinnPhong_Gouraud.txt");	//Blinn-Phong Reflection Model + Gouraud shading
	auto prog2 = Program::LoadFromFile("../resource/vs.txt", "../resource/fs_BlinnPhong.txt");	//Blinn-Phong Reflection Model + Phong shading
	
	auto text2 = Texture2D::LoadFromFile("../resource/sun.png");
	// Remove this line and see the difference
	text2.setFilter(FilterMode::eNearestMipmapLinear, FilterMode::eLinear);

	if (!sun.hasUV()) {
		std::cerr << "WARNING: The sun has no UV data\n";
	}

	auto view2 = glm::lookAt(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ -10.0f, -10.0f, -10.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	auto proj2 = glm::perspective(glm::pi<float>() / 4, 1600.0f / 1200.0f, 0.1f, 100.f);
	prog2["invPersMatrix"] = inverse(proj2);	//NEW!
	prog2["vp"] = proj2*view2;
	prog2["text"] = 0;
	auto uniform_model2 = prog2["model"];
	//-------------- earth end ------------------------
	//------------- mesh3 -------------------
	auto mesh3 = StaticMesh::LoadMesh("../resource/sphere.obj");
	auto prog3 = Program::LoadFromFile("../resource/vs_Flat.txt", "../resource/fs_Flat.txt");	//Phong Reflection Model + Flat shading
	auto text3 = Texture2D::LoadFromFile("../resource/mercury.png");
	// Remove this line and see the difference
	text3.setFilter(FilterMode::eNearestMipmapLinear, FilterMode::eLinear);

	if (!mesh3.hasUV()) {
		std::cerr << "WARNING: The mesh has no UV data\n";
	}

	auto view3 = glm::lookAt(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ -10.0f, -10.0f, -10.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	auto proj3 = glm::perspective(glm::pi<float>() / 4, 1600.0f / 1200.0f, 0.1f, 100.f);
	prog3["vp"] = proj3*view3;
	prog3["text"] = 0;
	auto uniform_model3 = prog3["model"];
	//-------------- mesh3 end ------------------------
	//------------- mesh4 -------------------
	auto mesh4 = StaticMesh::LoadMesh("../resource/sphere.obj");
	auto prog4 = Program::LoadFromFile("../resource/vs_Phong_Gouraud.txt", "../resource/fs_Phong_Gouraud.txt");	//Phong Reflection Model + Gouraud shading
	auto text4 = Texture2D::LoadFromFile("../resource/venus.png");
	// Remove this line and see the difference
	text4.setFilter(FilterMode::eNearestMipmapLinear, FilterMode::eLinear);

	if (!mesh4.hasUV()) {
		std::cerr << "WARNING: The mesh has no UV data\n";
	}

	auto view4 = glm::lookAt(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ -10.0f, -10.0f, -10.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	auto proj4 = glm::perspective(glm::pi<float>() / 4, 1600.0f / 1200.0f, 0.1f, 100.f);
	prog4["vp"] = proj4*view4;
	prog4["text"] = 0;
	auto uniform_model4 = prog4["model"];
	//-------------- mesh4 end ------------------------
	//------------- mesh5 -------------------
	auto mesh5 = StaticMesh::LoadMesh("../resource/sphere.obj");
	auto prog5 = Program::LoadFromFile("../resource/vs.txt", "../resource/fs.txt");	//Phong Reflection Model + Phong shading
	auto text5 = Texture2D::LoadFromFile("../resource/mars.png");
	// Remove this line and see the difference
	text5.setFilter(FilterMode::eNearestMipmapLinear, FilterMode::eLinear);

	if (!mesh5.hasUV()) {
		std::cerr << "WARNING: The mesh has no UV data\n";
	}

	auto view5 = glm::lookAt(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ -10.0f, -10.0f, -10.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	auto proj5 = glm::perspective(glm::pi<float>() / 4, 1600.0f / 1200.0f, 0.1f, 100.f);
	prog5["vp"] = proj5*view5;
	prog5["text"] = 0;
	auto uniform_model5 = prog5["model"];
	//-------------- mesh5 end ------------------------
	//------------- mesh6 -------------------
	auto mesh6 = StaticMesh::LoadMesh("../resource/sphere.obj");
	auto prog6 = Program::LoadFromFile("../resource/vs_Flat.txt", "../resource/fs_Lambert_Flat.txt");	//Lambert Reflection Model + Flat shading
	auto text6 = Texture2D::LoadFromFile("../resource/jupiter.png");
	// Remove this line and see the difference
	text6.setFilter(FilterMode::eNearestMipmapLinear, FilterMode::eLinear);

	if (!mesh6.hasUV()) {
		std::cerr << "WARNING: The mesh has no UV data\n";
	}

	auto view6 = glm::lookAt(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ -10.0f, -10.0f, -10.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	auto proj6 = glm::perspective(glm::pi<float>() / 4, 1600.0f / 1200.0f, 0.1f, 100.f);
	prog6["vp"] = proj6*view6;
	prog6["text"] = 0;
	auto uniform_model6 = prog6["model"];
	//-------------- mesh6 end ------------------------
	//------------- mesh7 -------------------
	auto mesh7 = StaticMesh::LoadMesh("../resource/sphere.obj");
	auto prog7 = Program::LoadFromFile("../resource/vs_Lambert_Gouraud.txt", "../resource/fs_Lambert_Gouraud.txt");	//Lambert Reflection Model + Gouraud shading
	auto text7 = Texture2D::LoadFromFile("../resource/saturn.png");
	// Remove this line and see the difference
	text7.setFilter(FilterMode::eNearestMipmapLinear, FilterMode::eLinear);

	if (!mesh7.hasUV()) {
		std::cerr << "WARNING: The mesh has no UV data\n";
	}

	auto view7 = glm::lookAt(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ -10.0f, -10.0f, -10.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	auto proj7 = glm::perspective(glm::pi<float>() / 4, 1600.0f / 1200.0f, 0.1f, 100.f);
	prog7["vp"] = proj7*view7;
	prog7["text"] = 0;
	auto uniform_model7 = prog7["model"];
	//-------------- mesh7 end ------------------------
	//------------- mesh8 -------------------
	auto mesh8 = StaticMesh::LoadMesh("../resource/sphere.obj");
	auto prog8 = Program::LoadFromFile("../resource/vs.txt", "../resource/fs_Lambert.txt");	//Lambert Reflection Model + Phong shading
	auto text8 = Texture2D::LoadFromFile("../resource/uranus.png");
	// Remove this line and see the difference
	text8.setFilter(FilterMode::eNearestMipmapLinear, FilterMode::eLinear);

	if (!mesh8.hasUV()) {
		std::cerr << "WARNING: The mesh has no UV data\n";
	}

	auto view8 = glm::lookAt(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ -10.0f, -10.0f, -10.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	auto proj8 = glm::perspective(glm::pi<float>() / 4, 1600.0f / 1200.0f, 0.1f, 100.f);
	prog8["vp"] = proj8*view8;
	prog8["text"] = 0;
	auto uniform_model8 = prog8["model"];
	//-------------- mesh8 end ------------------------
	//------------- mesh9 -------------------
	auto mesh9 = StaticMesh::LoadMesh("../resource/sphere.obj");
	auto prog9 = Program::LoadFromFile("../resource/vs_Flat.txt", "../resource/fs_BlinnPhong_Flat.txt");	//Blinn-Phone Reflection Model + Flat shading
	auto text9 = Texture2D::LoadFromFile("../resource/sun.png");
	// Remove this line and see the difference
	text9.setFilter(FilterMode::eNearestMipmapLinear, FilterMode::eLinear);

	if (!mesh9.hasUV()) {
		std::cerr << "WARNING: The mesh has no UV data\n";
	}

	auto view9 = glm::lookAt(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ -10.0f, -10.0f, -10.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	auto proj9 = glm::perspective(glm::pi<float>() / 4, 1600.0f / 1200.0f, 0.1f, 100.f);
	prog9["vp"] = proj9*view9;
	prog9["text"] = 0;
	auto uniform_model9 = prog9["model"];
	//-------------- mesh9 end ------------------------

	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        // draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//-------------- sun --------------------
		auto model_trans2 = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, -10.0f, -10.0f));
		auto model_rotat2 = glm::rotate(model_trans2, static_cast<float>(glfwGetTime())
			, glm::vec3(0.0f, 1.0f, 0.0f));

		uniform_model2 = model_rotat2;
		prog2.use();
		text2.bindToChannel(0);
		sun.draw();
		//------------- earth ---------------------
        auto model = glm::rotate(model_trans2, static_cast<float>(glfwGetTime())
            , glm::vec3(0.0f, 1.0f, 0.0f));
		auto model_trans = glm::translate(model, glm::vec3(0.0f, 0.0f, -9.0f));
		auto model_rotat = glm::rotate(model_trans, 15 * static_cast<float>(glfwGetTime()) / 10
			, glm::vec3(0.0f, 1.0f, 0.0f));
		auto model_scale = glm::scale(model_rotat, glm::vec3(0.6f, 0.6f, 0.6f));

        uniform_model = model_scale;
        prog.use();
        text.bindToChannel(0);
		earth.draw();
		//------------- earth_satel ---------------------
		auto model_satel = glm::rotate(model_trans, static_cast<float>(glfwGetTime())
			, glm::vec3(0.0f, 1.0f, 0.0f));
		auto model_satel_trans = glm::translate(model_satel, glm::vec3(0.0f, 0.0f, -2.0f));
		auto model_satel_rotat = glm::rotate(model_satel_trans, 9 * static_cast<float>(glfwGetTime()) / 10
			, glm::vec3(0.0f, 1.0f, 0.0f));
		auto model_satel_scale = glm::scale(model_satel_rotat, glm::vec3(0.3f, 0.3f, 0.3f));

		uniform_model_satel = model_satel_scale;
		prog_satel.use();
		text_satel.bindToChannel(0);
		earth_satel.draw();
		
		//-------------- mesh3 --------------------
		//auto model_rotat3 = glm::rotate(glm::mat4(1.0f), 18*static_cast<float>(glfwGetTime())/10
		//	, glm::vec3(0.0f, 1.0f, 0.0f));
		//auto model_trans3 = glm::translate(model_rotat3, glm::vec3(0.0f, 0.0f, -3.0f));
		//auto model_scale3 = glm::scale(model_trans3, glm::vec3(0.5f, 0.5f, 0.5f));
		//uniform_model3 = model_scale3;
        
		// #include <glm/gtc/quaternion.hpp> and <glm/gtx/quaternion.hpp>
		//Translate
		//auto pos = glm::vec3(-10.0f, -10.0f, -10.0f);
		//model3[3] = model[0] * pos[0] + model[1] * pos[1] + model[2] * pos[2] + model[3];
		//Rotate
		glm::quat quat_rotat;	// Creates an identity quaternion (no rotation)
		quat_rotat = normalize(glm::quat(0, sin(static_cast<float>(0.5*glfwGetTime())/2),0, cos(static_cast<float>(0.5*glfwGetTime())/2)));
		glm::mat4 matrix_rotate = glm::toMat4(quat_rotat); //quaternion to matrix
		auto model3 = matrix_rotate;
		//Translate
		auto v = glm::vec3(4.0f, 0.0f, 0.0f);
		model3[3] = model3[0]*v[0]+ model3[1]*v[1]+model3[2]*v[2]+ model3[3];
		//Rotate2
		glm::quat quat_rotat2;
		quat_rotat2 = normalize(glm::quat(sin(static_cast<float>(1.5*glfwGetTime()) / 2), sin(static_cast<float>(1.5*glfwGetTime()) / 2), 0, cos(static_cast<float>(1.5*glfwGetTime()) / 2)));
		glm::mat4 matrix_rotate2 = glm::toMat4(quat_rotat2);
		auto model_trans3 = model3*matrix_rotate2;
		//Scale
		auto u = glm::vec3(0.5f, 0.5f, 0.5f);
		model3[0] = model[0] * u[0];
		model3[1] = model[1] * u[1];
		model3[2] = model[2] * u[2];
		model3[3] = model[3];
		auto model_scale3 = model3*model_trans3;
		uniform_model3 = model_scale3;
		
		prog3.use();
		text3.bindToChannel(0);
		mesh3.draw();
		//-------------- mesh4 --------------------
		auto model_rotat4 = glm::rotate(model_trans2, 1*static_cast<float>(glfwGetTime()) / 10
			, glm::vec3(0.0f, 1.0f, 0.0f));
		auto model_trans4 = glm::translate(model_rotat4, glm::vec3(0.0f, 0.0f, -6.0f));
		auto model_rotat4_2 = glm::rotate(model_trans4, 12 * static_cast<float>(glfwGetTime()) / 10
			, glm::vec3(0.0f, 1.0f, 0.0f));
		auto model_scale4 = glm::scale(model_rotat4_2, glm::vec3(0.7f, 0.7f, 0.7f));

		uniform_model4 = model_scale4;
		prog4.use();
		text4.bindToChannel(0);
		mesh4.draw();
		//-------------- mesh5 --------------------
		auto model_rotat5 = glm::rotate(model_trans2, 15*static_cast<float>(glfwGetTime()) / 10
			, glm::vec3(0.0f, 1.0f, 0.0f));
		auto model_trans5 = glm::translate(model_rotat5, glm::vec3(0.0f, 0.0f, -12.0f));
		auto model_rotat5_2 = glm::rotate(model_trans5, 13 * static_cast<float>(glfwGetTime()) / 10
			, glm::vec3(0.0f, 1.0f, 0.0f));
		auto model_scale5 = glm::scale(model_rotat5_2, glm::vec3(0.9f, 0.9f, 0.9f));

		uniform_model5 = model_scale5;
		prog5.use();
		text5.bindToChannel(0);
		mesh5.draw();
		//-------------- mesh6 --------------------
		auto model_rotat6 = glm::rotate(model_trans2, 4*static_cast<float>(glfwGetTime()) / 10
			, glm::vec3(0.0f, 1.0f, 0.0f));
		auto model_trans6 = glm::translate(model_rotat6, glm::vec3(0.0f, 0.0f, -15.0f));
		auto model_rotat6_2 = glm::rotate(model_trans6, 7 * static_cast<float>(glfwGetTime()) / 10
			, glm::vec3(0.0f, 1.0f, 0.0f));
		auto model_scale6 = glm::scale(model_rotat6_2, glm::vec3(1.1f, 1.1f, 1.1f));

		uniform_model6 = model_scale6;
		prog6.use();
		text6.bindToChannel(0);
		mesh6.draw();
		//-------------- mesh7 --------------------
		auto model_rotat7 = glm::rotate(model_trans2, 7*static_cast<float>(glfwGetTime()) / 10
			, glm::vec3(0.0f, 1.0f, 0.0f));
		auto model_trans7 = glm::translate(model_rotat7, glm::vec3(0.0f, 0.0f, -18.0f));
		auto model_rotat7_2 = glm::rotate(model_trans7, 5 * static_cast<float>(glfwGetTime()) / 10
			, glm::vec3(0.0f, 1.0f, 0.0f));
		auto model_scale7 = glm::scale(model_rotat7_2, glm::vec3(1.0f, 1.0f, 1.0f));

		uniform_model7 = model_scale7;
		prog7.use();
		text7.bindToChannel(0);
		mesh7.draw();
		//-------------- mesh8 --------------------
		auto model_rotat8 = glm::rotate(model_trans2, 5*static_cast<float>(glfwGetTime()) / 10
			, glm::vec3(0.0f, 1.0f, 0.0f));
		auto model_trans8 = glm::translate(model_rotat8, glm::vec3(0.0f, 0.0f, -21.0f));
		auto model_rotat8_2 = glm::rotate(model_trans8, 3 * static_cast<float>(glfwGetTime()) / 10
			, glm::vec3(0.0f, 1.0f, 0.0f));
		auto model_scale8 = glm::scale(model_rotat8_2, glm::vec3(1.3f, 1.3f, 1.3f));

		uniform_model8 = model_scale8;
		prog8.use();
		text8.bindToChannel(0);
		mesh8.draw();
		//-------------- mesh9 --------------------
		auto model_rotat9 = glm::rotate(model_trans2, 2*static_cast<float>(glfwGetTime()) / 10
			, glm::vec3(0.0f, 1.0f, 0.0f));
		auto model_trans9 = glm::translate(model_rotat9, glm::vec3(0.0f, 0.0f, -24.0f));
		auto model_rotat9_2 = glm::rotate(model_trans9, 1 * static_cast<float>(glfwGetTime()) / 10
			, glm::vec3(0.0f, 1.0f, 0.0f));
		auto model_scale9 = glm::scale(model_rotat9_2, glm::vec3(1.2f, 1.2f, 1.2f));

		uniform_model9 = model_scale9;
		prog9.use();
		text9.bindToChannel(0);
		mesh9.draw();


        ////////////////
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	//earth
	earth.release();
    prog.release();
    text.release();
	//earth_satel
	earth_satel.release();
	prog_satel.release();
	text_satel.release();
	//sun
	sun.release();
	prog2.release();
	text2.release();
	//mesh3
	mesh3.release();
	prog3.release();
	text3.release();
	//mesh4
	mesh4.release();
	prog4.release();
	text4.release();
	//mesh5
	mesh5.release();
	prog5.release();
	text5.release();
	//mesh6
	mesh6.release();
	prog6.release();
	text6.release();
	//mesh7
	mesh7.release();
	prog7.release();
	text7.release();
	//mesh8
	mesh8.release();
	prog8.release();
	text8.release();
	//mesh9
	mesh9.release();
	prog9.release();
	text9.release();


    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
