#include "dialog.h"

#include <lx_value.hpp>

TDialog::TDialog(const std::string& tool)
	: Tool(tool)
{
}

void TDialog::Fail(const std::string& key)
{
	CLxUser_Message msg;
	Service.MessageAllocate(msg);

	std::string body = "body_" + key;
	std::string title = "@" + Tool + "@title_" + key + "@";


	msg.SetCode(LXe_FAILED);
	msg.SetMessage(Tool.c_str(), body.c_str(), 0);

	Service.MessageOpen(msg, title.c_str(), nullptr, nullptr);
}