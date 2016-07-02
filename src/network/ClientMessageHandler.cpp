#include "ClientMessageHandler.hpp"

#include "../Chatbox.hpp"
#include "../Game.hpp"
#include "../GameState.hpp"
#include "msgtypes/Chat.hpp"
#include "msgtypes/ChunkTransfer.hpp"

namespace Diggler {
namespace Net {

ClientMessageHandler::ClientMessageHandler(GameState &gameState) :
  GS(gameState) {
}

bool ClientMessageHandler::handleMessage(InMessage &msg) {
  using namespace Net::MsgTypes;
  switch (msg.getType()) {
    case MessageType::NetDisconnect:
      GS.GW->showMessage("Disconnected", "Timed out");
      return false;

    case MessageType::ChunkTransfer: {
      using S = ChunkTransferSubtype;
      switch (static_cast<S>(msg.getSubtype())) {
        case S::Request: {
          ; // No-op
        } break;
        case S::Response: {
          ChunkTransferResponse ctr;
          ctr.readFromMsg(msg);
          for (const ChunkTransferResponse::ChunkData &cd : ctr.chunks) {
            ChunkRef c = GS.G->U->getWorldEx(cd.worldId)->getNewEmptyChunk(
              cd.chunkPos.x, cd.chunkPos.y, cd.chunkPos.z);
            InMemoryStream ims(cd.data, cd.dataLength);
            c->read(ims);
            GS.holdChunksInMem.push_back(c);
          }
        } break;
        case S::Denied: {
          ChunkTransferDenied ctd;
          ctd.readFromMsg(msg);
        } break;
      }
    } break;
    case MessageType::Chat: {
      using S = ChatSubtype;
      switch (static_cast<S>(msg.getSubtype())) {
        case S::Send: {
          ; // No-op
        } break;
        case S::Announcement: {
          ChatAnnouncement ca;
          ca.readFromMsg(msg);
          // TODO better formatting abilities
          if (ca.msg.type == msgpack::type::STR) {
            GS.m_chatBox->addChatEntry(ca.msg.as<std::string>());
          }
        } break;
        case S::PlayerTalk: {
          ChatPlayerTalk cpt;
          cpt.readFromMsg(msg);
          // TODO better formatting abilities
          if (cpt.msg.type == msgpack::type::STR) {
            std::string playerName;
            if (cpt.player.display.type == msgpack::type::NIL) {
              playerName = GS.G->players.getById(cpt.player.id).name + "> ";
            } else if (cpt.player.display.type == msgpack::type::STR) {
              cpt.player.display.convert(playerName);
            }
            GS.m_chatBox->addChatEntry(playerName + cpt.msg.as<std::string>());
          }
        } break;
      }
    } break;
    case MessageType::PlayerJoin: {
      Player &plr = GS.G->players.add();
      plr.id = msg.readU32();
      plr.name = msg.readString();
      getDebugStream() << "Player " << plr.name << '(' << plr.id << ") joined the party!" << std::endl;
    } break;
    case MessageType::PlayerQuit: {
      uint32 id = msg.readU32();
      try {
        Player &plr = GS.G->players.getById(id);
        getOutputStream() << plr.name << " is gone :(" << std::endl;
        GS.G->players.remove(plr);
      } catch (const std::out_of_range &e) {
        getOutputStream() << "Phantom player #" << id << " disconnected" << std::endl;
      }
    } break;
    case MessageType::PlayerUpdate: {
      uint32 id = msg.readU32();
      try {
        Player &plr = GS.G->players.getById(id);
        switch (msg.getSubtype()) {
          case PlayerUpdateType::Move: {
            glm::vec3 pos = msg.readVec3(),
                  vel = msg.readVec3(),
                  acc = msg.readVec3();
            plr.setPosVel(pos, vel, acc);
            plr.angle = msg.readFloat();
          } break;
          case PlayerUpdateType::Die:
            plr.setDead(false, (Player::DeathReason)msg.readU8());
            break;
          case PlayerUpdateType::Respawn:
            plr.setDead(false);
            break;
          default:
            break;
        }
      } catch (const std::out_of_range &e) {
        getOutputStream() << "Invalid player update: #" << id << " is not on server" << std::endl;
      }
    } break;
    case MessageType::BlockUpdate: {
      // TODO proper count
      // TODO handle that in Chunk's ChangeHelper
      int count  = msg.getSubtype();
      for (int i=0; i < count; ++i) {
        const glm::ivec3 wcpos = msg.readIVec3();
        uint8 x = msg.readU8(),
              y = msg.readU8(),
              z = msg.readU8();
        WorldId wid = msg.readI16();
        uint16 id = msg.readU16();
        uint16 data = msg.readU16();
        GS.G->U->getWorld(wid)->setBlock(x, y, z, id, data);
      }
    } break;
    case MessageType::Event: {
      // switch (msg.getSubtype())
    } break;
    default:
      break;
  }
  return true;
}

}
}
