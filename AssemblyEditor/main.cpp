// dear imgui: standalone example application for FreeGLUT + OpenGL2, using legacy fixed pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.

// !!! GLUT IS OBSOLETE SOFTWARE. Using GLUT is not recommended unless you really miss the 90's. !!!
// !!! If someone or something is teaching you GLUT in 2019, you are being abused. Please show some resistance. !!!

#include <vld.h>

#include "imgui.h"
#include "imgui_impl_freeglut.h"
#include "imgui_impl_opengl2.h"
#include "TextEditor.h"
#include <GL/freeglut.h>

#include "cpu.hpp"

#ifdef _MSC_VER
#pragma warning (disable: 4505) // unreferenced local function has been removed
#endif

static bool show_demo_window = true;
static bool showRegistersWindow = false;
static bool showRAMWindow = false;
static bool showFlagsWindow = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

TextEditor textEditor;

int currentLine = -1;
bool stepByStep = false;
Cpu cpu;
std::vector<int> registers;
TextEditor::ErrorMarkers markers;

void my_display_code()
{
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
   // if (show_demo_window)
    //    ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Editor");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Yazaclari Goster", &showRegistersWindow); ImGui::SameLine();
		ImGui::Checkbox("Hafizayi Goster", &showRAMWindow); ImGui::SameLine();
		ImGui::Checkbox("Bayraklari Goster", &showFlagsWindow); ImGui::SameLine();

		if (ImGui::Button("Baslat"))
		{
			
			//if (currentLine <0)
			//{
			
			currentLine = 0;
			stepByStep = false;
			registers.clear();
			markers.clear();
			try
			{
				std::string script = textEditor.GetText();
				cpu.setRAM(script);
			}
			catch (Error err)
			{
				std::cout << err.what() << std::endl;
				markers[err.getLine()] = err.getMesssage();
				
			}

			//}
		}
		ImGui::SameLine();
		if (ImGui::Button("Adim Adim Ilerle"))
		{
			if (currentLine < 0)
			{
				currentLine = 0;
				stepByStep = true;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Durdur"))
		{
			if (currentLine >= 0)
			{
				currentLine = -1;
				stepByStep = false;
			}
		}

		textEditor.SetErrorMarkers(markers);
		textEditor.Render("Adsiz");

	if (currentLine >= 0)
	{
		try
		{
			
			bool running = cpu.step();
		
			if (!running)
			{
				currentLine = -1;
			}
			else
			{
				registers.push_back(cpu.REG[PC].Get());
				registers.push_back(cpu.REG[AC].Get());
				registers.push_back(cpu.REG[IR].Get());
				registers.push_back(cpu.REG[AR].Get());
				registers.push_back(cpu.REG[DR].Get());
				registers.push_back(cpu.REG[TR].Get());
				registers.push_back(cpu.REG[INPR].Get());
				registers.push_back(cpu.REG[INPR].Get());
				registers.push_back(cpu.REG[SC].Get());
			}
		}
		catch (Error err)
		{
			std::cout << err.what() << std::endl;
			markers[err.getLine()] = err.getMesssage();
		}
	}
    // 3. Show another simple window.
	if (showRegistersWindow)
    {
		ImGui::Begin("Yazaclar", &showRegistersWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	
		ImGui::Columns(REGISTER_MAX+1, "registers"); // 4-ways, with border
		ImGui::Separator();
		ImGui::Text("Islem No"); ImGui::NextColumn();
		ImGui::Text("PC"); ImGui::NextColumn();
		ImGui::Text("AC"); ImGui::NextColumn();
		ImGui::Text("IR"); ImGui::NextColumn();
		ImGui::Text("AR"); ImGui::NextColumn();
		ImGui::Text("DR"); ImGui::NextColumn();
		ImGui::Text("TR"); ImGui::NextColumn();
		ImGui::Text("INPR"); ImGui::NextColumn();
		ImGui::Text("OUTR"); ImGui::NextColumn();
		ImGui::Text("SC"); ImGui::NextColumn();
		ImGui::Separator();
		
		static int selected = -1;
		for (int i = 0; i < registers.size()/9; i++)
		{
			char label[32];
			sprintf(label, "%04d", i+1);
			if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns))
				selected = i;
			bool hovered = ImGui::IsItemHovered();
			ImGui::NextColumn();
			//ImGui::Text(names[i]); ImGui::NextColumn();
			for (int j = 0; j < 9; j++)
			{
				ImGui::Text("%x", registers[i*9+j]); ImGui::NextColumn();
			}
			
			//ImGui::Text("%d", hovered); ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::Separator();
        ImGui::End();
    }

	if (showFlagsWindow)
	{
		ImGui::Begin("Bayraklar", &showRegistersWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

		ImGui::Columns(FLAG_MAX+1, "flags"); // 4-ways, with border
		ImGui::Separator();
		ImGui::Text("Islem No"); ImGui::NextColumn();
		ImGui::Text("I"); ImGui::NextColumn();
		ImGui::Text("S"); ImGui::NextColumn();
		ImGui::Text("E"); ImGui::NextColumn();
		ImGui::Text("R"); ImGui::NextColumn();
		ImGui::Text("IEN"); ImGui::NextColumn();
		ImGui::Text("FGI"); ImGui::NextColumn();
		ImGui::Text("FGO"); ImGui::NextColumn();
		ImGui::Separator();

		static int selected = -1;
		for (int i = 0; i < 10; i++)
		{
			char label[32];
			sprintf(label, "%04d", i + 1);
			if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns))
				selected = i;
			bool hovered = ImGui::IsItemHovered();
			ImGui::NextColumn();
			//ImGui::Text(names[i]); ImGui::NextColumn();
			for (int j = 0; j < FLAG_MAX ; j++)
			{
				ImGui::Text("%d", 0); ImGui::NextColumn();
			}

			//ImGui::Text("%d", hovered); ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::End();
	}

	ImGui::End();
	}

}

void glut_display_func()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplFreeGLUT_NewFrame();

    my_display_code();

    // Rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers();
    glutPostRedisplay();
}

// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

int main(int argc, char** argv)
{
    // Create GLUT window
    glutInit(&argc, argv);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Dear ImGui FreeGLUT+OpenGL2 Example");

    // Setup GLUT display function
    // We will also call ImGui_ImplFreeGLUT_InstallFuncs() to get all the other functions installed for us,
    // otherwise it is possible to install our own functions and call the imgui_impl_freeglut.h functions ourselves.
    glutDisplayFunc(glut_display_func);

    // Setup Dear ImGui context
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplFreeGLUT_Init();
    ImGui_ImplFreeGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);


	textEditor.SetPalette(TextEditor::GetLightPalette());
	textEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Assembly());

    glutMainLoop();

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplFreeGLUT_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
