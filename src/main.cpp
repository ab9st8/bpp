#include <iostream>
#include <filesystem>
#include <bitset>
#include <SDL2/SDL.h>
#include <imgui.h>
#include <ImGuiFileDialog.h>
#include <imgui_sdl.h>
#include <ctime>

#include "bus.hpp"
#include "cpu.hpp"
#include "display.hpp"
#include "audio.hpp"
#include "memory.hpp"
#include "input.hpp"

#define MILLISECONDS std::milli::den
#define FRAMERATE 60.0f
#define FRAMELENGTH (MILLISECONDS / FRAMERATE)

#define RED ImVec4(1.0f, 0.0f, 0.0f, 1.0f)
#define GRAY ImVec4(0.5f, 0.5f, 0.5f, 1.0f)

int main(int argc, char** argv)
{
	// SDL initialisation
    SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow("Bytepusher++", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 700, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// There is a bug in ImGui where the background isn't cleared properly on Windows 10.
	// The workaround is to create a texture with the background color and use that to clear the screen instead
#ifdef _WIN32
	SDL_Texture* windowsClearFix = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, 1, 1);
	Uint8* pixels;
	int pitch;
	SDL_LockTexture(windowsClearFix, NULL, (void**)&pixels, &pitch);
	pixels[0] = 114;
	pixels[1] = 144;
	pixels[2] = 154;
	pixels[3] = 255;
	SDL_UnlockTexture(windowsClearFix);
#endif

	// VM initialisation
	Bus* bus = new Bus();

	Memory* memory = new Memory(bus);
	bus->memory = memory;

	CPU* cpu = new CPU(bus);
	bus->cpu = cpu;

	Display* display = new Display(bus, renderer);
	bus->display = display;

	Audio* audio = new Audio(bus);
	bus->audio = audio;

	Input* input = new Input(bus);
	bus->input = input;

	ImGui::CreateContext();
	ImGuiSDL::Initialize(renderer, 900, 700);
	ImGui::StyleColorsDark();

	std::time_t res = std::time(nullptr);
	std::string date = std::ctime(&res);
	date = date.substr(0, date.length() - 1);
	int snapshotIncrement = 1;

	bool run = true;
	bool paused = false;

	bool showBytepusher = true;
	bool showDebug = true;

	while (run)
	{
		size_t begin = SDL_GetTicks();

		ImGuiIO& io = ImGui::GetIO();

		int wheel = 0;

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) run = false;

			else if (e.type == SDL_KEYDOWN) {
				if (!paused && memory->ROMLoaded)
				input->HandleKeyDown(e.key.keysym.sym);
			} else if (e.type == SDL_KEYUP) {
				input->HandleKeyUp(e.key.keysym.sym);
			} else if (e.type == SDL_WINDOWEVENT) {
				if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					io.DisplaySize.x = static_cast<float>(e.window.data1);
					io.DisplaySize.y = static_cast<float>(e.window.data2);
				}
			} else if (e.type == SDL_MOUSEWHEEL) {
				wheel = e.wheel.y;
			}
		}

		int mouseX, mouseY;
		const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

		// Setup low-level inputs (e.g. on Win32, GetKeyboardState(), or write to those fields from your Windows message loop handlers, etc.)
		
		io.DeltaTime = 1.0f / 60.0f;
		io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
		io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
		io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
		io.MouseWheel = static_cast<float>(wheel);

		// Bytepusher code
		if (!paused && memory->ROMLoaded) {
			cpu->Reset();
			size_t i = 65536;
			do {
				cpu->Step();
			} while (i--);
			audio->Play();
			display->UpdateTexture();
		}

		// ImGui code

		ImGui::NewFrame();

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Open")) {
					ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose BytePusher ROM", ".bytepusher,.bp", "", 1);
				}
				if (ImGui::MenuItem("Close ROM")) {
					memory->Reset();
					display->Reset();
					display->UpdateTexture();
				}
				if (ImGui::MenuItem("Quit")) {
					run = false;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Debug")) {
				ImGui::Checkbox("Show debug window", &showDebug);
				ImGui::Separator();
				if (ImGui::MenuItem(paused? "Resume emulation###pausebtn" : "Pause emulation###pausebtn")) {
					paused = !paused;
				}
				if (ImGui::MenuItem("Snapshot RAM")) {
					// candidate for ugliest piece of code in history?
					std::string filepath = std::filesystem::path(argv[0])
						.parent_path()
						.parent_path()
						.string() 
						+ "/snapshots/" 
						+ date 
						+ ": " 
						+ std::to_string(snapshotIncrement++)
						+ ".bytepusher";

					if (!memory->SnapshotRAM(filepath)) {
						if (ImGui::BeginPopupModal("RAM snapshotting error")) {
							ImGui::TextColored(RED, "%s", memory->snapshotError.c_str());
							ImGui::EndPopup();
						}
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				ImGuiFileDialog::Instance()->Close();
				if (!memory->LoadROM(filePathName)) {
					if (ImGui::BeginPopupModal("ROM loading error")) {
						ImGui::TextColored(RED, "%s", memory->ROMLoadError.c_str());
						ImGui::EndPopup();
					}
				}
			}
		}
		
		if (showDebug) {
			ImGui::Begin("Debug", &showDebug, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
			ImGui::Text("-- ROM info --");
			if (ImGui::BeginTable("ROM info", 2)) {
				ImGui::TableNextColumn(); ImGui::TextColored(GRAY, "Has loaded ROM");
				ImGui::TableNextColumn(); ImGui::Text(memory->ROMLoaded? "Yes" : "No");
				ImGui::TableNextColumn(); ImGui::TextColored(GRAY, "ROM name");
				ImGui::TableNextColumn(); ImGui::Text("%s", std::filesystem::path(memory->ROMFilepath).filename().c_str());
				ImGui::TableNextColumn(); ImGui::TextColored(GRAY, "ROM size");
				ImGui::TableNextColumn(); ImGui::Text("%zu B", memory->ROMSize);
				ImGui::TableNextColumn(); ImGui::TextColored(GRAY, "Is paused");
				ImGui::TableNextColumn(); ImGui::Text(paused? "Yes" : "No");
				ImGui::EndTable();
			}
			ImGui::Separator();
			ImGui::Text("-- Emulation info --");
			if (ImGui::BeginTable("Emulation info", 2)) {
				ImGui::TableNextColumn(); ImGui::TextColored(GRAY, "Keyboard state");
				ImGui::TableNextColumn(); ImGui::Text("%s", std::bitset<16>((uint32_t)memory->mem[0] << 8 | memory->mem[1]).to_string().c_str());
				ImGui::TableNextColumn(); ImGui::TextColored(GRAY, "GFX block location");
				ImGui::TableNextColumn(); ImGui::Text("0x%02x", memory->mem[5]);
				ImGui::TableNextColumn(); ImGui::TextColored(GRAY, "Sound block location");
				ImGui::TableNextColumn(); ImGui::Text("0x%06x", (uint32_t)memory->mem[6] << 16 | memory->mem[7] << 8);
				ImGui::TableNextColumn(); ImGui::TextColored(GRAY, "Memory errors");
				ImGui::TableNextColumn(); ImGui::Text("%s\n%s", memory->ROMLoadError.c_str(), memory->snapshotError.c_str());
				ImGui::EndTable();
			}
			ImGui::End();
		}

		ImGui::Begin(paused? "[PAUSED] Bytepusher###bptitle" : "Bytepusher###bptitle", &showBytepusher, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
		ImGui::Image(display->displayTexture, ImVec2(512, 512));
		ImGui::End();

		SDL_SetRenderDrawColor(renderer, 114, 144, 154, 255);

#ifdef _WIN32
		SDL_RenderCopy(renderer, windowsClearFix, NULL, NULL);
#else
		SDL_RenderClear(renderer);
#endif

		ImGui::Render();
		ImGuiSDL::Render(ImGui::GetDrawData());

		SDL_RenderPresent(renderer);

		size_t length = SDL_GetTicks() - begin;
		if (length < FRAMELENGTH) {
			SDL_Delay(FRAMELENGTH - length);
		}
	}

	ImGuiSDL::Deinitialize();

	delete cpu;
	delete memory;
	delete display;
	delete audio;

	delete bus;
	ImGui::DestroyContext();

#ifdef _WIN32
	SDL_DestroyTexture(windowsClearFix);
#endif
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	return EXIT_SUCCESS;
}
