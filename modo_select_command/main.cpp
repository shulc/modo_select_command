#include <lxu_command.hpp>

#include <wrapper/log.h>
#include <wrapper/layer.h>
#include <wrapper/mesh.h>
#include <wrapper/polygon.h>
#include <wrapper/point.h>


constexpr const char* CMD_NAME = "mmm.command.select";


class TUnfuseCommand : public CLxCommand
{
public:
	TUnfuseCommand()
		: CLxCommand()
	{
		TMesh::InitModes();
	}

	void execute() final
	{
		{
			TLog() << "run select command";
		}
		TLayers layers;
		for (auto mesh : layers) {
			mesh.EachPolygon([](TPolygon& polygon) {
				for (auto point : polygon.Vertexes()) {
					point.SetMark(TMesh::ModeSelect);
					{
						TLog() << "found point: id = " << point.ID() << " index = " << point.Index();
					}
					}
				}, TMesh::ModeSelect);
			mesh.SetChange();
		}
	}
};

static CLxMeta_Command<TUnfuseCommand> CmdMeta(CMD_NAME);

static class CRoot : public CLxMetaRoot
{
	bool pre_init() final
	{
		CmdMeta.set_type_model();
		// CmdMeta.set_type_UI();
		CmdMeta.set_type_flags(LXfCMD_SELECT);
		add(&CmdMeta);
		return false;
	}
} root_meta;