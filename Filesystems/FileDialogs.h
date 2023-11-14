/* Purpose: Extracts tinyfiledialogs into a more readable format for higher level use.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
// disable loss of data warnings; we do this on purpose.
#pragma warning (disable:4244)

#include "../Externals/tinyfiledialogs.h"
#include <string>
#include <vector>

namespace Jauntlet {
enum class Dialog_Icon { INFO, WARNING, ERROR, QUESTION };
enum class Dialog_Type { OK, OKCANCEL, YESNO, YESNOCANCEL};
enum class Dialog_DefaultButton { CANCEL, OK, YES, NO };

uint8_t notifyPopup(const std::string& title, const std::string& message, Dialog_Icon icon);

uint8_t messageBox(const std::string& title, const std::string& message, Dialog_Type type, Dialog_Icon icon, Dialog_DefaultButton defaultButton);

// input box method here, requires testing
// uint8_t inputBox

std::string saveFilePrompt(const std::string& title, const std::string& defaultFilePath, const std::vector<std::string>& filters, const std::string& filterDescription);

std::string openFilePrompt(const std::string& title, const std::string& defaultFilePath, const std::vector<std::string>& filters, const std::string& filterDescription, bool allowSelectMultiple);

std::string selectFolderPrompt(const std::string& title, const std::string& defaultFilePath);

// Color chooser here, requires testing
}
// Jauntlet::Color colorChooser