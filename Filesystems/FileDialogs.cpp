#include "FileDialogs.h"

using namespace Jauntlet;

uint8_t Jauntlet::notifyPopup(const std::string& title, const std::string& message, Dialog_Icon icon) {
	switch (icon) {
	case (Dialog_Icon::INFO): 
		return tinyfd_notifyPopup(title.c_str(), message.c_str(), "info");
	case (Dialog_Icon::WARNING):
		return tinyfd_notifyPopup(title.c_str(), message.c_str(), "warning");
	default:
		// Question is not a valid prompt here, so all other options deafult to error
		return tinyfd_notifyPopup(title.c_str(), message.c_str(), "error");
	}
}

uint8_t Jauntlet::messageBox(const std::string& title, const std::string& message, Dialog_Type type, Dialog_Icon icon, Dialog_DefaultButton defaultButton) {
	std::string dialog, logo;
	switch (type) {
	case (Dialog_Type::OKCANCEL):
		dialog = "okcancel";
		break;
	case (Dialog_Type::YESNO):
		dialog = "yesno";
		break;
	case (Dialog_Type::YESNOCANCEL):
		dialog = "yesnocancel";
		break;
	default:
		dialog = "ok";
	}

	switch (icon) {
	case (Dialog_Icon::INFO):
		logo = "info";
		break;
	case (Dialog_Icon::QUESTION):
		logo = "question";
		break;
	case (Dialog_Icon::WARNING):
		logo = "warning";
		break;
	default:
		logo = "error";
	}

	switch (defaultButton) {
	case (Dialog_DefaultButton::YES):
	case (Dialog_DefaultButton::OK):
		// 1 = ok/yes
		return tinyfd_messageBox(title.c_str(), message.c_str(), dialog.c_str(), logo.c_str(), 1);
	case (Dialog_DefaultButton::CANCEL):
		// if no is an option, cancel is equal to 2, otherwise it is 0.
		return tinyfd_messageBox(title.c_str(), message.c_str(), dialog.c_str(), logo.c_str(), dialog == "yesnocancel" ? 2 : 0);
	default:
		// 0 is no/cancel
		return tinyfd_messageBox(title.c_str(), message.c_str(), dialog.c_str(), logo.c_str(), 0);
	}
}

// input box will be here

std::string Jauntlet::saveFilePrompt(const std::string& title, const std::string& defaultFilePath, const std::vector<std::string>& filters, const std::string& filterDescription) {
	return tinyfd_saveFileDialog(title.c_str(), defaultFilePath.c_str(), filters.size(), (const char* const*)filters.data(), filterDescription.c_str());
}

std::string Jauntlet::openFilePrompt(const std::string& title, const std::string& defaultFilePath, const std::vector<std::string>& filters, const std::string& filterDescription, bool allowSelectMultiple) {
	return tinyfd_openFileDialog(title.c_str(), defaultFilePath.c_str(), filters.size(), (const char* const*)filters.data(), filterDescription.c_str(), allowSelectMultiple ? 1 : 0);
}

std::string Jauntlet::selectFolderPrompt(const std::string& title, const std::string& defaultFilePath) {
	return tinyfd_selectFolderDialog(title.c_str(), defaultFilePath.c_str());
}

// color picker will be here