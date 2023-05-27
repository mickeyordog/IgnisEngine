#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdio.h>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include "SDLContext.h"
#include "GLContext.h"
#include "dearImGuiContext.h"
#include "gameObject.h"
#include "timer.h"
#include "inputHandler.h"
#include "shader.h"
#include "texture.h"
#include "geometry.h"

/* Return the number of arguments of the application command line */
static int numargs = 0;
static PyObject* emb_numargs(PyObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ":numargs"))
		return NULL;
	return PyLong_FromLong(numargs);
}

static PyObject* emb_multiply(PyObject *self, PyObject *args)
{
	float a, b;
	if (!PyArg_ParseTuple(args, "ff:multiply", &a, &b))
		return NULL;
	return PyFloat_FromDouble(a * b);
}

static PyObject *emb_dot(PyObject *self, PyObject *args)
{
	float x1, y1, x2, y2;
	if (!PyArg_ParseTuple(args, "(ff)(ff):dot", &x1, &y1, &x2, &y2))
		return NULL;
	return PyFloat_FromDouble(x1 * x2 + y1 * y2);
}

static PyMethodDef EmbMethods[] = {
	{"numargs", emb_numargs, METH_VARARGS, "Return the number of arguments received by the process."},
	{"multiply", emb_multiply, METH_VARARGS, "Multiply two numbers."},
	{"dot", emb_dot, METH_VARARGS, "Dot product between two vectors."},
	{NULL, NULL, 0, NULL}};

static PyModuleDef EmbModule = {
	PyModuleDef_HEAD_INIT, "emb", NULL, -1, EmbMethods,
	NULL, NULL, NULL, NULL};

static PyObject *
PyInit_emb(void)
{
	return PyModule_Create(&EmbModule);
}

int main(int argc, char *argv[])
{
	return 0;
	// argc = 4;
	// argv[1] = "ignis";
	// argv[2] = "test";
	// argv[3] = "3";
	numargs = argc;
	PyImport_AppendInittab("emb", &PyInit_emb);
	PyObject *pName, *pModule, *pFunc;
	PyObject *pArgs, *pValue;
	int i;

	if (argc < 3)
	{
		fprintf(stderr, "Usage: call pythonfile funcname [args]\n");
		return 1;
	}

	Py_Initialize();

	// Is this alright to add?
	PyObject *sys = PyImport_ImportModule("sys");
	PyObject *path = PyObject_GetAttrString(sys, "path");
	PyList_Append(path, PyUnicode_FromString("../src/script"));
	// need to dec ref?

	pName = PyUnicode_DecodeFSDefault(argv[1]);
	if (pName == NULL)
	{
		fprintf(stderr, "Error decoding filename\n");
		return 1;
	}

	pModule = PyImport_Import(pName);
	if (pModule == NULL) {
		PyErr_Print();
		fprintf(stderr, "Error importing module\n");
		Py_DECREF(pName);
		return 1;
	}
	Py_DECREF(pName);

	if (pModule != NULL)
	{
		pFunc = PyObject_GetAttrString(pModule, "get_component_classes");
		if (pFunc && PyCallable_Check(pFunc)) {
			pValue = PyObject_CallObject(pFunc, NULL);
			if (pValue != NULL) {
				PyObject* obj = PyList_GET_ITEM(pValue, 0);
				PyObject *repr = PyObject_Repr(pValue);
				PyObject *str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
				const char *bytes = PyBytes_AS_STRING(str);

				printf("REPR: %s\n", bytes);
			}
			else {
				PyErr_Print();
				printf("Error calling object\n");
			}
		}
		else {
			PyErr_Print();
			printf("Error getting component classes\n");
		}
			

		pFunc = PyObject_GetAttrString(pModule, argv[2]);
		/* pFunc is a new reference */

		if (pFunc && PyCallable_Check(pFunc))
		{
			pArgs = PyTuple_New(argc - 3);
			for (i = 0; i < argc - 3; ++i)
			{
				pValue = PyLong_FromLong(atoi(argv[i + 3]));
				if (!pValue)
				{
					Py_DECREF(pArgs);
					Py_DECREF(pModule);
					fprintf(stderr, "Cannot convert argument\n");
					return 1;
				}
				/* pValue reference stolen here: */
				PyTuple_SetItem(pArgs, i, pValue);
			}
			pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);
			if (pValue != NULL)
			{
				printf("Result of call: %ld\n", PyLong_AsLong(pValue));
				Py_DECREF(pValue);
			}
			else
			{
				Py_DECREF(pFunc);
				Py_DECREF(pModule);
				PyErr_Print();
				fprintf(stderr, "Call failed\n");
				return 1;
			}
		}
		else
		{
			if (PyErr_Occurred())
				PyErr_Print();
			fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
		}
		Py_XDECREF(pFunc);
		Py_DECREF(pModule);
	}
	else
	{
		PyErr_Print();
		fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
		return 1;
	}
	if (Py_FinalizeEx() < 0)
	{
		return 120;
	}
	return 0;
}

// TODO: extension that lets you add includes more easily
// TODO: fix window resizing
// TODO: allow editing gameobject fields in editor
// TODO: set up camera, render scene to texture, render texture to screen
// TODO: set up scene editor
// TODO: fancier scene rendering, eg 3D stuff
// TODO: script embed/implementation
int main2(int argc, char* args[])
{
	Py_Initialize();
	// PyRun_SimpleString("print('Hello from Python!')");
	// Py_DecodeLocale()
	FILE* file = fopen("../src/script/ignis.py", "rb");
	if (PyRun_SimpleFileExFlags(file, "ignis.py", true, nullptr) < 0)
	{
		std::cout << "PyRun_SimpleFile() failed" << std::endl;
	}
	if (Py_FinalizeEx() < 0)
	{
		std::cout << "Py_FinalizeEx() failed" << std::endl;
	}
	// return 0;

	SDLContext sdlContext("Ignis Engine", 1280, 720);
	GLContext glContext(&sdlContext);
	DearImGuiContext dearImGuiContext(&sdlContext, &glContext);

	bool show_demo_window = true;
	bool show_another_window = false;

	// glViewport(0, 0, 200, 100);
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	Texture texture("../assets/texture.png");
	Shader shader("../src/shader/vertex.vs", "../src/shader/fragment.fs");
	Geometry geometry(&texture, &shader);

	Timer frameTimer;
	float deltaTime = 0;

	bool quit = false;
	while(!quit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			// TODO: io.WantMouseCapture etc for imgui
			ImGui_ImplSDL2_ProcessEvent(&e);
			sdlContext.handleEvents(&e);
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		InputHandler::getInstance().updateKeys();

		deltaTime = frameTimer.read();
		frameTimer.reset();
		// std::cout << 1.0f/deltaTime << " fps" << std::endl;

		dearImGuiContext.newFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");		   // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);			 // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			float framerate = dearImGuiContext.getIO().Framerate;
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / framerate, framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::Image((void *)(uintptr_t)texture.textureHandle, ImVec2(texture.width, texture.height));
			ImGui::End();
		}

		glContext.clear(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		dearImGuiContext.render();

		// geometry.render();

		sdlContext.swapWindow();
	}

	return 0;
}