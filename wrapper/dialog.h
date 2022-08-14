#pragma once

#include <lxu_dialog.hpp>
#include <string>

class TDialog {
public:
	TDialog(const std::string& tool);
	void Fail(const std::string& key);

private:
	CLxUser_StdDialogService Service;
	std::string Tool;
};