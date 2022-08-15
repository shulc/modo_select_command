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
		CLxUser_SelectionService srvSel;
		CLxUser_VertexPacketTranslation vertexPacketTranslation;
		vertexPacketTranslation.autoInit();
		LXtID4 typeCode = srvSel.LookupType(LXsSELTYP_VERTEX);

		TLayers layers;
		for (TMesh mesh : layers) {
			mesh.EachPolygon([&mesh, &srvSel, &vertexPacketTranslation, typeCode](TPolygon& polygon) {
				for (auto point : polygon.Vertexes()) {
					// point.SetMark(TMesh::ModeSelect);
					void* packet = vertexPacketTranslation.Packet(point.ID(), polygon.ID(), mesh.ID());
					srvSel.Select(typeCode, packet);
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