#include "Menubar.h"

void Menubar::OnGUI()
{
    if (ImGui::BeginMenuBar()) {
        // File menu
        if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open Project")) {
				// Handle Open Project action
				Project* currentProject = Editor::GetInstance()->GetProject();
				if (currentProject != nullptr)
				{
					// Close the current project
					Editor::GetInstance()->SetProject(nullptr);
				}
			}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                // Handle Open action
                OPENFILENAME ofn;
                char szFileName[MAX_PATH] = "";
                ZeroMemory(&ofn, sizeof(OPENFILENAME));
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner = NULL;
                ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
                ofn.lpstrFile = szFileName;
                ofn.nMaxFile = sizeof(szFileName) / sizeof(WCHAR);
                ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

                if (GetOpenFileName(&ofn)) {
					// Process the selected file path (szFileName)
                    char* fileNameChar = new char[strlen(ofn.lpstrFile) + 1];
                    strcpy(fileNameChar, ofn.lpstrFile);

					// Output the converted string
					std::cout << "File name: " << fileNameChar << std::endl;

					Editor::GetInstance()->GetScene()->Deserialize(fileNameChar);
					// Don't forget to free the allocated memory
					delete[] fileNameChar;
				}
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                //Open Save dialog
                OPENFILENAME ofn;
                char szFileName[MAX_PATH] = "";
                ZeroMemory(&ofn, sizeof(OPENFILENAME));
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner = NULL;
                ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
                ofn.lpstrFile = szFileName;
                ofn.nMaxFile = sizeof(szFileName) / sizeof(WCHAR);
                ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

                if (GetSaveFileName(&ofn)) {
					// Process the selected file path (szFileName)
                    char* fileNameChar = new char[strlen(ofn.lpstrFile) + 1];
                    strcpy(fileNameChar, ofn.lpstrFile);

					// Output the converted string
					std::cout << "File name: " << fileNameChar << std::endl;

					Editor::GetInstance()->GetScene()->Serialize(fileNameChar);
					// Don't forget to free the allocated memory
					delete[] fileNameChar;
				}
            }
            if (ImGui::MenuItem("Exit")) {
                //Handle exit action
            }
            ImGui::EndMenu();
        }

        // Edit menu
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
                // Handle Undo action
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
                // Handle Redo action
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Assets")) {
            if (ImGui::MenuItem("Import")) {
				// Handle Import action
                OPENFILENAME ofn;
                char szFileName[MAX_PATH] = "";

                ZeroMemory(&ofn, sizeof(OPENFILENAME));

                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner = NULL;
                ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
                ofn.lpstrFile = szFileName;
                ofn.nMaxFile = sizeof(szFileName);
                ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

                if (GetOpenFileName(&ofn)) {
                    // Allocate a buffer for the narrow-character string
                    char* fileNameChar = new char[strlen(ofn.lpstrFile) + 1];
                    strcpy(fileNameChar, ofn.lpstrFile);

                    // Output the converted string
                    std::cout << "File name: " << fileNameChar << std::endl;

                    for (int i = 0; i < 5; i++)
                    {
                        Entity* entity = new Entity();

                        ModelAsset* modelAsset = AssetManager::GetInstance()->Load<ModelAsset>(std::string(fileNameChar));

                        //Model* model = new Model(fileNameChar, RenderSettings{ false });
                        entity->AddComponent<Model>(modelAsset->GetModelData());

                        Editor::GetInstance()->GetScene()->Add(*entity);

						entity->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, i * 2.0f));
                    }
                   

					

                    // Don't forget to free the allocated memory
                    delete[] fileNameChar;
                }

			}
			ImGui::EndMenu();
		}

        // Other menus can be added here

        ImGui::EndMenuBar();
    }
}